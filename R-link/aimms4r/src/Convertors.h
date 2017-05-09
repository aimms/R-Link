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
#include "IAimms.h"
#include "IDataTable.h"
#include "SetIdentifier.h"
#include <string>

namespace Aimms{
class IValueConvertor{
public:
  virtual ~IValueConvertor() {};
  virtual void assignIndex(int index, Value& dest) = 0;
  virtual void assignValue(const AimmsValueW& aimmsValue, Value& dest) = 0;
  virtual void assignAimmsIndex(const Value& src, std::vector<int> &dest) = 0;
  virtual void assignAimmsValue(const Value& src, AimmsValueW& dest) = 0;
};

template <typename T>
class StandardConvertor : public IValueConvertor{
public:
  StandardConvertor(){};
  virtual void assignIndex(int index, Value& dest) override{
    throw std::runtime_error("1should not reach");
  };
  virtual void assignValue(const AimmsValueW& aimmsValue, Value& dest) override{
    throw std::runtime_error("2should not reach");
  }
  virtual void assignAimmsIndex(const Value& src, std::vector<int> &dest) override{
    throw std::runtime_error("3should not reach");
  }

  virtual void assignAimmsValue( const Value& src, AimmsValueW& dest) override{
    throw std::runtime_error("4should not reach");
  }
};

template <>
void StandardConvertor<double>::assignValue(const AimmsValueW& aimmsValue, Value& dest){
  dest.pDouble =  &( const_cast<AimmsValueW&>(aimmsValue).Double );
}
template <>
void StandardConvertor<std::wstring>::assignValue(const AimmsValueW& aimmsValue, Value& dest){
  dest.String = aimmsValue.String;
}
template <>
void StandardConvertor<int>::assignValue(const AimmsValueW& aimmsValue, Value& dest){
  dest.pInteger =  &( const_cast<AimmsValueW&>(aimmsValue).Int );
}

template <>
void StandardConvertor<double>::assignAimmsValue(const Value& src, AimmsValueW& dest){
  dest.Double = *(src.pDouble);
}
template <>
void StandardConvertor<std::wstring>::assignAimmsValue(const Value& src, AimmsValueW& dest){
  dest.Length = wcslen(src.String);
  dest.String = const_cast<wchar_t*>(src.String);
}
template <>
void StandardConvertor<int>::assignAimmsValue(const Value& src, AimmsValueW& dest){
  dest.Int = *(src.pInteger);
}


template <typename T>
class SetConvertor : public IValueConvertor{
private:
  std::shared_ptr<SetIdentifier> m_Set;
  T data;
public:
  SetConvertor(std::shared_ptr<SetIdentifier> set )
    : m_Set(set)
  {};

  virtual ~SetConvertor(){
     // std::wcout << "~SetConvertor dtor()" << std::endl;
  }

  virtual void assignIndex(int index, Value& dest) override {
    throw std::runtime_error("4should not reach");
  };
  virtual void assignValue(const AimmsValueW& aimmsValue, Value& dest) override {
    throw std::runtime_error("5should not reach");
  }
  virtual void assignAimmsValue(const Value& src, AimmsValueW& dest) override {
    throw std::runtime_error("6should not reach");
  }

  virtual void assignAimmsIndex(const Value& src, std::vector<int> &dest) override{
    throw std::runtime_error("7should not reach");
  }
};

template <>
void SetConvertor<std::wstring>::assignAimmsIndex(const Value& src, std::vector<int> &dest){
  dest.push_back(m_Set->getElementFromLabel(src.String));
}

template <>
void SetConvertor<int>::assignAimmsIndex(const Value& src, std::vector<int> &dest){
  dest.push_back(m_Set->getElementFromLabel(std::to_wstring(*(src.pInteger)) ));
}

template <>
void SetConvertor<std::wstring>::assignIndex(int index, Value& dest){
  data = m_Set->getElementName(index);
  dest.String = const_cast<wchar_t*>(data.c_str());
}
template <>
void SetConvertor<int>::assignIndex(int index, Value& dest){
  data = index - 0x3ffffffe;
  dest.pInteger = &data;
}

template <>
void SetConvertor<std::wstring>::assignValue(const AimmsValueW& aimmsValue, Value& dest){
  data = m_Set->getElementName(aimmsValue.Int);
  dest.String = const_cast<wchar_t*>(data.c_str());
}
template <>
void SetConvertor<int>::assignValue(const AimmsValueW& aimmsValue, Value& dest){
  data = aimmsValue.Int - 0x3ffffffe;
  dest.pInteger = &data;
}

template <>
void SetConvertor<std::wstring>::assignAimmsValue(const Value& src, AimmsValueW& dest){
    int elem = m_Set->getElementFromLabel(src.String);
    data = elem;
    dest.Int = elem;
}

template <>
void SetConvertor<int>::assignAimmsValue(const Value& src, AimmsValueW& dest){
    data = *(src.pInteger) - 0x3ffffffe;
    dest.Int = data;
}

}
