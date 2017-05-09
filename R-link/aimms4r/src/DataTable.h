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
#include "RColumn.h"
#include "IDataTable.h"
namespace R{

class DataTable : public Aimms::IDataTable{
public:
  int size(){return m_Column.size();}
  virtual void setupColumns(
      const std::vector<std::wstring>& names,
      const std::vector<Aimms::ValueType>& types ) override{

        m_Column.resize(names.size(), nullptr);
        for (size_t i = 0; i < names.size(); ++i){
          //std::wcout << "Column " << i << " '" << names[i] << "' is ";
          switch (types[i]){
          case Aimms::ValueType::String:
            m_Column[i] = std::make_shared< R::Column<Rcpp::CharacterVector> >(i, w2a(names[i]));
            break;
          case Aimms::ValueType::Int:
            m_Column[i] = std::make_shared< R::Column<Rcpp::IntegerVector> >(i, w2a(names[i]));
            break;
          case Aimms::ValueType::Double:
            m_Column[i] = std::make_shared< R::Column<Rcpp::NumericVector> >(i, w2a(names[i]));
            break;
          default:
            throw std::runtime_error("setupColumns: unexpected type");
          break;
          }
        }
      }
  public:
  DataTable(){}
  ~DataTable(){
    //std::wcout << "~DataTable dtor()" << std::endl;
    m_Column.clear();
  }

protected:
  std::vector<std::shared_ptr<R::IColumn>> m_Column;

  };
};
