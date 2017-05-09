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
// An ASCII interface to a 3.13 (or earlier) ASCII libaimms
#define _CRT_SECURE_NO_WARNINGS
#include "iAimmsFactory.h"
#include <aimmsifc/iAimms.h>
#include <Common/DynamicLib.h>
#include <exception>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include "ImplHelpers.h"

// NO CONVERSIONS NECESSARY
// input C string
#define inCS( in )		in
// input AIMMS string
#define inAS( in )		in
// input AIMMS value
#define inV( in )		in
// output AIMMS string
#define outAS( out )	out
// output AIMMS string
#define outV( out )		out
// output AIMMS Value array
#define outVA( n, out )	out
// input ValueArray
#define inVA( n, in )	in


// AimmsAPI functions usings string do not have a 'W' or 'A' postfix in 3.13 or earlier, so we use DECORATED_DLL_NAME
#define DECORATED_DLL_NAME_T DECORATED_DLL_NAME

namespace aimmsifc {


class iAimmsW_uni_313uni : public iAimmsW, public iFactoryManage
{
	Common::DynamicLib m_Library;
	DECLARE_FUNCTION_POINTERS;

	// the types are they are defined in the underlying AimmsAPI
	typedef AimmsValueType<wchar_t> DllValueType;
	typedef AimmsStringType<wchar_t> DllStringType;
	typedef wchar_t DllCharType;

public:
	iAimmsW_uni_313uni(const char *szFileName)
		: m_Library(szFileName)
		, INIT_FUNCTION_POINTERS
	{}
	virtual ~iAimmsW_uni_313uni() {}

	DECLARE_ALL_NONCHAR_METHODS

	DECLARE_ALL_VALUE_N_METHODS

	DECLARE_ALL_STRING_METHODS

	virtual int APILastError( int *code, CharType *message, int messageSize )
	{ 
		GET_FCN(FCN_APILastError, DECORATED_DLL_NAME_T(AimmsAPILastError,8), int, (int*, DllCharType*) ) 
		if (messageSize==-1) {
			return fcn(code,message);
		} else {
			CharType buf[4096];
			int rval = fcn(code,buf);
			wcsncpy(message,buf,messageSize-1);
			return rval;
		}
	}

	virtual bool AimmsIsUnicode() 
	{
		return true;
	}

	virtual wchar_t *ConvertToAimms(wchar_t *outBuf, size_t outBufSize, const CharType *inBuf) 
	{
		if (!outBufSize || !outBuf || !inBuf) return 0;
		wcsncpy(outBuf,inBuf,outBufSize-1);
		return outBuf;
	}
	virtual char *ConvertToAimms(char *outBuf, size_t outBufSize, const CharType *inBuf)
	{
		throw std::runtime_error("ConvertToAimms type mismatch with underlying Aimms");
	}
	virtual CharType *ConvertFromAimms(CharType *outBuf, size_t outBufSize, const wchar_t *inBuf) 
	{
		return ConvertToAimms(outBuf,outBufSize,inBuf);
	}
	virtual CharType *ConvertFromAimms(CharType *outBuf, size_t outBufSize, const char *inBuf) 
	{
		throw std::runtime_error("ConvertToAimms type mismatch with underlying Aimms");
	}

private:
	// Helper function for ErrorMessage / ErrorCode / ErrorCategory
	const CharType *CallFunctionAndReturnString1Arg( const DllCharType* (WINAPI *fcn)(int), int n )
	{
        return fcn(n);
	}
	const CharType *CallFunctionAndReturnString2Arg( const DllCharType* (WINAPI *fcn)(int,int), int n1, int n2 )
	{
		return fcn(n1,n2);
	}


};

iAimmsW *CreateiAimmsW_uni_313uni(const char *dllname)
{
	return new iAimmsW_uni_313uni(dllname);
}

} // end namespace
