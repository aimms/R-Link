#pragma once
#include <memory>
#include <string>
#include "Convertors.h"

namespace Aimms{
class SetIdentifier;

class ColumnBase{
  virtual bool isColumn() = 0;
public:
  virtual ~ColumnBase(){};

  virtual std::wstring getName() const = 0;
  virtual ValueType getType() const = 0;
  virtual std::shared_ptr<IValueConvertor> getConvertor() const = 0;


  virtual void push(const AimmsValueW& aimmsValue) = 0;
  virtual void push(std::wstring&) = 0;
};

template <class T>
class Column : public ColumnBase{
private:
  std::vector<T> values;
  std::wstring m_Name;
  ValueType m_Type;
  std::shared_ptr<IValueConvertor> m_Convertor;
public:
  Column(std::wstring name, ValueType valueType, std::shared_ptr<IValueConvertor> valueConvertor)
    :m_Name(name)
    ,m_Type(valueType)
    ,m_Convertor(valueConvertor)
  {}
  virtual ~Column(){}

  virtual std::wstring getName() const override {
    return m_Name;

  }
  virtual ValueType getType() const override{
    return m_Type;
  }
  virtual std::shared_ptr<IValueConvertor> getConvertor() const override{
    return m_Convertor;
  }


  virtual void push(std::wstring&);
  virtual void push(const AimmsValueW& aimmsValue);
  virtual bool isColumn() override { return true; }
  void push(T value){ values.push_back(value); }
  std::vector<T> getValues(){ return values; }
  T getValue(int pos){ return values[pos]; }
};

template <>
void Column<int>::push(const AimmsValueW& aimmsValue) {
  this->push(aimmsValue.Int);
};

template <>
void Column<std::wstring>::push(const AimmsValueW& aimmsValue) {
  this->push(aimmsValue.String);
};

template <>
void Column<double>::push(const AimmsValueW& aimmsValue) {
  this->push(aimmsValue.Double);
};

template <>
void Column<std::wstring>::push(std::wstring& s) {
  values.push_back(s);
};

template <>
void Column<int>::push(std::wstring& s) {
  values.push_back(std::stoi(s));
};

template <>
void Column<double>::push(std::wstring& s) {
  values.push_back(std::stof(s));
};

class ColumnFactory{
public:
  static std::unique_ptr<ColumnBase> getSetStringColumn(const std::wstring& name, std::shared_ptr<SetIdentifier> set){
    std::shared_ptr<SetConvertor<std::wstring>> p(new SetConvertor<std::wstring>(set) );
    std::unique_ptr<Column<std::wstring>> ret(new Column<std::wstring>(name, ValueType::String,p));
    return std::move(ret);
  }
  static std::unique_ptr<ColumnBase> getSetIntegerColumn(const std::wstring& name, std::shared_ptr<SetIdentifier> set){
    std::shared_ptr<SetConvertor<int>> p(new SetConvertor<int>(set) );
    std::unique_ptr<Column<int>> ret(new Column<int>(name, ValueType::Int,p) );
    return std::move(ret);
  }
  static std::unique_ptr<ColumnBase> getStringColumn(const std::wstring& name){
    std::shared_ptr<StandardConvertor<std::wstring>> p(new StandardConvertor<std::wstring> );
    std::unique_ptr<Column<std::wstring>> ret(new Column<std::wstring>(name, ValueType::String,p));
    return std::move(ret);
  }
  static std::unique_ptr<ColumnBase> getIntegerColumn(const std::wstring& name){
    std::shared_ptr<StandardConvertor<int>> p(new StandardConvertor<int> );
    std::unique_ptr<Column<int>> ret(new Column<int>(name, ValueType::Int,p) );
    return std::move(ret);
  }
  static std::unique_ptr<ColumnBase> getDoubleColumn(const std::wstring& name){
    std::shared_ptr<StandardConvertor<double>> p(new StandardConvertor<double> );
    std::unique_ptr<Column<double>> ret(new Column<double>(name, ValueType::Double,p));
    return std::move(ret);
  }
};
};
