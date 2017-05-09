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
#include "Common/CharSetConverter.h"
#include "iconv/iconv.h"
#include <errno.h>

#include <stdlib.h>
#include <locale.h>
#include <string.h> // strerror


#ifdef _HOST_COMPILER_GCC
#include <langinfo.h>
#endif


#ifdef _HOST_COMPILER_MICROSOFT
#include <windows.h>

#include <algorithm>

namespace {
    struct CodePageToIconv{
        int codePage;
        const char* szIconvName;
        const char* szDescription;
    };

    static const CodePageToIconv g_codePageToIconv[] = {
        {37,  "IBM037",	"IBM EBCDIC US-Canada"},
        {437, "IBM437", "OEM United States"},
        {500, "IBM500", "IBM EBCDIC International"},
        {708, "ASMO-708", "Arabic (ASMO 708)"},
        {709, "ASMO_449", "Arabic (ASMO-449+, BCON V4)"},
        {710, "ARABIC", "Arabic - Transparent Arabic"},
        {720, "CP720", "Arabic (Transparent ASMO); Arabic (DOS)"},
        {737, "CP737", "OEM Greek (formerly 437G); Greek (DOS)"},
        {775, "IBM775", "OEM Baltic; Baltic (DOS)"},
        {850, "IBM850", "OEM Multilingual Latin 1; Western European (DOS)"},
        {852, "IBM852", "OEM Latin 2; Central European (DOS)"},
        {855, "IBM855", "OEM Cyrillic (primarily Russian)"},
        {857, "IBM857", "OEM Turkish; Turkish (DOS)"},
        {858, "CP858", "OEM Multilingual Latin 1 + Euro symbol"},
        {860, "IBM860", "OEM Portuguese; Portuguese (DOS)"},
        {861, "IBM861", "OEM Icelandic; Icelandic (DOS)"},
        {862, "IBM862", "OEM Hebrew; Hebrew (DOS)"},
        {863, "IBM863", "OEM French Canadian; French Canadian (DOS)"},
        {864, "IBM864", "OEM Arabic; Arabic (864)"},
        {865, "IBM865", "OEM Nordic; Nordic (DOS)"},
        {866, "IBM866", "OEM Russian; Cyrillic (DOS)"},
        {869, "IBM869", "OEM Modern Greek; Greek, Modern (DOS)"},
        {870, "IBM870", "IBM EBCDIC Multilingual/ROECE (Latin 2); IBM EBCDIC Multilingual Latin 2"},
        {874, "WINDOWS-874", "ANSI/OEM Thai (same as 28605, ISO 8859-15); Thai (Windows)"},
        {875, "CP875", "IBM EBCDIC Greek Modern"},
        {932, "CP932", "ANSI/OEM Japanese; Japanese (Shift-JIS)"},
        {936, "GB2312", "ANSI/OEM Simplified Chinese (PRC, Singapore); Chinese Simplified (GB2312)"},
        {949, "CP949", "ANSI/OEM Korean (Unified Hangul Code)"},
        {950, "BIG5", "ANSI/OEM Traditional Chinese (Taiwan; Hong Kong SAR, PRC); Chinese Traditional (Big5)"},
        {1026, "IBM1026", "IBM EBCDIC Turkish (Latin 5)"},
        {1047, "IBM1047", "IBM EBCDIC Latin 1/Open System"},
        {1140, "IBM1140", "IBM EBCDIC US-Canada (037 + Euro symbol); IBM EBCDIC (US-Canada-Euro)"},
        {1141, "IBM1141", "IBM EBCDIC Germany (20273 + Euro symbol); IBM EBCDIC (Germany-Euro)"},
        {1142, "IBM1142", "IBM EBCDIC Denmark-Norway (20277 + Euro symbol); IBM EBCDIC (Denmark-Norway-Euro)"},
        {1143, "IBM1143", "IBM EBCDIC Finland-Sweden (20278 + Euro symbol); IBM EBCDIC (Finland-Sweden-Euro)"},
        {1144, "IBM1144", "IBM EBCDIC Italy (20280 + Euro symbol); IBM EBCDIC (Italy-Euro)"},
        {1145, "IBM1145", "IBM EBCDIC Latin America-Spain (20284 + Euro symbol); IBM EBCDIC (Spain-Euro)"},
        {1146, "IBM1146", "IBM EBCDIC United Kingdom (20285 + Euro symbol); IBM EBCDIC (UK-Euro)"},
        {1147, "IBM1147", "IBM EBCDIC France (20297 + Euro symbol); IBM EBCDIC (France-Euro)"},
        {1148, "IBM1148", "IBM EBCDIC International (500 + Euro symbol); IBM EBCDIC (International-Euro)"},
        {1149, "IBM1149", "IBM EBCDIC Icelandic (20871 + Euro symbol); IBM EBCDIC (Icelandic-Euro)"},
        {1200, "UTF-16LE", "Unicode UTF-16, little endian byte order (BMP of ISO 10646); available only to managed applications"},
        {1201, "UTF-16BE", "Unicode UTF-16, big endian byte order; available only to managed applications"},
        {1250, "WINDOWS-1250", "ANSI Central European; Central European (Windows)"},
        {1251, "WINDOWS-1251", "ANSI Cyrillic; Cyrillic (Windows)"},
        {1252, "WINDOWS-1252", "ANSI Latin 1; Western European (Windows)"},
        {1253, "WINDOWS-1253", "ANSI Greek; Greek (Windows)"},
        {1254, "WINDOWS-1254", "ANSI Turkish; Turkish (Windows)"},
        {1255, "WINDOWS-1255", "ANSI Hebrew; Hebrew (Windows)"},
        {1256, "WINDOWS-1256", "ANSI Arabic; Arabic (Windows)"},
        {1257, "WINDOWS-1257", "ANSI Baltic; Baltic (Windows)"},
        {1258, "WINDOWS-1258", "ANSI/OEM Vietnamese; Vietnamese (Windows)"},
        {1361, "JOHAB", "Korean (Johab)"},
        {10000, "MAC", "MAC Roman; Western European (Mac)"},
        {10001, "OSF00010001", "Japanese (Mac)"},
        {10002, "OSF00010002", "MAC Traditional Chinese (Big5); Chinese Traditional (Mac)"},
        {10003, "OSF00010003", "Korean (Mac)"},
        {10004, "OSF00010004", "Arabic (Mac)"},
        {10005, "OSF00010005", "Hebrew (Mac)"},
        {10006, "OSF00010006", "Greek (Mac)"},
        {10007, "OSF00010007", "Cyrillic (Mac)"},
        {10008, "OSF00010008", "MAC Simplified Chinese (GB 2312); Chinese Simplified (Mac)"},
        {10010, "OSF00010010", "Romanian (Mac)"},
        {10017, "OSF00010017", "Ukrainian (Mac)"},
        {10021, "OSF00010021", "Thai (Mac)"},
        {10029, "OSF00010029", "MAC Latin 2; Central European (Mac)"},
        {10079, "OSF00010079", "Icelandic (Mac)"},
        {10081, "OSF00010081", "Turkish (Mac)"},
        {10082, "OSF00010082", "Croatian (Mac)"},
        {12000, "UTF-32LE", "Unicode UTF-32, little endian byte order; available only to managed applications"},
        {12001, "UTF-32BE", "Unicode UTF-32, big endian byte order; available only to managed applications"},
        {20000, "CP20000", "CNS Taiwan; Chinese Traditional (CNS)"},
        {20001, "CP20001", "TCA Taiwan"},
        {20002, "CP20002", "Eten Taiwan; Chinese Traditional (Eten)"},
        {20003, "CP20003", "IBM5550 Taiwan"},
        {20004, "CP20004", "TeleText Taiwan"},
        {20005, "CP20005", "Wang Taiwan"},
        {20105, "CP20105", "IA5 (IRV International Alphabet No. 5, 7-bit); Western European (IA5)"},
        {20106, "CP20106", "IA5 German (7-bit)"},
        {20107, "CP20107", "IA5 Swedish (7-bit)"},
        {20108, "CP20108", "IA5 Norwegian (7-bit)"},
        {20127, "US-ASCII", "US-ASCII (7-bit)"},
        {20261, "T.61", "T.61"},
        {20269, "ISO_6937", "ISO 6937 Non-Spacing Accent"},
        {20273, "IBM273", "IBM EBCDIC Germany"},
        {20277, "IBM277", "IBM EBCDIC Denmark-Norway"},
        {20278, "IBM278", "IBM EBCDIC Finland-Sweden"},
        {20280, "IBM280", "IBM EBCDIC Italy"},
        {20284, "IBM284", "IBM EBCDIC Latin America-Spain"},
        {20285, "IBM285", "IBM EBCDIC United Kingdom"},
        {20290, "IBM290", "IBM EBCDIC Japanese Katakana Extended"},
        {20297, "IBM297", "IBM EBCDIC France"},
        {20420, "IBM420", "IBM EBCDIC Arabic"},
        {20423, "IBM423", "IBM EBCDIC Greek"},
        {20424, "IBM424", "IBM EBCDIC Hebrew"},
        {20833, "CP20833", "IBM EBCDIC Korean Extended"},
        {20838, "CP20838", "IBM EBCDIC Thai"},
        {20866, "KOI8-R", "Russian (KOI8-R); Cyrillic (KOI8-R)"},
        {20871, "IBM871", "IBM EBCDIC Icelandic"},
        {20880, "IBM880", "IBM EBCDIC Cyrillic Russian"},
        {20905, "IBM905", "IBM EBCDIC Turkish"},
        {20924, "IBM00924", "IBM EBCDIC Latin 1/Open System (1047 + Euro symbol)"},
        {20932, "EUC-JP", "Japanese (JIS 0208-1990 and 0121-1990)"},
        {20936, "GB2312", "Simplified Chinese (GB2312); Chinese Simplified (GB2312-80)"},
        {20949, "CP20949", "Korean Wansung"},
        {21025, "CP1025", "IBM EBCDIC Cyrillic Serbian-Bulgarian"},
        {21866, "KOI8-U", "Ukrainian (KOI8-U); Cyrillic (KOI8-U)"},
        {28591, "ISO-8859-1", "ISO 8859-1 Latin 1; Western European (ISO)"},
        {28592, "ISO-8859-2", "ISO 8859-2 Central European; Central European (ISO)"},
        {28593, "ISO-8859-3", "ISO 8859-3 Latin 3"},
        {28594, "ISO-8859-4", "ISO 8859-4 Baltic"},
        {28595, "ISO-8859-5", "ISO 8859-5 Cyrillic"},
        {28596, "ISO-8859-6", "ISO 8859-6 Arabic"},
        {28597, "ISO-8859-7", "ISO 8859-7 Greek"},
        {28598, "ISO-IR-138", "ISO 8859-8 Hebrew; Hebrew (ISO-Visual)"},
        {28599, "ISO-8859-9", "ISO 8859-9 Turkish"},
        {28603, "ISO-8859-13", "ISO 8859-13 Estonian"},
        {28605, "ISO-8859-15", "ISO 8859-15 Latin 9"},
        {29001, "CP29001", "Europa 3"},
        {38598, "ISO-8859-8:1998", "ISO 8859-8 Hebrew; Hebrew (ISO-Logical)"},
        {50220, "ISO-2022-JP-2", "ISO 2022 Japanese with no halfwidth Katakana; Japanese (JIS)"},
        {50221, "ISO-2022-JP-3", "ISO 2022 Japanese with halfwidth Katakana; Japanese (JIS-Allow 1 byte Kana)"},
        {50222, "ISO-2022-JP", "ISO 2022 Japanese JIS X 0201-1989; Japanese (JIS-Allow 1 byte Kana - SO/SI)"},
        {50225, "ISO-2022-KR", "ISO 2022 Korean"},
        {50227, "ISO-2022-CN", "ISO 2022 Simplified Chinese; Chinese Simplified (ISO 2022)"},
        {50229, "ISO-2022-CN", "ISO 2022 Traditional Chinese"},
        {50930, "CP50930", "EBCDIC Japanese (Katakana) Extended"},
        {50931, "CP50931", "EBCDIC US-Canada and Japanese"},
        {50933, "CP50933", "EBCDIC Korean Extended and Korean"},
        {50935, "CP50935", "EBCDIC Simplified Chinese Extended and Simplified Chinese"},
        {50936, "CP50936", "EBCDIC Simplified Chinese"},
        {50937, "CP50937", "EBCDIC US-Canada and Traditional Chinese"},
        {50939, "CP50939", "EBCDIC Japanese (Latin) Extended and Japanese"},
        {51932, "EUC-JP", "EUC Japanese"},
        {51936, "EUC-CN", "EUC Simplified Chinese; Chinese Simplified (EUC)"},
        {51949, "EUC-KR", "EUC Korean"},
        {51950, "CP51950", "EUC Traditional Chinese"},
        {52936, "GB2312", "HZ-GB2312 Simplified Chinese; Chinese Simplified (HZ)"},
        {54936, "GB18030", "Windows XP and later: GB18030 Simplified Chinese (4 byte); Chinese Simplified (GB18030)"},
        {57002, "CP57002", "ISCII Devanagari"},
        {57003, "CP57003", "ISCII Bengali"},
        {57004, "CP57004", "ISCII Tamil"},
        {57005, "CP57005", "ISCII Telugu"},
        {57006, "CP57006", "ISCII Assamese"},
        {57007, "CP57007", "ISCII Oriya"},
        {57008, "CP57008", "ISCII Kannada"},
        {57009, "CP57009", "ISCII Malayalam"},
        {57010, "CP57010", "ISCII Gujarati"},
        {57011, "CP57011", "ISCII Punjabi"},
        {65000, "UTF7", "Unicode (UTF-7)"},
        {65001, "UTF8", "Unicode (UTF-8)"}
    };
    static const size_t g_nCodePageToIconv = sizeof(g_codePageToIconv)/sizeof(g_codePageToIconv[0]);


