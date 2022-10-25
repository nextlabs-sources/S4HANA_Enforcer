Name: %{getenv:VERSION_PRODUCT}
Version: %{getenv:VERSION_MAJOR}.%{getenv:VERSION_MINOR}.%{getenv:VERSION_PATCH}
Release: %{getenv:BUILD_NUMBER}
Summary: Nextlabs Entitle Management for SAP ERP data access.
Group: Server
License:GPL
URL:https://www.nextlabs.com
BuildRoot:  %_topdir/BUILDROOT
%description
Nextlabs Entitle Management for SAP ERP data access runs on SAP Application server and do enforcement when SAP Application user access data through SAP client. In this version the only enforcement action is mask. It is said that EMS4H will mask special fields based on the information of SAP Application user.


%install
%define project_folder %{getenv:NLBUILDROOT}
%define prod_folder %{project_folder}/prod
%define install_top_folder %{project_folder}/installer/rpm/buildroot
%define s4h_folder  %{install_top_folder}/usr/NextLabs/DAE




#remove rpmbuild folder
if [ -d %{buildroot} ];then
    rm -rf %{buildroot}/*
else
    mkdir %{buildroot}
fi

#remove content under install top folder
rm -rf %{install_top_folder}/*


#create bin folder, copy file to it
%define install_bin_folder %{s4h_folder}/bin
mkdir -p %{install_bin_folder}

cp -pf %{project_folder}/readme.txt %{s4h_folder}/


#cp -pf %{prod_folder}/hdb_new/verify_config  %{install_bin_folder}/
#chmod 754 %{install_bin_folder}/verify_config

cp -pf %{prod_folder}/hdb_new/libhdb_new.so  %{install_bin_folder}/
#cp -pf %{prod_folder}/subhook/libsubhook.so  %{install_bin_folder}/
cp -pf %{prod_folder}/funchook/libfunchook.so  %{install_bin_folder}/
cp -pf %{prod_folder}/tool/libtool.so  %{install_bin_folder}/
cp -pf %{prod_folder}/hdb_policymgr/libhdb_policymgr.so  %{install_bin_folder}/
 

cp -pf %{prod_folder}/QueryCloudAZSDKCpp/libQueryCloudAZSDKCpp.so %{install_bin_folder}/

#cp -pf %{prod_folder}/empolicymgr/libempolicymgr.so %{install_bin_folder}/
cp -pf %{prod_folder}/encrypttool/encrypttool %{install_bin_folder}/
cp -pf %{prod_folder}/bootstrap/bootstrap %{install_bin_folder}/
cp -pf %{prod_folder}/DAE_main/libdae_main.so %{install_bin_folder}/

#create config folder, and copy config file to it
%define install_config_folder %{s4h_folder}/config
mkdir -p %{install_config_folder}
cp -pf %{prod_folder}/hdb_new/DAE.conf  %{s4h_folder}/config/
#cp -pf %{prod_folder}/hdb_new/table_alias.conf  %{s4h_folder}/config/
chmod 774 %{s4h_folder}/config/DAE.conf
#chmod 774 %{s4h_folder}/config/table_alias.conf

#create log folder
%define install_log_folder %{s4h_folder}/log
mkdir -p %{install_log_folder} 
chmod 777 %{install_log_folder}

#create data folder
%define install_data_folder %{s4h_folder}/data
mkdir -p %{install_data_folder} 
chmod 777 %{install_data_folder}

#create tool folder
%define install_tool_folder %{s4h_folder}/bin
#mkdir -p %{install_tool_folder}
if [ "%{_target_cpu}" = "x86_64" ];then
cp -pf %{prod_folder}/patchelf/bin/x64/patchelf %{install_tool_folder}/
elif [ "%{_target_cpu}" = "ppc64le" ];then
cp -pf %{prod_folder}/patchelf/bin/ppc64le/patchelf %{install_tool_folder}/
elif [ "%{_target_cpu}" = "ppc" ];then
cp -pf %{prod_folder}/patchxcoff/patchxcoff %{install_tool_folder}/
else
echo "target_cpu=%{_target_cpu} is not support!!!!"
fi

#copy the whole directory to rpmbuild folder
cp -pfr %{install_top_folder}/*   %{buildroot}/



%files
/usr/*

%clean
%define RPM_PAGE_NAME_SRC %{name}-%{version}-%{release}.*%{_target_cpu}.rpm
%define RPM_PAGE_NAME_DST %{name}-%{version}-%{release}.$(uname).%{_target_cpu}.rpm
%define result_rpm_package %{_topdir}/RPMS/%{_target_cpu}/%{RPM_PAGE_NAME_SRC}
%define BUILD_OUTPUT_DIR  %{project_folder}/installer/rpm/output
if [ -d %{BUILD_OUTPUT_DIR} ];then
    rm -rf %{BUILD_OUTPUT_DIR}/*
else
    mkdir %{BUILD_OUTPUT_DIR}
fi
cp -f %{result_rpm_package} %{BUILD_OUTPUT_DIR}/%{RPM_PAGE_NAME_DST}
cd %{BUILD_OUTPUT_DIR}
zip ../%{name}-%{version}-%{release}-$(uname).%{_target_cpu}.$(date +"%Y%m%d%H%M").zip * ;
rm -r %{buildroot};