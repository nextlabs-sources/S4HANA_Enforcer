#DYNAMICCRT: yes/no
DYNAMICCRT=yes
include ${NLBUILDROOT}/prod/build_windows/Makefile

#target
SUBTARGET=dae_main

#add include path
compilerflags += /I../tool/


SRCS=./dae_main.cpp ./PreStartLog.cpp
OBJS = $(addprefix $(BINDIR)/, $(addsuffix .obj, $(basename $(notdir $(SRCS)))))

#library path
ifeq ($(TARGETENVARCH), x64)
linkerflags += 
else
linkerflags +=   
endif

linkerflags += /LIBPATH:..\tool\win_$(TARGETENVARCH)_$(BUILDTYPE)

compilerflags += /DWIN32_LEAN_AND_MEAN /DLIBTOOL_STATIC

#library
linkerflags += Shell32.lib Shlwapi.lib tool_static.lib

all: $(OBJS)  $(BINDIR)/$(SUBTARGET).dll

$(BINDIR)/$(SUBTARGET).dll: $(OBJS) $(RCOBJS)
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
