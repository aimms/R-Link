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
  {
  }

  virtual ~BaseIdentifier(){

    if (m_API && !m_ProjectIsClosed){
      if (m_API->IdentifierHandleDelete(m_Handle)){

      }
    }
  }

  virtual void setProjectIsClosed(bool projectIsClosed){
      m_ProjectIsClosed = projectIsClosed;
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
  bool m_ProjectIsClosed = true;
};
};
