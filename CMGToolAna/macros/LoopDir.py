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

    # exclude sample pattern from plotting
    if 'X' in fname:     return 'nan'
    elif 'Xttbb' in fname:     return 'nan'
    elif "XT1" in fname:  return 'sig'
    elif "X" in fname:  return 'sig'
    elif "QCD" in fname:    return 'bkg'

def doLegend(nameDict = {}, sigList = [], bkgList = []):

    leg = TLegend(0.63,0.525,0.87,0.875)
    leg.SetBorderSize(1)
    leg.SetTextFont(62)
    leg.SetTextSize(0.03321678)
    leg.SetLineColor(1)
    leg.SetLineStyle(1)
    leg.SetLineWidth(1)
    leg.SetFillColor(0)
    leg.SetFillStyle(1001)

    for hist in sigList+bkgList:
        leg.AddEntry(hist,nameDict[hist],'lf')

    return leg

def doLumi(lumi = 1.0):

    lumitext = "13 TeV, %d fb^{-1}, 50ns 40 PU" % lumi

    tex = TLatex(0.90,0.93,lumitext)
    tex.SetNDC()
    tex.SetTextAlign(31)
    tex.SetTextFont(42)
    tex.SetTextSize(0.048)
    tex.SetLineWidth(2)

    return tex

def paintHist(hist, lineCol,lineWid, lineSty, fillCol ):

    hist.SetLineColor(lineCol)
    hist.SetLineWidth(lineWid)
    hist.SetLineStyle(lineSty)
#    if not fillCol == 0:
    hist.SetFillColor(fillCol)

#        return hist

def custHists(histDict):

    for name, hist in histDict.items():
        #        print 'Hist file name', name
        hist.SetStats(0)
        hist.SetTitle("")

        # rebin histo
        hname = hist.GetName()
        nbins = hist.GetNbinsX()

        # hack related to our naming convention as HName_X, where X - cut number
        if nbins > 50 and '_' in hname:
            cutnumb = int(hname[hname.find('_')+1:])
            if cutnumb > 2:
                hist.Rebin(4)

        name = trimName(name)

        if "TT"  in name:     paintHist(hist, 1,1,1, kAzure-4)
        elif "Top"  in name:  paintHist(hist, 1,1,1, kAzure-3)
        elif "WJets" in name: paintHist(hist, 1,1,1, kViolet+5)
        elif "QCD"  in name:   paintHist(hist, 1,1,1, kCyan-6)
        elif "DY"  in name:   paintHist(hist, 1,1,1, kYellow)
        elif "1200" in name:  paintHist(hist, kBlack, 2,1,0)
        elif "1500" in name:  paintHist(hist, kMagenta, 2,1,0)
        elif "800" in name:   paintHist(hist, 2,2,1,0)
        elif "1300" in name:  paintHist(hist, 4,2,1,0)
        else:  paintHist(hist, 1,2,1,0)

        if checkType(name) == 'sig':
            hist.Scale(10)

    return histDict

def doStack(histList):

    for i, hist in enumerate(histList):
        # Init stack
        if i == 0:
            hs = THStack(hist.GetName(),hist.GetTitle())
        hs.Add(hist)

    if 'hs' in locals():
        return hs
    else:
        print 'Error in doStack, dict empty. No BKG given?'
        return 0

def custCanv(canv):

    canv.SetLogy()
    canv.SetTicky()
    canv.SetTickx()
    canv.SetBottomMargin(0.1306294);

    histList = canv.GetListOfPrimitives()
    # filter out TH1s
    histList = filter(lambda x: 'TH1' in str(type(x)), histList)

    # work with first drawn histogram
    hist = histList[0]
    xunit = hist.GetName()

    # sort hists accord to event number
    histList.sort(key=lambda x: x.Integral(), reverse = False)
    ymin = histList[0].GetMinimum()
    ymin = max([ymin,0.01])
    ymax = histList[-1].GetMaximum() * 1.3

    # sum of events in maximum bin:
    #    evsum = sum([int(x.GetMaximum()) for x in histList])
    #    ymax = evsum * 1.5

    # sort hists accord to last non zero bin --> find Xmax
    histList.sort(key=lambda x: x.FindLastBinAbove(0), reverse = True)
    lasthist=histList[0]
#    xmax = lasthist.GetBinCenter(lasthist.FindLastBinAbove())
    xmaxi = lasthist.FindLastBinAbove()

#    print xunit, xmaxi, ymin, ymax

#    hist.SetMinimum(minY)
#    hist.SetMaximum(maxY)
    hist.GetXaxis().SetRange(1,xmaxi)
    hist.GetXaxis().SetTitle(xunit)
    hist.GetYaxis().SetRangeUser(ymin,ymax)
    hist.GetXaxis().SetTitleSize(0.06)
    hist.GetYaxis().SetTitleSize(0.06)
    hist.GetXaxis().SetLabelSize(0.05)
    hist.GetYaxis().SetLabelSize(0.05)
    hist.GetXaxis().SetTitleOffset(0.95)
    #hist.GetYaxis().SetTitleOffset(0.85)

