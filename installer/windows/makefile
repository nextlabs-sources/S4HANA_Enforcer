

CURRENT_MKFILE_PATH    := $(abspath $(lastword $(MAKEFILE_LIST)))
CURRENT_MKFILE_DIRPATH := $(dir $(CURRENT_MKFILE_PATH))

ifeq ($(ProgramW6432), )
	ENV_OS=x86
	PROGRAM_FILES_X86=C:/Program Files
else
	ENV_OS=x64
	PROGRAM_FILES_X86=C:/Program Files (x86)
endif

PROGRAM_FILES_X86_DOS=$(subst /,\,$(PROGRAM_FILES_X86))

PERL=/usr/bin/perl
ISCMDBLD=$(PROGRAM_FILES_X86)/InstallShield/2014 SAB/System/IsCmdBld.exe
#SIGNTOOL_OFFICIAL_TOOL=$(PROGRAM_FILES_X86)/Windows Kits/8.1/bin/x64/signtool.exe
#SIGNTOOL_OFFICIAL_ARGS=sign /ac c:/release/bin/DigiCertAssuredIDRootCA.cer /f c:/release/bin/NextLabs.pfx /p IiVf1itvOrqJ /n "NextLabs Inc." /fd sha256 /tr http://timestamp.digicert.com
SIGNTOOL_OFFICIAL_TOOL=$(NLBUILDROOT)/prod/build_windows/$(SIGN_SCRIPT_NAME)
SIGNTOOL_OFFICIAL_ARGS=
SIGNTOOL_OFFICIAL='$(SIGNTOOL_OFFICIAL_TOOL)' $(SIGNTOOL_OFFICIAL_ARGS)

UNIX_NLBUILDROOT=$(subst \,/,$(NLBUILDROOT))
INSTALL_DIR=$(UNIX_NLBUILDROOT)/install
SCRIPT_DIR=$(UNIX_NLBUILDROOT)/install
INSTALL_BUILD_DIR=$(INSTALL_DIR)/build
ASSEMBLY_DIR=$(INSTALL_BUILD_DIR)/data

MSI_FILE64=$(VERSION_PRODUCT).msi
PACKAGE64_DIR=$(INSTALL_BUILD_DIR)/package64
VERSION_STR=$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_MAINTENANCE)
PACKAGE_FILE64=$(basename $(MSI_FILE64))-$(VERSION_STR).$(BUILD_NUMBER)-$(shell date +"%Y%m%d%H%M").zip
ISM_FILE_DIR=$(NLBUILDROOT)\installer\windows
ISM_TEMPLATE_FILE_NAME_X64=DAE64.ism
ISM_OUTPUT_DIR=$(ISM_FILE_DIR)\output
PUBLISH_DIR=$(ISM_FILE_DIR)\publish
MSI_FILE_PATH=$(ISM_OUTPUT_DIR)\PROJECT_ASSISTANT\SINGLE_MSI_IMAGE\DiskImages\DISK1\$(MSI_FILE64)

EETEMPDIR := $(shell mktemp -d)

#
# Print environment
#

$(info -----------------------------------------------------------------------------)
$(info BUILDTYPE                   = $(BUILDTYPE))
$(info VERSION_BUILD               = $(VERSION_BUILD))
$(info VERSION_BUILD_SHORT         = $(VERSION_BUILD_SHORT))
$(info BUILD_NUMBER                = $(BUILD_NUMBER))
$(info OFFICIALCERT                = $(OFFICIALCERT))
$(info Product directory name      = $(PRODUCT_DIR_NAME))
$(info Packaging list              = $(PACKAGE_LIST))
$(info Install directory           = $(INSTALL_DIR))
$(info Assembly directory          = $(ASSEMBLY_DIR))
$(info Output directory            = $(OUTPUT_DIR))
$(info Package directory (64-bit)  = $(PACKAGE64_DIR))
$(info Package file (64-bit)       = $(PACKAGE_FILE64))
$(info Publish directory           = $(PUBLISH_DIR))
$(info Current OS                  = $(ENV_OS))
$(info Program files (x86)         = $(PROGRAM_FILES_X86))
$(info Program files (x86) DOS     = $(PROGRAM_FILES_X86_DOS))
$(info ISM_TEMPLATE_FILE_NAME_X86  = $(ISM_TEMPLATE_FILE_NAME_X86))
$(info ISM_TEMPLATE_FILE_NAME_X64  = $(ISM_TEMPLATE_FILE_NAME_X64))
$(info ISM release                 = $(ISM_RELEASE))
$(info ISM product config (64-bit) = $(ISM_PRODUCT_CONFIG64))
$(info ISM output directory        = $(DEPLOY_DIR))
$(info -----------------------------------------------------------------------------)

INSTALL_ROOT=${NLBUILDROOT}/installer/windows
INSTALL_DAE=${INSTALL_ROOT}/NextLabs/DAE
INSTALL_DAE_BIN=${INSTALL_DAE}/bin
INSTALL_DAE_CFG=${INSTALL_DAE}/config


all: preparefile installer64 publish

