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
#include <vector>
#include <memory>
#include <iostream>

namespace Aimms{

  enum ValueType{ String, Double, Int };
  struct Value{
    wchar_t* String;
    double* pDouble;
    int* pInteger;

    Value()
      :String(nullptr)
      ,pDouble(nullptr)
      ,pInteger(nullptr)
    {}

    Value(wchar_t* pS, double* pD, int* pI)
      :String(pS), pDouble(pD), pInteger(pI){}
    Value(wchar_t* pS)
      :String(pS){}
    Value(double* pD)
      :pDouble(pD){}
    Value(int* pI)
      :pInteger(pI){}
    ~Value(){}
    void clear(){
      if(String){
        delete String;
        String = nullptr;
      }
    }

  };

  class IDataTable{
  public:
    virtual void setupColumns(
        const std::vector<std::wstring>& names,
        const std::vector<ValueType>& types ) = 0;
  };
};
