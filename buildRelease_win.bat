
echo off
echo build EM SAP for ECC HANA begin.

set BUILDTYPE=release
set TARGETENVARCH=x64
set OFFICIALCERT=1
set VERSION_PRODUCT=NextLabs DAE for SAP
set VERSION_MAJOR=2
set VERSION_MINOR=0
set VERSION_MAINTENANCE=0
set VERSION_PATCH=0

:compile source
echo "build tool"
cd .\prod\tool
call .\build_windows\build.bat %TARGETENVARCH% %BUILDTYPE%
cd ..\..\

echo "build jsoncpp"
cd .\prod\jsoncpp
call .\build_windows\build.bat %TARGETENVARCH% %BUILDTYPE%
cd ..\..\

echo "build hdb_policymgr"
cd .\prod\hdb_policymgr
call .\build_windows\build.bat %TARGETENVARCH% %BUILDTYPE%
cd ..\..\

echo "build QueryCloudAZSDKCpp"
cd .\prod\QueryCloudAZSDKCpp
call .\build_windows\build.bat %TARGETENVARCH% %BUILDTYPE%
cd ..\..\


echo "build hdb_new"
cd .\prod\hdb_new
call .\build_windows\build.bat %TARGETENVARCH% %BUILDTYPE%
cd ..\..\

echo "build DAE_main"
cd .\prod\DAE_main
call .\build_windows\build.bat %TARGETENVARCH% %BUILDTYPE%
cd ..\..\

echo "build encrypttool"
cd .\prod\encrypttool
call .\build_windows\build.bat %TARGETENVARCH% %BUILDTYPE%
cd ..\..\

echo "build bootstrap"
cd .\prod\bootstrap
call .\build_windows\build.bat %TARGETENVARCH% %BUILDTYPE%
cd ..\..\

echo "build patchpe"
cd .\prod\patchpe
call .\build_windows\build.bat %TARGETENVARCH% %BUILDTYPE%
cd ..\..\

:build MSI package
cd .\installer\windows
call build.bat %TARGETENVARCH% %BUILDTYPE%
cd ..\..\



