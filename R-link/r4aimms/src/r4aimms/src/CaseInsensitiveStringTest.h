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
#ifndef _INCLUDED_Common_CaseInsensitiveStringTest
#define _INCLUDED_Common_CaseInsensitiveStringTest

#include "Common/GlobalInclude.h"

#define CPPUNIT_DLL

#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept> // needed for runtime_error on Linux

namespace Common {
    class CaseInsensitiveStringTest : public CppUnit::TestFixture {
        CPPUNIT_TEST_SUITE(CaseInsensitiveStringTest );

            CPPUNIT_TEST( testFind );
            CPPUNIT_TEST( testWFind );

        CPPUNIT_TEST_SUITE_END();

    public:
       
        void testFind();
        void testWFind();

    }; // end class
};
// end namespace Common

#endif // end #ifndef _INCLUDED_Common_CaseInsensitiveStringTest
