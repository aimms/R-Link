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
#include "RValue.h"
#include "RColumn.h"
#include "AimmsException.h"

namespace R{
void Value::setupValueType(const Aimms::ValueType& type  ){
    switch (type){
    case Aimms::ValueType::String:
      m_Value = std::make_shared< Column<Rcpp::CharacterVector> >("Value");
      //std::wcout << "String!!!";
      break;
    case Aimms::ValueType::Int:
      //  std::wcout << "Int";
      m_Value = std::make_shared< Column<Rcpp::IntegerVector> >("Value");
      break;
    case Aimms::ValueType::Double:
      // std::wcout << "Double";
      m_Value = std::make_shared< Column<Rcpp::NumericVector> >("Value");
      break;
    default:
      throw std::runtime_error("setupColumn: unexpected type");
    break;
    }
}

void Value::getValue(Aimms::Value& dest){
    if(!m_Df){
      throw Aimms::AimmsException("Dataframe not set when trying to write Scalar");
    }

    Rcpp::DataFrame df(m_Df);
    if(df.size() > 1){
      throw Aimms::AimmsException("Writing to a scalar identifier requires 1 data frame dimension/column");
    }
    m_Value->getValue(df, 0, dest);
}

void Value::addValue(Aimms::Value& val){
    m_Value->addValue(val);
}

};
