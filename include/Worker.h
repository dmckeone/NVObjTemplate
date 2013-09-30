//
//  Worker.h
//  PostgreSQLBackground
//
//  Created by David McKeone on 11-11-01.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef WORKER_H_
#define WORKER_H_

#include "OmnisTools.he"
#include "Logging.he"

#include <string>
#include <queue>
#include <extcomp.he>

#include <boost/any.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/enable_shared_from_this.hpp>

class Queue;  // Forward declare to hold a pointer to the queue the object is in
class WorkerDelegate;

class Worker : public boost::enable_shared_from_this<Worker> {
public:    
    Worker();
    Worker(const OmnisTools::ParamMap& p, boost::shared_ptr<WorkerDelegate> d);
    Worker(const Worker& w);
    virtual ~Worker();
    
    // Starting worker
    void run();
    void start();
    void start(boost::shared_ptr<Queue>);
    
    bool running();
    void setRunning(bool r);
    
    bool complete();
    void setComplete(bool c);
    
    void cancel();
    bool cancelled();
    
    OmnisTools::ParamMap result();
    void setResult(const OmnisTools::ParamMap& pm);
    
    virtual void init(); // Code to be run prior to entering the thread
    virtual std::string desc();
    virtual void reset();
    
    void setWorkerName(std::string wn) { _workerName = wn; }
    
    boost::shared_ptr<WorkerDelegate> delegate();
    //void setDelegate(const boost::shared_ptr<WorkerDelegate>& d) { _delegate = d; }
    
protected:
    std::string _workerName;
    
    OmnisTools::ParamMap _params;
    OmnisTools::ParamMap _result;
private:
    
    class WorkerThread {
    public: 
        WorkerThread() {}
        WorkerThread(const boost::weak_ptr<Worker>& w, const boost::shared_ptr<WorkerDelegate>& wd) : _worker(w), _delegate(wd) {}
        
        void operator()();
    private:
        boost::weak_ptr<Worker> _worker;
        boost::shared_ptr<WorkerDelegate> _delegate;
    };
    
    bool _running;
    bool _complete;
    bool _cancelled;
    
    boost::shared_mutex _runMutex;
    boost::shared_mutex _completeMutex;
    boost::shared_mutex _cancelMutex;
    boost::shared_mutex _resultMutex;
    boost::mutex _workMutex;
    boost::thread _thread;
    
    boost::shared_ptr<Queue> _queue;
    boost::shared_ptr<WorkerDelegate> _delegate;
};

// Worker Delegate
class WorkerDelegate : public boost::enable_shared_from_this<WorkerDelegate> {
public:
    virtual void init(OmnisTools::ParamMap&) = 0;
    virtual OmnisTools::ParamMap run(OmnisTools::ParamMap&) = 0;
    virtual void cancel() = 0;
};

#endif // WORKER_H_



