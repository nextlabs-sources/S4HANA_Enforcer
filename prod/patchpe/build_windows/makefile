#DYNAMICCRT: yes/no
DYNAMICCRT=yes

#target
SUBTARGET=patchpe
SUBTARGET_EXTENSION=exe
BIN_TYPE=EXE

include ${NLBUILDROOT}/prod/build_windows/Makefile


#add include path
compilerflags += /I./  /I$(BOOST_ROOT) /I$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win32/include 

SRCS=CLHelper.cpp CommonTools.cpp FileMappingBase.cpp Main.cpp PEBase.cpp PEIDTInjection.cpp PEMgr.cpp PESectionInfo.cpp PETools.cpp PEView.cpp
OBJS = $(addprefix $(BINDIR)/, $(addsuffix .obj, $(basename $(notdir $(SRCS)))))


#library path
ifeq ($(TARGETENVARCH), x64)
linkerflags += /LIBPATH:$(BOOST_ROOT)/lib64-msvc-14.0 \
               /LIBPATH:$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win64/lib
else
linkerflags += /LIBPATH:$(BOOST_ROOT)/lib32-msvc-14.0 \
               /LIBPATH:$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win32/lib
endif

linkerflags += /LIBPATH:../commonlib/win_$(TARGETENVARCH)_$(BUILDTYPE)
compilerflags += /DBOOST_ALL_DYN_LINK /DLIBTOOL_IMP

#library
linkerflags += Shell32.lib

all: $(OBJS) $(BINDIR)/$(SUBTARGET).$(SUBTARGET_EXTENSION)

$(BINDIR)/$(SUBTARGET).$(SUBTARGET_EXTENSION): $(OBJS) $(RCOBJS)
	$(ld) $(linkerflags) $(OBJS) $(RCOBJS) /OUT:$@ /PDB:$@.pdb
	@if [ $(OFFICIALCERT) -eq 1 ]; then \
		cd $(BINDIR) ; \
		echo "'$(SIGNTOOL_OFFICIAL_TOOL)' $(SIGNTOOL_OFFICIAL_ARGS) ";   \
		'$(SIGNTOOL_OFFICIAL_TOOL)' $(SIGNTOOL_OFFICIAL_ARGS) ;    \
	fi  

$(OBJS): ${BINDIR}/%.obj: %.cpp
		@if [ ! -d $(BINDIR) ] ; then \
			mkdir -p $(BINDIR) ; \
		fi
		$(cl) $(compilerflags) $< /Fo$@ 

