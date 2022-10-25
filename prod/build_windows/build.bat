:build.bat {x86|x64} {release|debug}
echo NLBUILDROOT=%NLBUILDROOT%
echo BOOST_ROOT=%BOOST_ROOT%
echo OPENSSL_ROOT=%OPENSSL_ROOT%
echo CYGWIN_ROOT=%CYGWIN_ROOT%
echo VERSION_BUILD=%VERSION_BUILD%
echo VERSION_BUILD_SHORT=%VERSION_BUILD_SHORT%
:echo MAKEFILE=%1
setlocal
set TARGETENVARCH=%1
set SIGN_SCRIPT_NAME=signModulesByServer256Only.bat
set SIGNING_SERVER=PRASLIN
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" %1
set PATH=%PATH%;%CYGWIN_ROOT%\bin\;
:make.exe  -f %NLBUILDROOT%/prod/build_windows/makefile versioninfo
make.exe -j4  -f %3 TARGETENVARCH=%1 BUILDTYPE=%2
endlocal
