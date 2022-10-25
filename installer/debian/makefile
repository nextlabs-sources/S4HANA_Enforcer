.PHONY:copy_assembly make_installer 

all: copy_assembly make_installer

install_top_folder=Nextlabs\ EM\ SAP\ Data\ Access
package_name = $(install_top_folder).deb
s4h_folder = $(install_top_folder)/usr/nextlabs/ems4h
install_bin_folder=$(s4h_folder)/bin
prod_folder= ../prod
boost_folder=/usr/local/lib

copy_assembly:
	cp -pf $(prod_folder)/hdb_new/libhdb_new.so  $(install_bin_folder)/
	cp -pf $(prod_folder)/subhook/libsubhook.so  $(install_bin_folder)/
	cp -pf $(prod_folder)/funchook/libfunchook.so  $(install_bin_folder)/
	cp -pf $(prod_folder)/tool/libtool.so  $(install_bin_folder)/
	cp -pf $(prod_folder)/QueryCloudAZSDKCpp/libQueryCloudAZSDKCpp.so $(install_bin_folder)/
	cp -pf $(boost_folder)/libboost_system.so.1.67.0 $(install_bin_folder)/
	cp -pf $(boost_folder)/libboost_log.so.1.67.0    $(install_bin_folder)/
	cp -pf $(boost_folder)/libboost_thread.so.1.67.0 $(install_bin_folder)/
	cp -pf $(boost_folder)/libboost_filesystem.so.1.67.0 $(install_top_folder)/usr/local/lib64/
	cp -pf $(prod_folder)/hdb_new/nxs4h.conf  $(s4h_folder)/config/
	

make_installer:
	dpkg-deb -b $(install_top_folder) $(package_name)
