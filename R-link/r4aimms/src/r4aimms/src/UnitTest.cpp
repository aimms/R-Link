#include "UnitTest.h"
#define CPPUNIT_DLL
#include "cppunit/extensions/HelperMacros.h"

#include "UCSTransformerTest.h"
#include "CaseInsensitiveStringTest.h"

namespace Common
{
	void UnitTest::Register()
	{
		CPPUNIT_TEST_SUITE_REGISTRATION( Common::UCSTransformerTest );
        CPPUNIT_TEST_SUITE_REGISTRATION( Common::CaseInsensitiveStringTest );
	}

};
// end namespace TCP

