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
#ifndef _INCLUDED_Common_UCSTransformerTest
#define _INCLUDED_Common_UCSTransformerTest

#include "Common/GlobalInclude.h"

#define CPPUNIT_DLL

#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept> // needed for runtime_error on Linux

namespace Common {
    class UCSTransformerTest : public CppUnit::TestFixture {
        CPPUNIT_TEST_SUITE(UCSTransformerTest );
            CPPUNIT_TEST(testOpenWithUnicode);
            CPPUNIT_TEST(testEncodeToStdString_Simple);
            CPPUNIT_TEST(testEncodeToStdString_Empty);
            CPPUNIT_TEST(testEncodeToStdString_Resize);
            CPPUNIT_TEST(testEncodeToBuffer_Simple);
            CPPUNIT_TEST(testEncodeToBuffer_Resize);
            CPPUNIT_TEST(testDecodeToStdString_Simple);
            CPPUNIT_TEST(testDecodeToStdString_Empty);
            CPPUNIT_TEST(testDecodeToStdString_Resize);
            CPPUNIT_TEST(testDecodeToBuffer_Simple);
            CPPUNIT_TEST(testDecodeToBuffer_Resize);
            CPPUNIT_TEST(testCharSetEnumerator);
            CPPUNIT_TEST(testCharSetConvert_EmptyString);
            CPPUNIT_TEST(testNastyCharToWString);
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        void testOpenWithUnicode();

        void testEncodeToStdString_Simple();
        void testEncodeToStdString_Empty();
        void testEncodeToStdString_Resize();
        void testEncodeToBuffer_Simple();
        void testEncodeToBuffer_Resize();
        
        void testDecodeToStdString_Simple();
        void testDecodeToStdString_Empty();
        void testDecodeToStdString_Resize();
        void testDecodeToBuffer_Simple();
        void testDecodeToBuffer_Resize();

        void testCharSetEnumerator();

        void testCharSetConvert_EmptyString();

        void testNastyCharToWString();

    }; // end class
};
// end namespace Common

#endif // end #ifndef _INCLUDED_Common_UCSTransformerTest
