#ifndef _INCLUDED_aimmsifc_StringConversion
#define _INCLUDED_aimmsifc_StringConversion

#include <string>
#include <vector>

#include <string.h> // for memcpy
#include "Converters.h"
#include <aimmsifc/iAimmsCharTypes.h>

template <typename _char_t> size_t StrLength(_char_t *s) { return 0; }
template<> inline size_t StrLength<char>(char *s) { return strlen(s); }
template<> inline size_t StrLength<wchar_t>(wchar_t *s) { return wcslen(s); }

template <typename traits>
class InputString{
    typedef typename traits::char_in_t char_in_t;
    typedef typename traits::char_out_t char_out_t;
    typedef typename traits::string_out_t string_out_t;
    typedef typename traits::char_in_traits char_in_traits;
public:
    InputString( const char_in_t* in, traits& t ) : m_Traits(t){
        try{
            m_Traits.convert( in, char_in_traits::length(in), m_StringOut );
        }catch( ... ){}
    };

    operator const char_out_t* () const {
        return m_StringOut.c_str();
    };

protected:
    traits& m_Traits;
    string_out_t m_StringOut;
};

template <typename traits>
class InputAimmsValue{
public:
    typedef typename traits::char_in_t char_in_t;
    typedef typename traits::char_out_t char_out_t;
    typedef aimmsifc::AimmsValueType<typename traits::char_in_t> AimmsValueIn;
    typedef aimmsifc::AimmsValueType<typename traits::char_out_t> AimmsValueOut;
    typedef typename traits::string_out_t string_out_t;
public:
    InputAimmsValue( AimmsValueIn* in, traits& t ) : m_Traits(t){
        if (in->String.buf && in->String.buf[0] ){
            try{
                m_Traits.convert( in->String.buf, StrLength<char_in_t>(in->String.buf)+1, m_StringOut );
            }catch( ... ){}

            m_ValueOut.String.Length = static_cast<int>(m_StringOut.length())+1;
            m_ValueOut.String.buf = &m_StringOut[0];
        }
        else{
            m_ValueOut.String.Length = 0;
            m_ValueOut.String.buf = 0;
        }
    };

    operator const AimmsValueOut* () const {
        return &m_ValueOut;
    };

protected:
    traits& m_Traits;
    string_out_t m_StringOut;
    AimmsValueOut m_ValueOut;
};

template <typename traits>
class InputAimmsValueArray{
public:
    typedef typename traits::char_in_t char_in_t;
    typedef aimmsifc::AimmsValueType<typename traits::char_in_t> AimmsValueIn;
    typedef aimmsifc::AimmsValueType<typename traits::char_out_t> AimmsValueOut;
    typedef typename traits::string_out_t string_out_t;
public:
    InputAimmsValueArray( size_t n, AimmsValueIn* in, traits& t ) : m_Traits(t){

        m_vectorStringOut.resize(n);
        m_vectorValueOut.resize(n);

        for (size_t i = 0; i < n; ++i){
            string_out_t& stringOut = m_vectorStringOut[i];
            AimmsValueOut& valueOut = m_vectorValueOut[i];

			if (in[i].String.buf && in[i].String.buf[0] ){
                try{
                    m_Traits.convert( in[i].String.buf, StrLength<char_in_t>(in[i].String.buf)+1, stringOut );
                }catch( ... ){
				}

                valueOut.String.Length = static_cast<int>(stringOut.length())+1;
                valueOut.String.buf = &stringOut[0];
            }
            else{
                valueOut.String.Length = 0;
                valueOut.String.buf = 0;
            }
        }

    };

    operator const AimmsValueOut* () const {
        return &m_vectorValueOut[0];
    };

protected:
    traits& m_Traits;
    std::vector<string_out_t> m_vectorStringOut;
    std::vector<AimmsValueOut> m_vectorValueOut;
};


