#ifndef _INCLUDED__Common_CharSetEnumerator
#define _INCLUDED__Common_CharSetEnumerator

#include "Common/GlobalInclude.h"
#include "Common/CommonAPI.h"

namespace Common {

    class ICharSetEnumeratorCallbackW{
    public:
        // callback will be called for each charset specified
        // the callback should return false to stop enumerating, true otherwise
        virtual bool onCharSet( const wchar_t* wszCharSetName ) = 0;
    };

    class ICharSetEnumeratorCallbackA{
    public:
        // callback will be called for each charset specified
        // the callback should return false to stop enumerating, true otherwise
        virtual bool onCharSet( const char* szCharSetName ) = 0;
    };

    enum EnumeratorFlags {
        Supported = 0,
        Installed,
    };

    // enumerate the supported or installed CharSets
    COMMON_API void enumerateCharSets( ICharSetEnumeratorCallbackW* cb, EnumeratorFlags enumFlags = Supported);
    COMMON_API void enumerateCharSets( ICharSetEnumeratorCallbackA* cb, EnumeratorFlags enumFlags = Supported);
};

#endif // end #ifndef _INCLUDED__Common_CharSetEnumerator
