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
