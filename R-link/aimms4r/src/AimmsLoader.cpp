//#include "stdafx.h"
#ifdef _WIN32
#include <windows.h>
#include <Winbase.h >
#endif
#include "AimmsLoader.h"
#include "Utilities.h"

namespace Aimms{
AimmsLoader::AimmsLoader()
  : m_Library(nullptr)
  , m_Path(L"")
  , m_Functions(SupportedAimmsFunctions::ALL , (GenericAimmsFunction)0)
{
  PopulateFunctionNameOffsets();
}


AimmsLoader::AimmsLoader(std::wstring& path)
  :AimmsLoader()
{
  Load(path);
}

void AimmsLoader::PopulateFunctionNameOffsets()
{
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_ProjectOpen, AimmsFunction<int(wchar_t *, int*)>("AimmsServerProjectOpenW")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_ProjectClose, AimmsFunction<int(int, int)>("AimmsProjectClose")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsProcedureHandleCreateA, AimmsFunction<int(char *, int *, int *, int *)>("AimmsProcedureHandleCreateA")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsProcedureArgumentHandleCreate, AimmsFunction<int(int, int, int *)>("AimmsProcedureArgumentHandleCreate")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsAttributeNameW, AimmsFunction<int(int, Aimms::AimmsStringW *)>("AimmsAttributeNameW")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsValueCard, AimmsFunction<int(int, int *)>("AimmsValueCard")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsAttributeDimension, AimmsFunction<int(int, int *, int *)>("AimmsAttributeDimension")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsValueResetHandle, AimmsFunction<int(int)>("AimmsValueResetHandle")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsValueNextW, AimmsFunction<int(int, int *, Aimms::AimmsValueW *)>("AimmsValueNextW")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsAPILastError, AimmsFunction<int(int*, wchar_t*)>("AimmsAPILastErrorW")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsIdentifierHandleCreateW, AimmsFunction<int(wchar_t *, int *, int *,
      int , int *)>("AimmsIdentifierHandleCreateW")));

  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsAttributeDeclarationDomain, AimmsFunction<int(int, int *)>("AimmsAttributeDeclarationDomain")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsSetOrdinalToElement, AimmsFunction<int(int, int, int *)>("AimmsSetOrdinalToElement")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsSetElementToNameW, AimmsFunction<int(int, int, AimmsStringW *)>("AimmsSetElementToNameW")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsAttributeRootDomain, AimmsFunction<int(int, int*)>("AimmsAttributeRootDomain")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsAttributeStorage, AimmsFunction<int(int, int*)>("AimmsAttributeStorage")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsAttributeType, AimmsFunction<int(int, int*)>("AimmsAttributeType")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsIdentifierHandleDelete, AimmsFunction<int(int)>("AimmsIdentifierHandleDelete")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsAttributeElementRange, AimmsFunction<int(int,int*)>("AimmsAttributeElementRange")));

  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsValueRetrieveW, AimmsFunction<int(int, int*, AimmsValueW*)>("AimmsValueRetrieveW")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsValueRetrieve, AimmsFunction<int(int, int*, AimmsValue*)>("AimmsValueRetrieve")));

  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsValueNextMultiW, AimmsFunction<int(int, int *, int*, AimmsValueW *)>("AimmsValueNextMultiW")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsValueAssignW, AimmsFunction<int(int, int*, AimmsValueW *)>("AimmsValueAssignW")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsValueAssignA, AimmsFunction<int(int, int*, AimmsValue *)>("AimmsValueAssignA")));
  m_AimmsFunctions.insert(std::pair<SupportedAimmsFunctions, APIFunction>(SupportedAimmsFunctions::FCN_AimmsIdentifierEmpty, AimmsFunction<int(int)>("AimmsIdentifierEmpty")));

/*  AIMMSAPI(int) AimmsIdentifierHandleCreateW(wchar_t *name, int *domain, int *slicing,
      int flags, int *handle);*/

}

///////////////////////API calls that take into account calling conventions//////////////////////////////

AimmsLoader::fnProjectOpen AimmsLoader::ProjectOpen(){
  return (AimmsLoader::fnProjectOpen )GetAimmsFunctionCC<AimmsLoader::fnProjectOpen>(SupportedAimmsFunctions::FCN_ProjectOpen);
}

AimmsLoader::fnProjectClose AimmsLoader::ProjectClose(){
  return (AimmsLoader::fnProjectClose)GetAimmsFunctionCC<AimmsLoader::fnProjectClose>(SupportedAimmsFunctions::FCN_ProjectClose);
}

