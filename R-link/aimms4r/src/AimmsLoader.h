#pragma once
#ifdef _WIN32
#undef Realloc
#undef Free
#include <windows.h>
#include <Winbase.h >
#else
#include <dlfcn.h>
#endif

#include <string>
#include <vector>
#include <map>
#include <functional>
#include "FunctionTraits.h"
#include "IAimms.h"


// --
//   SECTION I: Determine compiler
// --
// the Microsoft compiler
#if defined(_MSC_VER)
#define _HOST_COMPILER_MICROSOFT
#define _HOST_COMPILER_DEFINED
#define _TARGET_PLATFORM_WINDOWS
#define _TARGET_PLATFORM_DEFINED
#if defined(_M_X64)
#define _TARGET_ARCHITECTURE_X64
#define _TARGET_ARCHITECTURE_DEFINED
#endif
#if defined(_M_IX86)
#define _TARGET_ARCHITECTURE_X86
#define _TARGET_ARCHITECTURE_DEFINED
#endif
#endif

// the gnu compiler
#if defined(__GNUC__)
#define _HOST_COMPILER_GCC
#define _HOST_COMPILER_DEFINED
#define _TARGET_PLATFORM_LINUX
#define _TARGET_PLATFORM_DEFINED
#if defined(__x86_64__)
#define _TARGET_ARCHITECTURE_X64
#define _TARGET_ARCHITECTURE_DEFINED
#endif
#if defined(__i386__)
#define _TARGET_ARCHITECTURE_X86
#define _TARGET_ARCHITECTURE_DEFINED
#endif
#endif

namespace Aimms{

    class APIFunction{
    protected:
        int offset;
        std::string functionName;
    public:
        APIFunction(){}
        APIFunction(std::string name) :functionName(name){}
        std::string getFunctionName(){ return functionName; }
        std::string getDecoratedName(){
#if defined(__i386__)
          //std::wcout << "This one? Win32?" << std::endl;
          return "_" + functionName + "@" + std::to_string(offset);
#else
          //std::wcout << "That one x64??" << std::endl;
          return   functionName;// + "@" + std::to_string(offset);
#endif
        }

        int getOffset(){ return offset; }

    };

    template <typename T>
    class AimmsFunction : public APIFunction{
    public:
        AimmsFunction(std::string name)
            : APIFunction(name)
        {
            offset = FunctionTraits<std::function<T>>::calculateSize();
        }
    };

    class AimmsLoader{
        using AimmsLibHandle = void *;
        using GenericAimmsFunction = void *;

#if defined(__i386__)
    #define CC __stdcall
#else
    #define CC
#endif

    private:
        enum SupportedAimmsFunctions{
            FCN_ProjectOpen,
            FCN_ProjectClose,
            FCN_AimmsProcedureHandleCreateA,
            FCN_AimmsProcedureArgumentHandleCreate,
            FCN_AimmsAttributeNameW,
            FCN_AimmsValueCard,
            FCN_AimmsAttributeDimension,
            FCN_AimmsValueResetHandle,
            FCN_AimmsValueNextW,
            FCN_AimmsAPILastError,
            FCN_AimmsIdentifierHandleCreateW,
            FCN_AimmsAttributeDeclarationDomain,
            FCN_AimmsSetOrdinalToElement,
            FCN_AimmsSetElementToNameW,
            FCN_AimmsAttributeRootDomain,
            FCN_AimmsAttributeStorage,
            FCN_AimmsAttributeType,
            FCN_AimmsIdentifierHandleDelete,
            FCN_AimmsAttributeElementRange,
            FCN_AimmsValueRetrieveW,
            FCN_AimmsValueRetrieve,
            FCN_AimmsValueNextMultiW,
            FCN_AimmsValueAssignW,
            FCN_AimmsValueAssignA,
            FCN_AimmsIdentifierEmpty,
            ALL
        };

        //Keeps track of the supported API functions & holds function call information such as
        //function name as string & offset of the accepted arguments of the API function
        std::map<SupportedAimmsFunctions, APIFunction> m_AimmsFunctions;
        //Caches the opened library (dll/so) pointer
        AimmsLibHandle m_Library;

        //Full path of the dll/so
        std::wstring m_Path;

        //Caches the API function pointers
        std::vector<GenericAimmsFunction> m_Functions;
        std::wstring m_LastError;

    public:
        std::wstring getPath(){ return m_Path; }
        AimmsLoader();
        AimmsLoader(std::wstring& path);
        bool Load(std::wstring& path);
        bool Unload();
        bool Reload();
        ~AimmsLoader();
        std::wstring LastError() const;

        //Supported & exposed API functions
        using fnProjectOpen = int(CC *)(wchar_t*, int*);
        fnProjectOpen ProjectOpen();

