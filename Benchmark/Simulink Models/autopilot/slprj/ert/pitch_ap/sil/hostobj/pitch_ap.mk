###########################################################################
## Makefile generated for Simulink model 'pitch_ap'. 
## 
## Makefile     : pitch_ap.mk
## Generated on : Mon Feb 03 08:38:29 2014
## MATLAB Coder version: 2.5 (R2013b)
## 
## Build Info:
## 
## Final product: .\pitch_ap_rtwlib.lib
## Product type : static library
## Build type   : Model Reference Library
## 
###########################################################################

###########################################################################
## MACROS
###########################################################################

# Macro Descriptions:
# PRODUCT_NAME            Name of the system to build
# MAKEFILE                Name of this makefile
# COMPUTER                Computer type. See the MATLAB "computer" command.
# MODELREF_LINK_RSPFILE   Include paths for the model reference build
# PERL                    PERL Tool
# GEN_LNK_SCRIPT          Perl script to generate the command file
# CMD_FILE                Command file
# MODELLIB                Static library target
# MODELREFS               List of referenced models
# MODELREF_LINK_LIBS      Link libraries for the model reference build

PRODUCT_NAME              = pitch_ap
MAKEFILE                  = pitch_ap.mk
COMPUTER                  = PCWIN64
MATLAB_ROOT               = C:\PROGRA~1\MATLAB\R2013b
MATLAB_BIN                = C:\Program Files\MATLAB\R2013b\bin
MATLAB_ARCH_BIN           = C:\Program Files\MATLAB\R2013b\bin\win64
MASTER_ANCHOR_DIR         = 
START_DIR                 = C:\Users\bpotter\LocalWorkArea\demos\autopilot_R2013b_workarea
ARCH                      = win64
SOLVER                    = 
SOLVER_OBJ                = 
CLASSIC_INTERFACE         = 0
TGT_FCN_LIB               = C89/C90 (ANSI)
MODELREF_LINK_RSPFILE_NAME = pitch_ap_ref.rsp
RELATIVE_PATH_TO_ANCHOR   = ..\..\..\..\..
MODELREF_LINK_RSPFILE     = pitch_ap_ref.rsp
PERL                      = $(MATLAB_ROOT)\sys\perl\win32\bin\perl.exe
GEN_LNK_SCRIPT            = $(MATLAB_ROOT)\rtw\c\tools\mkvc_lnk.pl
CMD_FILE                  = $(PRODUCT_NAME).lnk
NODEBUG                   = 1
SHARED_SRC_DIR            = ..\..\..\..\..\slprj\ert\_sharedutils
SHARED_SRC                = $(SHARED_SRC_DIR)\*.c*
SHARED_BIN_DIR            = ..\..\..\..\..\slprj\ert\_sharedutils\sil\hostobj
SHARED_LIB                = $(SHARED_BIN_DIR)\rtwshared.lib
MODELLIB                  = pitch_ap_rtwlib.lib
MODELREFS                 = 
MODELREF_LINK_LIBS        = Altitude_Mode_rtwlib.lib attitude_controller_rtwlib.lib 

###########################################################################
## TOOLCHAIN SPECIFICATIONS
###########################################################################

# Toolchain Name:          Microsoft Windows SDK v7.1 | nmake (64-bit Windows)
# Supported Version(s):    7.1
# ToolchainInfo Version:   R2013b
# Specification Revision:  1.0
# 

!include <ntwin32.mak>


#-------------------------------------------
# Macros assumed to be defined elsewhere
#-------------------------------------------

# NODEBUG
# cvarsdll
# cvarsmt
# conlibsmt
# ldebug
# conflags
# cflags

#-----------
# MACROS
#-----------

