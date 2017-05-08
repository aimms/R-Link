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
