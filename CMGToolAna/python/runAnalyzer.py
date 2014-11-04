#!/usr/bin/python

import sys, os
from ROOT import gROOT

# scenarios and samples
scenarios = ['MC','data']
#samples   = ['TTbar','T1tttt_1500_100','T1tttt_1200_800']
samples   = ['WJets','TTbar','TTbar_DiLep','TTbar_SinLep','T1tttt_1500_100','T1tttt_1200_800','T1tttt_800_450','T1tttt_1300_100']
treename = 'treeProducerSusySingleLepton/'
Lumi = 1 #given in fb^-1
Lumi = Lumi * 1000


def help():
        print 'First argument analysis:'
        print './runreader.py   SingleS TYPE_SAMPLE'
        print ' TYPE = MC, data'
        print ' SAMPLE = ',
        for sample in samples:
                print sample,
        sys.exit(0)

from operator import mul
def scale(fac,list):
        return map(mul,len(list)*[fac],list)

def GetNevents(loc):
        EvtFile = open(loc+"/ttHLepSkimmer/events.txt", "r")
        theInts = []
        for val in EvtFile.read().split():
                if val.isdigit():
                        theInts.append(val)
        EvtFile.close()
        return float(theInts[0])

# choose the analysis and a sample

#gROOT.LoadMacro('Objects.C+')
srcdir = '../src/'

#gROOT.LoadMacro(srcdir+'Objects.C+')

if len(sys.argv)>1:
        if sys.argv[1]=='SingleS_P':  # single lepton testing version
                gROOT.LoadMacro(srcdir+'readerSingleS_P.C+')
                from ROOT import readerSingleS_P as reader
        elif sys.argv[1]=='TreeAnalyzer':  # single lepton testing version
		gROOT.LoadMacro(srcdir+'ClassObjects.C+')
                gROOT.LoadMacro(srcdir+'TreeAnalyzer_SingleLep.C+')
                from ROOT import TreeAnalyzer as reader
        elif sys.argv[1]=='TreeAnalyzer_ext':  # test external object definitions
#                gROOT.LoadMacro(srcdir+'Objects.C+')
                gROOT.LoadMacro(srcdir+'TreeAnalyzer_ext.C+')
                from ROOT import TreeAnalyzer as reader
        elif sys.argv[1]=='SingleLeptCMSana':  # single lepton CMS artur
#                gROOT.LoadMacro(srcdir+'Objects.C+')
                gROOT.LoadMacro(srcdir+'SingleLeptCMSana.C+')
                from ROOT import TreeAnalyzer_ext as reader
        elif sys.argv[1]=='TreeAnalyzer_BKG':  # single lepton testing version
                gROOT.LoadMacro(srcdir+'TreeAnalyzer_BKG.C+')
                from ROOT import TreeAnalyzer_BKG as reader
        elif sys.argv[1]=='TreeOutput':  # test tree output
                gROOT.LoadMacro(srcdir+'ClassObjects.C+')
                gROOT.LoadMacro(srcdir+'TreeOutput.C+')
                from ROOT import TreeWriter as reader

        else:
                help()
else:
        help()


#prepare empty dictionaries
do={}
for scene in scenarios:
        do[scene]={}
        for samp in samples:
                do[scene][samp] = 0
flag=False
for e in sys.argv:
        for scene in scenarios:
                for samp in samples:
                        if e==scene+'_'+samp:
                                do[scene][samp] = 1
                                flag=True
if not flag: help()


# prepare empty dictionaries
dirsHT={}
inDir={}
xsec_lumi={}
evtgen={}
for scene in scenarios:
        inDir[scene]={}
#       weights[scene]={}

