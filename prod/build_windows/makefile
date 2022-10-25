cl=cl.exe
ld=link.exe
RC=rc.exe
static_ld=lib.exe
SUBTARGET_EXT=dll
srcdir=./src
compilerflags= /c  /Fd /Zi /EHsc /W3 /Gm-  /DWIN32 /FS
linkerflags=/DEBUG 
staticflags= 

ifneq ($(BIN_TYPE), EXE)
linkerflags+= /DLL
endif

BIN_TYPE=

ifeq ($(OFFICIALCERT), )
OFFICIALCERT=0
endif

ifeq ($(VERSION_BUILD), )
VERSION_BUILD=1
endif

#disable warning
compilerflags += /wd4101



RCFLAGS += -D_UNICODE -DUNICODE

BUILDROOT_LINUX=$(subst \,/,$(BUILDROOT))

PROGRAM_FILES_X86=C:/Program Files (x86)
#SIGNTOOL_OFFICIAL_TOOL=$(PROGRAM_FILES_X86)/Windows Kits/8.1/bin/x64/signtool.exe
#SIGNTOOL_OFFICIAL_ARGS=sign /ac c:/release/bin/DigiCertAssuredIDRootCA.cer /f c:/release/bin/NextLabs.pfx /p IiVf1itvOrqJ /n "NextLabs Inc." /fd sha256 /tr http://timestamp.digicert.com
SIGNTOOL_OFFICIAL_TOOL=$(NLBUILDROOT)/prod/build_windows/$(SIGN_SCRIPT_NAME)
SIGNTOOL_OFFICIAL_ARGS=


# By default, use build/config/use_default.rc. RCSRC in Makefile.inc overrides 
# this value.
RCSRC = $(srcdir)/use_default.rc


ifeq ($(BUILDTYPE), release)
compilerflags +=  /O2 
compilerflags +=  /DNDEBUG
ifeq ($(DYNAMICCRT), yes) 
compilerflags += /MD
else
compilerflags += /MT
endif 

else
compilerflags += /Od  /D"_DEBUG"

ifeq ($(DYNAMICCRT), yes)
compilerflags += /MDd
else
compilerflags += /MTd
endif 

endif 

compilerflags += /D_WIN32_WINNT=0x0601
compilerflags += /W4
# compilerflags += /WX
compilerflags += /wd4005    # macro redefinition
compilerflags += /wd4099	# type name first seen using 'class' now seen using 'struct'
compilerflags += /wd4267 	# conversion from 'size_t' to 'int', possible loss of data
compilerflags += /wd4244 	# onversion from 'int' to 'yytype_int16', possible loss of data
compilerflags += /wd4389	# '==': signed/unsigned mismatch, too match warning in the code, disable it now, but it is not a good way
compilerflags += /wd4800    # 'BOOL': forcing value to bool 'true' or 'false'
compilerflags += /wd4819    # The file contains a character that cannot be represented in the current code page (936). Save the file in Unicode format to prevent data loss

ifneq ($(DEF),)
linkerflags += /DEF:$(DEF)
endif

BINDIR=win_$(TARGETENVARCH)_$(BUILDTYPE)
#OBJS = $(addprefix $(BINDIR)/, $(addsuffix .obj, $(basename $(notdir $(SRC)))))
#RCOBJS = $(addprefix $(BINDIR)/, $(addsuffix .res, $(basename $(notdir $(RCSRC)))))
#TARGET = $(BINDIR)/$(SUBTARGET).$(SUBTARGET_EXT);