def doPlot(histDict):

    # separate hists in sig/bkg
    sigList = []
    bkgList = []
    nameDict = {}

    for name, hist in histDict.items():

        htype = checkType(name)
        if htype == 'bkg': bkgList += [hist]
        if htype == 'sig': sigList += [hist]

        # save inverted hjist dict: d[hist] = name
        nameDict[hist] = trimName(name)

        if 'canv' not in locals():
            canv=TCanvas(hist.GetName(),hist.GetTitle(),800,600)

    # sort lists according to the number of Entries
    sigList.sort(key=lambda x: x.Integral(), reverse = True)
    bkgList.sort(key=lambda x: x.Integral(), reverse = False)

    hbkg = doStack(bkgList)

    if hbkg:
        # important to set in pyRoot:
        SetOwnership( hbkg, 0 )
#        hbkg.Print()

    canv.cd()

    # first histogram on canvas
    if len(bkgList) > 0:
        #        print 'Drawing first highest histo'
        bkgList[-1].Draw('hist')
#        print 'Starting from bkg stack', hbkg
        hbkg.Draw('hist same')

    elif len(sigList) > 0:
        sigList[0].Draw('hist')

    else:
        print "Error: no histos provided"
        exit(0)

    for hist in sigList:
        #        print 'Drawing', hist
        hist.Draw('hist same')

    # TLegend
    leg = doLegend(nameDict, sigList, bkgList)
    SetOwnership( leg, 0 )
    leg.Draw()

    # Lumi, etc.
    lumi = doLumi(1.0)
    SetOwnership( lumi, 0 )
    lumi.Draw()

    # Customize canvas
    custCanv(canv)

    if canv:
        return canv
    else:
        print "Error: can't make canvas"
        return 0

'''
Here the functions for file handling are defined
'''

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
#    outdirlist = outfile.GetListOfKeys()

    for cutKey in indirlist:

        if cutKey.IsFolder() == 1:
            histDir = cutKey.ReadObj()

            # create same folder
            #if(outfile)
            outfile.mkdir(histDir.GetName())

def copyHist(fileList,outfile,histname,dirname=''):

    # go to outfile target dir
    outfile.cd(dirname)

    # empty file hist dict
    histDict = {}

    for tfile in fileList:
        histDict[tfile.GetName()] = findHisto(tfile,histname)

    custHists(histDict)

    canv = doPlot(histDict)

    if dirname=='':
        print 'Writing canvas for histo', histname, 'in root'
    else:
        print 'Writing canvas for histo', histname, 'in', dirname
    canv.Write()

    return 1

def walkCopyHists(fileList,outfile):

    if len(fileList) == 0:
        print 'Error: empty file list'
        exit(0)

    # reference file
    reffile = fileList[0]
    refdirlist = reffile.GetListOfKeys()

    # set to 0 for normal, 1 for test
    switch = 1

    for refKey in refdirlist:
        #if a folder
        if refKey.IsFolder() == 1:
            # create same folder
            histDir = refKey.ReadObj()
            outfile.mkdir(histDir.GetName())

            #loop over histos inside
            for histKey in histDir.GetListOfKeys():
                hist = histKey.ReadObj()
                # check whether histo
                if 'TH' in str(type(hist)):
                    copyHist(fileList,outfile,hist.GetName(),histDir.GetName())

            if switch:
                break
        # if not a folder
        else:
            refHist = refKey.ReadObj()
            # check whether histo
            if 'TH' in str(type(refHist)):
                copyHist(fileList,outfile,refHist.GetName())

    return 1

#copy(file1, outfile)

if __name__ == "__main__":

    fileDir = '../subEl/Output/'
    pattern = '*.root'

    # first argument is '-b' == for batch mode
    #    print sys.argv

    if len(sys.argv) > 2:
        fileDir = sys.argv[1]

    if len(sys.argv) > 3:
        pattern = sys.argv[2] + pattern

    print 'FileDir:', fileDir
    print 'Pattern', pattern
    nameList = glob.glob(fileDir+'*'+pattern)
    print 'Found', len(nameList), 'files'
    print [trimName(x) for x in nameList]
    print 80*'#'

    fileList = []
    # open files
    for name in nameList:
        fileList.append(TFile.Open(name))

    outfile  = TFile("StackPlots.root", "RECREATE")

    walkCopyHists(fileList,outfile)

    print 'Closing files'
    for tfile in fileList:
        tfile.Close()

    outfile.Close()

    print 80*'#'
    print 'Finished writing and closed files'
'''
END OF MAIN
'''

'''
histname = 'MET_0'
#    histname = 'CutFlow'

histDict = {}

for tfile in fileList:
histDict[tfile.GetName()] = findHisto(tfile,histname)

custHists(histDict)

ca = doPlot(histDict)
print 'writing'
ca.Write()
'''


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
