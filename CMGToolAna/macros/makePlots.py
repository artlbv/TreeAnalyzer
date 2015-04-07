#!/usr/bin/python

import sys
import glob
sys.argv.append( '-b' )

from ROOT import *
#TFile.Open._creates = True

# globals
firstPlot = True

def trimName(fname):

    prefix = 'CMG_MC_'
    suffix = '_his.root'

    nameBegin= fname.find(prefix)
    fname = fname[nameBegin:]
    fname = fname.replace(prefix,'')
    fname = fname.replace(suffix,'')

    return fname

def checkType(fname):

#    fname = trimName(fname)

    # exclude sample pattern from plotting
    if 'X' in fname:     return 'nan'
    elif "T1" in fname:  return 'sig'
    elif "" in fname:    return 'bkg'

def lookUpSample(name):

    # backgrounds
    if 'QCD' in name: title = 'QCD'
    elif 'TTbar' in name: title = 'TTJets'
    elif 'WJets' in name: title = 'WJets'
    elif 'TTV' in name: title = 'TTV'
    elif 'SingleTop' in name: title = 'SingleTop'
    elif 'DY' in name: title = 'DYJets'
    # signals
    elif 'T1tttt' in name:
        if '1500_100' in name: title = 'T1tttt(1500,100)'
        elif '1200_800' in name: title = 'T1tttt(1200,800)'
    # didn't find this sample
    else: title = name

    return title

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

    for hist in reversed(bkgList):
        leg.AddEntry(hist,lookUpSample(nameDict[hist]),'f')

    for hist in sigList:
        leg.AddEntry(hist,lookUpSample(nameDict[hist]),'l')

    return leg

def doLumi(lumi = 1.0):

    lumitext = "13 TeV, %d fb^{-1}, 25ns 20 PU" % lumi

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
    hist.SetFillColor(fillCol)

def lookUpVar(var):
    # replace histogram names with human readable variable names

    if 'MET' in var: var = 'ME_{T}'
    if 'HT' in var: var = 'H_{T}'
    if 'ST' in var: var = 'S_{T}'

    if 'nJet' in var: var = 'N_{jets}'
    if 'nBJet' in var: var = 'N_{btag}'

    if 'JetpT' in var:
        num = var[:var.find('JetpT')]
        var = 'p_{T} (%s jet)' %num

    if 'nLep' in var: var = 'N_{lep}'
    if 'nLeppT' in var: var = 'p_{T} (lep)'
    if 'nLepEta' in var: var = '#eta (lep)'

    if 'dPhiWLep' in var: var = '#Delta#phi(W,lep)'
    if 'dPhiMetLep' in var: var = '#Delta#phi(Met,lep)'

    return var

def getVar(hname):

    # expect hname like Var_X, where X is the cut number

    if '_' in hname:
        cutnumb = int(hname[hname.find('_')+1:])
        var = hname[:hname.find('_')]
        var = lookUpVar(var)

        return var
    else:
        return hname

def custHists(histDict):

    for name, hist in histDict.items():
#        print 'Hist file name', name
        hist.SetStats(0)
        hist.SetTitle("")