    bool CodePageToIconvLess ( const CodePageToIconv& elem1, const CodePageToIconv& elem2 )
    {
        return elem1.codePage < elem2.codePage;
    }

    const char* getCodePageName( UINT cp ){
        CodePageToIconv cpSearch;
        cpSearch.codePage = cp;
        const CodePageToIconv* pFound = std::lower_bound(&g_codePageToIconv[0], &g_codePageToIconv[g_nCodePageToIconv], cpSearch, CodePageToIconvLess );
        if (pFound !=  &g_codePageToIconv[g_nCodePageToIconv] && pFound->codePage == cp){
            return pFound->szIconvName;
        }
        return 0;
    }

}
#endif


namespace Common {
    CharSetConverter::Exception::Exception(const char* szMessage) : ::std::runtime_error(szMessage)
    {
    }

    CharSetConverter::Exception::~Exception() throw()
    {
    }

    CharSetConverter::UnmappableCharacterException::UnmappableCharacterException() : Exception("character cannot be represented in the output charset")
    {
    }

    CharSetConverter::UnmappableCharacterException::~UnmappableCharacterException() throw()
    {
    }

    CharSetConverter::MalformedCharacterException::MalformedCharacterException() : Exception("input bytes are invalid according to the input charset")
    {
    }

    CharSetConverter::MalformedCharacterException::~MalformedCharacterException() throw()
    {
    }

