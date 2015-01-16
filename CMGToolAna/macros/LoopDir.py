#!/usr/bin/python

import sys
from ROOT import *

file1 = TFile.Open('../subEl/Output/CMG_MC_WJets_his.root','READ')
file2 = TFile.Open('../subEl/Output/CMG_MC_TTbar_his.root','READ')
#file3 = TFile.Open('../subEl/Output/CMG_MC_T1tttt_1200_800_his.root','READ')

outfile  = TFile("StackPlots.root", "recreate")

def findHisto(file,name):
    for cutKey in file.GetListOfKeys():
        if cutKey.IsFolder() == 1:
            histDir = cutKey.ReadObj()
            for histKey in histDir.GetListOfKeys():
                if(histKey.GetName() == name):
                    return histKey.ReadObj()
    else:
        return 0

def copy(infile,outfile):
    indirlist = infile.GetListOfKeys()
    outdirlist = outfile.GetListOfKeys()

    for cutKey in indirlist:

        if cutKey.IsFolder() == 1:
            histDir = cutKey.ReadObj()

            # create same folder
            #if(outfile)
            outfile.mkdir(histDir.GetName())

#copy(file1, outfile)

histname = 'HT_1'

hist1 = findHisto(file1,histname)
hist2 = findHisto(file2,histname)

c1=TCanvas(histname,hist1.GetTitle(),800,600)
hist1.Draw("hist")
hist2.Draw("hist same")

outfile.cd()
c1.Write()
