SOURCES=src/TreeAnalyzer_FastJet.C
EXECUTABLE=TreeAnalyzer_FastJet
OBJECTS=$(SOURCES:.cpp=.o)
FASTJET=/nfs/dust/cms/user/clseitz/1LepSUS/CMSSW_7_0_6_patch1/src/fastjet-install/

CC=g++
CFLAGS=-c -I$(ROOTSYS)/include 
LDFLAGS=-L$(ROOTSYS)/lib $(shell root-config --glibs) $(shell root-config --cflags) `$(FASTJET)/bin/fastjet-config --cxxflags --libs --plugins`

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -rf *.o *~