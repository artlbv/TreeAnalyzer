# Makefile for the DesySusy ntuple analysis
#
# Author: Dirk Kruecker, 3.6.203
#
#----------------------- here you can input your files and libraries ----------
#
# we compile all *.C and *.cpp defined in SOURCES
#


  SOURCES = src/TreeAnalyzer_FastJet.C 
  SOURCES +=  src/ClassObjects.C
# and create this executable
#
  EXEC=TreeAnalyzer_FastJet

# with additional libs and includes
FASTJET=/nfs/dust/cms/user/clseitz/1LepSUS/CMSSW_7_0_6_patch1/src/fastjet-install/
# e.g. MORELIBS = -L/path -lblub
  MORELIBS = -L$(ROOTSYS)/lib  `$(FASTJET)/bin/fastjet-config --cxxflags --libs --plugins`
# e.g. -I/path
  MOREINCS = -I classes -I ExRootAnalysis 

OBJS	  = $(notdir $(SOURCES))
OBJS     := $(OBJS:.cpp=.o)
OBJS     := $(OBJS:.C=.o)



$(info Using the following files: $(SOURCES) )
$(info Using the following files: $(OBJS) )



#------------------------------------------------------------------------------

ifndef ROOTSYS
  $(error ROOTSYS is not defined!)
endif

ROOTLIBS    = `root-config --cflags --libs` -lGenVector
CXX         = g++
CXXFLAGS    = -g `root-config --cflags` -I./
LD          = g++
LDFLAGS     = `root-config --ldflags`

.SUFFIXES: .cpp .C .o .so

#------------------------------------------------------------------------------

all:           $(EXEC)

$(EXEC):  $(SOURCES)  Dict.o
	$(LD) $(LDFLAGS) $^ $(ROOTLIBS) $(MORELIBS)  -o $@
	@echo "$@ done"

Dict.cxx: Linkdef.h
		@echo "Generating dictionary $@..."
		@rootcint -f $@ -c  -p TLorentzVector.h Math/LorentzVector.h $^
Dict.o: Dict.cxx
	 $(CXX) $(CXXFLAGS) -D __NTHEADER___ -c $<	
clean:
	@echo rm -f $(EXEC) *Dict.*  core $(OBJS) *.d
	@rm -f $(EXEC) *Dict.*  core $(OBJS) *.d

.cpp.o:
		$(CXX) $(CXXFLAGS) $(MORELIBS) -c $(SOURCES)
.C.o:
		$(CXX) $(CXXFLAGS) $(MORELIBS) -c $(SOURCES)
