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