#        hist.SetName(hist.GetName()+name)

        # rebin histo
        hname = hist.GetName()
        nbins = hist.GetNbinsX()

        # hack related to our naming convention as HName_X, where X - cut number
        if nbins > 50 and '_' in hname:
            cutnumb = int(hname[hname.find('_')+1:])
            if cutnumb > 5:
                hist.Rebin(4)

        if "TTbar"  in name:  paintHist(hist, 1,1,1, kBlue-2)
        elif "Top"  in name:  paintHist(hist, 1,1,1, kViolet+5)
        elif "TTV"  in name:  paintHist(hist, 1,1,1, kOrange-3)
        elif "TTW"  in name:  paintHist(hist, 1,1,1, kOrange-3)
        elif "TTH"  in name:  paintHist(hist, 1,1,1, kOrange-3)
        elif "TTZ"  in name:  paintHist(hist, 1,1,1, kOrange-3)
        elif "WJets" in name: paintHist(hist, 1,1,1, kGreen-2)
        elif "QCD"  in name:   paintHist(hist, 1,1,1, kCyan-6)
        elif "DY"  in name:   paintHist(hist, 1,1,1, kRed-6)
        elif "800" in name:   paintHist(hist, 2,2,2,0)
        elif "1200" in name:  paintHist(hist, kBlack, 2,2,0)
        elif "1300" in name:  paintHist(hist, 4,2,2,0)
        elif "1500" in name:  paintHist(hist, kMagenta, 2,2,0)
        else:  paintHist(hist, 1,2,1,0)

        '''
        # workaround for Electron/Muon separation
        if "El" in name:
            col = hist.GetFillColor()
            hist.SetFillColor(0)
            hist.SetLineWidth(3)
            hist.SetLineColor(col+1)

        if "Mu" in name:
            col = hist.GetFillColor()
            hist.SetFillColor(0)
            hist.SetLineWidth(3)
            hist.SetLineColor(col-1)
        '''

#        if checkType(name) == 'sig':
#            hist.Scale(10)

    return histDict

def custCanv(canv):

    canv.SetLogy()
    canv.SetTicky()
    canv.SetTickx()
    canv.SetBottomMargin(0.1306294);

    histList = canv.GetListOfPrimitives()
    # filter out non THs
    histList = filter(lambda x: 'TH' in str(type(x)), histList)

    ymax = max([x.GetMaximum() for x in histList])
    ymax *= 1.3

    # work with first drawn histogram
    hist = histList[0]

    # axis title
    xunit = hist.GetName()
    if 'CutFlow' in xunit: ymax *= 2

    # filter out TH1s
    histList = filter(lambda x: 'TH1' in str(type(x)), histList)

    # find last x bin above 0
    xmaxi = max([x.FindLastBinAbove(0) for x in histList])

    ymin = 0.8*min([x.GetMinimum(0) for x in histList])

#    print xunit, xmaxi, ymin, ymax

    hist.GetXaxis().SetRange(1,xmaxi)
    hist.GetXaxis().SetTitle(getVar(xunit))
    hist.GetYaxis().SetRangeUser(ymin,ymax)
    hist.GetXaxis().SetTitleSize(0.06)
    hist.GetYaxis().SetTitleSize(0.06)
    hist.GetXaxis().SetLabelSize(0.05)
    hist.GetYaxis().SetLabelSize(0.05)
    hist.GetXaxis().SetTitleOffset(0.95)
    #hist.GetYaxis().SetTitleOffset(0.85)

def doStack(histList):

    for i, hist in enumerate(histList):
        # Init stack
        if i == 0:
            hs = THStack(hist.GetName(),hist.GetTitle())
        hs.Add(hist)

    if 'hs' in locals():
        return hs
    else:
        if firstPlot:
            print 'Error in doStack, dict empty. No BKG given?'
        return 0

