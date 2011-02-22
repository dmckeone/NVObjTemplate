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

/* OMNIS TOOLS (IMPLEMENTATION)
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

#if ! defined(MARKUP_SIZEOFWCHAR)
  #if __SIZEOF_WCHAR_T__ == 4 || __WCHAR_MAX__ > 0x10000
    #define MARKUP_SIZEOFWCHAR 4
  #else
    #define MARKUP_SIZEOFWCHAR 2
  #endif
#endif

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

// Get a list for a specific parameter from the EXTCompInfo
qbool OmnisTools::getParamList( tThreadData* pThreadData, qshort pParamNum, EXTqlist& pOutList, qbool pCanBeRow) {
	
	EXTParamInfo* param = ECOfindParamNum( pThreadData->mEci, pParamNum );
	if ( !param ) 
		return qfalse;
	
	EXTfldval fval(reinterpret_cast<qfldval>(param->mData));
	if ( isList(fval,pCanBeRow) != qtrue )
		return qfalse;
	
	fval.getList(&pOutList,qfalse);
	
	return qtrue;
}

// Check if an EXTfldval is a list or, optionally, a row.
qbool OmnisTools::isList( EXTfldval& pFVal, qbool pCanBeRow ) {
	ffttype fft; pFVal.getType(fft);
	return ( (fft == fftList || (pCanBeRow && fft == fftRow)) ? qtrue : qfalse );
}

// Get a qbool parameter for a specific parameter #
qbool OmnisTools::getParamBool( tThreadData* pThreadData, qshort pParamNum, qbool& pOutBool ) {
	EXTParamInfo* param = ECOfindParamNum( pThreadData->mEci, pParamNum );
	if ( !param )
		return qfalse;
	
	EXTfldval fval(reinterpret_cast<qfldval>(param->mData));
	
	ffttype fft; fval.getType(fft);
	if ( fft != fftInteger && fft != fftBoolean && fft != fftConstant )
		return qfalse;
	
	qlong x = fval.getLong();
	if ( x != 0 && x != 1 )
		return qfalse;
	
	pOutBool = qbool(x != 0);
	return qtrue;
}

// Get a qshort parameter for a specific parameter #
qbool OmnisTools::getParamShort( tThreadData* pThreadData, qshort pParamNum, qshort& pOutShort ) {
	
	qlong longVal;
	if( getParamLong(pThreadData, pParamNum, longVal) != qtrue )
		return qfalse;
	
	if (longVal < 0 || longVal > 255)
		return qfalse;
	
	pOutShort = static_cast<qshort>(longVal);
	return qtrue;	
}

// Get a qlong parameter for a specific parameter #
qbool OmnisTools::getParamLong( tThreadData* pThreadData, qshort pParamNum, qlong& pOutInteger ) {
	EXTParamInfo* param = ECOfindParamNum( pThreadData->mEci, pParamNum );
	if ( !param )
		return qfalse;
	
	EXTfldval fval(reinterpret_cast<qfldval>(param->mData));
	
	ffttype fft; fval.getType(fft);
	if ( fft == fftCharacter )
	{
		strxxx& s_num = fval.getChar();
		if ( stringToQlong( s_num, pOutInteger ) )
			return qtrue;
	}
	else if ( fft == fftInteger || fft == fftBoolean || fft == fftConstant || fft == fftNumber )
	{
		pOutInteger = fval.getLong();
		return qtrue;
	}
	
	return qfalse;
}

// Convenience method to return the type of an EXTfldval
ffttype OmnisTools::getType( EXTfldval& fVal ) {
	ffttype fft; fVal.getType(fft);
	return fft;
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

// Get a std::wstring from an EXTfldval object
std::wstring OmnisTools::getWStringFromEXTFldVal(EXTfldval& fVal) {
	std::wstring retString;
	
	// Get a qchar* string
	qlong maxLength = fVal.getBinLen()+1; // Use binary length as approximation of maximum size
	qlong length = 0, stringLength = 0;
	qchar* omnisString = new qchar[maxLength];
	fVal.getChar(maxLength, omnisString, length);
	
	wchar_t* cString;
#if MARKUP_SIZEOFWCHAR == 2
	// For 2-Byte UTF16 wchar_t* (Typically Windows)
	// Convert from UTF8 to UTF16 and set new stringLength
	
	// Translate qchar* string into UTF8 binary
	qbyte* utf8data = reinterpret_cast<qbyte*>(omnisString);
	stringLength = CHRunicode::charToUtf8(omnisString, length, utf8data);
	
	// Translate UTF8 to UTF16
	CHRconvToUtf16 utf16conv(utf8data, stringLength);
	UChar* utf16data = utf16conv.dataPtr();
	stringLength = utf16conv.len();
	
	// Translate UTF16 binary into char* string
	cString = reinterpret_cast<wchar_t*>(utf16data);
#else
	// For 4-Byte UTF32 wchar_t* (Typically Mac and Linux)
	// Convert from UTF8 to UTF32 and set new stringLength
	stringLength = length;
	CHRconvToUtf32FromChar utf32conv(omnisString, stringLength, qfalse);
	U32Char* utf32data = utf32conv.dataPtr();
	stringLength = utf32conv.len();
	
	// Translate UTF16 binary into char* string
	cString = reinterpret_cast<wchar_t*>(utf32data);
#endif
	
	// Create standard string
	retString = std::wstring(cString,stringLength);
	
	// Clean-up
	delete [] omnisString;
	
	return retString;
}

// Set an existing EXTfldval object from a std::wstring
void OmnisTools::getEXTFldValFromWString(EXTfldval& fVal, const std::wstring readString) {
	qlong length;
	qchar* omnisString = getQCharFromWString(readString, length);
	
	fVal.setChar(omnisString, length); // Set value of character field, but exclude the last character since it will be the null terminator from the C String
	
	// Clean-up
	delete [] omnisString;
}

// Get a dynamically allocated qchar* array from a std::wstring
qchar* OmnisTools::getQCharFromWString(const std::wstring readString, qlong &retLength) {
	qlong length = readString.size();
	
	// Cast-away constness of c_str() pointer 
	wchar_t* cString = const_cast<wchar_t*>(readString.c_str());
	
	qchar* omnisString;
#if MARKUP_SIZEOFWCHAR == 2
	// For 2-Byte UTF16 wchar_t* (Typically Windows)
	// Feed into raw byte data
	UChar* utf16data = reinterpret_cast<UChar*> (cString);
	
	// Convert to UTF-8
	CHRconvFromUtf16 utf16conv(utf16data, length);
	length = utf16conv.len();
	qbyte* utf8data = utf16conv.dataPtr();
	
	// Allocate new qchar* string
	omnisString = new qchar[length];
	
	// Convert to Omnis Character field
	retLength = CHRunicode::utf8ToChar(utf8data, length, omnisString);  // Convert characters into Omnis Char Field
#else
	// For 4-Byte UTF32 wchar_t* (Typically Mac and Linux)
	U32Char* utf32data = reinterpret_cast<U32Char*> (cString);
	
	// Convert to UTF-8
	CHRconvFromUtf32ToChar utf32conv(utf32data, length, qfalse);
	length = utf32conv.len();
	retLength = length;
	omnisString = new qchar[length];
	OMstrcpy(omnisString, utf32conv.dataPtr()); // Copy string so it lives past the end of this function
#endif
	
	return omnisString;
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
void OmnisTools::getEXTFldValFromString(EXTfldval& fVal, const std::string readString) {
	qlong length;
	qchar* omnisString = getQCharFromString(readString, length);
	
	fVal.setChar(omnisString, length); // Set value of character field, but exclude the last character since it will be the null terminator from the C String
	
	// Clean-up
	delete [] omnisString;
}

// Get a dynamically allocated qchar* array from a std::string
qchar* OmnisTools::getQCharFromString(const std::string readString, qlong &retLength) {
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

// Return a C++ bool from an EXTfldval
bool OmnisTools::getBoolFromEXTFldVal(EXTfldval& fVal) {
	qbool omnBool;
	fVal.getBool(&omnBool);
	return getBoolFromQBool(omnBool);
}

// Get an EXTfldval for a C++ bool
void OmnisTools::getEXTFldValFromBool(EXTfldval& fVal, bool b) {
	qbool omBool = getQBoolFromBool(b);
	fVal.setBool(omBool);
}

// Return a C++ int from an EXTfldval
int OmnisTools::getIntFromEXTFldVal(EXTfldval& fVal) {
	qlong omnInt = fVal.getLong();
	
	if (omnInt < INT_MIN || omnInt > INT_MAX) {
		qlong omnInt = 0; // zero out any numbers that exceed
	}
	
	return static_cast<int>( omnInt );
}

// Get an EXTfldval for a C++ int
void OmnisTools::getEXTFldValFromInt(EXTfldval& fVal, int i) {
	fVal.setLong(static_cast<qlong>(i));
}

// Return a C++ long from an EXTfldval
long OmnisTools::getLongFromEXTFldVal(EXTfldval& fVal) {
	return static_cast<long>( fVal.getLong() );
}

// Get an EXTfldval for a C++ long
void OmnisTools::getEXTFldValFromLong(EXTfldval& fVal, long l) {
	fVal.setLong(static_cast<qlong>(l));
}

// Return a C++ long from an EXTfldval
double OmnisTools::getDoubleFromEXTFldVal(EXTfldval& fVal) {
	qreal omnReal;
	qshort dp = dpFmask;
	fVal.getNum(omnReal, dp);
	
	return static_cast<double>( omnReal );
}

// Get an EXTfldval for a C++ double
void OmnisTools::getEXTFldValFromDouble(EXTfldval& fVal, double d) {
	qreal omnReal = static_cast<qreal>(d);
	qshort dp = dpFmask;
	fVal.setNum(omnReal, dp);
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