template <typename traits>
class InputAimmsString : public InputAimmsValue<traits>{
public:
    typedef aimmsifc::AimmsStringType<typename traits::char_in_t> AimmsStringIn;
    typedef aimmsifc::AimmsStringType<typename traits::char_out_t> AimmsStringOut;
    typedef aimmsifc::AimmsValueType<typename traits::char_in_t> AimmsValueIn;
    typedef aimmsifc::AimmsValueType<typename traits::char_out_t> AimmsValueOut;
    typedef typename traits::string_out_t string_out_t;

public:
    InputAimmsString( AimmsStringIn* in, traits& t)
        :   InputAimmsValue<traits>(reinterpret_cast< AimmsValueIn* >(in), t){
    }

    operator const AimmsStringOut* () const {
        return reinterpret_cast<AimmsValueOut*>(&this->m_ValueOut);
    };
};


template <typename traits>
class InputAimmsStringArray : public InputAimmsValueArray<traits>{
public:
    typedef aimmsifc::AimmsStringType<typename traits::char_in_t> AimmsStringIn;
    typedef aimmsifc::AimmsStringType<typename traits::char_out_t> AimmsStringOut;
    typedef aimmsifc::AimmsValueType<typename traits::char_in_t> AimmsValueIn;
    typedef aimmsifc::AimmsValueType<typename traits::char_out_t> AimmsValueOut;
public:
    InputAimmsStringArray( size_t n, AimmsStringIn* in, traits& t)
        :   InputAimmsValueArray<traits>(n, reinterpret_cast<AimmsValueIn*>(in), t){
    }

    operator const AimmsStringOut* () const {
        return reinterpret_cast<AimmsValueOut*>(&this->m_vectorValueOut[0]);
    };
};

template <typename traits>
class OutputAimmsValue{
public:
    typedef aimmsifc::AimmsValueType<typename traits::char_in_t> AimmsValueIn;
    typedef aimmsifc::AimmsValueType<typename traits::char_out_t> AimmsValueOut;
    typedef typename traits::string_in_t string_in_t;
    typedef typename traits::string_out_t string_out_t;
public:
    OutputAimmsValue( AimmsValueOut* out, traits& t ) : m_ValueOut(out), m_Traits(t){
        if ( m_ValueOut->String.Length > 1){
            m_DynamicIn.resize( m_ValueOut->String.Length - 1 );
            m_ValueIn.String.Length = static_cast<int>(m_DynamicIn.length()) + 1;
            m_ValueIn.String.buf = &m_DynamicIn[0];
        }
        else{
            m_DynamicIn.resize( 1 );
            m_ValueIn.String.Length =  m_ValueOut->String.Length;
            m_ValueIn.String.buf = &m_DynamicIn[0];
            m_DynamicIn.resize( 0 );
        }
    };

    ~OutputAimmsValue(){
        this->convert();
    }

    void convert(){
		size_t actualStringLength = m_ValueIn.String.Length-1;
		if (actualStringLength > m_DynamicIn.length()){
            try{
    	        m_Traits.convert( m_DynamicIn.c_str(), m_DynamicIn.length(), m_DynamicOut);
            }catch( ... ){}

		}
		else{
            try{
    	        m_Traits.convert( m_DynamicIn.c_str(), actualStringLength, m_DynamicOut);
            }catch( ... ){}
		}

        size_t n = m_DynamicOut.length()+1;
        if (m_ValueOut->String.Length < static_cast<int>(n)){
            n = m_ValueOut->String.Length;
        }
        memcpy( m_ValueOut->String.buf, m_DynamicOut.c_str(), n * sizeof(typename traits::char_out_t));
		// if indicated we require more memory then initially specified
		if (m_ValueIn.String.Length > m_ValueOut->String.Length ){
			// copy the required length
			m_ValueOut->String.Length = m_ValueIn.String.Length;
		}
		else{
			// otherwise copy the actual (converted) length
	        m_ValueOut->String.Length = static_cast<int>(m_DynamicOut.length() + 1);
		}
    }

    operator AimmsValueIn* (){
        return &m_ValueIn;
    };

protected:
    traits&         m_Traits;
    AimmsValueIn    m_ValueIn;
    AimmsValueOut*  m_ValueOut;

    string_in_t    m_DynamicIn;
    string_out_t   m_DynamicOut;
};

