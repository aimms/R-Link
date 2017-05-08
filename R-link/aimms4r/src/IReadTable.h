#pragma once
#include "DataTable.h"

namespace Aimms{

class IReadTable : public R::DataTable{
public:
  virtual void addRow(
      const std::vector<Aimms::Value>& types ) = 0;
};
};
