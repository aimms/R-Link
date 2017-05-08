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
