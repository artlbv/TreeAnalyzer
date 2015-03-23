#!/usr/bin/python

import sys
import os
from glob import glob
from sys import exit

# default samples location
locSamp = 'DESY'

def help():
    print 'First argument analysis:'
    print './runreader.py Analyzer TYPE_SAMPLE [Location (%s)]' % locSamp
    print ' TYPE = MC, data'
    print ' SAMPLE = ',

    from DESYsamples import SAMPLES
    print SAMPLES

    sys.exit(0)

def GetNevents(loc):
    EvtFile = open(loc+"ttHLepSkimmer/events.txt", "r")

    for line in EvtFile.readlines():
        if 'all events' in line:
            evts = line.split()[2]
            break

    return float(evts)

# choose the analysis and a sample
srcdir = '../src/'
exe = ' '
if len(sys.argv)>1:
    if sys.argv[1]=='TreeAnalyzer_example':
        exe = 'TreeAnalyzer_example.exe'
    # single muon compilable (Batool)
    elif sys.argv[1]=='TreeAnalyzer_Mu':
        exe = 'TreeAnalyzer_Mu.exe'
    # single lepton compilable (Artur)
    elif sys.argv[1]=='TreeAnalyzer_SingleLep':
        exe = 'TreeAnalyzer_SingleLep.exe'
    # example with compiled executable
    elif sys.argv[1]=='TreeAnalyzer_SingleEl':  # Single Electron with compiled executable
        exe = 'TreeAnalyzer_SingleEl.exe'
    elif sys.argv[1]=='TreeAnalyzer_SingleMu':  # Single Electron with compiled executable
        exe = 'TreeAnalyzer_SingleMu.exe'
    elif sys.argv[1]=='TreeAnalyzer_Gen':  # gen study
        exe = 'TreeAnalyzer_GenStudy.exe'
    elif sys.argv[1]=='TreeAnalyzer_Eff':  # efficiency study
        exe = 'TreeAnalyzer_EffStudy.exe'
    elif sys.argv[1]=='TreeAnalyzer_EffTree':  # efficiency study
        exe = 'TreeAnalyzer_EffStudyTree.exe'

    else:
        help()
else:
    help()

# Append relative path to exe
pyDir = sys.argv[0]
cwd = os.path.dirname(pyDir)

# get relative path
exe = cwd+'/../'+exe

# check sample location in sys.argv
if 'EOS' in sys.argv:
    from EOSsamples import *
else:
    from DESYsamples import *

# loop over the samples in the arguments
foundFlag=False

for arg in sys.argv:
    for scene in scenarios:
        for samp in SAMPLES:
            if arg == scene+'_'+samp:
                foundFlag=True
                fileNames=''
                print 'Sample', scene,  samp, inDir[scene][samp]

                # loop over HTbins (if any)
                for i,HT in enumerate(dirsHT[samp]):
                    if len(dirsHT[samp]) > 1:
                        print 'HTbin', HT

                    # Get sample directory
                    sampDir = inDir[scene][samp]+HT
                    # Calculate number of events
                    entries = GetNevents(sampDir)
                    print "cross section x lumi", xsec_lumi[samp][i], "Events generated", entries
                    fileNames+=inDir[scene][samp]+dirsHT[samp][i]+treename+' '+str(xsec_lumi[samp][i]/entries)+' '

                # process
                print "file name to be processed", fileNames
                print "Sample:", samp,scene
                if exe == ' ':
                    print fileNames,samp,scene
                    reader(fileNames,scene+'_'+samp)
                else:
                    print([exe, fileNames,scene+'_'+samp])
                    os.system(exe+" "+fileNames+" "+scene+'_'+samp)

if not foundFlag: help()
