#!/bin/bash

DEFAULT_CONFIGURATION=Debug64

if [ -z $1 ]; then CONFIGURATION=$DEFAULT_CONFIGURATION; else CONFIGURATION=$1; fi

CWD=`pwd`
SOLUTIONFOLDER=`pwd`/R-link

COMPILER=gcc61
COMPILERFOLDER=/opt/$COMPILER
COMPILERLIBFOLDER=$COMPILERFOLDER/lib64

TOOLSET=${COMPILER}_x64_release
INSTALLDIR=${SOLUTIONFOLDER}/target/$TOOLSET
TOOLSET_DEBUG=${COMPILER}_x64_debug
INSTALLDIR_DEBUG=${SOLUTIONFOLDER}/target/$TOOLSET_DEBUG

rm -rf $INSTALLDIR
mkdir -p $INSTALLDIR

#Place the artifacts in the target folder
rm -rf  ${SOLUTIONFOLDER}/target/${TOOLSET}
rm -rf  ${SOLUTIONFOLDER}/target/${TOOLSET_DEBUG}

mkdir  ${SOLUTIONFOLDER}/target/${TOOLSET}
mkdir  ${SOLUTIONFOLDER}/target/${TOOLSET_DEBUG}

#Let's build aimms4r
OLD_PATH=`pwd`
cd R-link && LD_LIBRARY_PATH=$COMPILERLIBFOLDER R CMD INSTALL --preclean -l $INSTALLDIR --with-keep.source aimms4r

#Let's build r4aimms
cd ${SOLUTIONFOLDER}/r4aimms/src/r4aimms && make clean
cd ${SOLUTIONFOLDER}/r4aimms/src/r4aimms && make build CONF=$CONFIGURATION


mkdir ${SOLUTIONFOLDER}/target/${TOOLSET}/Libraries
mkdir ${SOLUTIONFOLDER}/target/${TOOLSET_DEBUG}/Libraries

mkdir ${SOLUTIONFOLDER}/target/${TOOLSET}/Libraries/R4Aimms
mkdir ${SOLUTIONFOLDER}/target/${TOOLSET}/Libraries/R4Aimms/R-packages

mkdir ${SOLUTIONFOLDER}/target/${TOOLSET_DEBUG}/Libraries/R4Aimms
mkdir ${SOLUTIONFOLDER}/target/${TOOLSET_DEBUG}/Libraries/R4Aimms/R-packages

cp ${SOLUTIONFOLDER}/r4aimms/aimms/R4Aimms/Project.xml  ${SOLUTIONFOLDER}/target/${TOOLSET}/Libraries/R4Aimms
cp ${SOLUTIONFOLDER}/r4aimms/aimms/R4Aimms/R4Aimms.ams ${SOLUTIONFOLDER}/target/${TOOLSET}/Libraries/R4Aimms

cp ${SOLUTIONFOLDER}/r4aimms/aimms/R4Aimms/Project.xml  ${SOLUTIONFOLDER}/target/${TOOLSET_DEBUG}/Libraries/R4Aimms
cp ${SOLUTIONFOLDER}/r4aimms/aimms/R4Aimms/R4Aimms.ams ${SOLUTIONFOLDER}/target/${TOOLSET_DEBUG}/Libraries/R4Aimms


if [ -f "${SOLUTIONFOLDER}/r4aimms/src/r4aimms/dist/Release64/GNU-Linux/libr4aimms.so" ]
    then cp ${SOLUTIONFOLDER}/r4aimms/src/r4aimms/dist/Release64/GNU-Linux/libr4aimms.so ${SOLUTIONFOLDER}/target/${TOOLSET}/Libraries/R4Aimms
fi
if [ -f "${SOLUTIONFOLDER}/r4aimms/src/r4aimms/dist/Debug64/GNU-Linux/libr4aimms.so" ]
    then cp ${SOLUTIONFOLDER}/r4aimms/src/r4aimms/dist/Debug64/GNU-Linux/libr4aimms.so ${SOLUTIONFOLDER}/target/${TOOLSET_DEBUG}/Libraries/R4Aimms
fi


mkdir ${SOLUTIONFOLDER}/target/${TOOLSET}/Libraries/R4Aimms/R-packages/aimms4r
mkdir ${SOLUTIONFOLDER}/target/${TOOLSET_DEBUG}/Libraries/R4Aimms/R-packages/aimms4r

# R-link/target/gcc61_x64_release/aimms4r/libs/
echo ${SOLUTIONFOLDER}/target/${TOOLSET}/aimms4r
mv ${SOLUTIONFOLDER}/target/${TOOLSET}/aimms4r ${SOLUTIONFOLDER}/target/${TOOLSET}/Libraries/R4Aimms/R-packages

cp -r  ${SOLUTIONFOLDER}/target/${TOOLSET}/Libraries/R4Aimms/R-packages/aimms4r  ${SOLUTIONFOLDER}/target/${TOOLSET_DEBUG}/Libraries/R4Aimms/R-packages

rm -rf ${CWD}/target
mv ${SOLUTIONFOLDER}/target ${CWD}

