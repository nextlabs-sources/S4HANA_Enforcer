#DYNAMICCRT: yes/no
DYNAMICCRT=yes

include ${NLBUILDROOT}/prod/build_windows/Makefile

#target
SUBTARGET=QueryCloudAZSDKCpp


#add include path
compilerflags += /I./include  /I$(BOOST_ROOT) /I$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win32/include 

SRCS=   src/Attributes.cpp \
        src/CommonFun.cpp \
        src/Connection.cpp \
        src/ConnectionPool.cpp \
        src/NXLHttpClientAsync.cpp \
        src/Obligation.cpp \
        src/PolicyResult.cpp \
        src/PolicyRequest.cpp \
        src/QueryCloudAZExport.cpp \
        src/QueryCloudAZManager

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

compilerflags += /DBOOST_ALL_DYN_LINK /DWIN32_LEAN_AND_MEAN /DQUERYCLOUDAZ_EXPORT

#library
linkerflags += ssleay32.lib libeay32.lib

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

