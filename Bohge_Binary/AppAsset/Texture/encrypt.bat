@echo off 
set WORK_DIR=%CD%
pushd %WORK_DIR%
for /r %%i in ("*.*") do E:/Lord/Tools/Encrypt/Release/encrypt.exe "%%~fi" "bohge&penngames!"
popd
pause
