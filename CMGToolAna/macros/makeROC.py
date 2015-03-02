#!/usr/bin/python

import sys
import math
sys.argv.append( '-b' )

from ROOT import *
from array import array

_Debug = false

def getLegend():
#    leg = TLegend(0.63,0.525,0.87,0.875)
#    leg = TLegend(0.1,0.7,0.48,0.9)
    leg = TLegend(0.4,0.7,0.9,0.9)
    leg.SetBorderSize(1)
    leg.SetTextFont(62)
    leg.SetTextSize(0.03321678)
    leg.SetLineColor(1)
    leg.SetLineStyle(1)
    leg.SetLineWidth(1)
    leg.SetFillColor(0)
    leg.SetFillStyle(1001)

    return leg

def getLogBins(nbinsx,xmin,xmax):
    logxmin = math.log10(xmin)
    logxmax = math.log10(xmax)
    binwidth = (logxmax-logxmin)/nbinsx
    xbins = [ xmin + math.pow(10,logxmin+x*binwidth) for x in range(1,nbinsx+1)]
    xbins.sort(key=int)
    binc = array('d', xbins)

    return binc


def makeROC(hSig, hBkg, title = '', reject = false):

    nbins = hSig.GetNbinsX()

    if title == '':
        title = 'roc_'+hSig.GetName()

    sigEff = []
    bkgEff = []
    fomPts = []
    varPts = []

    sigIntTot = hSig.Integral(0,nbins)
    bkgIntTot = hBkg.Integral(0,nbins)

    for bin in range(nbins):

        var = hSig.GetBinCenter(bin)
        varPts.append(var)

        # get integrals
        sigInt = hSig.Integral(0,bin)
        bkgInt = hBkg.Integral(0,bin)

        if(_Debug):
            print 'Bin', bin, 'variable=',var,'sigInt=',sigInt,'bkgInt=',bkgInt

        # efficiencies
        sigEff.append(sigInt/sigIntTot)
        bkgEff.append(bkgInt/bkgIntTot)

    # plot ROC curve
    if not reject:
        # plot sig eff vs bkg eff
        hROC  = TH2F(title,'ROC curve for '+title+';sig eff;bkg eff',100,0,1,100,0,1)
        rocGraph = TGraph(nbins,array('d', sigEff),array('d', bkgEff));
        rocGraph.SetTitle('ROC curve for '+title+';sig eff;bkg eff')

    else:
        # plot sig eff vs bkg rejection
        hROC  = TH2F(title,'ROC curve for '+title+';sig eff;bkg rejection',100,0,1,100,0,1)

        bkgRej = [1-x for x in bkgEff]
        rocGraph = TGraph(nbins,array('d', sigEff),array('d', bkgRej));
        rocGraph.SetTitle('ROC curve for '+title+';sig eff;bkg rejection')

    for bin in range(nbins):

        if not reject:
            hROC.Fill(sigEff[bin],bkgEff[bin])
        else:
            hROC.Fill(sigEff[bin],1-bkgEff[bin])

    return rocGraph

def getSelection(tree,sigcut,bkgcut,var = 'relIso', title = ''):

    print '#Selecting histos from', tree.GetName()
    print '#Signal cut:', sigcut
    print '#Bkg with:', bkgcut

    if title == '':
        title = var

    if 'Iso' not in var:
        hSig = TH1F (var+title+'_sig',title+' signal selection',200,0,5)
        hBkg = TH1F (var+title+'_bkg',title+' bkg selection',200,0,5)
    else:
        # make log binning
        xbins = getLogBins(100,0.001,10)
        binc = array('d', xbins)

        hSig = TH1F (var+title+'_sig',title+' signal selection',len(binc)-1,binc)
        hBkg = TH1F (var+title+'_bkg',title+' bkg selection',len(binc)-1,binc)


    # add event weight
    bkgcut = 'EvWeight * (' + bkgcut + ')'
    sigcut = 'EvWeight * (' + sigcut + ')'

    hBkg.Sumw2()
    hSig.Sumw2()

    tree.Draw(var+' >> '+hBkg.GetName(),bkgcut)
    tree.Draw(var+' >> '+hSig.GetName(),sigcut)

    hBkg.SetStats(0)
    hSig.SetStats(0)

    hBkg.GetXaxis().SetTitle(var)
    hSig.GetXaxis().SetTitle(var)

    print 'Got', hBkg.GetEntries(), 'entries in background selection'
    print 'Got', hSig.GetEntries(), 'entries in signal selection'

    return [hSig,hBkg]

