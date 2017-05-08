#pragma once
#include "SetIdentifier.h"
#include "Column.h"
#include "IValue.h"
#include "IReadTable.h"
#include "IWriteTable.h"

namespace Aimms{

class MultiDimIdentifier : public BaseIdentifier{
public:
  enum StorageType{
    STRING,
    INTEGER,
    DOUBLE,
    ELEMENT
  };
public:
  MultiDimIdentifier(
    std::wstring identifierName,
    std::wstring identifierPrefix,
    std::shared_ptr<IAimms> api,
    int handle,
    std::vector<std::shared_ptr<SetIdentifier>> domains,
    StorageType storageType,
    std::shared_ptr<SetIdentifier> range)
    : BaseIdentifier(identifierName, identifierPrefix, api, handle)
  , m_Domain(domains)
  , m_StorageType(storageType)
  , m_Range(range){

    setupColumns();
  }

  virtual ~MultiDimIdentifier(){
    //std::wcout << "MultiDim dtor" << std::endl;
  }

  size_t getDimension() const{
    return m_Domain.size();
  }

  std::shared_ptr<SetIdentifier> getDomain(size_t pos) const{
    return m_Domain[pos];
  }

  private:
  void setupColumns(){
    for (auto &set : m_Domain){
      switch (set->getType()){
      case SetIdentifier::INTEGER:
        m_Columns.push_back(ColumnFactory::getSetIntegerColumn(set->getIdentifierName(), set));
        break;
      case SetIdentifier::STRING:
        m_Columns.push_back(ColumnFactory::getSetStringColumn(set->getIdentifierName(), set));
        break;
      default:
        throw std::runtime_error("unknown set type");
      }
    }

    switch(m_StorageType){
    case STRING:
      m_Value = ColumnFactory::getStringColumn(m_IdentifierName);
      break;
    case INTEGER:
      m_Value = ColumnFactory::getIntegerColumn(m_IdentifierName);
      break;
    case DOUBLE:
      m_Value = ColumnFactory::getDoubleColumn(m_IdentifierName);
      break;
    case ELEMENT:
      switch (m_Range->getType()){
      case SetIdentifier::INTEGER:
        m_Value = ColumnFactory::getSetIntegerColumn(m_Range->getIdentifierName(), m_Range);
        break;
      case SetIdentifier::STRING:
        m_Value = ColumnFactory::getSetStringColumn(m_Range->getIdentifierName(), m_Range);
        break;
      default:
        throw std::runtime_error("unknown range set type");
      }
      break;
    default:
      throw std::runtime_error("unknown storage type");
    }
  }

public:
  bool loadScalarFrom(Aimms::IValue &scalarValue){
      static const int nBuf = 300;
      static wchar_t buf[nBuf];
      AimmsValueW value;
      value.String = &buf[0];
      value.Length = nBuf;

      Aimms::Value ascalarValue;
      scalarValue.setupValueType( m_Value->getType() );
      scalarValue.getValue(ascalarValue);

      m_Value->getConvertor()->assignAimmsValue(ascalarValue, value);
      int ret = m_API->ValueAssignW (m_Handle, nullptr, &value);
      if(!ret){
        throw AimmsException("MultiDimIdentifier::loadScalarFrom", m_API);
      }

      return false;
  }

  bool storeScalarTo(Aimms::IValue &scalarValue){
    static const int nBuf = 300;
    static wchar_t buf[nBuf];
    AimmsValueW value;
    value.String = &buf[0];
    value.Length = nBuf;

    int ret = m_API->ValueRetrieveW (m_Handle, nullptr, &value);

    if(ret){
      Aimms::Value ascalarValue;
      m_Value->getConvertor()->assignValue(value, ascalarValue );
      scalarValue.setupValueType( m_Value->getType() );
      scalarValue.addValue(ascalarValue);

    }
    return false;
  }

  void storeTo(Aimms::IReadTable& dt){

    if (!m_Domain.size()){
      throw std::runtime_error("not yet for scalars");
    }

    setupTableColumns(&dt);
    int ret = m_API->ValueResetHandle(m_Handle);
    if (ret == 0){
      throw AimmsException("MultiDimIdentifier::storeTo", m_API);
    }

    std::vector<int> tuple;
    tuple.assign( m_Domain.size() , 0);

    std::vector<Aimms::Value> row(m_Domain.size()+1);

    static const int nBuf = 256;
    static wchar_t buf[nBuf];
    AimmsValueW value;
    value.String = &buf[0];
    value.Length = nBuf;

    while (true){
      ret = m_API->ValueNextW(m_Handle, &tuple[0], &value);
      if(!ret) break;
      for (size_t i = 0; i < m_Columns.size(); ++i){
        m_Columns[i]->getConvertor()->assignIndex( tuple[i], row[i] );
      }

      m_Value->getConvertor()->assignValue(value, row[m_Domain.size()] );
      dt.addRow(row);
    }

  }

  bool loadFrom(R::IWriteTable& wt){
    if(!m_Domain.size()){
      throw AimmsException("We do not support scalars yet");
    }
    setupTableColumns(&wt);
    int domainSize = (int) m_Domain.size();

    int currentRow = 0;
    int itemsInColumn = wt.columnSize();
    /*std::wcout << "Columnsize =>" << itemsInColumn << " size = " << wt.size() << std::endl;*/
    if(wt.size() != domainSize + 1){
      throw AimmsException("Identifier Dimension/Data Frame Column mismatch:  "+
                           w2a(getIdentifierName())+" has " + std::to_string( m_Domain.size() ) +
                           "+1 dimensions, inserted data frame has "+std::to_string(wt.size())+" columns");
    }

    int ret = m_API->IdentifierEmpty(m_Handle);
    if (ret == 0){
      throw AimmsException("MultiDimIdentifier::loadFrom::IdentifierEmpty",m_API);
    }

    AimmsValueW v;
    static const int nBuf = 300;
    static wchar_t buf[nBuf];
    v.String = &buf[0];
    v.Length = nBuf;

    std::vector<int> row;
    std::vector<Aimms::Value> values (domainSize + 1);

    while(currentRow < itemsInColumn){
        wt.getRow(currentRow, values);

        for(int i = 0;i<domainSize;i++){
             m_Columns[i]->getConvertor()->assignAimmsIndex(values[i],row );
        }

        m_Value->getConvertor()->assignAimmsValue( values[domainSize] ,v);

        if(!m_API->ValueAssignW(m_Handle, &row[0], &v) ){
            throw AimmsException("MultiDimIdentifier::loadFrom::assignAimmsValue",m_API);
        }

        currentRow++;
        row.clear();
    }

    return true;
  }

private:
  void setupTableColumns(Aimms::IDataTable *dt){
    std::vector<std::wstring> names;
    std::vector<ValueType> types;
    for (auto &col : m_Columns){
      names.push_back(col->getName());
      types.push_back(col->getType());
    }
    names.push_back(m_Value->getName());
    types.push_back(m_Value->getType());

    dt->setupColumns(names, types);
  }

  protected:
    std::vector<std::shared_ptr<SetIdentifier>> m_Domain;
    StorageType m_StorageType;
    std::shared_ptr<SetIdentifier> m_Range;

    std::vector<std::unique_ptr<ColumnBase>> m_Columns;
    std::unique_ptr<ColumnBase> m_Value;
  };
};

