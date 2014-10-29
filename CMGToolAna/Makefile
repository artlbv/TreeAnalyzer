# Makefile for the CMG ntuple analysis
#
# Author: Dirk Kruecker, 3.6.203, AL
#
#----------------------- here you can input your files and libraries ----------
#
# we compile all *.C and *.cpp defined in SOURCES
#

  SOURCES =  src/ClassObjects.C
  SOURCES += src/testReader.C

# and create this executable
#
  EXEC=reader

# with additional libs and includes
# e.g. MORELIBS = -L/path -lblub
  MORELIBS = -L./
# e.g. -I/path
#  MOREINCS = -I classes -I ExRootAnalysis

OBJS	  = $(SOURCES)
OBJS     := $(OBJS:.cpp=.o)
OBJS     := $(OBJS:.C=.o)

$(info Using the following files: $(SOURCES) )
$(info Using the following files: $(OBJS) )



#------------------------------------------------------------------------------

ifndef ROOTSYS
  $(error ROOTSYS is not defined!)
endif

ROOTLIBS    = `root-config --libs` -lGenVector
CXX         = clang++
CXXFLAGS    = -g `root-config --cflags` -std=c++11 -stdlib=libstdc++ -I./
LD          = clang++
LDFLAGS     = `root-config --ldflags`

.SUFFIXES: .cpp .C .o .so

#------------------------------------------------------------------------------

all:           $(EXEC)

$(EXEC):  $(OBJS)  Dict.o
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
		$(CXX) $(CXXFLAGS) -c $<
.C.o:
		$(CXX) $(CXXFLAGS) -c $<