def doPlot(histDict):

    # separate hists in sig/bkg
    sigList = []
    bkgList = []
    nameDict = {}

    for name, hist in histDict.items():

        htype = checkType(name)
        if htype == 'bkg': bkgList += [hist]
        if htype == 'sig': sigList += [hist]

        # save inverted hist dict: d[hist] = name
        nameDict[hist] = name

        if 'canv' not in locals():
            canv=TCanvas(hist.GetName(),hist.GetTitle(),800,600)

    # sort lists according to the number of Entries
    sigList.sort(key=lambda x: x.Integral(), reverse = True)
    bkgList.sort(key=lambda x: x.Integral(), reverse = False)

    # print info for first plot
    global firstPlot
    if firstPlot:
        print 'Information about the samples'
        print 80*'_'
        print 'Signal samples:'
        for hist in sigList:
            if "CutFlow" in hist.GetName():
                print nameDict[hist], '\t', hist.GetMaximum(), 'events'
            else:
                print nameDict[hist], '\t', hist.Integral(), 'events'
        print '---'
        print 'Background samples:'
        for hist in bkgList:
            if "CutFlow" in hist.GetName():
                print nameDict[hist], '\t', hist.GetMaximum(), 'events'
            else:
                print nameDict[hist], '\t', hist.Integral(), 'events'

        print 80*'_'

    # prepare BKG stack
    hbkg = doStack(bkgList)
    if hbkg:
        # important to set in pyRoot:
        SetOwnership( hbkg, 0 )

    canv.cd()

    # first histogram on canvas
    if len(bkgList) > 0:
        bkgList[-1].Draw('hist')
        hbkg.Draw('hist same')

    elif len(sigList) > 0:
        sigList[0].Draw('hist')

    else:
        print "Error: no histos provided"
        exit(0)

    for hist in sigList:
        hist.Draw('hist same')

    # TLegend
    leg = doLegend(nameDict, sigList, bkgList)
    SetOwnership( leg, 0 )
    leg.Draw()

    # Lumi, etc.
    lumi = doLumi(4.0)
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

def copyHist(fileList,outfile,histname,dirname=''):

    # go to outfile target dir
    outfile.cd(dirname)

    if dirname != '': dirname += '/'

    # empty file hist dict
    histDict = {}

    for tfile in fileList:
        histOnFile = tfile.Get(dirname+histname)

        if not histOnFile: continue

        hist = histOnFile.Clone()
        del histOnFile

        fname = tfile.GetName()
        fname = trimName(fname)

        if hist and checkType(fname) != 'nan':
#            hist.SetDirectory(0)
            histDict[fname] = hist

    if len(histDict) < 1: exit (0)

    # customise histograms
    custHists(histDict)

    global firstPlot
    canv = doPlot(histDict)
    firstPlot = False

    if dirname=='':
        print 'Writing canvas for histo', histname, 'in root'
    else:
        print 'Writing canvas for histo', histname, 'in', dirname

    canv.Write()

    del histDict
    del canv

    return 1

def walkCopyHists(fileList,outfile):

    if len(fileList) == 0:
        print 'Error: empty file list'
        exit(0)

    # reference file
    reffile = fileList[0]
    refdirlist = reffile.GetListOfKeys()

    # set to 0 for normal, 1 for test
    switch = 0

    for refKey in refdirlist:
        #if a folder
        if refKey.IsFolder() == 1:
            # create same folder
            histDir = refKey.ReadObj()
            fileDir = outfile.mkdir(histDir.GetName())

            #loop over histos inside
            for histKey in histDir.GetListOfKeys():
                hist = histKey.ReadObj()
                # check whether histo
                if 'TH' in str(type(hist)):
                    copyHist(fileList,outfile,hist.GetName(),histDir.GetName())
                del hist

            histDir.Close()
            fileDir.Close()

            del histDir
            del fileDir

            switch += 1
            if switch < 0:
                break
        # if not a folder
        else:
            reffile.cd()
            hist = refKey.ReadObj()
            # check whether histo
            if 'TH' in str(type(hist)):
                copyHist(fileList,outfile,hist.GetName())

            del hist

        print 80*'#'

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
    nameList = glob.glob(fileDir+'/'+pattern)
    print 'Found', len(nameList), 'files'
    print [trimName(x) for x in nameList]
    print 80*'#'

    fileList = []

    # open files
    for name in nameList:
#        fileList.append(TFile.Open(name))
        fileList.append(TFile(name,"READ"))

#    for f in fileList:
#        SetOwnership(f,True)

    outfile  = TFile("StackPlots.root", "RECREATE")
#    outfile  = TFile.Open("StackPlots.root", "RECREATE")

    walkCopyHists(fileList,outfile)

    print 'Saving new plots... to', outfile.GetName()
    outfile.Close()

    print 'Closing input files...'
    for tfile in fileList:
        print tfile.GetName()
        tfile.Close()

    print 80*'#'
    print 'Finished writing and closed files'
