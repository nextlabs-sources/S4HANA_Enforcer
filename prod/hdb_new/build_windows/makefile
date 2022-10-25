#DYNAMICCRT: yes/no
DYNAMICCRT=yes

include ${NLBUILDROOT}/prod/build_windows/Makefile

#target
SUBTARGET=hdb_new


#add include path
compilerflags += /I./include /I../tool /I../hdb/include /I../QueryCloudAZSDKCpp/include /I../hdb_policymgr/include /I$(BOOST_ROOT) /I$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win32/include 
compilerflags += /I./parser_yacc /I../Detours/include

SRCS=src/EnforcerCtxImpl.cpp src/hdb_new_windows.cpp src/hdbEnforce.cpp src/KeyMaskOpr.cpp src/MaskStrategyImpl.cpp 
SRCS+= src/MyDisp+Work_windows.cpp src/NxlDbDatamgr.cpp src/NXS4HConfig_remote.cpp src/NXS4HConfig.cpp src/PolicyManager.cpp src/QueryCAZPolicyOpr.cpp 
SRCS+= src/SAPMsgNotify.cpp src/SqlDbcResourceInfoMgr.cpp src/SqlDbcUserInfoMgr.cpp src/TableAliasItem.cpp src/PreStartLogProxy.cpp
SRCS+= src/TableAliasMgr_remote.cpp src/TableAliasMgr.cpp src/TablemetaDataImpl.cpp src/talk_with_hdb.cpp src/ViewManager.cpp src/QueryCAZPolicyCache.cpp src/TableDataCache.cpp
#SRCS=$(wildcard  ./src/*.cpp)
OBJS = $(addprefix $(BINDIR)/, $(addsuffix .obj, $(basename $(notdir $(SRCS)))))


SRCS_PARSER_YACC=/parser_yacc/EMMaskOpr.cpp  ./parser_yacc/node.cpp ./parser_yacc/sqlparser_hana_bison.cpp ./parser_yacc/sqlparser_hana_flex.cpp
SRCS_PARSER_YACC+= ./parser_yacc/parse_tool.cpp ./parser_yacc/EMS4HAst.cpp  ./parser_yacc/EMS4HSelectAst.cpp ./parser_yacc/EMS4HUpdateAst.cpp
SRCS_PARSER_YACC+= ./parser_yacc/EMS4HInsertAst.cpp  ./parser_yacc/EMS4HDeleteAst.cpp ./parser_yacc/EMS4HUpsertAst.cpp ./parser_yacc/EMS4HMutliSelectAst.cpp
SRCS_PARSER_YACC+= ./parser_yacc/EMMaskOpr.cpp
OBJS_PARSER_YACC = $(addprefix $(BINDIR)/parser_yacc/, $(addsuffix .obj, $(basename $(notdir $(SRCS_PARSER_YACC)))))

SRCS_PARSER_VIEW=  ./parser_view/EMViewStmt.cpp ./parser_view/EMColumnExpr.cpp ./parser_view/EMTableExpr.cpp ./parser_view/EMParser.cpp  ./parser_view/EMViewMgr.cpp
OBJS_PARSER_VIEW =  $(addprefix $(BINDIR)/parser_view/, $(addsuffix .obj, $(basename $(notdir $(SRCS_PARSER_VIEW)))))

#library path
ifeq ($(TARGETENVARCH), x64)
linkerflags += /LIBPATH:$(BOOST_ROOT)/lib64-msvc-14.0 \
               /LIBPATH:$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win64/lib
else
linkerflags += /LIBPATH:$(BOOST_ROOT)/lib32-msvc-14.0 \
               /LIBPATH:$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win32/lib
endif

linkerflags += /LIBPATH:../tool/win_$(TARGETENVARCH)_$(BUILDTYPE) /LIBPATH:../jsoncpp/win_$(TARGETENVARCH)_$(BUILDTYPE)
linkerflags += /LIBPATH:../hdb_policymgr/win_$(TARGETENVARCH)_$(BUILDTYPE)
linkerflags += /LIBPATH:../QueryCloudAZSDKCpp/win_$(TARGETENVARCH)_$(BUILDTYPE)
linkerflags += /LIBPATH:../DEtours/lib.$(TARGETENVARCH)

#compilerflags += /DBOOST_ALL_DYN_LINK /DWIN32_LEAN_AND_MEAN /DHDB_POLICYMGR_IMP /DYY_NO_UNISTD_H /DDAE_LOCAL_CONFIG
compilerflags += /DBOOST_ALL_DYN_LINK /DWIN32_LEAN_AND_MEAN /DHDB_POLICYMGR_IMP /DYY_NO_UNISTD_H

#library
linkerflags += tool.lib ssleay32.lib libeay32.lib jsoncpp.lib Ws2_32.lib hdb_policymgr.lib QueryCloudAZSDKcpp.lib Kernel32.lib Shlwapi.lib  
linkerflags += Shell32.lib detours.lib

all: $(OBJS) $(OBJS_PARSER_YACC) $(OBJS_PARSER_VIEW) $(BINDIR)/$(SUBTARGET).dll

PREPARE_COMPILE:
	@if [ ! -d ../QueryCloudAZSDKCpp/$(BINDIR) ] ; then \
			mkdir ../QueryCloudAZSDKCpp/$(BINDIR) ; \
	fi
	cp ${MSENFORCECOMMON}/bin/$(BUILDTYPE)_win_$(TARGETENVARCH)/QueryCloudAZSDKcpp.lib ../QueryCloudAZSDKCpp/$(BINDIR)/
	cp ${MSENFORCECOMMON}/bin/$(BUILDTYPE)_win_$(TARGETENVARCH)/QueryCloudAZSDKcpp.dll ../QueryCloudAZSDKCpp/$(BINDIR)/

$(BINDIR)/$(SUBTARGET).dll: $(OBJS) $(OBJS_PARSER_YACC) $(OBJS_PARSER_VIEW) $(RCOBJS)
	$(ld) $(linkerflags) $(OBJS) $(OBJS_PARSER_YACC) $(OBJS_PARSER_VIEW) $(RCOBJS) /OUT:$@ /PDB:$@.pdb
	@if [ $(OFFICIALCERT) -eq 1 ]; then \
		cd $(BINDIR) ; \
		echo "'$(SIGNTOOL_OFFICIAL_TOOL)' $(SIGNTOOL_OFFICIAL_ARGS) ";   \
		'$(SIGNTOOL_OFFICIAL_TOOL)' $(SIGNTOOL_OFFICIAL_ARGS) ;    \
	fi  

$(OBJS): ${BINDIR}/%.obj: src/%.cpp
		@if [ ! -d $(BINDIR) ] ; then \
			mkdir -p $(BINDIR) ; \
		fi
		$(cl) $(compilerflags) $< /Fo$@ 

$(OBJS_PARSER_YACC): ${BINDIR}/parser_yacc/%.obj: parser_yacc/%.cpp
		@if [ ! -d $(BINDIR)/parser_yacc ] ; then \
			mkdir -p $(BINDIR)/parser_yacc ; \
		fi
		$(cl) $(compilerflags) $< /Fo$@ 

$(OBJS_PARSER_VIEW): ${BINDIR}/parser_view/%.obj: parser_view/%.cpp
		@if [ ! -d $(BINDIR)/parser_view ] ; then \
			mkdir -p $(BINDIR)/parser_view ; \
		fi
		$(cl) $(compilerflags) $< /Fo$@ 