MEX_OPTS_FILE       = $(MATLAB_ROOT)\bin\$(ARCH)\mexopts\mssdk71opts.bat
MW_EXTERNLIB_DIR    = $(MATLAB_ROOT)\extern\lib\win64\microsoft
MW_LIB_DIR          = $(MATLAB_ROOT)\lib\win64
MEX_ARCH            = -win64
CPU                 = AMD64
APPVER              = 5.02
CVARSFLAG           = $(cvarsmt)
CFLAGS_ADDITIONAL   = -D_CRT_SECURE_NO_WARNINGS
CPPFLAGS_ADDITIONAL = -EHs -D_CRT_SECURE_NO_WARNINGS
LIBS_TOOLCHAIN      = $(conlibs)

TOOLCHAIN_SRCS = 
TOOLCHAIN_INCS = 
TOOLCHAIN_LIBS = 

#------------------------
# BUILD TOOL COMMANDS
#------------------------

# C Compiler: Microsoft Visual C Compiler
CC = cl

# Linker: Microsoft Visual C Linker
LD = link

# C++ Compiler: Microsoft Visual C++ Compiler
CPP = cl

# C++ Linker: Microsoft Visual C++ Linker
CPP_LD = link

# Archiver: Microsoft Visual C/C++ Archiver
AR = lib

# MEX Tool: MEX Tool
MEX_PATH = $(MATLAB_BIN)
MEX = $(MEX_PATH)\mex

# Download: Download
DOWNLOAD =

# Execute: Execute
EXECUTE = $(PRODUCT)

# Builder Application: NMAKE Utility
MAKE = nmake


#-------------------------
# Directives/Utilities
#-------------------------

CDEBUG              = -Zi
C_OUTPUT_FLAG       = -Fo
LDDEBUG             = /DEBUG
OUTPUT_FLAG         = -out:
CPPDEBUG            = -Zi
CPP_OUTPUT_FLAG     = -Fo
CPPLDDEBUG          = /DEBUG
OUTPUT_FLAG         = -out:
ARDEBUG             =
STATICLIB_OUTPUT_FLAG = -out:
MEX_DEBUG           = -g
RM                  = @del
ECHO                = @echo
MV                  = @ren
RUN                 = @cmd /C

#----------------------------------------
# "Faster Builds" Build Configuration
#----------------------------------------

ARFLAGS              = /nologo
CFLAGS               = $(cflags) $(CVARSFLAG) $(CFLAGS_ADDITIONAL) \
                       /Od /Oy-
CPPFLAGS             = /TP $(cflags) $(CVARSFLAG) $(CPPFLAGS_ADDITIONAL) \
                       /Od /Oy-
CPP_LDFLAGS          = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN)
CPP_SHAREDLIB_LDFLAGS  = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN) \
                         -dll -def:$(DEF_FILE)
DOWNLOAD_FLAGS       =
EXECUTE_FLAGS        =
LDFLAGS              = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN)
MEX_CFLAGS           = $(MEX_ARCH) OPTIMFLAGS="/Od /Oy- $(MDFLAG) $(DEFINES)" $(MEX_OPTS_FLAG)
MEX_LDFLAGS          = LDFLAGS=='$$LDFLAGS'
MAKE_FLAGS           = -f $(MAKEFILE)
SHAREDLIB_LDFLAGS    = $(ldebug) $(conflags) $(LIBS_TOOLCHAIN) \
                       -dll -def:$(DEF_FILE)

#--------------------
# File extensions
#--------------------

H_EXT               = .h
OBJ_EXT             = .obj
C_EXT               = .c
EXE_EXT             = .exe
SHAREDLIB_EXT       = .dll
HPP_EXT             = .hpp
OBJ_EXT             = .obj
CPP_EXT             = .cpp
EXE_EXT             = .exe
SHAREDLIB_EXT       = .dll
STATICLIB_EXT       = .lib
MEX_EXT             = .mexw64
MAKE_EXT            = .mk


###########################################################################
## OUTPUT INFO
###########################################################################

PRODUCT = .\pitch_ap_rtwlib.lib
PRODUCT_TYPE = "static library"
BUILD_TYPE = "Model Reference Library"

#################################################################
# INCLUDE PATHS
#################################################################