    CharSetConverter::SystemException::SystemException(int errNo) : Exception(strerror(errNo)), m_ErrNo(errNo)
    {
    }

    CharSetConverter::SystemException::~SystemException() throw()
    {
    }

    int CharSetConverter::SystemException::errNo() const
    {
        return m_ErrNo;
    }

    CharSetConverter::CharSetConverter() : m_Handle(0)
    {
    }


    CharSetConverter::~CharSetConverter()
    {
        close();
    }

    bool CharSetConverter::open(const char* szFrom, const char* szTo)
    {
        close();

        iconv_t cd = iconv_open(szTo, szFrom);
        if (cd == ((iconv_t) -1)){
            return false;
        }
        m_Handle = cd;
        return true;
    }
    namespace{
        std::string toMbs( const wchar_t* szSrc){
            std::string strDest;
            strDest.resize(wcslen(szSrc));
            size_t converted = wcstombs( &strDest[0], szSrc, strDest.size() );
            if (converted != strDest.size()) {
                throw CharSetConverter::SystemException(errno);
            }
            return strDest;
        }
    }

    bool CharSetConverter::open(const wchar_t* szFrom, const wchar_t* szTo)
    {
        std::string strFrom = toMbs(szFrom);
        std::string strTo = toMbs(szTo);
        return open(strFrom.c_str(), strTo.c_str());
    }

