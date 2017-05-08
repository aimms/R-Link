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
