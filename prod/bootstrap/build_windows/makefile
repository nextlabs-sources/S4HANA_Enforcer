#DYNAMICCRT: yes/no
DYNAMICCRT=yes
BIN_TYPE=EXE
include ${NLBUILDROOT}/prod/build_windows/Makefile

#target
SUBTARGET=bootstrap

#add include path
compilerflags += /I$(BOOST_ROOT) /I$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win32/include /I../tool/


SRCS= ./CommonTools_windows.cpp ./CommonTools.cpp ./AddDependence_windows.cpp ./GetInternalFunction_windows.cpp ./log.cpp ./main.cpp ./NXLHttpClientAsync.cpp ./NXS4HConfig.cpp ./VerifyConfig.cpp
OBJS = $(addprefix $(BINDIR)/, $(addsuffix .obj, $(basename $(notdir $(SRCS)))))

#library path
ifeq ($(TARGETENVARCH), x64)
linkerflags += /LIBPATH:$(BOOST_ROOT)/lib64-msvc-14.0 \
               /LIBPATH:$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win64/lib
else
linkerflags += /LIBPATH:$(BOOST_ROOT)/lib32-msvc-14.0 \
               /LIBPATH:$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win32/lib
endif

linkerflags += /LIBPATH:..\tool\win_$(TARGETENVARCH)_$(BUILDTYPE)

compilerflags += /DWIN32_LEAN_AND_MEAN

#library
linkerflags += Shell32.lib Shlwapi.lib Shell32.lib tool.lib ssleay32.lib libeay32.lib dbghelp.lib

all: $(OBJS)  $(BINDIR)/$(SUBTARGET).exe

$(BINDIR)/$(SUBTARGET).exe: $(OBJS) $(RCOBJS)
	$(ld) $(linkerflags) $(OBJS) $(RCOBJS) /OUT:$@ /PDB:$@.pdb
	@if [ $(OFFICIALCERT) -eq 1 ]; then \
		cd $(BINDIR) ; \
		echo "'$(SIGNTOOL_OFFICIAL_TOOL)' $(SIGNTOOL_OFFICIAL_ARGS) ";   \
		'$(SIGNTOOL_OFFICIAL_TOOL)' $(SIGNTOOL_OFFICIAL_ARGS) ;    \
	fi  

$(OBJS): ${BINDIR}/%.obj: ./%.cpp
		@if [ ! -d $(BINDIR) ] ; then \
			mkdir -p $(BINDIR) ; \
		fi
		$(cl) $(compilerflags) $< /Fo$@ 
