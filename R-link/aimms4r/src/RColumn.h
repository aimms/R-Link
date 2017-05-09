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
#include "IColumn.h"
#include "Utilities.h"
#include <wchar.h>
#include "AimmsException.h"

namespace R{

template <typename VectorType>
class Column : public IColumn{
public:


  Column( std::string name ) : m_Name(name),
  m_Id(0){};
  Column( int id , std::string name ) : m_Name(name),
  m_Id(id){};

  virtual ~Column(){
    if(m_Last){
         delete m_Last;
    }
  }

  virtual void addValue( const Aimms::Value& val ) override {
    // should never be reached
    throw std::runtime_error("internal error: no specialization found for Column type");
  }

  virtual void commit(Rcpp::DataFrame::Parent& dfObj, Rcpp::CharacterVector& names) override{
    dfObj.push_back(m_Vector);
    names.push_back(m_Name);
  }
  VectorType& getColumn() const{return m_Vector;}
  SEXP createSExpr();
  virtual Aimms::Value* getValue(int pos) ;
  virtual void getValue(Rcpp::DataFrame& dfObj, int pos, Aimms::Value&);

private:
  wchar_t *m_Last = nullptr; // We keep a reference to the string allocated by Columnn<Rcpp::CharacterVector>:: getValue()::wcsdup & free it in the end
  std::string m_Name;
  VectorType m_Vector;
  int m_Id;
};

template<>
inline void Column<Rcpp::IntegerVector>::addValue(const Aimms::Value& val){
  m_Vector.push_back(*val.pInteger);
}

template<>
inline void Column<Rcpp::NumericVector>::addValue(const Aimms::Value& val){
  m_Vector.push_back(*val.pDouble);
}

template<>
inline void Column<Rcpp::CharacterVector>::addValue(const Aimms::Value& val){
  std::wstring strVal ( val.String);
  m_Vector.push_back(w2a(strVal ) );
}


template<>
inline SEXP Column<Rcpp::IntegerVector>::createSExpr(){
  if(m_Vector.size() >= 1){
      return Rcpp::wrap(m_Vector[0]);
  }
  return Rcpp::wrap(  "" );
}

template<>
inline SEXP Column<Rcpp::NumericVector>::createSExpr(){
  if(m_Vector.size() >= 1){
      return Rcpp::wrap(m_Vector[0]);
  }
  return Rcpp::wrap(  "" );
}

template<>
inline SEXP Column<Rcpp::CharacterVector>::createSExpr(){
  if(m_Vector.size() >= 1){
      return Rcpp::wrap( Rcpp::as<std::string> (m_Vector[0]) );
  }
  return Rcpp::wrap(  "" );
}

template<>
inline Aimms::Value* Column<Rcpp::IntegerVector>::getValue(int pos){
  Aimms::Value* val = new Aimms::Value(  &m_Vector[pos]  );
  return val;
}

template<>
inline Aimms::Value* Column<Rcpp::NumericVector>::getValue(int pos){
  Aimms::Value* val = new Aimms::Value(  &m_Vector[pos]  );
  return val;
}

template<>
inline Aimms::Value* Column<Rcpp::CharacterVector>::getValue(int pos){
  std::string label  (  m_Vector[pos] );
  wchar_t* p = const_cast<wchar_t*>( a2w(label).c_str() );
  Aimms::Value* val = new Aimms::Value(  p  );
  return val;
}


template<>
inline void Column<Rcpp::IntegerVector>::getValue(Rcpp::DataFrame& dfObj, int pos, Aimms::Value& dest){
  Rcpp::IntegerVector conv( dfObj[m_Id]);
  dest.pInteger = &conv[pos];
}

template<>
inline void  Column<Rcpp::NumericVector>::getValue(Rcpp::DataFrame& dfObj, int pos, Aimms::Value& dest){
  Rcpp::NumericVector conv( dfObj[m_Id]);
  dest.pDouble = &(conv[pos]);
}

template<>
inline void  Column<Rcpp::CharacterVector>::getValue(Rcpp::DataFrame& dfObj, int pos, Aimms::Value& dest){
  auto p = a2w(std::string( (Rcpp::CharacterVector( dfObj[m_Id]) )[pos] ) );
  wchar_t* copy = wcsdup(  p.c_str() );
  if(dest.String){
    free(dest.String);
  }
  dest.String = copy;
  m_Last = copy;
}


};
