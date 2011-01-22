/* SIMPLE OBJECT (IMPLEMENTATION)
 * 
 * This object has some simple functionality to show how to do basic method calls and property setting.
 *
 * March 30, 2010 David McKeone (Created)
 */

#include <extcomp.he>
#include "Simple.he"

using namespace OmnisTools;

/**************************************************************************************************
 **                       CONSTRUCTORS / DESTRUCTORS                                             **
 **************************************************************************************************/

NVObjSimple::NVObjSimple(qobjinst objinst, tThreadData *pThreadData) : NVObjBase(objinst), myProperty(0)
{ }

NVObjSimple::~NVObjSimple()
{ }

/**************************************************************************************************
 **                              PROPERTY DECLERATION                                            **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project it is also used as the Unique ID.
const static qshort cPropertyMyProperty = 3100;

/**************************************************************************************************
 **                               METHOD DECLERATION                                             **
 **************************************************************************************************/

// This is where the resource # of the methods is defined.  In this project is also used as the Unique ID.
const static qshort cMethodSquareNum = 2100,
                    cMethodEmpty     = 2101;

/**************************************************************************************************
 **                                 INSTANCE METHODS                                             **
 **************************************************************************************************/

// Call a method
qlong NVObjSimple::methodCall( tThreadData* pThreadData )
{
	qshort funcId = (qshort)ECOgetId(pThreadData->mEci);
	qshort paramCount = ECOgetParamCount(pThreadData->mEci);

	switch( funcId )
	{
		case cMethodSquareNum:
			pThreadData->mCurMethodName = "$squareNum";
			methodSquare(pThreadData, paramCount);
			break;
		case cMethodEmpty:
			pThreadData->mCurMethodName = "$empty";
			methodEmpty(pThreadData, paramCount);
			break;
	}

	return 0L;
}

/**************************************************************************************************
 **                                PROPERTIES                                                    **
 **************************************************************************************************/

// Assignability of properties
qlong NVObjSimple::canAssignProperty( tThreadData* pThreadData, qlong propID ) {
	switch (propID) {
		case cPropertyMyProperty:
			return qtrue;
		default:
			return qfalse;
	}
}

// Method to retrieve a property of the object
qlong NVObjSimple::getProperty( tThreadData* pThreadData ) 
{
	EXTfldval fValReturn;

	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cPropertyMyProperty:
			fValReturn.setLong(myProperty); // Put property into return value
			ECOaddParam(pThreadData->mEci, &fValReturn); // Return to caller
			break;	       
	}

	return 1L;
}

// Method to set a property of the object
qlong NVObjSimple::setProperty( tThreadData* pThreadData )
{
	// Retrieve value to set for property, always in first parameter
	EXTfldval fVal;
	if( getParamVar( pThreadData->mEci, 1, fVal) == qfalse ) 
		return qfalse;

	// Assign to the appropriate property
	qlong propID = ECOgetId( pThreadData->mEci );
	switch( propID ) {
		case cPropertyMyProperty:
			myProperty = fVal.getLong();
			break;
	}

	return 1L;
}

/**************************************************************************************************
 **                                        STATIC METHODS                                        **
 **************************************************************************************************/

/* METHODS */

// Table of parameter resources and types.
// Note that all parameters can be stored in this single table and the array offset can be  
// passed via the MethodsTable.
//
// Columns are:
// 1) Name of Parameter (Resource #)
// 2) Return type (fft value)
// 3) Parameter flags of type EXTD_FLAG_xxxx
// 4) Extended flags.  Documentation states, "Must be 0"
ECOparam cSimpleMethodsParamsTable[] = 
{
	4000, fftNumber, 0, 0
};

// Table of Methods available for Simple
// Columns are:
// 1) Unique ID 
// 2) Name of Method (Resource #)
// 3) Return Type 
// 4) # of Parameters
// 5) Array of Parameter Names (Taken from MethodsParamsTable.  Increments # of parameters past this pointer) 
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOmethodEvent cSimpleMethodsTable[] = 
{
	cMethodSquareNum, cMethodSquareNum, fftNumber, 1, &cSimpleMethodsParamsTable[0], 0, 0,
	cMethodEmpty    , cMethodEmpty    , fftNone  , 0, 0                     , 0, 0
};

// List of methods in Simple
qlong NVObjSimple::returnMethods(tThreadData* pThreadData)
{
	const qshort cMethodCount = sizeof(cSimpleMethodsTable) / sizeof(ECOmethodEvent);
	
	return ECOreturnMethods( gInstLib, pThreadData->mEci, &cSimpleMethodsTable[0], cMethodCount );
}

/* PROPERTIES */

// Table of properties available from Simple
// Columns are:
// 1) Unique ID 
// 2) Name of Property (Resource #)
// 3) Return Type 
// 4) Flags describing the property
// 5) Additional Flags describing the property
// 6) Enum Start (Not sure what this does, 0 = disabled)
// 7) Enum Stop (Not sure what this does, 0 = disabled)
ECOproperty cSimplePropertyTable[] = 
{
	cPropertyMyProperty, cPropertyMyProperty, fftInteger, EXTD_FLAG_PROPCUSTOM, 0, 0, 0 /* Shows under Custom category */
};

// List of properties in Simple
qlong NVObjSimple::returnProperties( tThreadData* pThreadData )
{
	const qshort propertyCount = sizeof(cSimplePropertyTable) / sizeof(ECOproperty);

	return ECOreturnProperties( gInstLib, pThreadData->mEci, &cSimplePropertyTable[0], propertyCount );
}

/**************************************************************************************************
 **                              CUSTOM (YOUR) METHODS                                           **
 **************************************************************************************************/

// Simple method that squares the result $method(5.0) = 25.0
void NVObjSimple::methodSquare( tThreadData* pThreadData, qshort pParamCount )
{
	EXTfldval fvalNumber, fvalReturn;
	if( pParamCount != 1)
		return;

	getParamVar(pThreadData, pParamCount, fvalNumber);
	
	// Extract qreal from field value
	qreal num, result; qshort dp = dpFloat;
	fvalNumber.getNum( num, dp );
	
	// Perform calculation
	result = num * num;
	
	// Setup return field value 
	fvalReturn.setNum( result, dp );

	// Add a parameter; This actually sets up the return value
	ECOaddParam(pThreadData->mEci, &fvalReturn);
}

// Empty method.  Determines call over head for a method
void NVObjSimple::methodEmpty( tThreadData* pThreadData, qshort pParamCount )
{ }
