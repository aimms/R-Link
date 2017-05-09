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
#include "IReadTable.h"
namespace R{
    class ReadTable : public Aimms::IReadTable{
    public:
      ReadTable(){}
      virtual ~ReadTable(){
        //std::wcout <<"~ReadTable dtor()" << std::endl;
        m_Column.clear();
      }

      virtual void addRow( const std::vector<Aimms::Value>& row ) override {
        for (size_t i = 0; i < m_Column.size(); ++i){
            try{
                m_Column[i]->addValue(row[i]);
            }
            catch( ... ){
                std::wcout << "Exception when adding column " << i << std::endl;
                throw;
            }
        }
      }
      Rcpp::DataFrame createDataFrame(){

        Rcpp::DataFrame::Parent dfObj;
        Rcpp::CharacterVector names;
        for (auto column : m_Column){
          column->commit(dfObj, names);
        }
        dfObj.attr("names") = names;
        return Rcpp::DataFrame(dfObj);
      }
    };
};
