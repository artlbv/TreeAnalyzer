# TreeAnalyzer for CMGtuples
This code is used to run ntuples produced by CMG tools.
It is not necessary to know the exact content of the ntuple beforehand, branches
can be read on the fly. The tree name is hard coded and would need to be adjusted.

##Explanation:

* `src/ClassObjects.cc`

  This file contains all the object definitions (leptons, jets, met, genparticles etc).
  The function GetKinVariables() contains event variables such as HT, ST, etc.
  All varibales and functions are defined in `interface/ClassObjects.h`

* `src/TreeAnalyzer_example.cc`

  Contains the example code to run over ntuples with the event loop.
  A few basic plots are filled and examples are given for a more complex histogram structure.
  The cutflow can also be written out here into a textfile.

* `Makefile`

  The variable CODE needs the name of the analyzer in the 
  src directory that should be compiled (i.e.: TreeAnalzyer_example).

##How to get code to run:
```bash
make
cd python
./runAnalyzer.py TreeAnalyzer_example MC_TTbar
```

-------------------------------------------------------------------------------

### More info
* runAnalyzer.py: python run script that executes the chosen tree analyzer
* DESYsamples.py: defines all samples, cross sections, number of events, lumi
* TreeAnalyzer_X.cc: actuall C++ code to run over the ntuple and make plots
* NtupleTools2.h: code to read the nutple on the fly
* go.sh, setupNAF.sh: scripts to submit jobs at DESY run like

  `./go.sh TreeAnalyzer`
  
* alternative: ./SubmitAna.sh which executes the jobTemplate.sh in separate temp subdirs of CMGana

## ToDo List:
 * adjust "0" cut to represent the ntuple cuts for consistency check
 * allow input of multiple files
 * implement analysis, histograms, etc.

## Done:
 * event weight is fixed, automatic read in fixed
 * fix setupNAF.sh script to use work properly on SL6 machines
 * implement jetID (applied already in CMG), muon, electron id
