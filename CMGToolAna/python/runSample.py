#!/usr/bin/python

import sys
import os
from glob import glob
from sys import exit

from ROOT import gROOT
from ROOT import TFile

def help():
    print 'First argument analysis:'
    print './runreader.py SampleDir'
    sys.exit(0)

def GetNevents(loc):
    EvtFile = open(loc+"ttHLepSkimmer/events.txt", "r")
    theInts = []
    for val in EvtFile.read().split():
        if val.isdigit():
            theInts.append(val)
        EvtFile.close()
    return float(theInts[0])


# Settings
treename = 'treeProducerSusySingleLepton'
lumi = 1000 #pb-1
# choose the analysis and a sample
exe = 'TreeAnalyzer_SingleEl.exe'

# Get sample directory
if len(sys.argv) > 1:
    sampDir = sys.argv[1]
else:
    help()

# Calculate number of events
entries = GetNevents(sampDir+'/')
#remove the trailing / : rstrip('//')
sample = os.path.basename(sampDir.rstrip('//'))

# get scenario MC/Data
if 'MC' in sampDir: scene = 'MC'
else: scene = 'Data'

print sampDir, 'has', entries, 'entries for sample', scene, '_', sample

# NB! weight is lumi/entries
fileName = sampDir+'/'+treename+'/ '+str(lumi/entries)+' '

print([".././"+exe, fileName,scene+'_'+sample])
os.system(".././"+exe+" "+fileName+" "+scene+'_'+sample)
