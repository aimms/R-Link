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
