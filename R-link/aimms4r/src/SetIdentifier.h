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
#include "BaseIdentifier.h"
#include "Utilities.h"
#include "AimmsException.h"
#include <map>

namespace Aimms{

class SetIdentifier : public BaseIdentifier{
public:
  enum SetElementType{
    STRING,
    INTEGER
  };

private:
  SetElementType m_Type;
  AimmsStringW m_CachedElementName;

  std::vector<const wchar_t*> m_Labels;
  std::map<std::wstring,int> m_LabelsToElements;
  int m_First = -1;
  int m_Card = -1;
public:

  SetIdentifier(std::wstring identifierName,
                std::wstring identifierPrefix,
                std::shared_ptr<IAimms> api,
                int handle,
                SetElementType elementType)
    : BaseIdentifier(identifierName, identifierPrefix, api, handle)
  ,  m_Type(elementType){

    static const size_t bufLen = 300;
    static wchar_t buf[bufLen];
    m_CachedElementName.Length = bufLen;
    m_CachedElementName.String = &buf[0];

    if( api->ValueCard(handle, &m_Card) ){
      m_Labels.resize(m_Card);
    }
    else{
      throw AimmsException("SetIdentifier::ctor::ValueCard", m_API);
    }

    if(m_API->SetOrdinalToElement(m_Handle, 1, &m_First)){
    }
    else{
      throw AimmsException("SetIdentifier::ctor::SetOrdinalToElement",m_API);
    }

    if(!load()) throw AimmsException("SetIdentifier::ctor::load", m_API);

  }

  int getCard() const{return (int)m_Labels.size();}

  std::wstring getElementName(int elem){
    return m_First != -1 && m_Card != -1 ? m_Labels[elem - m_First] : L"";
  }

  int getElementFromLabel(std::wstring label){
      if( m_LabelsToElements.find(label) == m_LabelsToElements.end() ){
          throw AimmsException("Couldn't find set element with label " + w2a( label) + " in Set "+w2a(m_IdentifierName) );
      }
      return m_LabelsToElements[label] + m_First;
  }


  bool load(){
    if(m_Card == -1 || m_First == -1) return false;

    int elem = 0;
    int i = 1;
    int ret = 0;
    std::map<std::wstring,int>::const_iterator it;
    while (true){
      ret = m_API->SetOrdinalToElement(m_Handle, i, &elem);
      if (!ret) break;
      /*std::wcout << elem  << "-" << m_First << " = "<< elem - m_First << std::endl;*/

      m_CachedElementName.Length = 300;
      ret = m_API->SetElementToNameW(m_Handle, elem, &m_CachedElementName);

      m_LabelsToElements[m_CachedElementName.String] = elem - m_First;
      it = m_LabelsToElements.find(m_CachedElementName.String);
      m_Labels[elem - m_First] = it->first.c_str();
      i++;
    }
    return true;

  }

  const SetElementType& getType() const{
    return m_Type;
  }

  virtual ~SetIdentifier(){ }

};
};
