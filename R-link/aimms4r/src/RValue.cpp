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
