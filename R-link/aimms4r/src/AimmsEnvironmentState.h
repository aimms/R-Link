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
#include "IAimms.h"
#include "AimmsAPI.h"
#include <memory>
#include "AimmsSpace.h"

namespace Aimms{
    class AimmsResources{
        std::shared_ptr<IAimms> m_SharedAimmsAPI;
        int m_ProjectHandle;
        std::unique_ptr<AimmsSpace> m_Space;

    public:
        AimmsResources(){}
        ~AimmsResources(){}
        int getProjectHandle(){ return m_ProjectHandle; }
        void setProjectHandle(int projectHandle){
          m_ProjectHandle = projectHandle;
        }
        AimmsSpace* getSpace(){
          if (m_Space.get()==nullptr){
#ifdef _WIN32
              m_SharedAimmsAPI = AimmsAPI::getSharedAimmsAPI(L"libaimms3.dll");
              if(m_SharedAimmsAPI == nullptr){
                    std::wcout<< "Couldnt load libaimms3.dll" << std::endl;
              }
#else
              m_SharedAimmsAPI = AimmsAPI::getSharedAimmsAPI(L"libaimms3.so");
              if(m_SharedAimmsAPI == nullptr){
                    std::wcout<< "Couldnt load libaimms3.so" << std::endl;
              }
#endif
              if(m_SharedAimmsAPI == nullptr){
                  return nullptr;
              }
              m_Space = std::unique_ptr<AimmsSpace>(new AimmsSpace ( m_SharedAimmsAPI) );
          }
          return m_Space.get();
        }

        IAimms* getAimmsAPI(){
             return m_SharedAimmsAPI.get() ? m_SharedAimmsAPI.get() : nullptr;
        }
        bool loadAimmsAPI(std::wstring aimmsLibPath){
             if(m_SharedAimmsAPI.get()){
                 //std::wcout << "Reloading aimms"<< std::endl;
                 //Invalidate current AimmsSpace, the dll is reloaded
                 m_Space->clear();
                 if( m_ProjectHandle ) m_SharedAimmsAPI->ProjectClose(m_ProjectHandle, 0);
                 m_ProjectHandle = 0;
                 bool ret = m_SharedAimmsAPI.get()->Reload(aimmsLibPath);
                 if(!ret) return false;
                 m_Space->setAimmsAPI(m_SharedAimmsAPI);
             }
             m_SharedAimmsAPI = AimmsAPI::getSharedAimmsAPI(aimmsLibPath);
             if(!m_SharedAimmsAPI.get()) return false;
             m_Space = std::unique_ptr<AimmsSpace>(new AimmsSpace ( m_SharedAimmsAPI) );
             return true;
        }
    };

    class AimmsEnvironmentState{
    private:
        AimmsEnvironmentState(){ }


    public:
        static AimmsEnvironmentState& getInstance(){
            static AimmsEnvironmentState instance;
            return instance;
        }

        static AimmsResources& getAimmsResources(){
            static AimmsResources resources;
            return resources;
        }

    };
}
