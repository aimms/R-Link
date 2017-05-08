set OLDPATH=%PATH%
IF "%1"=="Win32" goto run32
goto run64 

:run32
set TARGET_ARCH=i386
set TARGET_BITS=32
goto run

:run64
set TARGET_ARCH=x64
set TARGET_BITS=64
goto run

:run
set TARGETDIR=%~dp0target
set INSTALLDIR=%TARGETDIR%\mingw_all_release
set R_HOME=c:/R
@rem set R_HOME=%ProgramFiles%\R\R-3.3.2

set R_TOOLS_HOME=C:/Rtools
set R_ARCH=/%TARGET_ARCH%
set BINPREF=%R_TOOLS_HOME%/mingw_$(WIN)/bin/
set PATH=C:/Rtools/bin;C:/R/bin/%TARGET_ARCH%;%PATH%


if exist %CWD%\target rmdir /s/q %CWD%\target
if not exist %INSTALLDIR% mkdir %INSTALLDIR%

set CWD=%cd%

CD /D %CWD%\R-link

call "%R_HOME%\Bin\%TARGET_ARCH%\Rcmd.exe" INSTALL --preclean --with-keep.source -l %INSTALLDIR% aimms4r

CD /D %CWD%

if exist %CWD%\tmp rmdir /s/q %CWD%\tmp

CD /D %CWD%\R-link\r4aimms

call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat"

set SOLUTION_NAME=Rlink-vc120.sln
msbuild /m %SOLUTION_NAME% /property:Configuration=%2 /property:Platform="%1" /t:Rebuild


CD /D %CWD%
set PATH=%OLDPATH%

:setup
@rem Copy aimms4r to r4aimms/target
xcopy /s %CWD%\target\mingw_all_release\aimms4r %CWD%\R-link\r4aimms\target\%3_%1_%2\Libraries\R4Aimms\R-packages\aimms4r

@rem Copy %CWD%\r4aimms\target to %CWD%\target
mkdir %CWD%\final
xcopy /y /s /D %CWD%\R-link\r4aimms\target %CWD%\final

rmdir /s/q %CWD%\target
ren %CWD%\final target


set AIMMSROOTLOCATION=%CWD%\target\%3_%1_%2

:end

