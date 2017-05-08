@echo Cleaning up the test environment
@echo off
rmdir /s /q target >nul 2>&1
rmdir /s /q .cache >nul 2>&1
rmdir /s /q deps >nul 2>&1
rmdir /s /q R-link\r4aimms\target
del /s results.txt >nul 2>&1
del /s RFunctionalModel.lis >nul 2>&1
del /s R-link\aimms4r\src\aimms4r.dll >nul 2>&1
del /s runFunctionalModel.elg >nul 2>&1
del /s runFunctionalModel.err >nul 2>&1
del /s runFunctionalModel.blg >nul 2>&1
del /s runFunctionalModel.xlg >nul 2>&1
del /s runFunctionalModel.usageLog >nul 2>&1
del /s runFunctionalModelDebug.elg >nul 2>&1
del /s runFunctionalModelDebug.err >nul 2>&1
del /s runFunctionalModelDebug.blg >nul 2>&1
del /s runFunctionalModelDebug.xlg >nul 2>&1
del /s runFunctionalModelDebug.usageLog >nul 2>&1
pause