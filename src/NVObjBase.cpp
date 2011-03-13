/* Copyright (c) 2010 David McKeone
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 * derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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

void NVObjBase::callErrorMethod( tThreadData* pThreadData, tResult pError )
{
	// If the method completed ok, then there is no need to check for errors.
	if (pError == ERR_OK || pError == ERR_RETURN_ZERO || pError == METHOD_DONE_RETURN || pError == METHOD_OK)
		return;
	
	std::string txt = translateObjectError(pError);
	
	// Error wasn't object specific, look for default error
	if (txt.empty())
		txt = translateDefaultError(pError);
	
	// Fill parameters for error method (ErrorCode, ErrorDesc, ExtraErrorText, MethodName)
	EXTfldval params[4];
	params[0].setLong( pError ); // ErrorCode
	
	getEXTFldValFromString(params[1], txt); // ErrorDesc
	
	if ( !(pThreadData->mExtraErrorText.empty()) )
		getEXTFldValFromString(params[2], pThreadData->mExtraErrorText); // ExtraErrorText
	
	if ( !(pThreadData->mCurMethodName.empty()) )
		getEXTFldValFromString(params[3], pThreadData->mCurMethodName); // MethodName
	
	// Call $error method
	str31 s_error(initStr31("$error"));
	ECOdoMethod( mObjInst, &s_error, params, 4 );
}

// Built in error codes
std::string NVObjBase::translateDefaultError( OmnisTools::tResult pError ) {
	
	std::string txt;
	switch (pError)
	{
			txt = "";
		case ERR_OK:
		case ERR_RETURN_ZERO:
		case METHOD_DONE_RETURN:
			break;
		case ERR_METHOD_FAILED:
			txt = "Method failed.";
			break;
		case ERR_BAD_METHOD:
			txt = "Invalid method called. Internal method index error.";
			break;
		case ERR_BAD_PARAMS:
			txt = "Invalid parameters";
			break;
		case ERR_NOMEMORY:
			txt = "Out of memory.";
			break;
		case ERR_NOT_SUPPORTED:
			txt = "The feature is not supported";
			break;
		case ERR_BAD_CALCULATION:
			txt = "Calculation is invalid";
			break;
		case ERR_NOT_IMPLEMENTED:
			txt = "Feature not implemented.";
			break;
		default:
			break;
	}
	
	return txt;
}

// Object specific error codes (Should be overridden when object specific error codes are specified)
std::string NVObjBase::translateObjectError( OmnisTools::tResult pError ) { 
	return std::string(""); 
}
