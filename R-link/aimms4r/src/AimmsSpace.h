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
#include <memory>
#include "SetIdentifier.h"
#include "MultiDimIdentifier.h"

namespace Aimms{
class AimmsSpace{
public:
  AimmsSpace(std::shared_ptr<IAimms> api)
    :m_API(api){}

  ~AimmsSpace(){
    clear();
  }
public:
  void clear(){
      m_Sets.clear();
  }

  void setAimmsAPI(std::shared_ptr<IAimms> api)
  {
    clear();
    m_API = api;
  }

  int getActiveSets(){
    return m_Sets.size();
  }

  void setProjectIsClosed(bool isClosed){
      for(auto &s: m_Sets){
        s.second->setProjectIsClosed(isClosed);
      }
  }

  std::shared_ptr<BaseIdentifier> getIdentifier(const std::wstring &name) const{
    std::map<std::wstring, std::shared_ptr<SetIdentifier>>::const_iterator setIt = m_Sets.find(name);
    if (setIt == m_Sets.end()){ return nullptr;}
    return (*setIt).second;
  }


  bool identifierIsLoaded(const std::wstring& idName) const {
   if(m_Sets.find(idName) == m_Sets.end()){ return false;}
   return true;
  }

  std::shared_ptr<MultiDimIdentifier> obtainIdentifier(const std::wstring &name){
    return createMultiDimIdentifier(name);
  }

  std::shared_ptr<SetIdentifier> getSet(std::wstring & name){
    if (m_Sets.find(name) == m_Sets.end()) return nullptr;
    return m_Sets[name];
  }


private:

  void splitName( const std::wstring& fullName, std::wstring& identifierName, std::wstring& identifierPrefix  ){
    std::wstring::size_type pos = fullName.rfind(L"::");
    if (pos == std::wstring::npos){
      // not found
      identifierName = fullName;
      identifierPrefix = L"";
    }
    else{
      identifierName = fullName.substr(pos + 2);
      identifierPrefix = fullName.substr(0, pos);
    }
  }


  // helper methods:
  std::wstring getAttributeName(int identifierHandle)
  {
    AimmsStringW astrValue;
    static const int bufLen = 256;
    static wchar_t szBuf[bufLen];
    astrValue.String = &szBuf[0];
    astrValue.Length = bufLen;

    int ret = 0;

    ret = m_API->AttributeNameW(identifierHandle, &astrValue);
    if (ret == 0){
      throw AimmsException("AimmsSpace::getAttributeName", m_API);
    }
    return std::wstring(astrValue.String);
  }

  int getAttributeType(int identifierHandle){
    int A3APItype = 0;
    int ret = 0;
    ret = m_API->AttributeType(identifierHandle, &A3APItype);
    if (ret == 0){
      throw AimmsException("AimmsSpace::getAttributeType", m_API);
    }
    return A3APItype;
  }

  int getAttributeDimension(int identifierHandle){
    int dimSlice = -1;
    int dimension = -1;
    int ret = 0;
    ret = m_API->AttributeDimension(identifierHandle, &dimension, &dimSlice);
    if (ret == 0){
      throw AimmsException("AimmsSpace::getAttributeDimension", m_API);
    }
    if (dimension != dimSlice){
      throw std::runtime_error("dimension unequal to slice dimension");
    }
    return dimension;
  }

  MultiDimIdentifier::StorageType getStorageDataType( int identifierHandle){
    int storagetype = -1;
    int ret = 0;
    ret = m_API->AttributeStorage(identifierHandle, &storagetype);
    if (ret == 0){
      throw AimmsException("AimmsSpace::getStorageDataType", m_API);
    }
    switch (storagetype){
    case STORAGE_BINARY:
      return MultiDimIdentifier::INTEGER;
      break;
    case STORAGE_INT:
      return MultiDimIdentifier::INTEGER;
      break;
    case STORAGE_DOUBLE:
      return MultiDimIdentifier::DOUBLE;
      break;
    case STORAGE_STRING:
      return MultiDimIdentifier::STRING;
      break;
    default:
      throw std::runtime_error("Unsupported storage type");
    }
  }

  int getAttributeElementRange( int identifierHandle){
    int rangeHandle = 0;
    int ret = 0;
    ret = m_API->AttributeElementRange(identifierHandle, &rangeHandle);
    if (ret == 0){
      throw AimmsException("AimmsSpace::getAttributeElementRange", m_API);
    }
    return rangeHandle;
  }

  std::vector<int> getAttributeRootDomain( int identifierHandle){
    std::vector<int> vectorRootDomainHandles;

    int dim = getAttributeDimension(identifierHandle);
    if (dim == 0){
      // we're done
    }
    else if (dim == 1){
      int ret = 0;
      vectorRootDomainHandles.resize(dim, 0);
      ret = m_API->AttributeRootDomain(identifierHandle, &vectorRootDomainHandles[0]);
      if (ret == 0){
        throw AimmsException("AimmsSpace::getAttributeRootDomain", m_API);
      }
    }
    else{
      throw std::runtime_error("getAttributeRootDomain: Unexpected amount of root domains");
    }
    return vectorRootDomainHandles;
  }

