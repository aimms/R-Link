#include "CaseInsensitiveStringTest.h"
#include "Common/CaseInsensitiveString.h"

namespace Common {


    void CaseInsensitiveStringTest::testFind()
    {
    
        CaseInsensitiveString cis("UTF-8");


        //find
        size_t pos = cis.find("utf");
        CPPUNIT_ASSERT_EQUAL(size_t(0), pos);

        pos = cis.find("-");
        CPPUNIT_ASSERT_EQUAL(size_t(3), pos);

        pos = cis.find("8");
        CPPUNIT_ASSERT_EQUAL(size_t(4), pos);    
    
        //not find
        pos = cis.find("_");
        size_t endpos = std::string::npos;
        CPPUNIT_ASSERT_EQUAL(endpos, pos);

        pos = cis.find("ftu");
        CPPUNIT_ASSERT_EQUAL(std::string::npos, pos);

        pos = cis.find("16");
        CPPUNIT_ASSERT_EQUAL(std::string::npos, pos);
    }


     void CaseInsensitiveStringTest::testWFind()
    {
    
        CaseInsensitiveWString cis(L"UTF-8");


        //find
        size_t pos = cis.find(L"utf");
        CPPUNIT_ASSERT_EQUAL(size_t(0), pos);

        pos = cis.find(L"-");
        CPPUNIT_ASSERT_EQUAL(size_t(3), pos);

        pos = cis.find(L"8");
        CPPUNIT_ASSERT_EQUAL(size_t(4), pos);    
    
        //not find
        pos = cis.find(L"_");
        size_t endpos = std::string::npos;
        CPPUNIT_ASSERT_EQUAL(endpos, pos);

        pos = cis.find(L"ftu");
        CPPUNIT_ASSERT_EQUAL(std::string::npos, pos);

        pos = cis.find(L"16");
        CPPUNIT_ASSERT_EQUAL(std::string::npos, pos);
    }

}