    void CharSetConverter::close()
    {
        if (m_Handle){
            iconv_t cd = m_Handle;
            m_Handle = 0;
            iconv_close(cd);
        }
    }

    CharSetConverter::ConversionResult CharSetConverter::convert( const char*& pInput, size_t& nInputBytesLeft, char*& pOutput, size_t& nOutputBytesLeft )
    {
        size_t result = iconv( m_Handle, const_cast<char**>(&pInput), &nInputBytesLeft, &pOutput, &nOutputBytesLeft);
        if (result == -1){
            switch (errno){
            case EILSEQ:
                throw UnmappableCharacterException();
                break;
            case E2BIG:
                return Overflow;
                break;
            case EINVAL:
                throw MalformedCharacterException();
                break;
            default:
                throw SystemException(errno);
                break;
            }
        }
        if (nInputBytesLeft > 0){
            return Underflow;
        }
        return Done;
    }

    CharSetConverter::ConversionResult CharSetConverter::flush(char*& pOutput, size_t& nOutputBytesLeft)
    {
        size_t result = iconv( m_Handle, 0, 0, &pOutput, &nOutputBytesLeft);
        if (result == -1){
            switch (errno){
            case EILSEQ:
                throw UnmappableCharacterException();
                break;
            case E2BIG:
                return Overflow;
                break;
            case EINVAL:
                throw MalformedCharacterException();
                break;
            default:
                throw SystemException(errno);
                break;
            }
        }
        return Done;
    }

