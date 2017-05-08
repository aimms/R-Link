#pragma once
#include "IValue.h"
#include <memory>
#include "ReadTable.h"
#include "IDataTable.h"

namespace R{

class Value : public Aimms::IValue{
public:
  Value()
    :m_Df(nullptr){}
  Value(SEXP dfObj){
      m_Df = dfObj;
  }
  virtual ~Value(){}
  SEXP createSExpr(){
      return m_Value->createSExpr();
  }
  virtual void setupValueType(const Aimms::ValueType& type ) override;
  void addValue(Aimms::Value& val) override;
  void getValue(Aimms::Value&) override;
private:
  std::shared_ptr<IColumn> m_Value;
  SEXP m_Df;
};
};
