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
#include "IWriteTable.h"
#include <Rcpp.h>

namespace R{
    class WriteTable : public IWriteTable{
    private:
        Rcpp::DataFrame& m_Df;
    public:
        WriteTable(Rcpp::DataFrame *df)
        :m_Df(*df){}

        virtual int columnSize() const {
          if(!m_Column.empty()){
            Rcpp::List list = m_Df(0);
            return list.size();
          }
          return -1;
        }

        virtual int size(){
          return m_Df.size();
        }
        virtual ~WriteTable(){}

        virtual void getRow(int pos, std::vector<Aimms::Value> & v) override{
            int last = m_Column.size() -1;
            for(int i = 0;i<last ;i++){
                m_Column[i]->getValue(m_Df, pos , v[i]);
            }

            m_Column[last]->getValue(m_Df, pos, v[last]);
        }

    };
}
