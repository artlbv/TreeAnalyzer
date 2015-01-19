#!/usr/bin/python

import sys
import os
import glob
#sys.argv.insert(1, '-b' )
sys.argv.append( '-b' )

from ROOT import *

def trimName(fname):

    prefix = 'CMG_MC_'
    suffix = '_his.root'

    nameBegin= fname.find(prefix)
    fname = fname[nameBegin:]
    fname = fname.replace(prefix,'')
    fname = fname.replace(suffix,'')

    return fname

def checkType(fname):

    fname = trimName(fname)

    if "X" in fname:
        print fname, "is a signal"
        return 'sig'

    else:
        print fname, "is a bkg"
        return 'bkg'

'''
def doStack(histlist):

    hs = THStack()

    for i,hist in enumerate(histlist):

        # Init canvas/stack
        if i == 0:
            canv=TCanvas(hist.GetName(),hist.GetTitle(),800,600)
            hist.Draw("hist")
        else:
            hist.Draw("hist same")

    return canv
'''

def doLegend(histDict):

	leg = TLegend(0.63,0.525,0.87,0.875)
        leg.SetBorderSize(1)
        leg.SetTextFont(62)
        leg.SetTextSize(0.03321678)
        leg.SetLineColor(0)
        leg.SetLineStyle(1)
        leg.SetLineWidth(1)
        leg.SetFillColor(0)
        leg.SetFillStyle(1001)

        for name, hist in histDict.items():
            hname = trimName(name)

            leg.AddEntry(hist,hname,'l')
            '''
            if "TTbar" in name: leg.AddEntry(hist,hname,'l')
            if "WJets" in name: leg.AddEntry(hist,hname,'l')
            if "QCD" in name:   leg.AddEntry(hist,hname,'l')
            if "1200" in name:  leg.AddEntry(hist,hname,'l')
            if "1500" in name:  leg.AddEntry(hist,hname,'l')
            if "800" in name:   leg.AddEntry(hist,hname,'l')
            if "1300" in name:  leg.AddEntry(hist,hname,'l')
            '''
        return leg

def paintHist(hist, lineCol,lineWid, lineSty, fillCol ):

        hist.SetLineColor(lineCol)
        hist.SetLineWidth(lineWid)
        hist.SetLineStyle(lineSty)
#        hist.SetFillColor(fillCol)

#        return hist

def paintHists(histDict):

    for name, hist in histDict.items():
#        print 'Hist file name', name

        if "TTbar" in name: paintHist(hist, 1,1,1, kAzure-4)
        if "WJets" in name: paintHist(hist, 1,1,1, kViolet+5)
        if "QCD" in name:   paintHist(hist, 1,1,1, kCyan-6)
        if "1200" in name:  paintHist(hist, 2,1,0, kBlack)
        if "1500" in name:  paintHist(hist, 2,1,0, kMagenta)
        if "800" in name:   paintHist(hist, 2,1,0, 2)
        if "1300" in name:  paintHist(hist, 2,1,0, 4)

    return histDict

def doStack(histList):

    for i, hist in enumerate(histList):
        # Init stack
        if i == 0:
            hs = THStack(hist.GetName(),hist.GetTitle())
            print 'Creating stack', hs
        #add to stack
        print 'Adding to stack', hist
        hs.Add(hist)

    if 'hs' in locals():
        return hs
    else:
        print 'Error in doStack: dict empty?'
        return 0

def doPlot(histDict):

    print histDict
    # separate hists in sig/bkg
    sigList = []
    bkgList = []

    for name, hist in histDict.items():

        htype = checkType(name)
        if htype == 'bkg': bkgList += [hist]
        if htype == 'sig': sigList += [hist]

        if 'canv' not in locals():
            canv=TCanvas(hist.GetName(),hist.GetTitle(),800,600)
            canv.SetLogy()

    # sort lists according to the number of Entries
    sigList.sort(key=lambda x: x.Integral(), reverse = True)
    bkgList.sort(key=lambda x: x.Integral(), reverse = True)


    hbkg = doStack(bkgList)
    if hbkg:
        for h in  hbkg.GetHists():
            print 'Stack hist:', h

#        print 'Stack len', len(hbkg.GetHists())
    '''
    hbkg = 0
    hist = sigList[0]
    hs = THStack(hist.GetName(),hist.GetTitle())

    hs.Add(sigList[0])
    hs.Add(sigList[1])
    '''

    hbkg.Print()

    canv.cd()

    # first histogram on canvas
    if len(bkgList) > 0:
        print 'Starting from bkg stack', hbkg
        bkgList[0].Draw('hist')
        hbkg.Draw('hist same')
        print '-> Done'
    elif len(sigList) > 0:
        sigList[0].Draw('hist')
    else:
        print "Error: no histos provided"
        exit(0)

    for hist in sigList:
        print 'Drawing', hist
        hist.Draw('hist same')

#        hbkg.Draw('hist same')
#    hs.GetStack().Last().Draw('hist')
#    hs.Draw('hist same')

    leg = doLegend(histDict)

    leg.Draw()

    if canv:
        return canv
    else:
        print "Error: can't make canvas"
        return 0

def findHisto(file,name):
    for cutKey in file.GetListOfKeys():
        if cutKey.IsFolder() == 1:
            histDir = cutKey.ReadObj()
            for histKey in histDir.GetListOfKeys():
                if(histKey.GetName() == name):
                    return histKey.ReadObj()

        if(cutKey.GetName() == name):
            return cutKey.ReadObj()

    else:
        return 0

def getHist(file,name,dir=''):
    file.cd(dir)
    return file.Get(name)

def copyStruct(infile,outfile):
    indirlist = infile.GetListOfKeys()
    outdirlist = outfile.GetListOfKeys()

    for cutKey in indirlist:

        if cutKey.IsFolder() == 1:
            histDir = cutKey.ReadObj()

            # create same folder
            #if(outfile)
            outfile.mkdir(histDir.GetName())

#copy(file1, outfile)

if __name__ == "__main__":

    fileDir = '../subEl/Output/'
    pattern = '*.root'

    print sys.argv

    # first argument is '-b' == for batch mode

    if len(sys.argv) > 2:
        fileDir = sys.argv[1]

    if len(sys.argv) > 3:
        pattern = sys.argv[2] + pattern

    print fileDir, pattern
    nameList = glob.glob(fileDir+'*'+pattern)

    fileList = []
    # open files
    for name in nameList:
        fileList.append(TFile.Open(name))

    outfile  = TFile("StackPlots.root", "recreate")

    histname = 'MET_0'
#    histname = 'CutFlow'

    histDict = {}

    for tfile in fileList:
        histDict[tfile.GetName()] = findHisto(tfile,histname)

    paintHists(histDict)

    ca = doPlot(histDict)
    print 'writing'
    ca.Write()

    for tfile in fileList:
        tfile.Close()

    outfile.Close()

    '''
    c1=TCanvas(histname,hist1.GetTitle(),800,600)
    hist1.Draw("hist")
    hist2.Draw("hist same")

    outfile.cd()
    c1.Write()
    '''

#    histList = [hist1,hist2]

#    print histList
#    histList.sort(key=lambda x: x.Integral())
#    print histList

#    c2 = doStack(histList)
#    c2.Write()
#    outfile.cd()

    '''
    hist1 = findHisto(file1,histname)
    hist2 = findHisto(file2,histname)
    hist3 = findHisto(file3,histname)

    histDict[file1.GetName()] = hist1
    histDict[file2.GetName()] = hist2
    histDict[file3.GetName()] = hist3
    '''
