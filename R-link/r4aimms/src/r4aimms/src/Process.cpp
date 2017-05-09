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
#include "Common/Process.h"
#include "Common/EnvMap.h"

#include "Common/Array.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <memory>

#ifdef _TARGET_PLATFORM_WINDOWS
#include <process.h>
#include <errno.h>
#endif

#ifdef _TARGET_PLATFORM_LINUX
#include <errno.h>
#include <fcntl.h>
#include <spawn.h>
#include <signal.h>
#include <string.h> // strerror
#endif

#if defined(_HOST_COMPILER_GCC)
#include <unistd.h>
#endif

namespace Common {


    namespace {

        typedef std::vector<std::string> StringVector;

        void StartNewProcess(const StringVector& _argv, const StringVector& _env) {
            int i;

            Array<const char*> argv(_argv.size() + 1);
            for (i = 0; i < static_cast<int> (_argv.size()); i++) {
                argv[i] = _argv[i].c_str();
            }
            argv[i] = 0;

            Array<const char*> env(_env.size() + 1);
            for (i = 0; i < static_cast<int> (_env.size()); i++) {
                env[i] = _env[i].c_str();
            }
            env[i] = 0;
#ifdef _TARGET_PLATFORM_WINDOWS
			// strip quoted argv[0] of its quotes to overcome checks in spawn that
			// do not account for long file paths with spaces
			std::string app = argv[0];
			if (app[0] == '\"' && app[app.size()-1] == '\"') {
				app = app.substr(1, app.size()-2);
			}

            if (_spawnve(_P_DETACH, app.c_str(), argv.data(), env.data()) != 0) {
                std::ostringstream ossTmp;
                ossTmp << "_spawnve: " << strerror(errno);
                throw std::runtime_error(ossTmp.str().c_str());
            }
#endif
#ifdef _TARGET_PLATFORM_LINUX

            signal(SIGCHLD, SIG_IGN);
            pid_t pid;
            posix_spawn_file_actions_t fileActions;
            posix_spawn_file_actions_init(&fileActions);

            for (int i = getdtablesize(); i-- > 3;) {
                if (fcntl(i, F_GETFD) != -1) {
                    posix_spawn_file_actions_addclose(&fileActions, i);
                }
            }
            if (posix_spawn(&pid, argv[0], &fileActions, 0, const_cast<char* const*> (argv.data()), const_cast<char* const*> (env.data())) != 0) {
                std::ostringstream ossTmp;
                ossTmp << "posix_spawn: " << strerror(errno);
                throw std::runtime_error(ossTmp.str().c_str());
            }
            posix_spawn_file_actions_destroy(&fileActions);
#endif
        }
    }

     void ProcessFactory::createProcess(IProcessStartInfo* processStartInfo)
    {
        StringVector vectorArguments;
        StringVector vectorEnvironment;

        for (size_t argc = 0; argc < processStartInfo->getNumberOfArguments(); ++argc) {
            vectorArguments.push_back(processStartInfo->getArgument(argc));
        }

        Common::EnvMap envMap(processStartInfo->getMergeEnvironment());

        for (size_t envc = 0; envc < processStartInfo->getNumberOfEnvVars(); ++envc) {
            switch (processStartInfo->getEnvVarsMode(envc)) {
                case IProcessStartInfo::Env_Append:
                    envMap.append(processStartInfo->getEnvVarsKey(envc), processStartInfo->getEnvVarsVal(envc));
                    break;

                case IProcessStartInfo::Env_Prepend:
                    envMap.prepend(processStartInfo->getEnvVarsKey(envc), processStartInfo->getEnvVarsVal(envc));
                    break;

                case IProcessStartInfo::Env_Set:
                default:
                    envMap.set(processStartInfo->getEnvVarsKey(envc), processStartInfo->getEnvVarsVal(envc));
                    break;
            }
        }


        // finally convert the environment map back to vector of strings
        envMap.asEnvVarVector(vectorEnvironment);

        StartNewProcess(vectorArguments, vectorEnvironment);
    }


}  // namespace Common
