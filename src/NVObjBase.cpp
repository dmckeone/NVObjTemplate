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