sample = 'TTbar'
dirsHT[sample]  = ['/']
xsec_lumi[sample] = [809.1*Lumi] #cross section in pb
inDir['MC'][sample] = '/afs/desy.de/user/s/safarzad/dust/13TeV/ISOTrck/TTJets/TTJets_MSDecaysCKM_central_PU_S14_POSTLS170/'
evtgen[sample]  =  [GetNevents(inDir['MC'][sample])]
sample = 'TTbar_SinLep'
dirsHT[sample]  = ['/']
xsec_lumi[sample] = [809.1*Lumi] #cross section in pb
inDir['MC'][sample] = '/afs/desy.de/user/s/safarzad/dust/13TeV/ISOTrck/TTJets/TTJets_MSDecaysCKM_central_PU_S14_POSTLS170/'
evtgen[sample]  =  [GetNevents(inDir['MC'][sample])]
sample = 'TTbar_DiLep'
dirsHT[sample]  = ['/']
xsec_lumi[sample] = [809.1*Lumi] #cross section in pb
inDir['MC'][sample] = '/afs/desy.de/user/s/safarzad/dust/13TeV/ISOTrck/TTJets/TTJets_MSDecaysCKM_central_PU_S14_POSTLS170/'
print (inDir['MC'][sample])
evtgen[sample]  =  [GetNevents(inDir['MC'][sample])]

sample = 'WJets'
dirsHT[sample]  = ['100-200/','200-400/','400-600/','600-Inf/']
xsec_lumi[sample] = [2234.9,580.06,68.38,23.14] 
xsec_lumi[sample] = scale(Lumi,xsec_lumi[sample])
inDir['MC'][sample] = '/afs/desy.de/user/s/safarzad/dust/13TeV/ISOTrck/WJets/' 
print inDir['MC'][sample]
evtgen[sample] = {}
k=0
print evtgen
for HT in dirsHT[sample]:
	evtgen[sample][k]=GetNevents(inDir['MC'][sample]+HT)
	print "from func",evtgen[sample][k]
	k=k+1


sample = 'T1tttt_1500_100'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [0.0141903*Lumi]
inDir['MC'][sample] = '/afs/desy.de/user/s/safarzad/dust/13TeV/ISOTrck/T1tttt/SMS_T1tttt_2J_mGl1500_mLSP100_PU_S14_POSTLS170/'
evtgen[sample] = [GetNevents(inDir['MC'][sample])]

sample = 'T1tttt_1200_800'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [0.085641*Lumi]
inDir['MC'][sample] = '/afs/desy.de/user/s/safarzad/dust/13TeV/ISOTrck/T1tttt/SMS_T1tttt_2J_mGl1200_mLSP800_PU_S14_POSTLS170/'
evtgen[sample] = [GetNevents(inDir['MC'][sample])]
sample = 'T1tttt_800_450'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [1.489*Lumi]
inDir['MC'][sample] = '/afs/desy.de/user/s/safarzad/dust/13TeV/ISOTrck/T1tttt/SMS_T1tttt_2J_mGl800_mLSP450_PU_S14_POSTLS170/'
evtgen[sample] = [GetNevents(inDir['MC'][sample])]
sample = 'T1tttt_1300_100'
dirsHT[sample] = ['/']
xsec_lumi[sample] = [0.044*Lumi]
inDir['MC'][sample] = '/afs/desy.de/user/s/safarzad/dust/13TeV/ISOTrck/T1tttt/SMS_T1tttt_2J_mGl1300_mLSP100_PU_S14_POSTLS170/'
evtgen[sample] = [GetNevents(inDir['MC'][sample])]

from ROOT import TFile
from glob import glob
from sys import exit

def GetTreeName(file):
        keylist = file.GetListOfKeys()
        treeKey = keylist.At(0)
        treeName = treeKey.GetName()

        if 'tree' in treeName:
                return treeName
        else:
                print 'Tree not found in ', file.GetName()
                exit(0)

# do it
for scene in scenarios:

        for samp in samples:
                if do[scene][samp]:
                        f=''
                        print dirsHT, samp
                        for i in range(len(dirsHT[samp])):
                                entries = evtgen[samp][i]
                                print "cross section x lumi",xsec_lumi[samp], "Events generated", entries
                                f=f+inDir[scene][samp]+dirsHT[samp][i]+treename+' '+str(xsec_lumi[samp][i]/entries)+' '
				print "file name to be processed", f
                        print f,samp,scene
                        reader(f,scene+'_'+samp)

