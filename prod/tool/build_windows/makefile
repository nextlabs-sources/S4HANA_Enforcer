#DYNAMICCRT: yes/no
DYNAMICCRT=yes

include ${NLBUILDROOT}/prod/build_windows/Makefile

#target
SUBTARGET=tool


#add include path
compilerflags += /I./  /I$(BOOST_ROOT) /I$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win32/include 

SRCS=aes.cpp AESEncrypt.cpp log.cpp tool.cpp md5.cpp
OBJS = $(addprefix $(BINDIR)/, $(addsuffix .obj, $(basename $(notdir $(SRCS)))))
OBJS_static = $(addprefix $(BINDIR)/, $(addsuffix _static.obj, $(basename $(notdir $(SRCS)))))


#library path
ifeq ($(TARGETENVARCH), x64)
linkerflags += /LIBPATH:$(BOOST_ROOT)/lib64-msvc-14.0 \
               /LIBPATH:$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win64/lib
else
linkerflags += /LIBPATH:$(BOOST_ROOT)/lib32-msvc-14.0 \
               /LIBPATH:$(OPENSSL_ROOT)/OpenSSL-1.0.1e-Win32/lib
endif

linkerflags += /LIBPATH:../commonlib/win_$(TARGETENVARCH)_$(BUILDTYPE)
#compilerflags += /DBOOST_ALL_DYN_LINK /DLIBTOOL_IMP

#library
linkerflags += Shell32.lib

all: $(BINDIR)/$(SUBTARGET)_static.lib $(BINDIR)/$(SUBTARGET).dll

$(BINDIR)/$(SUBTARGET).dll: $(OBJS) $(RCOBJS)
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
		$(cl) $(compilerflags) /DBOOST_ALL_DYN_LINK /DLIBTOOL_IMP $< /Fo$@ 

$(BINDIR)/$(SUBTARGET)_static.lib: $(OBJS_static)
	$(static_ld) $(OBJS_static) /OUT:$@

$(OBJS_static): ${BINDIR}/%_static.obj: %.cpp
		@if [ ! -d $(BINDIR) ] ; then \
			mkdir -p $(BINDIR) ; \
		fi
		$(cl) $(compilerflags) /DLIBTOOL_STATIC $< /Fo$@ 