    CharSetConverter::ConversionResult CharSetConverter::reset()
    {
        size_t result = iconv( m_Handle, 0, 0, 0, 0);
        if (result == -1){
            switch (errno){
            case EILSEQ:
                throw UnmappableCharacterException();
                break;
            case E2BIG:
                return Overflow;
                break;
            case EINVAL:
                throw MalformedCharacterException();
                break;
            default:
                throw SystemException(errno);
                break;
            }
        }
        return Done;
    }

    void CharSetConverter::convert( const std::string& strInput, std::string& strOutput, double fExpectedBytesPerOutputChar )
    {
        size_t expectedByteLen = (size_t)(fExpectedBytesPerOutputChar * strInput.length());
        strOutput.resize(expectedByteLen);
        if (strInput.length() == 0) return;

        // the input descriptors
        const char* pByteInput = &strInput[0];
        size_t nBytesInput = strInput.size();

        // the output descriptors
        char* pByteOutput = (char*) &strOutput[0];
        size_t nBytesOutput = strOutput.size();

        // variables used in loop/switch
        size_t nCurrentPos = 0;
        ConversionResult cr = Done;

        while (nBytesInput > 0){
            cr = this->convert( pByteInput, nBytesInput, pByteOutput, nBytesOutput );
            switch (cr){
            case Done:
                // we're done
                break;

            case Overflow:
                // allocate more memory and continue
                nCurrentPos = strOutput.size() - nBytesOutput;
                strOutput.resize(strOutput.size()*2);
                pByteOutput = (char*) &strOutput[nCurrentPos];
                nBytesOutput = strOutput.size()-nCurrentPos;
                break;

            case Underflow:
                // there is no more input, but this function is assumed to
                // receive a well-formed input string, throw error
                throw CharSetConverter::MalformedCharacterException();
                break;
            default:
                throw std::runtime_error("unexpected conversion result");
                break;
            }
        }
        // flush the potentially remaining output bytes
        do{
            cr = this->flush(pByteOutput, nBytesOutput);
            switch (cr){
            case Done:
                break;

            case Overflow:
                // allocate more memory and continue
                nCurrentPos = strOutput.size() - nBytesOutput;
                strOutput.resize(strOutput.size()*2);
                pByteOutput = (char*) &strOutput[nCurrentPos];
                nBytesOutput = strOutput.size()-nCurrentPos;
                break;

            case Underflow:
                // fall through, since there is no more input (guaranteed OK)
                // we should not receive this error here
            default:
                throw std::runtime_error("unexpected conversion result");
                break;
            }
        } while(cr != Done);

        // clamp the remaining bytes from the output buffer
        if (nBytesOutput > 0){
            strOutput.resize( strOutput.size() - nBytesOutput);
        }
        this->reset();
    }

