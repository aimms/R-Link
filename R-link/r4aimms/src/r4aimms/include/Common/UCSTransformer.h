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
#ifndef _INCLUDED__Common_UCSTransformer
#define _INCLUDED__Common_UCSTransformer

#include "Common/CharSetConverter.h"
#include <string.h> // strlen

namespace Common {

    class  UCSTransformer{
    public:
        typedef CharSetConverter::ConversionResult  ConversionResult;
    public:
        UCSTransformer();
        ~UCSTransformer();

        bool open( const char* szTarget, double fAvgBytesPerCharacter = 1.0 );
        bool open( const wchar_t* wszTarget, double fAvgBytesPerCharacter = 1.0 );
        void close();

        // Core functionality: Decoding from a characterset to UCS

        // decode the specified input (using the active charset) into a wide-character buffer
        // returns the number of characters written to the buffer if successfull, 0 otherwise 
        size_t decodeFrom( const char* buffer, size_t maxLen, wchar_t* wszDest, size_t maxDestLen);

        // decode the specified input (using the active charset) into a wide-character string
        void decodeFrom( const char* buffer, size_t maxLen, std::wstring& wstrDest);

        // Core functionality: Encoding from UCS to a characterset
        
        // encode the specified source to a byte buffer (without a terminating zero)
        // returns the number of bytes written to buffer if successfull, 0 otherwise 
        size_t encodeTo( const wchar_t* wszSrc, size_t wszLen, char* buffer, size_t maxLen );

        // encode the specified source to a std::string (this assumes a compatible target charset)
        void encodeTo( const wchar_t* wszSrc, size_t wszLen, std::string& dest );




        // convenience decoding methods
        inline size_t decodeFrom( const char* szSrc, wchar_t* wszDest, size_t maxDestLen){
            size_t ret = decodeFrom( szSrc, strlen(szSrc), wszDest, maxDestLen - 1 );
            if (ret > 0 ){
                wszDest[ret] = 0;
                return ret;
            }
            return 0;
        }

        inline size_t decodeFrom( const std::string& strSrc, wchar_t* wszDest, size_t maxDestLen ){
            size_t ret = decodeFrom( strSrc.c_str(), strSrc.length(), wszDest, maxDestLen - 1 );
            if (ret > 0 ){
                wszDest[ret] = 0;
                return ret;
            }
            return 0;
        }

        inline void decodeFrom( const char* szSrc, std::wstring& dest ){
            decodeFrom( szSrc, strlen(szSrc), dest );
        }

        inline void decodeFrom( const std::string& strSrc, std::wstring& dest ){
            decodeFrom( strSrc.c_str(), strSrc.length(), dest );
        }

        // convenience encoding methods
        inline size_t encodeTo( const wchar_t* wszSrc, char* buffer, size_t maxLen ){
            size_t ret = encodeTo( wszSrc, wcslen(wszSrc), buffer, maxLen - 1 );
            if (ret > 0){
                buffer[ret] = 0;
                return ret;
            }
            return 0;
        }

        inline size_t encodeTo( const std::wstring& wstrSrc, char* buffer, size_t maxLen  ){
            size_t ret = encodeTo( wstrSrc.c_str(), wstrSrc.length(), buffer, maxLen - 1);
            if (ret > 0 ){
                buffer[ret] = 0;
                return ret;
            }
            return 0;
        }

        inline void encodeTo( const wchar_t* wszSrc, std::string& dest ){
            return encodeTo( wszSrc, wcslen(wszSrc), dest );
        }

        inline void encodeTo( const std::wstring& wstrSrc, std::string& dest ){
            return encodeTo( wstrSrc.c_str(), wstrSrc.length(), dest );
        }

        // convenience streaming methods
        inline ConversionResult streamTo( const wchar_t*& pInput, size_t& nCharactersInput, char*& pOutput, size_t& nBytesOutput ){
            const char* pByteInput = reinterpret_cast<const char*>(pInput);
            size_t nBytesInput = sizeof(wchar_t)*nCharactersInput;

            ConversionResult cr = m_Encoder.convert( pByteInput, nBytesInput, pOutput, nBytesOutput );

            nCharactersInput = nBytesInput / sizeof(wchar_t);
            assert(nCharactersInput*sizeof(wchar_t) == nBytesInput );
            pInput = reinterpret_cast<const wchar_t*>(pByteInput);
            return cr;
        }

        inline ConversionResult streamFrom( const char*& pInput, size_t& nBytesInput, wchar_t*& pOutput, size_t& nCharactersOutput ){
            char* pByteOutput = reinterpret_cast<char*>(pOutput);
            size_t nBytesOutput = sizeof(wchar_t)*nCharactersOutput;

            ConversionResult cr = m_Decoder.convert( pInput, nBytesInput, pByteOutput, nBytesOutput );

            nCharactersOutput = nBytesOutput / sizeof(wchar_t);
            assert(nCharactersOutput*sizeof(wchar_t) == nBytesOutput );
            pOutput = reinterpret_cast<wchar_t*>(pByteOutput);
            return cr;
        }
    private:
        CharSetConverter m_Encoder;
        CharSetConverter m_Decoder;
        double m_fAvgBytesPerCharacter;
    };
};

#endif // end #ifndef _INCLUDED__Common_UCSTransformer
