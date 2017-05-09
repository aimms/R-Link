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
﻿#include "UCSTransformerTest.h"
#include "Common/UCSTransformer.h"
#include "Common/CharSetEnumerator.h"
#include <vector>

namespace Common {

    namespace{
        static const char* g_szExampleSrc = "The quick brown fox jumps over the lazy dog.";
        static const wchar_t* g_wszExampleSrc = L"The quick brown fox jumps over the lazy dog.";
        static const char* g_szUTF8CodePage = "UTF8";

        static const char g_szNastyExampleSrc[] = {(char)0x22U, (char)0xc3U, (char)0xabU, (char)0xc3U, (char)0xa8U, (char)0xc3U, (char)0xafU, (char)0x22U, (char)0x00U} ;
        static const wchar_t* g_wszNastyExampleSrc = L"\"ëèï\"";
    }
    

    void UCSTransformerTest::setUp() {
    }

    void UCSTransformerTest::tearDown() {
    }

    void UCSTransformerTest::testOpenWithUnicode()
    {
        UCSTransformer ut;
        bool bOpen = false;
        bOpen = ut.open(L"UTF8");
        CPPUNIT_ASSERT_EQUAL( true, bOpen );

        std::string strCurrentLocale;
        // setlocale(LC_ALL,"");
        bool bSuccess = CharSetConverter::getCurrentLocaleCharSet(strCurrentLocale);
        CPPUNIT_ASSERT_EQUAL( true, bSuccess );
    }

    void UCSTransformerTest::testEncodeToStdString_Simple()
    {
        UCSTransformer ut;
        bool bOpen = false;
        bOpen = ut.open(g_szUTF8CodePage);
        CPPUNIT_ASSERT_EQUAL( true, bOpen );

        std::wstring wstrSource(g_wszExampleSrc);
        std::string strExpected(g_szExampleSrc);

        std::string strResult;
        ut.encodeTo(wstrSource, strResult);
        CPPUNIT_ASSERT_EQUAL(strExpected, strResult );
    }

    void UCSTransformerTest::testEncodeToStdString_Empty()
    {
        UCSTransformer ut;
        bool bOpen = false;
        bOpen = ut.open(g_szUTF8CodePage);
        CPPUNIT_ASSERT_EQUAL( true, bOpen );

        std::wstring wstrSource;
        std::string strExpected;

        std::string strResult;
        ut.encodeTo(wstrSource, strResult);
        CPPUNIT_ASSERT_EQUAL(strExpected, strResult );
    }
    void UCSTransformerTest::testEncodeToStdString_Resize()
    {
        UCSTransformer ut;
        bool bOpen = false;
        bOpen = ut.open(g_szUTF8CodePage, .35);
        CPPUNIT_ASSERT_EQUAL( true, bOpen );

        std::wstring wstrSource(g_wszExampleSrc);
        std::string strExpected(g_szExampleSrc);

        std::string strResult;
        ut.encodeTo(wstrSource, strResult);
        CPPUNIT_ASSERT_EQUAL(strExpected, strResult );
    }

    void UCSTransformerTest::testEncodeToBuffer_Simple()
    {
        UCSTransformer ut;
        bool bOpen = false;
        bOpen = ut.open(g_szUTF8CodePage);
        CPPUNIT_ASSERT_EQUAL( true, bOpen );

        std::wstring wstrSource(g_wszExampleSrc);
        std::string strExpected(g_szExampleSrc);

        static const size_t bufSize = 45;
        char szBuffer[bufSize];

        size_t nBytesConverted = ut.encodeTo(wstrSource, szBuffer, bufSize );

        CPPUNIT_ASSERT_EQUAL(wstrSource.length(), nBytesConverted );
        std::string strResult( &szBuffer[0], &szBuffer[nBytesConverted]);
        CPPUNIT_ASSERT_EQUAL(strExpected, strResult );
    }

    void UCSTransformerTest::testEncodeToBuffer_Resize()
    {
        UCSTransformer ut;
        bool bOpen = false;
        bOpen = ut.open(g_szUTF8CodePage);
        CPPUNIT_ASSERT_EQUAL( true, bOpen );

        std::wstring wstrSource(g_wszExampleSrc);
        std::string strExpected(g_szExampleSrc);

        static const size_t bufSize = 32;
        char szBuffer[bufSize];

        size_t nBytesConverted = ut.encodeTo(wstrSource, szBuffer, bufSize );

        CPPUNIT_ASSERT_EQUAL(0, nBytesConverted );
    }

