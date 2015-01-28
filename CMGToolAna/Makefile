#Make file to compile TreeAnalzyer
#define code to be compiled
#  CODE = TreeAnalyzer_SingleEl
  CODE = TreeAnalyzer_GenStudy
#  CODE = TreeAnalyzer_example
#define sources
  SOURCE = src/$(CODE).cc
  EXTRA =  src/ClassObjects.cc
#
# and create this executable
#
  EXEC = $(CODE).exe
## if FastJet is being used (would need installation, maybe works within CMSSW?)
FASTJET=/nfs/dust/cms/user/clseitz/1LepSUS/CMSSW_7_0_6_patch1/src/fastjet-install/
MORELIBS = -L$(ROOTSYS)/lib  `$(FASTJET)/bin/fastjet-config --cxxflags --libs --plugins`

OBJS	  = $(notdir $(SOURCE))
OBJS     := $(OBJS:.cpp=.o)
OBJS     := $(OBJS:.cc=.o)



$(info Using the following files: $(SOURCE) $(EXTRA))
$(info Using the following files: $(OBJS) )



#------------------------------------------------------------------------------

ifndef ROOTSYS
  $(error ROOTSYS is not defined!)
endif

ROOTLIBS    = `root-config --cflags --libs` -lGenVector
LD          = g++
LDFLAGS     = `root-config --ldflags`

.SUFFIXES: .cpp .cc .o .so

#------------------------------------------------------------------------------

all:           $(EXEC)
$(EXEC):       $(SOURCE) $(EXTRA)
	$(LD) $(LDFLAGS) $^ $(ROOTLIBS)  -o $@
	@echo "$@ done"

clean:
	@echo rm -f $(EXEC) *Dict.*  core $(OBJS) *.d
	@rm -f $(EXEC) *Dict.*  core $(OBJS) *.d $(CODE)