INCLUDES_BUILDINFO = $(START_DIR)\slprj\ert\pitch_ap;$(START_DIR);$(MATLAB_ROOT)\extern\include;$(MATLAB_ROOT)\simulink\include;$(MATLAB_ROOT)\rtw\c\src;$(MATLAB_ROOT)\rtw\c\src\ext_mode\common;$(MATLAB_ROOT)\rtw\c\ert;$(START_DIR)\slprj\ert\_sharedutils;.\referenced_model_includes

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_UNSPECIFIED = -DMODEL=pitch_ap -DNUMST=1 -DNCSTATES=0 -DHAVESTDIO -DONESTEPFCN=1 -DTERMFCN=0 -DMAT_FILE=0 -DMULTI_INSTANCE_CODE=0 -DINTEGER_CODE=0 -DMT=0 -DCLASSIC_INTERFACE=0 -DTID01EQ=0 -DPORTABLE_WORDSIZES
DEFINES_BUILD_ARGS = -DONESTEPFCN=1 -DTERMFCN=0 -DMAT_FILE=0 -DMULTI_INSTANCE_CODE=0 -DINTEGER_CODE=0 -DMT=0 -DCLASSIC_INTERFACE=0
DEFINES_IMPLIED = -DTID01EQ=0
DEFINES_STANDARD = -DMODEL=pitch_ap -DNUMST=1 -DNCSTATES=0 -DHAVESTDIO

DEFINES = $(DEFINES_UNSPECIFIED) $(DEFINES_BUILD_ARGS) $(DEFINES_IMPLIED) $(DEFINES_STANDARD)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(START_DIR)\slprj\ert\pitch_ap\pitch_ap.c

ALL_SRCS = $(SRCS)

###########################################################################
## OBJECTS
###########################################################################

OBJS = pitch_ap.obj

ALL_OBJS = $(OBJS)

###########################################################################
## PREBUILT OBJECT FILES
###########################################################################

PREBUILT_OBJS = 

###########################################################################
## LIBRARIES
###########################################################################

LIBS = $(SHARED_LIB)

###########################################################################
## SYSTEM LIBRARIES
###########################################################################

SYSTEM_LIBS = 

###########################################################################
## ADDITIONAL TOOLCHAIN FLAGS
###########################################################################

#---------------
# C Compiler
#---------------

CFLAGS_BASIC = $(DEFINES) 

CFLAGS = $(CFLAGS) $(CFLAGS_BASIC)

#-----------------
# C++ Compiler
#-----------------

CPPFLAGS_BASIC = $(DEFINES) 

CPPFLAGS = $(CPPFLAGS) $(CPPFLAGS_BASIC)

###########################################################################
## PHONY TARGETS
###########################################################################

.PHONY : all build clean info prebuild download execute set_environment_variables


all : build
	@cmd /C "@echo ### Successfully generated all binary outputs."


build : set_environment_variables prebuild $(PRODUCT)


prebuild : 


download : build


execute : download


set_environment_variables : 
	@set INCLUDE=$(INCLUDES);$(INCLUDE)
	@set LIB=$(LIB)


###########################################################################
## FINAL TARGET
###########################################################################

#---------------------------------
# Create a static library         
#---------------------------------

$(PRODUCT) : $(OBJS) $(PREBUILT_OBJS) $(LIBS)
	$(PERL) $(GEN_LNK_SCRIPT) $(CMD_FILE) $(OBJS)
	@cmd /C "@echo ### Creating static library "$(PRODUCT)" ..."
	$(AR) $(ARFLAGS) -out:$(PRODUCT) @$(CMD_FILE)
	@cmd /C "@echo ### Created: $(PRODUCT)"
	$(RM) $(CMD_FILE)


###########################################################################
## INTERMEDIATE TARGETS
###########################################################################

#---------------------
# SOURCE-TO-OBJECT
#---------------------

