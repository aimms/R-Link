#pragma once
#include "IDataTable.h"
#include <Rcpp.h>

namespace R{

class IColumn{
public:
  virtual ~IColumn(){};
  virtual void addValue( const Aimms::Value& val ) = 0;
  virtual void commit(Rcpp::DataFrame::Parent& dfObj, Rcpp::CharacterVector& names) = 0;
  virtual SEXP createSExpr() = 0;
  virtual void getValue(Rcpp::DataFrame& dfObj, int pos, Aimms::Value&) = 0;

};

};