def plotHists(histList, title = ''):

    dosame = ''

    if title == '':
        title = histList[0].GetTitle()

    # replace hists with clones
    histList = [h.Clone() for h in histList]

    canv = TCanvas('c'+title.replace(' ',''),title,800,600)

    leg = getLegend()

    for indx,hist in enumerate(histList):
        hist.Draw(dosame+'hist')

        leg.AddEntry(hist,hist.GetTitle(),'l')
        SetOwnership( hist, 0 )

        hist.SetLineColor(indx+1)
        hist.SetLineWidth(2)

        if dosame == '': dosame = 'same'

    histList[0].GetYaxis().SetRangeUser(max(histList[0].GetMinimum(),0.01),histList[0].GetMaximum()*1.5)
    leg.Draw()

    histList[0].SetTitle(title)

    SetOwnership( canv, 0 )
    SetOwnership( leg, 0 )

    return canv

def plotGraphs(graphList, title = ''):

    # initial draw command
    dosame = 'APC'

    if title == '':
        title = graphList[0].GetTitle()

    # replace graphs with clones
    graphList = [g.Clone() for g in graphList]

    canv = TCanvas('c'+title.replace(' ',''),title,800,600)
#    multiGraph = TMultiGraph();

    leg = getLegend()

    for indx,graph in enumerate(graphList):
        graph.Draw(dosame)

        title = graph.GetTitle()
        title = title[:title.find(';')]

        leg.AddEntry(graph,title,'pl')
        SetOwnership( graph, 0 )

        graph.SetLineWidth(2)
        graph.SetLineColor(indx+1)
        graph.SetMarkerColor(indx+1)
        graph.SetMarkerStyle(indx+20)

        if dosame == 'APC': dosame = 'PC same'

    graphList[0].GetXaxis().SetRangeUser(0,1)
    graphList[0].GetYaxis().SetRangeUser(0,1)

    leg.Draw()

    graphList[0].SetTitle(title)

    SetOwnership( canv, 0 )
    SetOwnership( leg, 0 )

    return canv


# first argument is '-b' == for batch mode
#    print sys.argv

if __name__ == "__main__":

    if len(sys.argv) > 2:
        infile = sys.argv[1]

    tfile  = TFile(infile, "READ")
    outfile = TFile('ROC_'+infile, "RECREATE")

    if not tfile:
        print "Couldn't open the file"
        exit(0)

    # get the TTree
    leptree = tfile.Get('Leptons')
    print 'Found', leptree.GetEntries(), 'entries in Lepton tree'

    promptElCut = 'nGoodLep ==1 && pdgID == 11 && passMVA && match && prompt'
    nonPromptElCut = 'nGoodLep ==1 && pdgID == 11 && passMVA && match && !prompt'
    unmatchedElCut = 'nGoodLep ==1 && pdgID == 11 && passMVA && !match'

    print 'Processing relIso'

    # Electrons relIso
    promptElhists = getSelection(leptree,promptElCut,unmatchedElCut,'relIso','Prompt')
    nonPromptElhists = getSelection(leptree,nonPromptElCut,unmatchedElCut,'relIso','NonPrompt')

    canv = plotHists(promptElhists+nonPromptElhists,'El relIso')
    canv.Write()

    canv = plotHists(promptElhists,'prompt El relIso')
    canv.Write()

    canv = plotHists(nonPromptElhists,'non prompt El relIso')
    canv.Write()

    rocPromptEl = makeROC(promptElhists[0],promptElhists[1],'relIso',true)
    rocNonPromptEl = makeROC(nonPromptElhists[0],nonPromptElhists[1],'relIso',true)

    canv = plotGraphs([rocPromptEl,rocNonPromptEl],'RelIso ROC curve')
    canv.Write()

    print 'Processing miniIso'

    promptElhists = getSelection(leptree,promptElCut,unmatchedElCut,'miniIso','Prompt')
    nonPromptElhists = getSelection(leptree,nonPromptElCut,unmatchedElCut,'miniIso','NonPrompt')

    canv = plotHists(promptElhists+nonPromptElhists,'El miniIso')
    canv.Write()

    canv = plotHists(promptElhists,'prompt El miniIso')
    canv.Write()

    canv = plotHists(nonPromptElhists,'non prompt El miniIso')
    canv.Write()

    rocPromptElmini = makeROC(promptElhists[0],promptElhists[1],'miniIso',true)
    rocNonPromptElmini = makeROC(nonPromptElhists[0],nonPromptElhists[1],'miniIso',true)

    canv = plotGraphs([rocPromptElmini,rocNonPromptElmini],'MiniIso ROC curve')
    canv.Write()

    canv = plotGraphs([rocPromptEl,rocPromptElmini],'Rel vs MiniIso ROC curve')
    canv.Write()

    tfile.Close()
    outfile.Close()