        using fnProjectClose = int (CC *)(int, int);
        fnProjectClose ProjectClose();

        using fnProcedureHandleCreateA = int (CC *)(char *, int *, int *, int *);
        fnProcedureHandleCreateA ProcedureHandleCreateA();

        using fnProcedureArgumentHandleCreate = int (CC *) (int, int, int *);
        fnProcedureArgumentHandleCreate ProcedureArgumentHandleCreate();

        using fnAttributeNameW = int(CC *) (int, Aimms::AimmsStringW *);
        fnAttributeNameW  AttributeNameW();

        using fnValueCard = int(CC *) (int, int*);
        fnValueCard ValueCard();

        using fnAttributeDimension = int(CC *) (int, int *, int *);
        fnAttributeDimension AttributeDimension();

        using fnValueResetHandle = int(CC *)(int);
        fnValueResetHandle ValueResetHandle();

        using fnValueNextW = int(CC *)(int, int *, Aimms::AimmsValueW *);
        fnValueNextW ValueNextW();

        using fnAPILastError = int(CC *) (int*, wchar_t*);
        fnAPILastError APILastError();

        using fnIdentifierHandleCreateW = int(CC *) (wchar_t *, int *, int *, int, int *);
        fnIdentifierHandleCreateW IdentifierHandleCreateW();


        using fnAttributeDeclarationDomain = int(CC *)(int, int*);
        fnAttributeDeclarationDomain AttributeDeclarationDomain();

        using fnSetOrdinalToElement =  int(CC *)(int, int, int*);
        fnSetOrdinalToElement SetOrdinalToElement();

        using fnSetElementToNameW = int(CC *)(int, int, Aimms::AimmsStringW *);
        fnSetElementToNameW SetElementToNameW();

        using fnAttributeRootDomain =  int(CC *) (int, int*);
        fnAttributeRootDomain AttributeRootDomain();

        using fnAttributeStorage =  int(CC *) (int, int*);
        fnAttributeStorage AttributeStorage();

        using fnAttributeType = int(CC *)(int, int *);
        fnAttributeType AttributeType();

        using fnIdentifierHandleDelete = int(CC *)(int);
        fnIdentifierHandleDelete IdentifierHandleDelete();

        using fnAttributeElementRange = int(CC *)(int, int*);
        fnAttributeElementRange AttributeElementRange();

        using fnValueRetrieveW = int(CC *) (int, int*, AimmsValueW*);
        fnValueRetrieveW ValueRetrieveW();

        using fnValueRetrieve = int(CC *) (int, int*, AimmsValue*);
        fnValueRetrieve ValueRetrieve();


        using fnValueNextMultiW = int (CC *) (int, int *, int*, AimmsValueW *);
        fnValueNextMultiW ValueNextMultiW();

        using fnValueAssignW = int (CC *)(int, int*, AimmsValueW *);
        fnValueAssignW ValueAssignW();

        using fnValueAssignA = int(CC *)(int, int*, AimmsValue *);
        fnValueAssignA ValueAssignA();

        using fnIdentifierEmpty =  int(CC *)(int);
        fnIdentifierEmpty IdentifierEmpty();

      private:
        void StoreLastError(std::wstring);
        void PopulateFunctionNameOffsets();

        template<class T>
        std::function<T> GetAimmsFunction(SupportedAimmsFunctions func){

            if (!m_Library) return nullptr;
            if (m_AimmsFunctions.find(func) == m_AimmsFunctions.end()) return nullptr;
            if (!m_Functions[func]){
#ifdef _WIN32
                m_Functions[func] = (GenericAimmsFunction)GetProcAddress((HMODULE)m_Library, m_AimmsFunctions[func].getDecoratedName().c_str());
#else
                m_Functions[func] = (GenericAimmsFunction)dlsym(m_Library, m_AimmsFunctions[func].getDecoratedName().c_str());
#endif
            }
            return reinterpret_cast<T*> (m_Functions[func]);
        }

        template<class T>
        T GetAimmsFunctionCC(SupportedAimmsFunctions func){
          if (!m_Library) return nullptr;
          if (m_AimmsFunctions.find(func) == m_AimmsFunctions.end()){ return nullptr;}
          if (!m_Functions[func]){
#ifdef _WIN32
            m_Functions[func] = (GenericAimmsFunction)GetProcAddress((HMODULE)m_Library, m_AimmsFunctions[func].getDecoratedName().c_str());
#else
            m_Functions[func] = (GenericAimmsFunction)dlsym(m_Library, m_AimmsFunctions[func].getDecoratedName().c_str());
#endif
          }
          return reinterpret_cast<T> (m_Functions[func]);
        }


    };
};