AimmsLoader::fnProcedureHandleCreateA AimmsLoader::ProcedureHandleCreateA(){
  return (AimmsLoader::fnProcedureHandleCreateA)GetAimmsFunctionCC<AimmsLoader::fnProcedureHandleCreateA >(SupportedAimmsFunctions::FCN_AimmsProcedureHandleCreateA);
}

AimmsLoader::fnProcedureArgumentHandleCreate AimmsLoader::ProcedureArgumentHandleCreate(){
  return (AimmsLoader::fnProcedureArgumentHandleCreate)GetAimmsFunctionCC<AimmsLoader::fnProcedureArgumentHandleCreate>(SupportedAimmsFunctions::FCN_AimmsProcedureArgumentHandleCreate);
}

AimmsLoader::fnAttributeNameW AimmsLoader::AttributeNameW(){
    return (AimmsLoader::fnAttributeNameW) GetAimmsFunctionCC<AimmsLoader::fnAttributeNameW>(SupportedAimmsFunctions::FCN_AimmsAttributeNameW);
}

AimmsLoader::fnValueCard AimmsLoader::ValueCard(){
    return (AimmsLoader::fnValueCard) GetAimmsFunctionCC<AimmsLoader::fnValueCard>(SupportedAimmsFunctions::FCN_AimmsValueCard);
}

AimmsLoader::fnAttributeDimension AimmsLoader::AttributeDimension(){
    return (AimmsLoader::fnAttributeDimension) GetAimmsFunctionCC<AimmsLoader::fnAttributeDimension>(SupportedAimmsFunctions::FCN_AimmsAttributeDimension);
}

AimmsLoader::fnValueResetHandle AimmsLoader::ValueResetHandle(){
    return (AimmsLoader::fnValueResetHandle) GetAimmsFunctionCC<AimmsLoader::fnValueResetHandle>(SupportedAimmsFunctions::FCN_AimmsValueResetHandle);
}

AimmsLoader::fnValueNextW AimmsLoader::ValueNextW(){
    return (AimmsLoader::fnValueNextW) GetAimmsFunctionCC<AimmsLoader::fnValueNextW>(SupportedAimmsFunctions::FCN_AimmsValueNextW);
}

AimmsLoader::fnAPILastError AimmsLoader::APILastError(){
    return (AimmsLoader::fnAPILastError) GetAimmsFunctionCC<AimmsLoader::fnAPILastError>(SupportedAimmsFunctions::FCN_AimmsAPILastError);
}

AimmsLoader::fnIdentifierHandleCreateW AimmsLoader::IdentifierHandleCreateW(){
    return (AimmsLoader::fnIdentifierHandleCreateW) GetAimmsFunctionCC<AimmsLoader::fnIdentifierHandleCreateW>(SupportedAimmsFunctions::FCN_AimmsIdentifierHandleCreateW);
}

AimmsLoader::fnAttributeDeclarationDomain AimmsLoader::AttributeDeclarationDomain(){
    return (AimmsLoader::fnAttributeDeclarationDomain) GetAimmsFunctionCC<AimmsLoader::fnAttributeDeclarationDomain>(SupportedAimmsFunctions::FCN_AimmsAttributeDeclarationDomain);
}

AimmsLoader::fnSetOrdinalToElement AimmsLoader::SetOrdinalToElement (){
    return (AimmsLoader::fnSetOrdinalToElement) GetAimmsFunctionCC<AimmsLoader::fnSetOrdinalToElement>(SupportedAimmsFunctions::FCN_AimmsSetOrdinalToElement);
}

AimmsLoader::fnSetElementToNameW AimmsLoader::SetElementToNameW(){
    return (AimmsLoader::fnSetElementToNameW) GetAimmsFunctionCC<AimmsLoader::fnSetElementToNameW>(SupportedAimmsFunctions::FCN_AimmsSetElementToNameW);
}

AimmsLoader::fnAttributeRootDomain AimmsLoader::AttributeRootDomain(){
    return (AimmsLoader::fnAttributeRootDomain) GetAimmsFunctionCC<AimmsLoader::fnAttributeRootDomain>(SupportedAimmsFunctions::FCN_AimmsAttributeRootDomain);
}

AimmsLoader::fnAttributeStorage AimmsLoader::AttributeStorage(){
    return (AimmsLoader::fnAttributeStorage) GetAimmsFunctionCC<AimmsLoader::fnAttributeStorage>(SupportedAimmsFunctions::FCN_AimmsAttributeStorage);
}

