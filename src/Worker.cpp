//
//  Worker.cpp
//  PostgreSQLBackground
//
//  Created by David McKeone on 11-11-01.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "Worker.h"
#include "Logging.he"
#include "OmnisTools.he"

#include <boost/format.hpp>

using namespace OmnisTools;
using boost::format;

static const int SLEEP_MS = 100;  // Time to sleep when waiting for connection to finish on PostgreSQL server side
static const int WAIT_MS = 500;  // Time to sleep when nothing is done and waiting for notifications

Worker::Worker() : _complete(false), _running(false), _cancelled(false) 
{ }

Worker::Worker(const OmnisTools::ParamMap& p, boost::shared_ptr<WorkerDelegate> d) : _params(p), _delegate(d), _complete(false), _running(false), _cancelled(false) 
{ }

Worker::Worker(const Worker& w)
{
    _workerName = w._workerName;
    _params = w._params;
    _result = w._result;
    
    _running = w._running;
    _complete = w._complete;
    _cancelled = w._cancelled;
    
    _queue = w._queue;
    _delegate = w._delegate;
} 

Worker::~Worker() {
    cancel();
    //_thread.join();  // Don't destruct objects until thread has finished
}

// Description of object used for logging
std::string Worker::desc() {
    if (!_workerName.empty()) {
        return str(format("Worker (%s)") % _workerName);
    } else {
        return "Worker";
    }  
}

// Override point for sub-classes to init objects that must run in the main thread
void Worker::init() {
    if (_delegate) {
        _delegate->init(_params);
    }
	_complete = false;
	_cancelled = false;
	_running = false;
}

bool Worker::running() {
    // Get shared lock (multiple readers / one writer)
    boost::shared_lock<boost::shared_mutex> lock(_runMutex);
    return _running; 
}

void Worker::setRunning(bool r) {
    // Get unique lock
    boost::unique_lock<boost::shared_mutex> lock(_runMutex);
    _running = r; 
}

bool Worker::complete() {
    // Get shared lock (multiple readers / one writer)
    boost::shared_lock<boost::shared_mutex> lock(_completeMutex);
    
    return _complete; 
}

void Worker::setComplete(bool c) {
    // Get unique lock
    boost::unique_lock<boost::shared_mutex> lock(_completeMutex);
    _complete = c;
}

OmnisTools::ParamMap Worker::result() 
{ 
    // Get shared lock (multiple readers / one writer)
    boost::shared_lock<boost::shared_mutex> lock(_resultMutex);
    
    return _result; 
}

void Worker::setResult(const OmnisTools::ParamMap& pm) 
{ 
    boost::unique_lock<boost::shared_mutex> lock(_resultMutex);
    _result = pm; 
}

boost::shared_ptr<WorkerDelegate> Worker::delegate() {
    return _delegate;
}

// Cancel status
bool Worker::cancelled() {
    // Get shared lock (multiple readers / one writer)
    boost::shared_lock<boost::shared_mutex> lock(_cancelMutex);
    
    return _cancelled; 
}

// Cancel request
void Worker::cancel() { 
    boost::unique_lock<boost::shared_mutex>(_cancelMutex); // Locked so many callers can cancel at the same time
    
    _cancelled = true; 
    
    if(_delegate) {
        _delegate->cancel();
    }
    LOG_DEBUG << desc() << " requested cancel.";
    
    if( !_thread.interruption_requested() ) {
        _thread.interrupt();
    }  
}

// Reset the current worker objects list
void Worker::reset() {
}

// Run worker
void Worker::run() {
    if(_delegate) {
        setResult( _delegate->run(_params) );
        setComplete(true);
    }
}

// Start-up to run item on a thread
void Worker::start() {
    
    if(running() == true) {
        // Only start if not already running
        return;
    }
    
    // Run thread
    _thread = boost::thread(WorkerThread(shared_from_this(), _delegate));
}

// Start-up to run item on a thread
void Worker::start(boost::shared_ptr<Queue> q) {
    
    if(running() == true) {
        // Only start if not already running
        return;
    }
    
    _queue = q;
    
    // Run thread
    _thread = boost::thread(WorkerThread(shared_from_this(), _delegate));
}

// Thread entry point
void Worker::WorkerThread::operator()() {
    
    // Get shared pointer
    boost::shared_ptr<Worker> ptr;
    OmnisTools::ParamMap params;
    {
        try { 
            ptr = _worker.lock();
        } catch (const boost::bad_weak_ptr& e) {
            return;
        }
        
        // Lock work mutex
        boost::unique_lock<boost::mutex> lock(ptr->_workMutex);
        
        if(ptr->running()) {
            // Thread is already running
            return;
        }
        
        // Mark worker as running        
        ptr->setRunning(true);
        
        LOG_INFO << ptr->desc() << " started";
        
        // Copy Params
        params = ptr->_params;
        
        // Release shared pointer prior to starting logic (otherwise it holds a reference and prevents worker destruct)
        ptr.reset(); 
    }
    
    // Perform worker work
    OmnisTools::ParamMap result = _delegate->run(params); 
    
    // Re-acquire shared pointer
    try { 
        ptr = _worker.lock();
    } catch (const boost::bad_weak_ptr& e) {
        // Worker out of scope
        LOG_INFO << "Worker out of scope at completion";
        return;
    }
    
    if (ptr) {
        ptr->setResult(result);
        
        if(ptr->cancelled()) {
            LOG_INFO << ptr->desc() << " canceled";
        } else {
            LOG_INFO << ptr->desc() << " complete";
        }
        
        ptr->setComplete(true);
        ptr->setRunning(false);
    }
}

