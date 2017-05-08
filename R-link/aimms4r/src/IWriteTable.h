#pragma once
#include "DataTable.h"
namespace R{

class IWriteTable : public DataTable {
public:
  virtual void getRow(int pos, std::vector<Aimms::Value>& v) = 0;
  virtual int columnSize() const = 0;
  virtual int size() = 0;
};

};
