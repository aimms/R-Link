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

#include "IAimms.h"
#include "Utilities.h"
#include <stdexcept>
#include <memory>

namespace Aimms{
  class AimmsException : public std::runtime_error{
  public:
    static std::string generateAimmsException(std::string& traceMsg, std::shared_ptr<IAimms> aimms){
      std::wostringstream wosTmp;
      int errorCode = 0;
      static const int nBufSize = 512;
      wchar_t szBuf[nBufSize];

      int ret = aimms->APILastError(&errorCode, szBuf);
      if (ret == 0){
        wosTmp << L"unknown aimmserror";
        return "["+traceMsg+"]::"+w2a(wosTmp.str());
      }
      else{
        wosTmp << L"aimmserror " << errorCode << L": " << szBuf;
      }
      return w2a(wosTmp.str());
    }

    AimmsException( std::string traceMsg, std::shared_ptr<IAimms> aimms ) : std::runtime_error( generateAimmsException(traceMsg, aimms)){};
    AimmsException( std::string msg ) : std::runtime_error(msg){};


  };
};
