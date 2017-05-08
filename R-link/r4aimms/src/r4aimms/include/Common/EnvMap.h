#ifndef _INCLUDED_Common_EnvMap
#define _INCLUDED_Common_EnvMap

#include "Common/GlobalInclude.h"
#include "Common/CommonAPI.h"

#include <string>
#include <map>
#include <vector>

namespace Common{

    // EnvMap provides a copy of the currently active environment variables
    // On the windows platform this a case-insensitive map, on linux
    // a case-sensitive map is used
    class COMMON_API EnvMap{
    public:
        EnvMap(bool bLoadImmediately = true);
        ~EnvMap();

        // (re)loads the active environment into this map
        void reload();

        const char* get( const char* szKey ) const;

        void set( const char* szKey , const char* szValue );
        void append( const char* szKey, const char* szValue );
        void prepend( const char* szKey, const char* szValue );
        void clear();
        void remove( const char* szKey );

        void asEnvVarVector( std::vector<std::string>& vectorEnvVar ) const;
    private:
        struct Impl* pImpl;
    };
}

#endif // #ifndef _INCLUDED_Common_EnvMap
