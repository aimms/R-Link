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
#ifndef _INCLUDED_aimmsifc_iAimmsFactory
#define _INCLUDED_aimmsifc_iAimmsFactory

#include <aimmsifc/iAimms.h>
#include <string>

namespace aimmsifc {

	class iFactoryManage {
	public:
		virtual ~iFactoryManage() {}
		virtual void Delete() { delete this; }
	};
	
	iAimmsA *CreateiAimmsA_CCP_313asc(const char *dllname);
	iAimmsA *CreateiAimmsA_CCP_313uni(const char *dllname);
	iAimmsA *CreateiAimmsA_CCP_314(const char *dllname);

	iAimmsA *CreateiAimmsA_UTF8_313asc(const char *dllname);
	iAimmsA *CreateiAimmsA_UTF8_313uni(const char *dllname);
	iAimmsA *CreateiAimmsA_UTF8_314(const char *dllname);

	iAimmsW *CreateiAimmsW_uni_313asc(const char *dllname);
	iAimmsW *CreateiAimmsW_uni_313uni(const char *dllname);
	iAimmsW *CreateiAimmsW_uni_314(const char *dllname);

	class iAimmsFactory {
		std::string            m_DllName;
		bool                   m_UsedInExternalAimmsDLL; // if true, this class is used from with a dll that is already linked to a running Aimms
		                                                 // for example when called from within an AIMMS external function.
														 // In this case, we can search the modules in the current process for the correct libaimms(3).dll
		iAimmsA               *m_Existing_iAimmsCCP;
		iAimmsA               *m_Existing_iAimmsUTF8;
		iAimmsW               *m_Existing_iAimmsUNI;
	
		iAimmsFactory();
		~iAimmsFactory();
	public:
		static iAimmsFactory &Singleton();

		iAimmsA *GetCCPInterface(bool usedInExternalAimmsDLL, const char *dllSearchPath);
		iAimmsA *GetUTF8Interface(bool usedInExternalAimmsDLL, const char *dllSearchPath);
		iAimmsW *GetUnicodeInterface(bool usedInExternalAimmsDLL, const char *dllSearchPath);
	private:
		int FindLibAimmsInCurrentModules(std::string &dllPath); // only for Windows
		int FindLibAimmsViaTrialAndError(const char *dllNameHint, std::string &dllSearchPath);
		int GetAimmsDllPath(bool usedInExternalAimmsDLL, const char *dllNameHint, std::string &dllSearchPath);
	};
}


#endif //  _INCLUDED_aimmsifc_iAimmsFactory