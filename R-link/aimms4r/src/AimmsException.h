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