  std::vector<int> getAttributeDeclarationDomain(int identifierHandle){
    std::vector<int> vectorDeclarationDomainHandles;

    int dim = getAttributeDimension(identifierHandle);
    if (dim == 0){
      // we're done

    }
    else {
      vectorDeclarationDomainHandles.resize(dim, 0);

      int ret = 0;
      ret = m_API->AttributeDeclarationDomain(identifierHandle, &vectorDeclarationDomainHandles[0]);
      if (ret == 0){
        throw AimmsException("AimmsSpace::getAttributeDeclarationDomain", m_API);
      }
    }
    return vectorDeclarationDomainHandles;
  }

  std::shared_ptr<SetIdentifier> createSetIdentifier( int handle ){
    std::wstring fullName = getAttributeName(handle);

    std::wstring identifierName;
    std::wstring identifierPrefix;

    splitName(fullName, identifierName, identifierPrefix);
    SetIdentifier::SetElementType elementType = SetIdentifier::STRING;

    int A3APItype = getAttributeType( handle);
    switch (A3APItype){
    case SIMPLE_SET_ROOT:
    case SIMPLE_SET_SUBSET:
    {
      std::vector<int> rootSets = getAttributeRootDomain(handle);
      if (rootSets.size() > 1){
        throw std::runtime_error("too many root sets");
      }

      std::wstring rootSetName;
      std::wstring rootSetPrefix;

      if (rootSets.size() == 0){
        rootSetName = identifierName;
        rootSetPrefix = identifierPrefix;
      }
      else{
        std::wstring fullRootSetName = getAttributeName(rootSets[0]);
        splitName(fullRootSetName, rootSetName, rootSetPrefix);
      }
      if (rootSetName == L"Integers"){
        elementType = SetIdentifier::INTEGER;
      }

      for (auto hSet : rootSets){
        m_API->IdentifierHandleDelete(hSet);
      }
      break;
    }
    default:
      {
        std::wostringstream wosTmp;
        wosTmp << L"unsported set type for '" << fullName << L"'";
        throw std::runtime_error(w2a(wosTmp.str()));
      }
    }

    return std::make_shared<SetIdentifier>(identifierName, identifierPrefix, m_API, handle, elementType);
  }


  std::shared_ptr<MultiDimIdentifier> createMultiDimIdentifier( std::wstring idName ){
    int  idHandle = 0;
    int* domain = nullptr;
    int* slicing = nullptr;
    int  flags = 0;

    int ret = m_API->IdentifierHandleCreateW(const_cast<wchar_t*>(idName.c_str()), domain, slicing, flags, &idHandle);
    if (ret == 0){
      throw AimmsException("AimmsSpace::createMultiDimIdentifier", m_API);
    }

    std::wstring fullName = getAttributeName(idHandle);

    std::wstring identifierName;
    std::wstring identifierPrefix;

    splitName(fullName, identifierName, identifierPrefix);

    MultiDimIdentifier::StorageType storageType;
    std::shared_ptr<SetIdentifier> range;

    // get the storage type
    int A3APItype = getAttributeType(idHandle);
    switch (A3APItype){
    case PARAMETER_NUMERICS:
    case VARIABLE:
      storageType = getStorageDataType(idHandle);
      break;
    case PARAMETER_ELEMENTS:
    case ELEMENT_VARIABLE:
      storageType = MultiDimIdentifier::ELEMENT;
      range = getOrCreateSet(getAttributeElementRange(idHandle));
      break;
    case PARAMETER_STRINGS:
      storageType = MultiDimIdentifier::STRING;
      break;
    default:
      {
        std::wostringstream wosTmp;
        wosTmp << "unsported identifier type for '" << idName << L"'";
        throw std::runtime_error(w2a(wosTmp.str()));
      }
    }
    std::vector<std::shared_ptr<SetIdentifier>> declarationDomain;

    std::vector<int> vectorAttributeDeclarationDomain = getAttributeDeclarationDomain(idHandle);
    for (auto aHandle : vectorAttributeDeclarationDomain){
      auto hSet = aHandle;
      switch (getAttributeType(aHandle)){
      case INDEX:
        std::vector<int> declaringSets = getAttributeDeclarationDomain(aHandle);
        if (declaringSets.size() != 1){
          throw std::runtime_error("unable to determine declaration domain");
        }
        hSet = declaringSets[0];
        break;
      }
      declarationDomain.push_back(getOrCreateSet(hSet));
      m_API->IdentifierHandleDelete(aHandle);

    }
    return std::make_shared<MultiDimIdentifier>(identifierName, identifierPrefix, m_API, idHandle, declarationDomain, storageType, range);
  }

  std::shared_ptr<SetIdentifier> getOrCreateSet(int setHandle){
    auto setName = getAttributeName(setHandle);
    if(m_Sets.find(setName) != m_Sets.end()){
      //std::wcout << setName << "was cached!" << std::endl;
      return m_Sets[setName];
    }

    auto theNewSet = createSetIdentifier(setHandle);
    m_Sets[setName] = theNewSet;
    return theNewSet;
  }

private:
 // std::shared_ptr<AimmsLoader> m_Loader;
  std::shared_ptr<IAimms> m_API;
  std::map<std::wstring, std::shared_ptr<SetIdentifier>> m_Sets;
};
};
