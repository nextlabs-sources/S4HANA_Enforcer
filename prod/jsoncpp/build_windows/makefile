#DYNAMICCRT: yes/no
DYNAMICCRT=yes

include ${NLBUILDROOT}/prod/build_windows/Makefile

#target
SUBTARGET=jsoncpp


#add include path
compilerflags += /I./include /I../tool /I../jsoncpp/include 

#SRCS=src/Lex.cpp src/log.cpp src/NXLHttpClient.cpp src/parse_expression.cpp src/patch.cpp src/policy_expression.cpp src/policy_regex.cpp src/Policy.cpp src/PolicyExport.cpp src/PolicyModelList.cpp src/TalkWithCC_Request.cpp src/TalkWithCC_Search.cpp src/TalkWithCC.cpp
SRCS=$(wildcard  ./src/lib_json/*.cpp)
OBJS = $(addprefix $(BINDIR)/, $(addsuffix .obj, $(basename $(notdir $(SRCS)))))


#library path
ifeq ($(TARGETENVARCH), x64)
linkerflags += 
else
linkerflags += 
endif

linkerflags += 
compilerflags +=  /DWIN32_LEAN_AND_MEAN

#library
linkerflags +=

all: $(OBJS) $(BINDIR)/$(SUBTARGET).lib

$(BINDIR)/$(SUBTARGET).lib: $(OBJS)
	$(static_ld) $(staticflags) $(OBJS) /OUT:$@

$(OBJS): ${BINDIR}/%.obj: src/lib_json/%.cpp
		@if [ ! -d $(BINDIR) ] ; then \
			mkdir -p $(BINDIR) ; \
		fi
		$(cl) $(compilerflags) $< /Fo$@ 

