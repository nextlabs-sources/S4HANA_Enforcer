#!/bin/bash
source /opt/rh/devtoolset-7/enable

#get parameters
TARGETENVARCH=x64
if [[ -n $1 ]]
then
TARGETENVARCH=$1
fi
echo "TARGETENVARCH=${TARGETENVARCH}"

if [ -n "${NLBUILDROOT+1}" ]; then
  echo "NLBUILDROOT=${NLBUILDROOT}"
else
  echo "NLBUILDROOT is not defined"
  exit 1
fi

OPERATE_SYSTEM=linux
if [[ -n $2 ]]
then
OPERATE_SYSTEM=$2
fi
echo "OPERATE_SYSTEM=${OPERATE_SYSTEM}"




export VERSION_PRODUCT=DAE_For_SAP_HANA
export VERSION_MAJOR=2
export VERSION_MINOR=0
export VERSION_MAINTENANCE=0
export VERSION_PATCH=0

#version:$VERSION_MAJOR.$VERSION_MINOR.$BUILD_NUMBER echo $VERSION_MAJOR.$VERSION_MINOR.$BUILD_NUMBER
#sed -i "s/version:[^\n]*/version:${VERSION_MAJOR-0}.${VERSION_MINOR-0}.${BUILD_NUMBER-0}/" readme.txt
echo -e "\nversion:${VERSION_MAJOR-0}.${VERSION_MINOR-0}.${BUILD_NUMBER-0}" >>readme.txt

cd ${NLBUILDROOT}/prod
cmake  -DTARGETENVARCH=${TARGETENVARCH} -DOPERATE_SYSTEM=${OPERATE_SYSTEM} -DOPENSSL_ROOT=${OPENSSL_ROOT} -DBOOST_ROOT=${BOOST_ROOT} .
make -j4

#change log location from %DAEInstallPath%\log to /usr/NextLabs/DAE/log
sed -i "s/location=[^\n]*/location=\/usr\/NextLabs\/DAE\/log/" ${NLBUILDROOT}/prod/hdb_new/DAE.conf
cd ${NLBUILDROOT}/installer/rpm
make
