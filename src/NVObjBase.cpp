/* NON-VISUAL OBJECT BASE (IMPLEMENTATION)
 * 
 * Base class for all Non-Visual objects based Omnis objects
 *
 * March 30, 2010 David McKeone (Created)
 */

#include <extcomp.he>
#include "NVObjBase.he"

using namespace OmnisTools;

// Constructor for building from an existing object instance
NVObjBase::NVObjBase( qobjinst objinst ) 
{
	mObjInst = objinst;	// we remember the objects instance
}

// Generic destruction
NVObjBase::~NVObjBase()
{
	// Insert any memory deletion code or general cleanup code
}

// Duplicate an object into a new object
NVObjBase* NVObjBase::dup( qlong propID, qobjinst objinst, tThreadData *pThreadData  )
{
	NVObjBase* copy = (NVObjBase*)createObject(propID, objinst, pThreadData);  // Defined in jsoncpp
	return copy;
}

// Copy one objects contents into another objects contents.
void NVObjBase::copy( NVObjBase* pObj )
{
	qobjinst inst = mObjInst;
	*this = *pObj;
	
	mObjInst = inst;
}

// Methods Available and Method Call Handling (These should be overriden by a sub-class)
qlong NVObjBase::returnMethods( tThreadData* pThreadData ) { return 1L; }
qlong NVObjBase::methodCall( tThreadData* pThreadData ) { return 1L; }

// Properties and Property Call Handling (These should be overriden by a sub-class)
qlong NVObjBase::returnProperties( tThreadData* pThreadData ) { return 1L; }
qlong NVObjBase::getProperty( tThreadData* pThreadData ) { return 1L; }
qlong NVObjBase::setProperty( tThreadData* pThreadData ) { return 1L; }
qlong NVObjBase::canAssignProperty( tThreadData* pThreadData, qlong propID ) { return qfalse; }
