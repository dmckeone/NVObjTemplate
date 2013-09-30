//
//  ThreadTimer.cpp
//  TM_WORK_QUEUE_HE
//
//  Created by David McKeone on 11-10-09.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "ThreadTimer.he"
#include "NVObjBase.he"
#include <map>
#include <boost/foreach.hpp>

#define TIMER_MS 100

// Init static storage
FARPROC ThreadTimer::_omnisTimer = NULL;
std::map<NVObjBase*,ThreadTimer::nextTimer> subscribers;
bool timerProcessing = false;

ThreadTimer::ThreadTimer() : _timerID(0) {
    _omnisTimer = WNDmakeTimerProc((WNDtimerProc)ThreadTimer::timerMsgProc, gInstLib);
}

ThreadTimer::~ThreadTimer() {
    if ( _omnisTimer )
		WNDdisposeTimerProc ( _omnisTimer );
}

ThreadTimer& ThreadTimer::instance() {
    static ThreadTimer theInst;
    
    return theInst;    
}

void OMNISWNDPROC ThreadTimer::timerMsgProc( HWND hwnd, UINT Msg, UINT idTimer, qulong time )
{
    if (timerProcessing) {
        return;
    }
    
    timerProcessing = true;

    
    NVObjBase* objPointer;
    std::map<NVObjBase*,nextTimer>::iterator it = subscribers.begin();
    while( it != subscribers.end() ) {
        if( it->second == kTimerContinue) {
            objPointer = it->first;
            it->second = (ThreadTimer::nextTimer) objPointer->notify();  // Cast int return value to enum
        }
        
        if(it->second == kTimerStop) {
            subscribers.erase(it++);  // Move past current iterator position, but remove item
        } else {
            ++it;
        }
    }   
    
    timerProcessing = false;
}

// Subscribe an object to the timer
void ThreadTimer::subscribe(const NVObjBase* obj) {
    NVObjBase* objPointer = const_cast<NVObjBase*>(obj);
    subscribers[objPointer] = kTimerContinue;
    
    if(!_timerID) {
        _timerID = WNDsetTimer(NULL, 0, TIMER_MS, _omnisTimer);
    }
}

// Unsubscribe an object from the timer
void ThreadTimer::unsubscribe(const NVObjBase* obj) {
    NVObjBase* objPointer = const_cast<NVObjBase*>(obj);
    
    std::map<NVObjBase*,nextTimer>::iterator it;
    it = subscribers.find(objPointer);
    if(it != subscribers.end()) {
        subscribers.erase(it);
    }
    
    if(subscribers.empty()) {
        WNDkillTimer( NULL, _timerID );
        _timerID = 0;
    }
}

// TODO: Make subscribe method that uses 
//   WNDsetTimer( NULL, 0, milliSeconds, gTimerProc );
//       to create a timer on a short timeline
//
// TODO: Must also create unsubscribe that stops the timer when the last 
//       item has been unsubscribed