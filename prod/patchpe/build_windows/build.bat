rem echo off
set MAKEFILE_INC=%NLBUILDROOT%/prod/patchpe/build_windows/makefile
call %NLBUILDROOT%/prod/build_windows/build.bat %1 %2 %MAKEFILE_INC%

