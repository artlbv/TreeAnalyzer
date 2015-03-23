#!/usr/bin/python

import sys
import os
from glob import glob
from sys import exit

from ROOT import gROOT
from ROOT import TFile

# default samples location
locSamp = 'DESY'

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

def GetTreeName(file):
    keylist = file.GetListOfKeys()
    treeKey = keylist.At(0)
    treeName = treeKey.GetName()

    if 'tree' in treeName:
        return treeName
    else:
        print 'Tree not found in ', file.GetName()
        exit(0)

# choose the analysis and a sample
srcdir = '../src/'
exe = 'TreeAnalyzer_SingleEl.exe'

# Get sample directory
if len(sys.argv) > 1:
    sampDir = sys.argv[1]
else:
    help()

# Calculate number of events
entries = GetNevents(sampDir+'/')

treename = 'treeProducerSusySingleLepton'
lumi = 1000 #pb-1
#remove the trailing / : rstrip('//')
sample = os.path.basename(sampDir.rstrip('//'))

if 'MC' in sampDir: scene = 'MC'
else: scene = 'Data'

print sampDir, 'has', entries, 'entries for sample', scene, '_', sample

fileName = sampDir+'/'+treename+'/ '+str(lumi/entries)+' '

print([".././"+exe, fileName,scene+'_'+sample])
os.system(".././"+exe+" "+fileName+" "+scene+'_'+sample)

'''
print "cross section x lumi", xsec_lumi[samp][i], "Events generated", entries
fileNames+=inDir[scene][samp]+dirsHT[samp][i]+treename+' '+str(xsec_lumi[samp][i]/entries)+' '

                # process
                print "file name to be processed", fileNames
                print "Sample:", samp,scene
                if exe == ' ':
                    print fileNames,samp,scene
                    reader(fileNames,scene+'_'+samp)
                else:
                    print([".././"+exe, fileNames,scene+'_'+samp])
                    os.system(".././"+exe+" "+fileNames+" "+scene+'_'+samp)
'''
