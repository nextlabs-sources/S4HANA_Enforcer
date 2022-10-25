#DYNAMICCRT: yes/no
DYNAMICCRT=yes

include ${NLBUILDROOT}/prod/build_windows/Makefile

#target
SUBTARGET=hdb_policymgr


#add include path
compilerflags += /I./include /I../tool /I../jsoncpp/include /I$(BOOST_ROOT) /I$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win32/include 

SRCS=src/Lex.cpp src/log.cpp src/NXLHttpClient.cpp src/parse_expression.cpp src/patch.cpp src/policy_expression.cpp src/policy_regex.cpp src/Policy.cpp src/PolicyEngine_windows.cpp src/PolicyExport.cpp src/PolicyModelList.cpp src/TalkWithCC_Request.cpp src/TalkWithCC_Search.cpp src/TalkWithCC.cpp
#SRCS=$(wildcard  ./src/*.cpp)
OBJS = $(addprefix $(BINDIR)/, $(addsuffix .obj, $(basename $(notdir $(SRCS)))))


#library path
ifeq ($(TARGETENVARCH), x64)
linkerflags += /LIBPATH:$(BOOST_ROOT)/lib64-msvc-14.0 \
               /LIBPATH:$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win64/lib
else
linkerflags += /LIBPATH:$(BOOST_ROOT)/lib32-msvc-14.0 \
               /LIBPATH:$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win32/lib
endif

linkerflags += /LIBPATH:../tool/win_$(TARGETENVARCH)_$(BUILDTYPE) /LIBPATH:../jsoncpp/win_$(TARGETENVARCH)_$(BUILDTYPE)
compilerflags += /DBOOST_ALL_DYN_LINK /DWIN32_LEAN_AND_MEAN /DHDB_POLICYMGR_IMP

#library
linkerflags += tool.lib ssleay32.lib libeay32.lib jsoncpp.lib

all: $(OBJS) $(BINDIR)/$(SUBTARGET).dll

$(BINDIR)/$(SUBTARGET).dll: $(OBJS) $(RCOBJS)
	$(ld) $(linkerflags) $(OBJS) $(RCOBJS) /OUT:$@ /PDB:$@.pdb
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

