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