    void CharSetConverter::convert( const std::wstring& strInput, std::string& dest, double fExpectedBytesPerOutputChar)
    {
        size_t expectedByteLen = (size_t)(fExpectedBytesPerOutputChar * strInput.length());
        dest.resize(expectedByteLen);
        if (strInput.length() == 0) return;

        // the input descriptors
        const char* pByteInput = reinterpret_cast<const char*>(strInput.c_str());
        size_t nBytesInput = strInput.length()*sizeof(wchar_t);

        // the output descriptors
        char* pByteOutput = (char*) &dest[0];
        size_t nBytesOutput = dest.size();

        // variables used in loop/switch
        size_t nCurrentPos = 0;
        ConversionResult cr = CharSetConverter::Done;

        while (nBytesInput > 0){
            cr = this->convert( pByteInput, nBytesInput, pByteOutput, nBytesOutput );
            switch (cr){
            case CharSetConverter::Done:
                // we're done
                break;

            case CharSetConverter::Overflow:
                // allocate more memory and continue
                nCurrentPos = dest.size() - nBytesOutput;
                dest.resize(dest.size()*2);
                pByteOutput = (char*) &dest[nCurrentPos];
                nBytesOutput = dest.size()-nCurrentPos;
                break;

            case CharSetConverter::Underflow:
                // there is no more input, but this function is assumed to
                // receive a well-formed input string, throw error
                throw CharSetConverter::MalformedCharacterException();
                break;
            default:
                throw std::runtime_error("unexpected conversion result");
                break;
            }
        }
        // flush the potentially remaining output bytes
        do{
            cr = this->flush(pByteOutput, nBytesOutput);
            switch (cr){
            case CharSetConverter::Done:
                break;

            case CharSetConverter::Overflow:
                // allocate more memory and continue
                nCurrentPos = dest.size() - nBytesOutput;
                dest.resize(dest.size()*2);
                pByteOutput = (char*) &dest[nCurrentPos];
                nBytesOutput = dest.size()-nCurrentPos;
                break;

            case CharSetConverter::Underflow:
                // fall through, since there is no more input (guaranteed OK)
                // we should not receive this error here
            default:
                throw std::runtime_error("unexpected conversion result");
                break;
            }
        } while(cr != CharSetConverter::Done);

        // clamp the remaining bytes from the output buffer
        if (nBytesOutput > 0){
            dest.resize( dest.size() - nBytesOutput);
        }

    }

    void CharSetConverter::convert( const std::string& strInput, std::wstring& wstrDest)
    {
        wstrDest.resize( strInput.length() );
        if (strInput.length() == 0) return;

        // the input descriptors
        const char* pByteInput = strInput.c_str();
        size_t nBytesInput = strInput.length();

        // the output descriptors
        char* pByteOutput = (char*) &wstrDest[0];
        size_t nBytesOutput = wstrDest.size()*sizeof(wchar_t);

        // variables used in loop/switch
        size_t nCharsOutput = wstrDest.size();
        size_t nCurrentPos = 0;
        ConversionResult cr = CharSetConverter::Done;

        while (nBytesInput > 0){
            cr = this->convert( pByteInput, nBytesInput, pByteOutput, nBytesOutput );
            switch (cr){
            case CharSetConverter::Done:
                // we're done
                break;

            case CharSetConverter::Overflow:
                // allocate more memory and continue
                nCharsOutput = nBytesOutput / sizeof(wchar_t);
                // assert we're working with whole wchar's
                assert( (nCharsOutput * sizeof(wchar_t)) == nBytesOutput );
                // remember current post
                nCurrentPos = wstrDest.size() - nCharsOutput;
                // resize to twice the size
                wstrDest.resize(wstrDest.size()*2);
                // recompute the offset & lengths
                pByteOutput = (char*) &wstrDest[nCurrentPos];
                nCharsOutput = wstrDest.size() - nCurrentPos;
                nBytesOutput = nCharsOutput * sizeof(wchar_t);
                break;

            case CharSetConverter::Underflow:
                // there is no more input, but this function is assumed to
                // receive a well-formed input string, throw error
                throw CharSetConverter::MalformedCharacterException();
                break;
            default:
                throw std::runtime_error("unexpected conversion result");
                break;
            }
        }
        // flush the potentially remaining output bytes
        do{
            cr = this->flush(pByteOutput, nBytesOutput);
            switch (cr){
            case CharSetConverter::Done:
                break;

            case CharSetConverter::Overflow:
                // allocate more memory and continue
                nCharsOutput = nBytesOutput / sizeof(wchar_t);
                // assert we're working with whole wchar's
                assert( (nCharsOutput * sizeof(wchar_t)) == nBytesOutput );
                // remember current post
                nCurrentPos = wstrDest.size() - nCharsOutput;
                // resize to twice the size
                wstrDest.resize(wstrDest.size()*2);
                // recompute the offset & lengths
                pByteOutput = (char*) &wstrDest[nCurrentPos];
                nCharsOutput = wstrDest.size() - nCurrentPos;
                nBytesOutput = nCharsOutput * sizeof(wchar_t);
                break;

            case CharSetConverter::Underflow:
                // fall through, since there is no more input (guaranteed OK)
                // we should not receive this error here
            default:
                throw std::runtime_error("unexpected conversion result");
                break;
            }
        } while(cr != CharSetConverter::Done);

        // clamp the remaining bytes from the output buffer
        if (nBytesOutput > 0){
            nCharsOutput = nBytesOutput / sizeof(wchar_t);
            // assert we're working with whole wchar's
            assert( (nCharsOutput * sizeof(wchar_t)) == nBytesOutput );

            wstrDest.resize( wstrDest.size() - nCharsOutput);
        }

    }