preparefile:
	@echo "INFO: Prepare assembly for file info"
	mkdir -p $(INSTALL_ROOT)
	mkdir -p $(INSTALL_DAE)
	mkdir -p $(INSTALL_DAE_BIN)
	mkdir -p $(INSTALL_DAE_CFG)
	cp -pf $(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win64/libeay32.dll $(INSTALL_DAE_BIN)/
	cp -pf $(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win64/ssleay32.dll $(INSTALL_DAE_BIN)/
	cp -pf $(BOOST_ROOT)/lib64-msvc-14.0/boost_date_time-vc140-mt-x64-1_67.dll  $(INSTALL_DAE_BIN)/
	cp -pf $(BOOST_ROOT)/lib64-msvc-14.0/boost_filesystem-vc140-mt-x64-1_67.dll  $(INSTALL_DAE_BIN)/
	cp -pf $(BOOST_ROOT)/lib64-msvc-14.0/boost_log-vc140-mt-x64-1_67.dll  $(INSTALL_DAE_BIN)/
	cp -pf $(BOOST_ROOT)/lib64-msvc-14.0/boost_system-vc140-mt-x64-1_67.dll  $(INSTALL_DAE_BIN)/
	cp -pf $(BOOST_ROOT)/lib64-msvc-14.0/boost_thread-vc140-mt-x64-1_67.dll  $(INSTALL_DAE_BIN)/
	cp -pf $(NLBUILDROOT)/prod/bootstrap/win_$(TARGETENVARCH)_$(BUILDTYPE)/bootstrap.exe  $(INSTALL_DAE_BIN)/
	cp -pf $(NLBUILDROOT)/prod/bootstrap/Init.ps1  $(INSTALL_DAE_BIN)/
	cp -pf $(NLBUILDROOT)/prod/DAE_main/win_$(TARGETENVARCH)_$(BUILDTYPE)/dae_main.dll  $(INSTALL_DAE_BIN)/
	cp -pf $(NLBUILDROOT)/prod/hdb_new/win_$(TARGETENVARCH)_$(BUILDTYPE)/hdb_new.dll  $(INSTALL_DAE_BIN)/
	cp -pf $(NLBUILDROOT)/prod/hdb_policymgr/win_$(TARGETENVARCH)_$(BUILDTYPE)/hdb_policymgr.dll  $(INSTALL_DAE_BIN)/
	cp -pf $(NLBUILDROOT)/prod/QueryCloudAZSDKcpp/win_$(TARGETENVARCH)_$(BUILDTYPE)/QueryCloudAZSDKcpp.dll  $(INSTALL_DAE_BIN)/
	cp -pf $(NLBUILDROOT)/prod/tool/win_$(TARGETENVARCH)_$(BUILDTYPE)/tool.dll  $(INSTALL_DAE_BIN)/
	cp -pf $(NLBUILDROOT)/prod/encrypttool/win_$(TARGETENVARCH)_$(BUILDTYPE)/encrypttool.exe  $(INSTALL_DAE_BIN)/
	cp -pf $(NLBUILDROOT)/prod/patchpe/win_$(TARGETENVARCH)_$(BUILDTYPE)/patchpe.exe  $(INSTALL_DAE_BIN)/
	cp -pf $(NLBUILDROOT)/readme.txt  $(INSTALL_DAE)/
	cp -pf $(NLBUILDROOT)/prod/hdb_new/DAE.conf  $(INSTALL_DAE_CFG)/
	cp -pf $(NLBUILDROOT)/prod/hdb_new/table_alias.conf  $(INSTALL_DAE_CFG)/

installer64:preparefile
	@echo "INFO: Create 64-bit installer package"
	'$(ISCMDBLD)' -x -p "$(ISM_FILE_DIR)/$(ISM_TEMPLATE_FILE_NAME_X64)" -b "$(ISM_OUTPUT_DIR)"
	@if [ $(OFFICIALCERT) -ne 0 ]; then \
		echo "INFO: Signing $(MSI_FILE64) msi file" ; \
		cd "$(ISM_OUTPUT_DIR)/PROJECT_ASSISTANT/SINGLE_MSI_IMAGE/DiskImages/DISK1/" ; \
		echo "'$(SIGNTOOL_OFFICIAL_TOOL)' $(SIGNTOOL_OFFICIAL_ARGS) " ; \
		'$(SIGNTOOL_OFFICIAL_TOOL)' $(SIGNTOOL_OFFICIAL_ARGS) ; \
	fi

publish:installer64
	@echo "INFO: Publish installer packages"
	@if [ "${MSI_FILE64}" != "" ]; then \
		mkdir -p '$(PUBLISH_DIR)' ; \
		cp -pf '$(MSI_FILE_PATH)' '$(PUBLISH_DIR)' ; \
		cp -pf '$(ISM_FILE_DIR)\resource\Policy_DAE4SAP.bin' '$(PUBLISH_DIR)' ; \
		cd '$(PUBLISH_DIR)'; zip '..\$(PACKAGE_FILE64)' * ; \
	fi
