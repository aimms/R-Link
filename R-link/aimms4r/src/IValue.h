#pragma once
#include "IDataTable.h"
namespace Aimms{
    class IValue{
    public:
      virtual ~IValue(){}
      virtual void setupValueType(const Aimms::ValueType& type ) = 0;
      virtual void addValue(Value& val) = 0;
      virtual void getValue(Value& val) = 0;

    };
}
