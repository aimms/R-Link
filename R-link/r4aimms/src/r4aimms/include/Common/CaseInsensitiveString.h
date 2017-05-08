#ifndef _INCLUDED__Common_CaseInsensitiveString
#define _INCLUDED__Common_CaseInsensitiveString

#include <string>
#include <cstring>
#include <wctype.h>

namespace Common {
    //std::_Char_traits_base
    struct ci_char_traits : public std::char_traits<char>
    {
        static bool eq( char c1, char c2 )
        { return toupper(c1) == toupper(c2); }

        static bool ne( char c1, char c2 )
        { return toupper(c1) != toupper(c2); }

        static bool lt( char c1, char c2 )
        { return toupper(c1) <  toupper(c2); }

        static int compare( const char* s1, const char* s2, size_t n ) {
#ifdef _HOST_COMPILER_GCC
            return strncasecmp( s1, s2, n );
#else
            return _strnicmp( s1, s2, n );
#endif
        }

        static const char* find( const char* s, size_t n, char a ) {
            for(;  n-- > 0; ++s){
                if(toupper(*s) == toupper(a)) {
                    return s;
                }
            }
            return 0;
        }
    };


    //std::_Char_traits_base
    struct ci_wchar_t_traits : public std::char_traits<wchar_t>
    {
        static bool eq( wchar_t c1, wchar_t c2 )
        { return towupper(c1) == towupper(c2); }

        static bool ne( wchar_t c1, wchar_t c2 )
        { return towupper(c1) != towupper(c2); }

        static bool lt( wchar_t c1, wchar_t c2 )
        { return towupper(c1) <  towupper(c2); }

        static int compare( const wchar_t* s1, const wchar_t* s2, size_t n ) {
#ifdef _HOST_COMPILER_GCC
            return wcsncasecmp( s1, s2, n );
#else
            return _wcsnicmp( s1, s2, n );
#endif
        }

        static const wchar_t* find( const wchar_t* s, size_t n, wchar_t a ) {
             for(;  n-- > 0; ++s){
                if(toupper(*s) == toupper(a)) {
                    return s;
                }
            }
            return 0;
        }
    };

    typedef std::basic_string<char, ci_char_traits> CaseInsensitiveString;
    typedef std::basic_string<wchar_t, ci_wchar_t_traits> CaseInsensitiveWString;
};

#endif // end #ifndef _INCLUDED__Common_CaseInsensitiveString