{..\..}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{..\..}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(RELATIVE_PATH_TO_ANCHOR)}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(MATLAB_ROOT)\rtw\c\src}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(MATLAB_ROOT)\rtw\c\src}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(MATLAB_ROOT)\simulink\src}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(MATLAB_ROOT)\simulink\src}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(START_DIR)}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(START_DIR)}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


{$(START_DIR)\slprj\ert\pitch_ap}.c.obj :
	$(CC) $(CFLAGS) -Fo"$@" "$<"


{$(START_DIR)\slprj\ert\pitch_ap}.cpp.obj :
	$(CPP) $(CPPFLAGS) -Fo"$@" "$<"


#-----------------------------
# BUILD-IN-PLACE LIBRARIES
#-----------------------------


SHARED_OBJS = $(SHARED_BIN_DIR)\*.obj

$(SHARED_LIB) : $(SHARED_SRC)
	@cmd /C "echo $?" > $(SHARED_SRC_DIR)\SHARED_RSP.txt
	@$(CC) $(CFLAGS) -Fo$(SHARED_BIN_DIR)\ @$(SHARED_SRC_DIR)\SHARED_RSP.txt
	@cmd /C "del $(SHARED_SRC_DIR)\SHARED_RSP.txt"
	$(AR) $(ARFLAGS) -out:$(SHARED_LIB) $(SHARED_OBJS)


###########################################################################
## DEPENDENCIES
###########################################################################

$(ALL_OBJS) : $(MAKEFILE) rtw_proj.tmw


###########################################################################
## MISCELLANEOUS TARGETS
###########################################################################

info : 
	@cmd /C "@echo ###  PRODUCT = $(PRODUCT)"
	@cmd /C "@echo ###  PRODUCT_TYPE = $(PRODUCT_TYPE)"
	@cmd /C "@echo ###  BUILD_TYPE = $(BUILD_TYPE)"
	@cmd /C "@echo ###  INCLUDES = $(INCLUDES)"
	@cmd /C "@echo ###  DEFINES = $(DEFINES)"
	@cmd /C "@echo ###  ALL_SRCS = $(ALL_SRCS)"
	@cmd /C "@echo ###  ALL_OBJS = $(ALL_OBJS)"
	@cmd /C "@echo ###  LIBS = $(LIBS)"
	@cmd /C "@echo ###  MODELREF_LIBS = $(MODELREF_LIBS)"
	@cmd /C "@echo ###  SYSTEM_LIBS = $(SYSTEM_LIBS)"
	@cmd /C "@echo ###  TOOLCHAIN_LIBS = $(TOOLCHAIN_LIBS)"
	@cmd /C "@echo ###  CFLAGS = $(CFLAGS)"
	@cmd /C "@echo ###  LDFLAGS = $(LDFLAGS)"
	@cmd /C "@echo ###  SHAREDLIB_LDFLAGS = $(SHAREDLIB_LDFLAGS)"
	@cmd /C "@echo ###  CPPFLAGS = $(CPPFLAGS)"
	@cmd /C "@echo ###  CPP_LDFLAGS = $(CPP_LDFLAGS)"
	@cmd /C "@echo ###  CPP_SHAREDLIB_LDFLAGS = $(CPP_SHAREDLIB_LDFLAGS)"
	@cmd /C "@echo ###  ARFLAGS = $(ARFLAGS)"
	@cmd /C "@echo ###  MEX_CFLAGS = $(MEX_CFLAGS)"
	@cmd /C "@echo ###  MEX_LDFLAGS = $(MEX_LDFLAGS)"
	@cmd /C "@echo ###  DOWNLOAD_FLAGS = $(DOWNLOAD_FLAGS)"
	@cmd /C "@echo ###  EXECUTE_FLAGS = $(EXECUTE_FLAGS)"
	@cmd /C "@echo ###  MAKE_FLAGS = $(MAKE_FLAGS)"


clean : 
	$(ECHO) "### Deleting all derived files..."
	@if exist $(PRODUCT) $(RM) $(PRODUCT)
	$(RM) $(ALL_OBJS)
	$(ECHO) "### Deleted all derived files."