    void UCSTransformerTest::testDecodeToStdString_Simple()
    {
        UCSTransformer ut;
        bool bOpen = false;
        bOpen = ut.open(g_szUTF8CodePage);
        CPPUNIT_ASSERT_EQUAL( true, bOpen );

        std::string strSource(g_szExampleSrc);
        std::wstring wstrExpected(g_wszExampleSrc);

        std::wstring wstrResult;
        ut.decodeFrom(strSource, wstrResult);
        CPPUNIT_ASSERT_EQUAL(wstrExpected, wstrResult );
    }

    void UCSTransformerTest::testDecodeToStdString_Empty()
    {
        UCSTransformer ut;
        bool bOpen = false;
        bOpen = ut.open(g_szUTF8CodePage);
        CPPUNIT_ASSERT_EQUAL( true, bOpen );

        std::string strSource;
        std::wstring wstrExpected;

        std::wstring wstrResult;
        ut.decodeFrom(strSource, wstrResult);
        CPPUNIT_ASSERT_EQUAL(wstrExpected, wstrResult );
    }

    void UCSTransformerTest::testDecodeToStdString_Resize()
    {
        UCSTransformer ut;
        bool bOpen = false;
        bOpen = ut.open(g_szUTF8CodePage, .35);
        CPPUNIT_ASSERT_EQUAL( true, bOpen );

        std::string strSource(g_szExampleSrc);
        std::wstring wstrExpected(g_wszExampleSrc);

        std::wstring wstrResult;
        ut.decodeFrom(strSource, wstrResult);
        CPPUNIT_ASSERT_EQUAL(wstrExpected, wstrResult );
    }

    void UCSTransformerTest::testDecodeToBuffer_Simple()
    {
        UCSTransformer ut;
        bool bOpen = false;
        bOpen = ut.open(g_szUTF8CodePage);
        CPPUNIT_ASSERT_EQUAL( true, bOpen );

        std::string strSource(g_szExampleSrc);
        std::wstring wstrExpected(g_wszExampleSrc);

        static const size_t bufSize = 45;
        wchar_t wszBuffer[bufSize];

        size_t nCharsConverted = ut.decodeFrom(strSource, wszBuffer, bufSize );

        CPPUNIT_ASSERT_EQUAL(strSource.length(), nCharsConverted );
        std::wstring wstrResult( &wszBuffer[0], &wszBuffer[nCharsConverted]);
        CPPUNIT_ASSERT_EQUAL(wstrExpected, wstrResult );
    }

    void UCSTransformerTest::testDecodeToBuffer_Resize()
    {
        UCSTransformer ut;
        bool bOpen = false;
        bOpen = ut.open(g_szUTF8CodePage);
        CPPUNIT_ASSERT_EQUAL( true, bOpen );

        std::string strSource(g_szExampleSrc);
        std::wstring wstrExpected(g_wszExampleSrc);

        static const size_t bufSize = 32;
        wchar_t wszBuffer[bufSize];

        size_t nCharsConverted = ut.decodeFrom(strSource, wszBuffer, bufSize );

        CPPUNIT_ASSERT_EQUAL(0, nCharsConverted );
    }

    class CharSetCollector : public ICharSetEnumeratorCallbackA
    {
    public:
        std::vector<std::string> m_vectorCharSet;
    public:
        virtual bool onCharSet( const char* szCharSetName )
        {
            m_vectorCharSet.push_back(szCharSetName);
            return true;
        }
    };

    void UCSTransformerTest::testCharSetEnumerator()
    {
        CharSetCollector cscSupported;
        enumerateCharSets( &cscSupported, Supported);

        CharSetCollector cscInstalled;
        enumerateCharSets( &cscInstalled, Installed);

        CPPUNIT_ASSERT( cscSupported.m_vectorCharSet.size() >= cscInstalled.m_vectorCharSet.size() );
    }

    void UCSTransformerTest::testCharSetConvert_EmptyString()
    {
        CharSetConverter csc;
        std::string strCurrentLocale;
        // setlocale(LC_ALL,"");
        bool bSuccess = CharSetConverter::getCurrentLocaleCharSet(strCurrentLocale);

        bool bOpenConverter = csc.open("UTF8", strCurrentLocale.c_str());
        CPPUNIT_ASSERT(bOpenConverter);

        std::string strEmptyUtf8String;
        std::string strActual;
        std::string strEmptyCCPString;

        csc.convert(strEmptyUtf8String, strActual);

        CPPUNIT_ASSERT_EQUAL( strEmptyCCPString, strActual);
        CPPUNIT_ASSERT_EQUAL( strEmptyCCPString.length(), strActual.length());
    }

    
    void UCSTransformerTest::testNastyCharToWString()
    {
        UCSTransformer ucs;
        CPPUNIT_ASSERT(ucs.open("UTF8"));

        std::wstring dest;
        ucs.decodeFrom(g_szNastyExampleSrc, dest);
        CPPUNIT_ASSERT_EQUAL(g_wszNastyExampleSrc, dest);
    }

};
// end namespace ARMI

