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
