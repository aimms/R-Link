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
#ifndef _INCLUDED__Common_CPU
#define _INCLUDED__Common_CPU

#include "Common/GlobalInclude.h"
#include "Common/CommonAPI.h"
#include "Common/ExportSTL.h"
#include <string>

// this is to get rid of warning C4521, which actually is an error
// see also http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
EXPORT_STL_STRING( COMMON_API, char)

namespace Common {

// Query information about the processor.
class COMMON_API CPU {
 public:
  // Constructor
  CPU();

  // Accessors for CPU information.
  const char* vendor_name() const { return cpu_vendor_.c_str(); }
  int stepping() const { return stepping_; }
  int model() const { return model_; }
  int family() const { return family_; }
  int type() const { return type_; }
  int extended_model() const { return ext_model_; }
  int extended_family() const { return ext_family_; }
  int has_mmx() const { return has_mmx_; }
  int has_sse() const { return has_sse_; }
  int has_sse2() const { return has_sse2_; }
  int has_sse3() const { return has_sse3_; }
  int has_ssse3() const { return has_ssse3_; }
  int has_sse41() const { return has_sse41_; }
  int has_sse42() const { return has_sse42_; }

 private:
  // Query the processor for CPUID information.
  void Initialize();

  int type_;  // process type
  int family_;  // family of the processor
  int model_;  // model of processor
  int stepping_;  // processor revision number
  int ext_model_;
  int ext_family_;
  bool has_mmx_;
  bool has_sse_;
  bool has_sse2_;
  bool has_sse3_;
  bool has_ssse3_;
  bool has_sse41_;
  bool has_sse42_;

  std::string cpu_vendor_;
};

}  // namespace base

#endif  // _INCLUDED__Common_CPU