AimmsLoader::fnAttributeType AimmsLoader::AttributeType(){
    return (AimmsLoader::fnAttributeType) GetAimmsFunctionCC<AimmsLoader::fnAttributeType>(SupportedAimmsFunctions::FCN_AimmsAttributeType);
}

AimmsLoader::fnIdentifierHandleDelete AimmsLoader::IdentifierHandleDelete(){
    return (AimmsLoader::fnIdentifierHandleDelete) GetAimmsFunctionCC<AimmsLoader::fnIdentifierHandleDelete>(SupportedAimmsFunctions::FCN_AimmsIdentifierHandleDelete);
}

AimmsLoader::fnAttributeElementRange AimmsLoader::AttributeElementRange(){
    return (AimmsLoader::fnAttributeElementRange) GetAimmsFunctionCC<AimmsLoader::fnAttributeElementRange>(SupportedAimmsFunctions::FCN_AimmsAttributeElementRange);
}

AimmsLoader::fnValueRetrieveW AimmsLoader::ValueRetrieveW(){
    return (AimmsLoader::fnValueRetrieveW) GetAimmsFunctionCC<AimmsLoader::fnValueRetrieveW>(SupportedAimmsFunctions::FCN_AimmsValueRetrieveW);
}

AimmsLoader::fnValueRetrieve AimmsLoader::ValueRetrieve(){
    return (AimmsLoader::fnValueRetrieve) GetAimmsFunctionCC<AimmsLoader::fnValueRetrieve>(SupportedAimmsFunctions::FCN_AimmsValueRetrieve);
}

AimmsLoader::fnValueNextMultiW AimmsLoader::ValueNextMultiW(){
    return (AimmsLoader::fnValueNextMultiW) GetAimmsFunctionCC<AimmsLoader::fnValueNextMultiW>(SupportedAimmsFunctions::FCN_AimmsValueNextMultiW);
}

AimmsLoader::fnValueAssignW AimmsLoader::ValueAssignW(){
    return (AimmsLoader::fnValueAssignW) GetAimmsFunctionCC<AimmsLoader::fnValueAssignW>(SupportedAimmsFunctions::FCN_AimmsValueAssignW);
}

AimmsLoader::fnValueAssignA AimmsLoader::ValueAssignA(){
    return (AimmsLoader::fnValueAssignA) GetAimmsFunctionCC<AimmsLoader::fnValueAssignA>(SupportedAimmsFunctions::FCN_AimmsValueAssignA);
}

AimmsLoader::fnIdentifierEmpty AimmsLoader::IdentifierEmpty(){
  return (AimmsLoader::fnIdentifierEmpty) GetAimmsFunctionCC<AimmsLoader::fnIdentifierEmpty>(SupportedAimmsFunctions::FCN_AimmsIdentifierEmpty);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////



bool AimmsLoader::Load(std::wstring& path)
{
  if (!Unload()) return false;

  m_Path = path;
  m_Functions.assign(SupportedAimmsFunctions::ALL, (GenericAimmsFunction)0);

#ifdef _WIN32
  m_Library = (AimmsLibHandle)LoadLibraryEx(w2a(path).c_str(), 0, LOAD_WITH_ALTERED_SEARCH_PATH);
  if (!m_Library) {
    StoreLastError(L"Couldn't load library" + path + L". Exit with " + std::to_wstring(GetLastError()));
    return false;
  }
#else
  m_Library = (AimmsLibHandle)dlopen(w2a(path).c_str(), RTLD_NOW);
  if (!m_Library) {
      StoreLastError(L"Couldn't load library" + path + L". " + a2w(dlerror()));
      return false;
  }
#endif
  return true;
}

bool AimmsLoader::Reload(){
    return Load(m_Path);
}

bool AimmsLoader::Unload()
{
  m_Functions.erase(m_Functions.begin(), m_Functions.end());

#ifdef _WIN32
  if (m_Library){
    if (!FreeLibrary((HMODULE)m_Library)){
      StoreLastError(L"Couldn't unload library" + m_Path + L". Exit with " + std::to_wstring(GetLastError()));
      return false;
    }
  }
#else
  if (m_Library) {
    if (dlclose(m_Library) != 0){
      StoreLastError(L"Couldn't unload library" + m_Path + L". " + a2w(dlerror()));
      return false;
    }
  }
#endif
  m_Library = nullptr;
  return true;
}

AimmsLoader::~AimmsLoader()
{
  Unload();
  m_LastError = L"";
  m_Path = L"";
}

std::wstring AimmsLoader::LastError() const
{
  return m_LastError;
}

void AimmsLoader::StoreLastError(std::wstring msg)
{
  m_LastError = msg;
}
};

