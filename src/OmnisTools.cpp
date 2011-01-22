/* OMNIS TOOLS (HEADER)
 * 
 * This is a colleciton of tools for creating omnis components
 *
 * NOTE: All headers for external components must have the .he extension instead of the standard .h
 *       To enable code-highliting in Visual Studio 2008 go to Tools->Options-Text Editor-File Extension 
 *       and add "he" as a "Microsoft Visual C++" experience.
 *
 * October 4, 2010 David McKeone (Created)
 */

#include "OmnisTools.he"
#include <sstream>

// Get a parameter from the thread data
qbool OmnisTools::getParamVar( tThreadData* pThreadData, qshort pParamNum, EXTfldval& pOutVar )
{
	return getParamVar(pThreadData->mEci, pParamNum, pOutVar);
}

// Get a parameter from the EXTCompInfo
qbool OmnisTools::getParamVar( EXTCompInfo* pEci, qshort pParamNum, EXTfldval& pOutVar )
{
	EXTParamInfo* param = ECOfindParamNum( pEci, pParamNum );
	if( !param )
		return qfalse;
	
	pOutVar.setFldVal((qfldval)param->mData);
	pOutVar.setReadOnly( qfalse );
	
	return qtrue;
}

// Get a qbool from a C++ boolean
qbool OmnisTools::getQBoolFromBool(bool b) {
	if (b == true) {
		return 2;
	} else if (b == false) {
		return 1;
	} else {
		return 0;
	}
}

// Get a C++ boolean from a qbool
bool OmnisTools::getBoolFromQBool(qbool qb) {
	if (qb == 2) {
		return true;
	} else if (qb == 1) {
		return false;
	} else {
		return NULL;
	}
}

// Get a std::string from an EXTfldval object
std::string OmnisTools::getStringFromEXTFldVal(EXTfldval& fVal) {
	std::string retString;
	
	// Get a qchar* string
	qlong maxLength = fVal.getBinLen()+1; // Use binary length as approximation of maximum size
	qlong length = 0, stringLength = 0;
	qchar* omnisString = new qchar[maxLength];
	fVal.getChar(maxLength, omnisString, length);
	
	// Translate qchar* string into UTF8 binary
	qbyte* utf8data = reinterpret_cast<qbyte*>(omnisString);
	stringLength = CHRunicode::charToUtf8(omnisString, length, utf8data);
	
	// Translate UTF8 binary into char* string
	char* cString = reinterpret_cast<char*> (utf8data);
	
	// Create standard string
	retString = std::string(cString,stringLength);
	
	// Clean-up
	delete [] omnisString;
	
	return retString;
}

// Set an existing EXTfldval object from a std::string
void OmnisTools::getEXTFldValFromString(EXTfldval& fVal, std::string readString) {
	qlong length;
	qchar* omnisString = getQCharFromString(readString, length);
	
	fVal.setChar(omnisString, length); // Set value of character field, but exclude the last character since it will be the null terminator from the C String
	
	// Clean-up
	delete [] omnisString;
}

// Get a dynamically allocated qchar* array from a std::string
qchar* OmnisTools::getQCharFromString(std::string readString, qlong &retLength) {
	qlong length = readString.size();
	
	// Cast-away constness of c_str() pointer 
	char* cString = const_cast<char*>(readString.c_str());
	
	// Feed into raw byte data
	qbyte* utf8data = reinterpret_cast<qbyte*> (cString);
	
	// Allocate new qchar* string
	qchar* omnisString = new qchar[length];
	
	// Convert to Omnis Character field
	retLength = CHRunicode::utf8ToChar(utf8data, length, omnisString);  // Convert characters into Omnis Char Field
	
	return omnisString;
}

// Get an ISO 8601 Formatted Date String from EXTFldVal
std::string OmnisTools::getISO8601DateStringFromEXTFldVal(EXTfldval& fVal) {
	datestamptype theDate;
	std::string retString;
	std::stringstream sin(retString);
	
	fVal.getDate(theDate, dpFdtimeC);
	ffttype valType; qshort subType; fVal.getType(valType, &subType);
	
	if (valType != fftDate)
		return "";
	
	// Set date part of string
	switch (subType) {
		case dpFdate1900:
		case dpFdate1980:
		case dpFdate2000:
		case dpFdtime1900:
		case dpFdtime1980:
		case dpFdtime2000:
	    case dpFdtimeC:
			sin << theDate.mYear << "-" << theDate.mMonth << "-" << theDate.mDay; 
			break;
		default:
			break;
	}
	
	// Set time part of string
	switch (subType) {
		case dpFdtime1900:
		case dpFdtime1980:
		case dpFdtime2000:
	    case dpFdtimeC:
		case dpFtime:
			sin << "T" << theDate.mHour << ":" << theDate.mMin << ":" << theDate.mSec;
			break;
		default:
			break;
	}
	
	return retString;
}


