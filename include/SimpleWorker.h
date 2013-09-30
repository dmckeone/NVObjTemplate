//
//  SimpleWorker.h
//  NVObjTemplate
//
//  Created by Mac Build on 9/30/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#ifndef NVObjTemplate_SimpleWorker_h
#define NVObjTemplate_SimpleWorker_h

#include "Worker.h"
#include "OmnisTools.he"

class SimpleWorkerDelegate : public WorkerDelegate {
public:
    virtual void init(OmnisTools::ParamMap&);
    virtual OmnisTools::ParamMap run(OmnisTools::ParamMap&);
    virtual void cancel();
    
private:
    boost::shared_ptr<EXTqlist> _listResult;
};

#endif
