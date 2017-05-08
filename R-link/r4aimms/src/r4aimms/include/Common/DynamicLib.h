#ifndef _INCLUDED_Common_DynamicLib
#define _INCLUDED_Common_DynamicLib

#include "Common/CommonAPI.h"

namespace Common
{
	class COMMON_API DynamicLib
	{
	public:
		DynamicLib(const char* szFileName = 0);
		virtual ~DynamicLib();

		virtual bool Open( const char* szFileName );
		virtual void Close();

		virtual void* GetProcAddress( const char* szProcName );
		virtual bool Valid() const;

	private:
		void* m_hModule;
	};
}

#endif // end #ifndef _INCLUDED_Common_DynamicLib
