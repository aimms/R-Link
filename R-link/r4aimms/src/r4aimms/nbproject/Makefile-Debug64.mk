#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=/opt/${DISTRO}/bin/g++
CXX=/opt/${DISTRO}/bin/g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug64
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/CPU.o \
	${OBJECTDIR}/src/CharSetConverter.o \
	${OBJECTDIR}/src/CharSetEnumerator.o \
	${OBJECTDIR}/src/ConditionVariable.o \
	${OBJECTDIR}/src/DynamicLib.o \
	${OBJECTDIR}/src/EnvMap.o \
	${OBJECTDIR}/src/Lock.o \
	${OBJECTDIR}/src/LockImpl.o \
	${OBJECTDIR}/src/PlatformThread.o \
	${OBJECTDIR}/src/Process.o \
	${OBJECTDIR}/src/REngine.o \
	${OBJECTDIR}/src/ThreadLocalStorage.o \
	${OBJECTDIR}/src/Time.o \
	${OBJECTDIR}/src/UCSTranformer.o \
	${OBJECTDIR}/src/aimmsifcFactory.o \
	${OBJECTDIR}/src/dummy.o \
	${OBJECTDIR}/src/iAimmsA_CCP_313asc.o \
	${OBJECTDIR}/src/iAimmsA_CCP_313uni.o \
	${OBJECTDIR}/src/iAimmsA_CCP_314.o \
	${OBJECTDIR}/src/iAimmsA_UTF8_313asc.o \
	${OBJECTDIR}/src/iAimmsA_UTF8_313uni.o \
	${OBJECTDIR}/src/iAimmsA_UTF8_314.o \
	${OBJECTDIR}/src/iAimmsFactory.o \
	${OBJECTDIR}/src/iAimmsW_uni_313asc.o \
	${OBJECTDIR}/src/iAimmsW_uni_313uni.o \
	${OBJECTDIR}/src/iAimmsW_uni_314.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/win_iconv.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-fPIC -fvisibility=hidden
CXXFLAGS=-fPIC -fvisibility=hidden

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../deps/gcc61_x64_debug/lib -L../../deps/gcc49_x64_debug/lib -L/usr/lib64/R/lib -ldl

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libr4aimms.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libr4aimms.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	/opt/${DISTRO}/bin/g++ -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libr4aimms.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/src/CPU.o: nbproject/Makefile-${CND_CONF}.mk src/CPU.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CPU.o src/CPU.cpp

${OBJECTDIR}/src/CharSetConverter.o: nbproject/Makefile-${CND_CONF}.mk src/CharSetConverter.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CharSetConverter.o src/CharSetConverter.cpp

${OBJECTDIR}/src/CharSetEnumerator.o: nbproject/Makefile-${CND_CONF}.mk src/CharSetEnumerator.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/CharSetEnumerator.o src/CharSetEnumerator.cpp

${OBJECTDIR}/src/ConditionVariable.o: nbproject/Makefile-${CND_CONF}.mk src/ConditionVariable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ConditionVariable.o src/ConditionVariable.cpp

${OBJECTDIR}/src/DynamicLib.o: nbproject/Makefile-${CND_CONF}.mk src/DynamicLib.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/DynamicLib.o src/DynamicLib.cpp

${OBJECTDIR}/src/EnvMap.o: nbproject/Makefile-${CND_CONF}.mk src/EnvMap.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/EnvMap.o src/EnvMap.cpp

${OBJECTDIR}/src/Lock.o: nbproject/Makefile-${CND_CONF}.mk src/Lock.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Lock.o src/Lock.cpp

${OBJECTDIR}/src/LockImpl.o: nbproject/Makefile-${CND_CONF}.mk src/LockImpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/LockImpl.o src/LockImpl.cpp

${OBJECTDIR}/src/PlatformThread.o: nbproject/Makefile-${CND_CONF}.mk src/PlatformThread.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/PlatformThread.o src/PlatformThread.cpp

${OBJECTDIR}/src/Process.o: nbproject/Makefile-${CND_CONF}.mk src/Process.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Process.o src/Process.cpp