template <typename traits>
class OutputAimmsValueArray{
public:
    typedef aimmsifc::AimmsValueType<typename traits::char_in_t> AimmsValueIn;
    typedef aimmsifc::AimmsValueType<typename traits::char_out_t> AimmsValueOut;
    typedef typename traits::string_in_t string_in_t;
    typedef typename traits::string_out_t string_out_t;
public:
    OutputAimmsValueArray( int& n, AimmsValueOut* out, traits& t ) : m_arrayValueOut(out), m_Traits(t), m_N(n) {
        m_vectorValueIn.resize(n);
        m_vectorDynamicIn.resize(n);
        for (int i= 0 ; i < n; ++i){
            string_in_t& dynamicIn = m_vectorDynamicIn[i];
            AimmsValueIn& valueIn = m_vectorValueIn[i];
            AimmsValueOut& valueOut = m_arrayValueOut[i];

            if (valueOut.String.Length > 1 ){
                dynamicIn.resize( valueOut.String.Length - 1 );
                valueIn.String.Length = static_cast<int>(dynamicIn.length()) + 1;
                valueIn.String.buf = &dynamicIn[0];
            }
            else{
                dynamicIn.resize( 1 );
                valueIn.String.Length = valueOut.String.Length;
                valueIn.String.buf = &dynamicIn[0];
                dynamicIn.resize( 0 );
            }
        }
    };

    ~OutputAimmsValueArray(){
        convert();
    }

    void convert(){
        for (int i = 0 ; i < m_N; ++i){
            m_DynamicOut.resize(0);
            string_in_t& dynamicIn = m_vectorDynamicIn[i];
            AimmsValueOut& valueOut = m_arrayValueOut[i];
            AimmsValueIn& valueIn = m_vectorValueIn[i];

			size_t actualStringLength = valueIn.String.Length-1;
			if (actualStringLength > dynamicIn.length()){
                try{
				    m_Traits.convert( dynamicIn.c_str(), dynamicIn.length(), m_DynamicOut);
                }catch( ... ){}
			}
			else{
                try{
				    m_Traits.convert( dynamicIn.c_str(), actualStringLength, m_DynamicOut);
                }catch( ... ){}
			}

			size_t n = m_DynamicOut.length() + 1;
			if (valueOut.String.Length < static_cast<int>(n)){
				n = valueOut.String.Length;
			}
			memcpy( valueOut.String.buf, m_DynamicOut.c_str(), n * sizeof(typename traits::char_out_t));
			// if indicated we require more memory then initially specified
			if (valueIn.String.Length > valueOut.String.Length ){
				// copy the required length
				valueOut.String.Length = valueIn.String.Length;
			}
			else{
				// otherwise copy the actual (converted) length
				valueOut.String.Length = static_cast<int>(m_DynamicOut.length()) + 1;
			}
        }
    }

    operator AimmsValueIn* (){
        return &m_vectorValueIn[0];
    };

protected:
    traits&                     m_Traits;
    std::vector<AimmsValueIn>   m_vectorValueIn;
    AimmsValueOut*              m_arrayValueOut;

    std::vector<string_in_t>    m_vectorDynamicIn;
    string_out_t                m_DynamicOut;
	int&						m_N;
};


template <typename traits>
class OutputAimmsString : public OutputAimmsValue<traits>{
public:
    typedef aimmsifc::AimmsStringType<typename traits::char_in_t> AimmsStringIn;
    typedef aimmsifc::AimmsStringType<typename traits::char_out_t> AimmsStringOut;
    typedef aimmsifc::AimmsValueType<typename traits::char_in_t> AimmsValueIn;
    typedef aimmsifc::AimmsValueType<typename traits::char_out_t> AimmsValueOut;
public:
    OutputAimmsString( AimmsStringOut* out, traits& t ) 
        :   OutputAimmsValue<traits>(reinterpret_cast<AimmsValueOut*>(out), t){
    }

    operator AimmsStringIn* (){
        return reinterpret_cast<AimmsStringIn*>(&this->m_ValueIn);
    };
};


#endif // end #ifndef _INCLUDED_aimmsifc_StringConversion