    namespace{
        bool getCharSetFromLocaleName(const char* szLocaleName, std::string& strCodeSet)
        {
            // format of a locale name typically takes the form of:
            // language[_territory][.codeset][@modifier]
            std::string strLocaleName = szLocaleName;
            std::string strModifier = "";

            std::string::size_type pos = strLocaleName.find('.');
            if (pos != std::string::npos && pos+1 < strLocaleName.length()){
                strCodeSet = strLocaleName.substr(pos+1);
                pos = strCodeSet.find('@');
                if (pos != std::string::npos && pos+1 < strCodeSet.length()){
                    strModifier = strCodeSet.substr(pos+1);
                    strCodeSet = strCodeSet.substr(0, pos);
                    // LOG4CXX_WARN(g_Logger, "ignoring locale modifier '" << strModifier << "'");
                }
                return true;
            }
            return false;
        }
        
        bool getActiveCharSetFromSystem(std::string& strCharSet)
        {
            // LOG4CXX_TRACE(g_Logger, "setActiveCharSetFromSystem");
#ifdef _HOST_COMPILER_MICROSOFT
            const char* szFound = getCodePageName(GetACP());
            if (szFound != 0){
                strCharSet = szFound;
                return true;
            }
            return false;
#endif

#ifdef _HOST_COMPILER_GCC
            // 1st try to get LANG env var:
            const char* langEnv = getenv("LANG");
            if (langEnv != 0  && getCharSetFromLocaleName(langEnv, strCharSet) ){
                // LOG4CXX_DEBUG(g_Logger, "using charset '" << strCharSet << "' from environment variable LANG=" << langEnv);
            }
            else{
                strCharSet = nl_langinfo(CODESET);
                // LOG4CXX_DEBUG(g_Logger, "using charset '" << strCharSet << "' from nl_langinfo(CODESET)");
            }
            return true;
#endif
        }
    }


    bool CharSetConverter::getCurrentLocaleCharSet( std::string& strActiveCharSet)
    {
        const char* localeName =  setlocale(LC_CTYPE, 0);
        if (localeName == 0){
            // LOG4CXX_ERROR(g_Logger, "setlocale failed to query current LC_CTYPE, trying default system charset");
            return getActiveCharSetFromSystem(strActiveCharSet);
        }

        std::string strCharSet;
        if (!getCharSetFromLocaleName(localeName,strActiveCharSet)){
            // LOG4CXX_WARN(g_Logger, "unable to determine ActiveCharSet from locale name '" << localeName<< "', trying default system charset");
            return getActiveCharSetFromSystem(strActiveCharSet);
        }

        return true;
    }

};