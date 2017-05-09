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