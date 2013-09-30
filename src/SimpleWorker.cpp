//
//  SimpleWorker.cpp
//  NVObjTemplate
//
//  Created by David McKeone on 9/30/13.
//  Copyright 2013 __MyCompanyName__. All rights reserved.
//

#include "SimpleWorker.h"

using namespace OmnisTools;

void SimpleWorkerDelegate::init(OmnisTools::ParamMap&)
{
    // DEV NOTE: Lists can be populated in a background object, but must be allocated on the main thread.
    _listResult = boost::shared_ptr<EXTqlist>(new EXTqlist(listVlen));
}

void SimpleWorkerDelegate::cancel()
{
}

OmnisTools::ParamMap SimpleWorkerDelegate::run(OmnisTools::ParamMap& params) 
{
    OmnisTools::ParamMap result;
    
    // Do the work that needs to be done
    
    // E.g. add some data to a list
    EXTfldval colVal;
    str255 colName = initStr255("Example");
    _listResult->addCol(fftCharacter, dpFcharacter, 10000000, &colName);
    
    _listResult->insertRow();
    _listResult->getColValRef(1,1,colVal,qtrue);
    colVal.setChar(initStr255("Row 1"));
    
    _listResult->insertRow();
    _listResult->getColValRef(2,1,colVal,qtrue);
    colVal.setChar(initStr255("Row 2"));
    
    _listResult->insertRow();
    _listResult->getColValRef(3,1,colVal,qtrue);
    colVal.setChar(initStr255("Row 3"));
    
    // Check cancellation of worker by catching Boost exceptions at interruption points and thread sleep.
    try {
        // Cancel checked in sleep
        boost::this_thread::sleep(boost::posix_time::millisec(5000));
        
        // and at an explicit interruption point
        boost::this_thread::interruption_point();
        
    } catch (const boost::thread_interrupted&) {
        // Cancelled
    }
    
    // Return list via parameters
    result["Result"] = _listResult;
    
	return result;
}
