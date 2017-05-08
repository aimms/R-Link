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
