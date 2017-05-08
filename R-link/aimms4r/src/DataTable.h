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
