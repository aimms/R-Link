#include "Common/UCSTransformer.h"
#include <stdlib.h> // wcstombs
#include <errno.h>


namespace Common {
    UCSTransformer::UCSTransformer() : m_fAvgBytesPerCharacter(1.0){
    }

    UCSTransformer::~UCSTransformer(){
    }

    bool UCSTransformer::open( const char* szTarget, double fAvgBytesPerCharacter ){
#ifdef _TARGET_PLATFORM_LINUX
        const char* szUCSCharset = "UTF32LE";
#endif
#ifdef _TARGET_PLATFORM_WINDOWS
        const char* szUCSCharset = "UTF16LE";
#endif
        if (!m_Encoder.open( szUCSCharset, szTarget ) || !m_Decoder.open( szTarget, szUCSCharset )){
            close();
            return false;
        }
        m_fAvgBytesPerCharacter = fAvgBytesPerCharacter;
        return true;
    }

    bool UCSTransformer::open( const wchar_t* wszTargetCodePage, double fAvgBytesPerCharacter)
    {
        std::string strTargetCodePage;
        strTargetCodePage.resize(wcslen(wszTargetCodePage));
        size_t converted = wcstombs( &strTargetCodePage[0], wszTargetCodePage, strTargetCodePage.size() );
        if (converted != strTargetCodePage.size()) {
            throw CharSetConverter::SystemException(errno);
        }
        return open(strTargetCodePage.c_str(), fAvgBytesPerCharacter);
    }

    void UCSTransformer::close(){
        m_Encoder.close();
        m_Decoder.close();
    }

    size_t UCSTransformer::decodeFrom( const char* buffer, size_t maxLen, wchar_t* wszDest, size_t maxDestLen){
        // the input descriptors
        const char* pByteInput = buffer;
        size_t nBytesInput = maxLen;

        // the output descriptors
        char* pByteOutput = (char*) wszDest;
        size_t nBytesOutput = maxDestLen*sizeof(wchar_t);

        // variables used in loop/switch
        size_t nCharsOutput = maxDestLen;
        UCSTransformer::ConversionResult cr = CharSetConverter::Done;

        while (nBytesInput > 0){
            cr = m_Decoder.convert( pByteInput, nBytesInput, pByteOutput, nBytesOutput );
            switch (cr){
            case CharSetConverter::Done:
                // we're done
                break;

            case CharSetConverter::Overflow:
                // there is no more space to be written to in the buffer
                return 0;
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
            cr = m_Decoder.flush(pByteOutput, nBytesOutput);
            switch (cr){
            case CharSetConverter::Done:
                break;

            case CharSetConverter::Overflow:
                // there is no more space to be written to in the buffer
                return 0;
                break;

            case CharSetConverter::Underflow:
                // fall through, since there is no more input (guaranteed OK)
                // we should not receive this error here
            default:
                throw std::runtime_error("unexpected conversion result");
                break;
            }
        } while(cr != CharSetConverter::Done);

        nCharsOutput = nBytesOutput / sizeof(wchar_t);
        // assert we're working with whole wchar's
        assert( (nCharsOutput * sizeof(wchar_t)) == nBytesOutput );

        return maxDestLen - nCharsOutput;
    }

    // decode the specified input (using the active charset) into a wide-character string
    void UCSTransformer::decodeFrom( const char* buffer, size_t maxLen, std::wstring& wstrDest  )
    {
        size_t expectedCharLen = (size_t)(((double)maxLen) / m_fAvgBytesPerCharacter);
        if (expectedCharLen < 1) expectedCharLen = 1;
        wstrDest.resize( expectedCharLen );

        // the input descriptors
        const char* pByteInput = buffer;
        size_t nBytesInput = maxLen;

        // the output descriptors
        char* pByteOutput = (char*) &wstrDest[0];
        size_t nBytesOutput = wstrDest.size()*sizeof(wchar_t);

        // variables used in loop/switch
        size_t nCharsOutput = wstrDest.size();
        size_t nCurrentPos = 0;
        UCSTransformer::ConversionResult cr = CharSetConverter::Done;

        while (nBytesInput > 0){
            cr = m_Decoder.convert( pByteInput, nBytesInput, pByteOutput, nBytesOutput );
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
            cr = m_Decoder.flush(pByteOutput, nBytesOutput);
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

    size_t UCSTransformer::encodeTo( const wchar_t* wszSrc, size_t wszLen, char* buffer, size_t maxLen ){
        // the input descriptors
        const char* pByteInput = reinterpret_cast<const char*>(wszSrc);
        size_t nBytesInput = wszLen*sizeof(wchar_t);

        // the output descriptors
        char* pByteOutput = (char*) buffer;
        size_t nBytesOutput = maxLen;

        // variables used in loop/switch
        UCSTransformer::ConversionResult cr = CharSetConverter::Done;

        while (nBytesInput > 0){
            cr = m_Encoder.convert( pByteInput, nBytesInput, pByteOutput, nBytesOutput );
            switch (cr){
            case CharSetConverter::Done:
                // we're done
                break;

            case CharSetConverter::Overflow:
                // there is no more space to be written to in the buffer
                return 0;
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
            cr = m_Encoder.flush(pByteOutput, nBytesOutput);
            switch (cr){
            case CharSetConverter::Done:
                break;

            case CharSetConverter::Overflow:
                // there is no more space to be written to in the buffer
                return 0;
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
        return maxLen - nBytesOutput;
    }

    void UCSTransformer::encodeTo( const wchar_t* wszSrc, size_t wszLen, std::string& dest ){
        size_t expectedByteLen = (size_t)(m_fAvgBytesPerCharacter * wszLen);
        if (expectedByteLen < 1) expectedByteLen = 1;
        dest.resize( expectedByteLen );

        // the input descriptors
        const char* pByteInput = reinterpret_cast<const char*>(wszSrc);
        size_t nBytesInput = wszLen*sizeof(wchar_t);

        // the output descriptors
        char* pByteOutput = (char*) &dest[0];
        size_t nBytesOutput = dest.size();

        // variables used in loop/switch
        size_t nCurrentPos = 0;
        UCSTransformer::ConversionResult cr = CharSetConverter::Done;

        while (nBytesInput > 0){
            cr = m_Encoder.convert( pByteInput, nBytesInput, pByteOutput, nBytesOutput );
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
            cr = m_Encoder.flush(pByteOutput, nBytesOutput);
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
};