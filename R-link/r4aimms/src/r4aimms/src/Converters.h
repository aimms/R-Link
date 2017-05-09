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
#ifndef _INCLUDED_aimmsifc_Converters
#define _INCLUDED_aimmsifc_Converters

#include <string>
#include <vector>

#include "Common/CharSetConverter.h"

namespace aimmsifc {

template <class _char_in_t, class _char_out_t>
class BaseConverter{
public:
    typedef _char_in_t		char_in_t;
    typedef _char_out_t		char_out_t;

    typedef std::basic_string<char_in_t>    string_in_t;
    typedef std::basic_string<char_out_t>   string_out_t;
    typedef std::char_traits<char_in_t>     char_in_traits;
    typedef std::char_traits<char_out_t>	char_out_traits;
public:
	BaseConverter(){
	};

	BaseConverter( const char* from, const char* to){
		m_Converter.open(from, to);
	};

	~BaseConverter(){
	}

    void convert( const char_in_t* src, size_t nMaxInputLen, string_out_t& dest)
    {
        dest.resize( nMaxInputLen );
        if (nMaxInputLen == 0 ) return;

        // the input descriptors
        const char* pByteInput = reinterpret_cast<const char*>(src);
        size_t nBytesInput = nMaxInputLen*sizeof(char_in_t);

        // the output descriptors
        char* pByteOutput = (char*) &dest[0];
        size_t nBytesOutput = dest.size()*sizeof(char_out_t);

        // variables used in loop/switch
		size_t nCharsOutput = dest.size();
        size_t nCurrentPos = 0;
        Common::CharSetConverter::ConversionResult cr = Common::CharSetConverter::Done;

        while (nBytesInput > 0){
            cr = m_Converter.convert( pByteInput, nBytesInput, pByteOutput, nBytesOutput );
            switch (cr){
            case Common::CharSetConverter::Done:
                // we're done
                break;

			case Common::CharSetConverter::Overflow:
                // allocate more memory and continue
                nCharsOutput = nBytesOutput / sizeof(char_out_t);
                // assert we're working with whole wchar's
                assert( (nCharsOutput * sizeof(char_out_t)) == nBytesOutput );
                // remember current post
                nCurrentPos = dest.size() - nCharsOutput;
                // resize to twice the size
                dest.resize(dest.size()*2);
                // recompute the offset & lengths
                pByteOutput = (char*) &dest[nCurrentPos];
                nCharsOutput = dest.size() - nCurrentPos;
                nBytesOutput = nCharsOutput * sizeof(char_out_t);
                break;

			case Common::CharSetConverter::Underflow:
                // there is no more input, but this function is assumed to
                // receive a well-formed input string, throw error
				throw std::runtime_error("malformed character");
                break;
            default:
                throw std::runtime_error("unexpected conversion result");
                break;
            }
        }
        // flush the potentially remaining output bytes
        do{
            cr = m_Converter.flush(pByteOutput, nBytesOutput);
            switch (cr){
			case Common::CharSetConverter::Done:
                break;

            case Common::CharSetConverter::Overflow:
                // allocate more memory and continue
                nCharsOutput = nBytesOutput / sizeof(char_out_t);
                // assert we're working with whole wchar's
                assert( (nCharsOutput * sizeof(char_out_t)) == nBytesOutput );
                // remember current post
                nCurrentPos = dest.size() - nCharsOutput;
                // resize to twice the size
                dest.resize(dest.size()*2);
                // recompute the offset & lengths
                pByteOutput = (char*) &dest[nCurrentPos];
                nCharsOutput = dest.size() - nCurrentPos;
                nBytesOutput = nCharsOutput * sizeof(char_out_t);
				break;

			case Common::CharSetConverter::Underflow:
                // fall through, since there is no more input (guaranteed OK)
                // we should not receive this error here
            default:
                throw std::runtime_error("unexpected conversion result");
                break;
            }
        } while(cr != Common::CharSetConverter::Done);

        // clamp the remaining bytes from the output buffer
        if (nBytesOutput > 0){
            nCharsOutput = nBytesOutput / sizeof(char_out_t);
            // assert we're working with whole wchar's
            assert( (nCharsOutput * sizeof(char_out_t)) == nBytesOutput );

            dest.resize( dest.size() - nCharsOutput);
		}
    }

    void convert( const char_in_t* src, string_out_t& dest)
	{
		this->convert(src, char_in_traits::length(src), dest);
	}
protected:
	Common::CharSetConverter m_Converter;
};


#ifdef _TARGET_PLATFORM_WINDOWS
	#define UNI_ENCODING "UTF16LE"
#endif
#ifdef _TARGET_PLATFORM_LINUX
	#define UNI_ENCODING "UTF32LE"
#endif

class utf82uni_t : public BaseConverter<char, wchar_t>{
public:
	utf82uni_t() : BaseConverter<char, wchar_t>("UTF8", UNI_ENCODING){}
};

class uni2utf8_t : public BaseConverter<wchar_t, char>{
public:
	uni2utf8_t() : BaseConverter<wchar_t, char>(UNI_ENCODING, "UTF8"){}
};

class utf82ccp_t : public BaseConverter<char, char>{
public:
	utf82ccp_t() : BaseConverter<char, char>()
    {
        std::string currentLocale;
        m_Converter.getCurrentLocaleCharSet(currentLocale);
        m_Converter.open("UTF8", currentLocale.c_str());
    }
};

class ccp2utf8_t : public BaseConverter<char, char>{
public:
    ccp2utf8_t() : BaseConverter<char, char>()
    {
        std::string currentLocale;
        m_Converter.getCurrentLocaleCharSet(currentLocale);
        m_Converter.open(currentLocale.c_str(), "UTF8");
    }
};

class uni2ccp_t : public BaseConverter<wchar_t, char>{
public:
	uni2ccp_t() : BaseConverter<wchar_t, char>()
    {
        std::string currentLocale;
        m_Converter.getCurrentLocaleCharSet(currentLocale);
        m_Converter.open(UNI_ENCODING, currentLocale.c_str());
    }
};

class ccp2uni_t : public BaseConverter<char, wchar_t>{
public:
    ccp2uni_t() : BaseConverter<char, wchar_t>()
    {
        std::string currentLocale;
        m_Converter.getCurrentLocaleCharSet(currentLocale);
        m_Converter.open(currentLocale.c_str(), UNI_ENCODING);
    }
};


} // end namespace aimmsifc

#endif // end #ifndef _INCLUDED_aimmsifc_Converters
