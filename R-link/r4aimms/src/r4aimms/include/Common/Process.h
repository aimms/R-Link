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
#ifndef _INCLUDED__Common_Process
#define _INCLUDED__Common_Process

#include "Common/GlobalInclude.h"
#include "Common/CommonAPI.h"

#include <stddef.h> // size_t

namespace Common
{
    class IProcessStartInfo {
    public:
        enum EnvVarMode{
            Env_Prepend,
            Env_Set,
            Env_Append
        };

        // retrieve the arguments, this is intended to 
        // be used like argc/argv, i.e. the first argument 
        // points to the pathname of the executable
        virtual size_t getNumberOfArguments() const = 0;
        virtual const char* getArgument(size_t argNo) const = 0;

        virtual bool getMergeEnvironment() const = 0;
        virtual size_t getNumberOfEnvVars() const = 0;
        virtual const char* getEnvVarsKey(size_t argNo) const = 0;
        virtual const char* getEnvVarsVal(size_t argNo) const = 0;
        virtual EnvVarMode getEnvVarsMode(size_t argNo) const = 0;
    };

	class ProcessFactory {
	public:
        static COMMON_API void createProcess(IProcessStartInfo* processStartInfo);
	};

};
// end namespace Common

#endif // end #ifndef _INCLUDED__Common_Process
