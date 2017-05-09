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