${OBJECTDIR}/src/REngine.o: nbproject/Makefile-${CND_CONF}.mk src/REngine.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/REngine.o src/REngine.cpp

${OBJECTDIR}/src/ThreadLocalStorage.o: nbproject/Makefile-${CND_CONF}.mk src/ThreadLocalStorage.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ThreadLocalStorage.o src/ThreadLocalStorage.cpp

${OBJECTDIR}/src/Time.o: nbproject/Makefile-${CND_CONF}.mk src/Time.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Time.o src/Time.cpp

${OBJECTDIR}/src/UCSTranformer.o: nbproject/Makefile-${CND_CONF}.mk src/UCSTranformer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/UCSTranformer.o src/UCSTranformer.cpp

${OBJECTDIR}/src/aimmsifcFactory.o: nbproject/Makefile-${CND_CONF}.mk src/aimmsifcFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/aimmsifcFactory.o src/aimmsifcFactory.cpp

${OBJECTDIR}/src/dummy.o: nbproject/Makefile-${CND_CONF}.mk src/dummy.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dummy.o src/dummy.cpp

${OBJECTDIR}/src/iAimmsA_CCP_313asc.o: nbproject/Makefile-${CND_CONF}.mk src/iAimmsA_CCP_313asc.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/iAimmsA_CCP_313asc.o src/iAimmsA_CCP_313asc.cpp

${OBJECTDIR}/src/iAimmsA_CCP_313uni.o: nbproject/Makefile-${CND_CONF}.mk src/iAimmsA_CCP_313uni.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/iAimmsA_CCP_313uni.o src/iAimmsA_CCP_313uni.cpp

${OBJECTDIR}/src/iAimmsA_CCP_314.o: nbproject/Makefile-${CND_CONF}.mk src/iAimmsA_CCP_314.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/iAimmsA_CCP_314.o src/iAimmsA_CCP_314.cpp

${OBJECTDIR}/src/iAimmsA_UTF8_313asc.o: nbproject/Makefile-${CND_CONF}.mk src/iAimmsA_UTF8_313asc.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/iAimmsA_UTF8_313asc.o src/iAimmsA_UTF8_313asc.cpp

${OBJECTDIR}/src/iAimmsA_UTF8_313uni.o: nbproject/Makefile-${CND_CONF}.mk src/iAimmsA_UTF8_313uni.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/iAimmsA_UTF8_313uni.o src/iAimmsA_UTF8_313uni.cpp

${OBJECTDIR}/src/iAimmsA_UTF8_314.o: nbproject/Makefile-${CND_CONF}.mk src/iAimmsA_UTF8_314.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/iAimmsA_UTF8_314.o src/iAimmsA_UTF8_314.cpp

${OBJECTDIR}/src/iAimmsFactory.o: nbproject/Makefile-${CND_CONF}.mk src/iAimmsFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/iAimmsFactory.o src/iAimmsFactory.cpp

${OBJECTDIR}/src/iAimmsW_uni_313asc.o: nbproject/Makefile-${CND_CONF}.mk src/iAimmsW_uni_313asc.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/iAimmsW_uni_313asc.o src/iAimmsW_uni_313asc.cpp

${OBJECTDIR}/src/iAimmsW_uni_313uni.o: nbproject/Makefile-${CND_CONF}.mk src/iAimmsW_uni_313uni.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/iAimmsW_uni_313uni.o src/iAimmsW_uni_313uni.cpp

${OBJECTDIR}/src/iAimmsW_uni_314.o: nbproject/Makefile-${CND_CONF}.mk src/iAimmsW_uni_314.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/iAimmsW_uni_314.o src/iAimmsW_uni_314.cpp

${OBJECTDIR}/src/main.o: nbproject/Makefile-${CND_CONF}.mk src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.cpp

${OBJECTDIR}/src/win_iconv.o: nbproject/Makefile-${CND_CONF}.mk src/win_iconv.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DUNICODE -D_DEBUG -Iinclude -Isrc -std=c++14 -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/win_iconv.o src/win_iconv.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libr4aimms.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
