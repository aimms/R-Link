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
#include "REngine.h"
#include "Common/GlobalInclude.h"
#ifdef _HOST_COMPILER_MICROSOFT
#include <windows.h>
#endif

#include <sstream>

#ifdef _HOST_COMPILER_GCC
#include <stddef.h> // ptrdiff_t
#include <dlfcn.h>
#endif

namespace r4aimms{

    namespace {

        inline std::wstring a2w(std::string mbString){
            std::wstring wideString;
            if (mbString.length() > 0){
                size_t nLenRequired = mbstowcs(0, mbString.c_str(), 0);
                if (nLenRequired == ((size_t)-1)){
                    throw std::runtime_error("mbstowcs: conversion failed");
                }
                wideString.resize(nLenRequired);
                size_t result = mbstowcs(&wideString[0], mbString.c_str(), nLenRequired + 1);
                if (result != nLenRequired){
                    throw std::runtime_error("mbstowcs: conversion failed");
                }
            }
            return wideString;
        }

        inline std::string w2a(std::wstring wideString){
            std::string mbsString;
            if (wideString.length() > 0){
                size_t nLenRequired = wcstombs(0, wideString.c_str(), 0);
                if (nLenRequired == ((size_t)-1)){
                    throw std::runtime_error("wcstombs: conversion failed");
                }
                mbsString.resize(nLenRequired);
                size_t result = wcstombs(&mbsString[0], wideString.c_str(), nLenRequired + 1);
                if (result != nLenRequired){
                    throw std::runtime_error("wcstombs: conversion failed");
                }
            }
            return mbsString;
        }
    }

#define PROTECT(s)	m_RModule->Rf_protect(s)
#define UNPROTECT(n)	m_RModule->Rf_unprotect(n)

    // from Rinternals.h(127)
#define STRSXP 16 



    struct REngine::RModule{
        typedef int (*Rf_initEmbeddedR_t)(int argc, char *argv[]);
        Rf_initEmbeddedR_t Rf_initEmbeddedR;

        typedef void (*Rf_endEmbeddedR_t)(int fatal);
        Rf_endEmbeddedR_t Rf_endEmbeddedR;

        typedef void* SEXP;

        typedef SEXP (*Rf_protect_t)(SEXP);
        Rf_protect_t Rf_protect;

        typedef void (*Rf_unprotect_t)(int);
        Rf_unprotect_t Rf_unprotect;

        typedef SEXP(*Rf_allocVector_t)(unsigned int, ptrdiff_t);
        Rf_allocVector_t Rf_allocVector;

        typedef void(*SET_STRING_ELT_t)(SEXP x, ptrdiff_t i, SEXP v);
        SET_STRING_ELT_t SET_STRING_ELT;

        typedef SEXP (*Rf_mkChar_t)(const char *);
        Rf_mkChar_t Rf_mkChar;

        typedef enum {
            PARSE_NULL,
            PARSE_OK,
            PARSE_INCOMPLETE,
            PARSE_ERROR,
            PARSE_EOF
        } ParseStatus;

        typedef SEXP (*R_ParseVector_t)(SEXP, int, ParseStatus*, SEXP);
        R_ParseVector_t R_ParseVector;

        typedef ptrdiff_t  (*Rf_length_t)(SEXP);
        Rf_length_t Rf_length;

        typedef SEXP (*R_tryEval_t)(SEXP, SEXP, int *);
        R_tryEval_t R_tryEval;

        typedef const char* (*R_curErrorBuf_t)();
        R_curErrorBuf_t R_curErrorBuf;

        typedef SEXP(*VECTOR_ELT_t)(SEXP x, ptrdiff_t i);
        VECTOR_ELT_t VECTOR_ELT;


        SEXP* R_NilValue;
        SEXP* R_GlobalEnv;

        void* hModule;

        RModule() 
            : Rf_initEmbeddedR(0)
            , Rf_endEmbeddedR(0)
            , Rf_protect(0)
            , Rf_unprotect(0)
            , Rf_allocVector(0)
            , SET_STRING_ELT(0)
            , Rf_mkChar(0)
            , R_ParseVector(0)
            , Rf_length(0)
            , R_tryEval(0)
            , R_curErrorBuf(0)
            , VECTOR_ELT(0)
            , R_NilValue(0)
            , R_GlobalEnv(0)
        {};

        ~RModule(){
            close();
        }

        template <typename T>
        T GetAddress(const char* symbolName){
#ifdef _HOST_COMPILER_MICROSOFT
            void* symbolAddress = ::GetProcAddress((HMODULE)hModule, symbolName);
#endif
#ifdef _HOST_COMPILER_GCC
            void* symbolAddress = dlsym(hModule,symbolName);
#endif
            if (symbolAddress){
                return (T)symbolAddress;
            }
            throw std::runtime_error(std::string("could not resolve '") + symbolName + "'");
        }

        bool open(const wchar_t* szAddress){
#ifdef _HOST_COMPILER_MICROSOFT
            hModule = LoadLibraryExW(szAddress, 0, LOAD_WITH_ALTERED_SEARCH_PATH);
#endif
#ifdef _HOST_COMPILER_GCC
            hModule = dlopen(w2a(szAddress).c_str(), RTLD_NOW);
#endif
            if (hModule){
                Rf_initEmbeddedR = GetAddress<Rf_initEmbeddedR_t>("Rf_initEmbeddedR");
                Rf_endEmbeddedR = GetAddress<Rf_endEmbeddedR_t>("Rf_endEmbeddedR");
                Rf_protect = GetAddress<Rf_protect_t>("Rf_protect");
                Rf_unprotect = GetAddress<Rf_unprotect_t>("Rf_unprotect");
                Rf_allocVector = GetAddress<Rf_allocVector_t>("Rf_allocVector");
                SET_STRING_ELT = GetAddress<SET_STRING_ELT_t>("SET_STRING_ELT");
                Rf_mkChar = GetAddress<Rf_mkChar_t>("Rf_mkChar");
                R_ParseVector = GetAddress<R_ParseVector_t>("R_ParseVector");
                Rf_length = GetAddress<Rf_length_t>("Rf_length");
                R_tryEval = GetAddress<R_tryEval_t>("R_tryEval");
                R_curErrorBuf = GetAddress<R_curErrorBuf_t>("R_curErrorBuf");
                VECTOR_ELT = GetAddress<VECTOR_ELT_t>("VECTOR_ELT");
                R_NilValue = GetAddress<SEXP*>("R_NilValue");
                R_GlobalEnv = GetAddress<SEXP*>("R_GlobalEnv");
                return true;
            }
            return false;
        }

