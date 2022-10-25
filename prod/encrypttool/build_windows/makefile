#DYNAMICCRT: yes/no
DYNAMICCRT=yes
BIN_TYPE=EXE
include ${NLBUILDROOT}/prod/build_windows/Makefile

#target
SUBTARGET=encrypttool

#add include path
compilerflags += /I../tool


SRCS=./encrypttool.cpp
OBJS = $(addprefix $(BINDIR)/, $(addsuffix .obj, $(basename $(notdir $(SRCS)))))

#library path
ifeq ($(TARGETENVARCH), x64)
linkerflags += /LIBPATH:../tool/win_${TARGETENVARCH}_${BUILDTYPE}
else
linkerflags +=   
endif

compilerflags += /DWIN32_LEAN_AND_MEAN

#library
linkerflags += tool.lib

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
