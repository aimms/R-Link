#ifndef _INCLUDED__Common_CharSetConverter
#define _INCLUDED__Common_CharSetConverter

#include "Common/GlobalInclude.h"
#include "Common/CommonAPI.h"

#include <string>
#include <stdexcept>

namespace Common {

    // CharSetConverter provides an engine that can transform a sequence of bytes in a specific charset 
    // into another sequence of bytes of another charset. The CharSetConverter should always be used by 
    // making the following sequence of method invocations, hereinafter referred to as the conversion operation:
    //  1. Reset the engine via the reset method, unless it has not been used before;
    //  2. Invoke the decode method zero or more times, as long as additional input may be available, 
    //     filling the input buffer and flushing the output buffer between invocations;
    //  3. Invoke the flush method so that the engine can flush any internal state to the output buffer.
	class  CharSetConverter{
    public:
        class Exception : public std::runtime_error
        {
        public:
            Exception(const char* szMessage);
            virtual ~Exception() throw();
        };

        // exception that is thrown when a sequence of input units denotes a 
        // character that cannot be represented in the output charset
        class UnmappableCharacterException : public Exception
        {
        public:
            UnmappableCharacterException();
            virtual ~UnmappableCharacterException() throw();
        };

        // exception that is thrown when a sequence of input units is not 
        // well-formed according to specified input character set
        class MalformedCharacterException : public Exception
        {
        public:
            MalformedCharacterException();
            virtual ~MalformedCharacterException() throw();
        };

        class SystemException : public Exception
        {
        public:
            SystemException(int errNo);
            virtual ~SystemException() throw();

            virtual int errNo() const;
        private:
            int m_ErrNo;
        };

	public:
        CharSetConverter();
        virtual ~CharSetConverter();

        // open will close (if any) the current conversion context
        // and open the specified conversion. When successful
        // open will return true, false otherwise
        bool open(const char* szFrom, const char* szTo);
        bool open(const wchar_t* szFrom, const wchar_t* szTo);

        // closes (if any) the current conversion context
        void close();

        enum ConversionResult{
            Done = 0,   // the input buffer has been completely consumed and converted
            Underflow,  // the input buffer is not yet empty, but additional input is required.
            Overflow    // there is insufficient room in the output buffer.
        };

        // convert at most nInputBytesLeft bytes pointed to by pInput into at most nOutputBytesLeft starting at pOutput
        // upon return all arguments will be updated to their active state, e.g if 5 bytes of input were converted to 3 bytes
        // of output, this routine would update the arguments similar to
        //   pInput += 5; nInputBytesLeft-= 5; pOutput+=3; nOutputBytesLeft-=3;
        // ConversionResult will indicate the current state of the conversion process
        // convert might throw a UnmappableCharacterException, MalformedCharacterException, or a SystemException
        ConversionResult convert( const char*& pInput, size_t& nInputBytesLeft, char*& pOutput, size_t& nOutputBytesLeft );

        // flush any remaining output characters to the output buffer
        // flush might throw a UnmappableCharacterException, MalformedCharacterException, or a SystemException
        ConversionResult flush( char*& pOutput, size_t& nOutputBytesLeft);

        // reset the current conversion context to a pristine state
        // reset might throw a UnmappableCharacterException, MalformedCharacterException, or a SystemException
        ConversionResult reset();

        // convenience method that converts any byte-sequence (represented as a std::string) to another
        // byte-sequence (also represented as std::string)
        void convert( const std::string& strInput, std::string& strOutput, double fExpectedBytesPerOutputChar = 1.0 );
        // convenience method that converts a std::wstring to a byte-sequence (represented as std::string)
        void convert( const std::wstring& strInput, std::string& strOutput, double fExpectedBytesPerOutputChar = 1.0 );
        // convenience method that converts a std::string to a std::wstring
        void convert( const std::string& strInput, std::wstring& strOutput );

        static bool getCurrentLocaleCharSet( std::string& strActiveCharSet);
    private:
        void* m_Handle;
	};
};

#endif // end #ifndef _INCLUDED__Common_CharSetConverter
