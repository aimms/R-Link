#pragma once
#include "LoadedAimmsAPI.h"
#include <memory>

namespace Aimms{
class AimmsAPI{
public:
    static std::unique_ptr<IAimms> getAimmsAPI(std::wstring aimmsLibPath){
        LoadedAimmsAPI* api = new LoadedAimmsAPI(aimmsLibPath);
        if(!api->IsAPILoaded() ){delete api;api = nullptr; return nullptr;}
        std::unique_ptr<IAimms> ret (api);
        return ret;
    }

  static std::shared_ptr<IAimms> getSharedAimmsAPI(std::wstring aimmsLibPath){
    LoadedAimmsAPI* api = new LoadedAimmsAPI(aimmsLibPath);
    if(!api->IsAPILoaded() ){delete api;api = nullptr; return nullptr;}
    std::shared_ptr<IAimms> ret (api);
    return ret;
  }

};
}
