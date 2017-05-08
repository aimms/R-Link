#pragma once
#include <string>
#include <memory>
#include "IAimms.h"
#include "Utilities.h"

namespace Aimms{
class BaseIdentifier{

public:
  BaseIdentifier(std::wstring identifierName, std::wstring identifierPrefix, std::shared_ptr<IAimms> api, int handle)
    : m_IdentifierName(identifierName)
  , m_IdentifierPrefix(identifierPrefix)
  , m_API(api)
  , m_Handle(handle)
  {}

  virtual ~BaseIdentifier(){

    if (m_API){
      if (m_API->IdentifierHandleDelete(m_Handle)){
      }
    }
  }

  const std::wstring& getIdentifierPrefix() const{
    return m_IdentifierPrefix;
  }
  const std::wstring& getIdentifierName() const{
    return m_IdentifierName;
  }

protected:
  std::wstring m_IdentifierName;
  std::wstring m_IdentifierPrefix;
  std::shared_ptr<IAimms> m_API;
  int m_Handle;
};
};
