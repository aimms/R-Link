/*  This file is part of R-Link.

    R-Link is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    R-Link is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with R-Link.  If not, see <http://www.gnu.org/licenses/>.

*/
// An ASCII interface (assuming UTF8) to a 3.13 (or earlier) Ascii libaimms
#define _CRT_SECURE_NO_WARNINGS
#include "iAimmsFactory.h"
#include <aimmsifc/iAimms.h>
#include <Common/DynamicLib.h>
#include <exception>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include "ImplHelpers.h"

#include "StringConversion.h"
#include "Converters.h"

typedef aimmsifc::utf82ccp_t CONVERTER_TO_AIMMS;
typedef aimmsifc::ccp2utf8_t CONVERTER_FROM_AIMMS;

// input C string
#define inCS( in )		InputString<CONVERTER_TO_AIMMS>(in, m_ConvertToAimms)
// input AIMMS string
#define inAS( in )		InputAimmsString<CONVERTER_TO_AIMMS>(in, m_ConvertToAimms)
// input AIMMS value
#define inV( in )		InputAimmsValue<CONVERTER_TO_AIMMS>(in, m_ConvertToAimms)
// output AIMMS string
#define outAS( out )	OutputAimmsString<CONVERTER_FROM_AIMMS>(out, m_ConvertFromAimms)
// output AIMMS string
#define outV( out )		OutputAimmsValue<CONVERTER_FROM_AIMMS>(out, m_ConvertFromAimms)
// output AIMMS Value array
#define outVA( n, out )	OutputAimmsValueArray<CONVERTER_FROM_AIMMS>(n, out, m_ConvertFromAimms)
// input ValueArray
#define inVA( n, in )	InputAimmsValueArray<CONVERTER_TO_AIMMS>(n, in, m_ConvertToAimms)

// AimmsAPI functions usings string do not have a 'W' or 'A' postfix in 3.13 or earlier, so we use DECORATED_DLL_NAME
#define DECORATED_DLL_NAME_T DECORATED_DLL_NAME

namespace aimmsifc {

class iAimmsA_UTF8_313asc : public iAimmsA, public iFactoryManage
{
	Common::DynamicLib m_Library;
	DECLARE_FUNCTION_POINTERS;

	// the types are they are defined in the underlying AimmsAPI
	typedef AimmsValueType<char> DllValueType;
	typedef AimmsStringType<char> DllStringType;
	typedef char DllCharType;

	// either std::string or std::wstring matching iAimms::CharType
	typedef std::string iAimmsStdString;

	CONVERTER_TO_AIMMS   m_ConvertToAimms;     // from iAimms::CharType to DllCharType
	CONVERTER_FROM_AIMMS m_ConvertFromAimms;   // from DllCharType to iAimms::CharType

public:
	iAimmsA_UTF8_313asc(const char *szFileName)
		: m_Library(szFileName)
		, INIT_FUNCTION_POINTERS
	{}
	virtual ~iAimmsA_UTF8_313asc() {}

	DECLARE_ALL_NONCHAR_METHODS

	DECLARE_ALL_VALUE_N_METHODS

	DECLARE_ALL_STRING_METHODS

	virtual int APILastError( int *code, CharType *message, int messageSize )
	{ 
		GET_FCN(FCN_APILastError, DECORATED_DLL_NAME_T(AimmsAPILastError,8), int, (int*, DllCharType*) ) 

		static const size_t bufSize = 4096;
		DllCharType szBuf[bufSize];
		iAimmsStdString msg;
		int ret = fcn( code, szBuf );
		m_ConvertFromAimms.convert( szBuf, msg);
		if (messageSize == -1) messageSize = (int)msg.length()+1;
		strncpy( message,  msg.c_str(), messageSize-1);
		return ret;
	}
	
	virtual bool AimmsIsUnicode() 
	{
		return false;
	}

	virtual wchar_t *ConvertToAimms(wchar_t *outBuf, size_t outBufSize, const CharType *inBuf) 
	{
		throw std::runtime_error("ConvertToAimms type mismatch with underlying Aimms");
	}
	virtual char *ConvertToAimms(char *outBuf, size_t outBufSize, const CharType *inBuf)
	{
		if (!outBufSize || !outBuf || !inBuf) return 0;
		std::string tmp;
		m_ConvertToAimms.convert(inBuf,tmp);
		strncpy(outBuf,tmp.c_str(),outBufSize-1);
		outBuf[outBufSize-1] = 0;
		return outBuf;
	}
	virtual CharType *ConvertFromAimms(CharType *outBuf, size_t outBufSize, const wchar_t *inBuf) 
	{
		throw std::runtime_error("ConvertToAimms type mismatch with underlying Aimms");
	}
	virtual CharType *ConvertFromAimms(CharType *outBuf, size_t outBufSize, const char *inBuf) 
	{
		if (!outBufSize || !outBuf || !inBuf) return 0;
		std::string tmp;
		m_ConvertFromAimms.convert(inBuf,tmp);
		strncpy(outBuf,tmp.c_str(),outBufSize-1);
		return outBuf;
	}

private:
	// Helper function for ErrorMessage / ErrorCode / ErrorCategory
	const CharType *CallFunctionAndReturnString1Arg( const DllCharType* (WINAPI *fcn)(int), int n )
	{
		static iAimmsStdString result;
		const DllCharType* x = fcn(n); 
		m_ConvertFromAimms.convert( x, result);
        return result.c_str();
	}
	const CharType *CallFunctionAndReturnString2Arg( const DllCharType* (WINAPI *fcn)(int,int), int n1, int n2 )
	{
		static iAimmsStdString result;
		const DllCharType* x = fcn(n1,n2); 
		m_ConvertFromAimms.convert( x, result);
        return result.c_str();
	}
};

iAimmsA *CreateiAimmsA_UTF8_313asc(const char *dllname)
{
	return new iAimmsA_UTF8_313asc(dllname);
}

} // end namespace