        void close(){
            if (hModule){
                Rf_initEmbeddedR = 0;
                Rf_endEmbeddedR = 0;
                Rf_protect = 0;
                Rf_unprotect = 0;
                Rf_allocVector = 0;
                SET_STRING_ELT = 0;
                Rf_mkChar = 0;
                R_ParseVector = 0;
                Rf_length = 0;
                R_tryEval = 0;
                R_curErrorBuf = 0;
                VECTOR_ELT = 0;
                R_NilValue = 0;
                R_GlobalEnv = 0;
#ifdef _HOST_COMPILER_MICROSOFT
                FreeLibrary((HMODULE)hModule);
                
#endif
#ifdef _HOST_COMPILER_GCC
                dlclose(hModule);
#endif
            }
        }

    };

    REngine::REngine() : m_RModule(0)
    {

    }

    REngine::~REngine()
    {

    }

    void REngine::initialize(const wchar_t* szHomeFolderR)
    {


        std::wstring dllName = szHomeFolderR;
        std::wstring binaryFolder = szHomeFolderR;
#ifdef _TARGET_PLATFORM_WINDOWS
        std::string rHomeEnv = "R_HOME=";
        rHomeEnv += w2a(szHomeFolderR);
        _putenv(rHomeEnv.c_str());
#ifdef _TARGET_ARCHITECTURE_X86
        binaryFolder += L"\\bin\\i386";
#endif
#ifdef _TARGET_ARCHITECTURE_X64
        binaryFolder += L"\\bin\\x64";
#endif
        dllName = binaryFolder + L"\\R.dll";
        std::wstring envPath = _wgetenv(L"Path");
        envPath = binaryFolder + L";" + envPath;
        _wputenv((std::wstring(L"Path=") +  envPath).c_str());
#endif
        
#ifdef _TARGET_PLATFORM_LINUX
        setenv("R_HOME", w2a(szHomeFolderR).c_str(), 1);
        dllName += L"/lib/libR.so";
#endif

        m_RModule = new RModule();
        if (!m_RModule->open(dllName.c_str())){
            delete m_RModule;
            m_RModule = 0;
            throw std::runtime_error("Could not locate R, shared object "+w2a(dllName)+ " could not be opened..." );
        }
        const char *R_argv[] = { (char*)"r4aimms", "--gui=none", "--no-save", "--silent", "--vanilla", "--slave", "--no-readline" };
        int R_argc = sizeof(R_argv) / sizeof(R_argv[0]);
        m_RModule->Rf_initEmbeddedR(R_argc, (char**)R_argv);

    }


    void REngine::terminate()
    {
        if (m_RModule){
            m_RModule->Rf_endEmbeddedR(0);
            m_RModule->close();
            delete m_RModule;
            m_RModule = 0;
        }
    }

    void REngine::executeScript(const wchar_t* szScript)
    {
        RModule::SEXP ans = *m_RModule->R_NilValue;
        RModule::SEXP cmdSexp = *m_RModule->R_NilValue;
        RModule::SEXP cmdexpr = *m_RModule->R_NilValue;
        int errorOccurred = 0;

        std::string utf8Script = w2a(szScript);

        PROTECT(cmdSexp = m_RModule->Rf_allocVector(STRSXP, 1));

        m_RModule->SET_STRING_ELT(cmdSexp, 0, m_RModule->Rf_mkChar(utf8Script.c_str()));

        try{
            RModule::ParseStatus status;
            cmdexpr = PROTECT(m_RModule->R_ParseVector(cmdSexp, -1, &status, *m_RModule->R_NilValue));
            switch (status){
            case RModule::PARSE_OK:
                // Loop is needed here as EXPSEXP might be of length > 1
                for (ptrdiff_t i = 0; i < m_RModule->Rf_length(cmdexpr); i++){
                    ans = m_RModule->R_tryEval(m_RModule->VECTOR_ELT(cmdexpr, i), *m_RModule->R_GlobalEnv, &errorOccurred);
                    if (errorOccurred) {
                        std::ostringstream ossTmp;
                        ossTmp << "R_tryEval: " << m_RModule->R_curErrorBuf();
                        throw std::runtime_error(ossTmp.str().c_str());
                    }
                }
                break;
            case RModule::PARSE_INCOMPLETE:
                throw std::runtime_error("R_tryEval: PARSE_INCOMPLETE");
                break;
            case RModule::PARSE_NULL:
                throw std::runtime_error("R_tryEval: PARSE_NULL");
                break;
            case RModule::PARSE_ERROR:
                throw std::runtime_error("R_tryEval: PARSE_ERROR");
                break;
            case RModule::PARSE_EOF:
                throw std::runtime_error("R_tryEval: PARSE_EOF");
                break;

            default:
                throw std::runtime_error("R_tryEval: unknown error");
                break;
            }
            UNPROTECT(2);
        }
        catch (...){
            UNPROTECT(2);
            throw;
        }
    }

}


