#!/usr/bin/python

import sys
from ROOT import *

file1 = TFile.Open('../subEl/Output/CMG_MC_WJets_his.root')
file2 = TFile.Open('../subEl/Output/CMG_MC_T1tttt_1200_800_his.root')

outfile  = TFile("StackPlots.root", "recreate")

def findHisto(file,name):
    for cutKey in file.GetListOfKeys():
        histList = cutKey.ReadObj()

        if cutKey.IsFolder() == 1:
            for histKey in histList.GetListOfKeys():
                if(histKey.GetName() == name):
                    return histKey.ReadObj()
    else:
        return 0

hist1 = findHisto(file1,"HT_1")
hist2 = findHisto(file2,"HT_1")

c1=TCanvas('c1','',800,600)
hist1.Draw("hist")
hist2.Draw("hist same")

outfile.cd()
c1.Write()
