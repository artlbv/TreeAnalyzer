#!/usr/bin/python

import sys
import math
sys.argv.append( '-b' )

from ROOT import *
from array import array

_Debug = false

def getLegend(pos = 'ne'):
#    leg = TLegend(0.63,0.525,0.87,0.875)
#    leg = TLegend(0.1,0.7,0.48,0.9)
    if pos == 'ne':
        leg = TLegend(0.5,0.7,0.9,0.9)
    elif pos == 'logiso':
        leg = TLegend(0.6,0.8,0.99,0.99)
    elif pos == 'roc':
        leg = TLegend(0.15,0.2,0.7,0.4)

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

    hSigEff = hSig.Clone()
    hBkgEff = hBkg.Clone()
    hBkgRej = hBkg.Clone()

    hSigEff.GetYaxis().SetTitle('sig efficiency')
    hBkgEff.GetYaxis().SetTitle('bkg efficiency')
    hBkgRej.GetYaxis().SetTitle('bkg rejection')

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

        hSigEff.SetBinContent(bin,sigInt/sigIntTot)
        hBkgEff.SetBinContent(bin,bkgInt/bkgIntTot)
        hBkgRej.SetBinContent(bin,1-bkgInt/bkgIntTot)

    # plot ROC curve
    if not reject:
        # plot sig eff vs bkg eff
        hROC  = TH2F(title,'ROC curve for '+title,100,0,1,100,0,1)
        rocGraph = TGraph(nbins,array('d', sigEff),array('d', bkgEff));
        rocGraph.SetTitle('ROC curve for '+title)
        rocGraph.GetYaxis().SetTitle('bkg eff')

    else:
        # plot sig eff vs bkg rejection
        hROC  = TH2F(title,'ROC curve for '+title+';sig eff;bkg rejection',100,0,1,100,0,1)

        bkgRej = [1-x for x in bkgEff]
        rocGraph = TGraph(nbins,array('d', sigEff),array('d', bkgRej));
        rocGraph.SetTitle('ROC curve for '+title)
        rocGraph.GetYaxis().SetTitle('bkg rejection')

    rocGraph.GetXaxis().SetTitle(hSig.GetTitle()+' eff')

    '''
    for bin in range(nbins):
        if not reject:
            hROC.Fill(sigEff[bin],bkgEff[bin])
        else:
            hROC.Fill(sigEff[bin],1-bkgEff[bin])
    '''

    if reject:
#        return [rocGraph,hSigEff,hBkgRej]
        return [rocGraph,hSigEff,hBkgEff]
    else:
        return [rocGraph,hSigEff,hBkgEff]

def getHistFromTree(tree,selcut,var = 'relIso', title = ''):

    print '#Selecting histos from', tree.GetName()
    print '#With cut:', selcut

    if title == '':
        title = var
        hname = var
    else:
        # remove spaces
        hname = var+'_'+title.replace(' ','')


    if 'Iso' in var:
        hist = TH1F (hname,title,200,0,5)
    else:
        # make log binning
        xbins = getLogBins(100,0.001,10)
        binc = array('d', xbins)

        hist = TH1F (hname,title,len(binc)-1,binc)


    # add event weight
    selcut = 'EvWeight * (' + selcut + ')'

    hist.Sumw2()

    tree.Draw(var+' >> '+hist.GetName(),selcut)

    hist.SetStats(0)
    hist.GetXaxis().SetTitle(var)

    print 'Got', hist.GetEntries(), 'entries in selection'

    return hist

def plotHists(histList, title = '', legpos = 'ne'):

    dosame = ''

    if title == '':
        title = histList[0].GetTitle()

    # replace hists with clones
    histList = [h.Clone() for h in histList]

    canv = TCanvas(title.replace(' ',''),title,800,600)

    leg = getLegend(legpos)

    for indx,hist in enumerate(histList):
        hist.Draw(dosame+'hist')

        leg.AddEntry(hist,hist.GetTitle(),'l')
        SetOwnership( hist, 0 )

        hist.SetLineColor(indx+1)
        hist.SetLineWidth(2)

        if dosame == '': dosame = 'same'

    ymin = max(0.001,min([h.GetMinimum() for h in histList]))
    ymax = max([h.GetMaximum() for h in histList])

#    histList[0].GetYaxis().SetRangeUser(max(histList[0].GetMinimum(),0.01),histList[0].GetMaximum()*1.5)
    histList[0].GetYaxis().SetRangeUser(ymin,ymax*1.5)

    leg.Draw()

    histList[0].SetTitle(title)

    SetOwnership( canv, 0 )
    SetOwnership( leg, 0 )

    return canv

def plotGraphs(graphList, title = '', legpos = 'ne'):

    # initial draw command
    dosame = 'APC'

    if title == '':
        title = graphList[0].GetTitle()

    # replace graphs with clones
    graphList = [g.Clone() for g in graphList]

    canv = TCanvas(title.replace(' ',''),title,800,600)
#    multiGraph = TMultiGraph();

    leg = getLegend(legpos)

    for indx,graph in enumerate(graphList):
        graph.Draw(dosame)

        gtitle = graph.GetTitle()
#        gtitle = gtitle[:gtitle.find(';')]

        leg.AddEntry(graph,gtitle,'pl')
        SetOwnership( graph, 0 )

        graph.SetLineWidth(2)
        graph.SetLineColor(indx+1)
        graph.SetMarkerColor(indx+1)
        graph.SetMarkerStyle(indx+20)

        if dosame == 'APC': dosame = 'PC same'

    graphList[0].GetXaxis().SetRangeUser(0.5,1)
    graphList[0].GetYaxis().SetRangeUser(0,1)

    leg.Draw()

    graphList[0].SetTitle(title)

    SetOwnership( canv, 0 )
    SetOwnership( leg, 0 )

    return canv


# first argument is '-b' == for batch mode
#    print sys.argv

if __name__ == "__main__":

    gStyle.SetOptTitle(kFALSE)

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


    promptElCut = 'pdgID == 11 && passID && match && prompt'
    nonPromptElCut = 'pdgID == 11 && passID && match && !prompt'
    unmatchedElCut = 'pdgID == 11 && passID && !match'

    print 'Processing relIso'

    # Electrons relIso
    hPromptEl_RelIso = getHistFromTree(leptree,promptElCut,'relIso','Prompt Electrons')
    hNonPromptEl_RelIso = getHistFromTree(leptree,nonPromptElCut,'relIso','NonPrompt Electrons')
    hUnmatchedEl_RelIso = getHistFromTree(leptree,unmatchedElCut,'relIso','Unmatched Electrons')
    hFakeEl_RelIso = hNonPromptEl_RelIso.Clone()
    hFakeEl_RelIso.SetName('hFakeEl_RelIso')
    hFakeEl_RelIso.Add(hUnmatchedEl_RelIso)
    hFakeEl_RelIso.SetTitle('Fake Electrons')

    El_relIso_hists = [hPromptEl_RelIso,hNonPromptEl_RelIso,hUnmatchedEl_RelIso,hFakeEl_RelIso]
    canv = plotHists(El_relIso_hists,'El relIso','logiso')
    canv.SetLogy()
    canv.SetLogx()

    canv.Write()

    # El ROC
    roc_NP_El_RelIso = makeROC(hPromptEl_RelIso,hNonPromptEl_RelIso,'NonPrompt (relIso)',true)
    roc_UM_El_RelIso = makeROC(hPromptEl_RelIso,hUnmatchedEl_RelIso,'UnMatched (relIso)',true)
    roc_Fake_El_RelIso = makeROC(hPromptEl_RelIso,hFakeEl_RelIso,'Fake (relIso)',true)

    El_relIso_rocs = [roc_NP_El_RelIso[0],roc_UM_El_RelIso[0],roc_Fake_El_RelIso[0]]

    canv = plotGraphs(El_relIso_rocs,'El RelIso ROC curve','roc')
    canv.SetGridx()
    canv.SetGridy()

    canv.Write()

    El_relIso_eff = roc_NP_El_RelIso[1:]+roc_UM_El_RelIso[2:]+roc_Fake_El_RelIso[2:]

    canv = plotHists(El_relIso_eff,'El RelIso cut efficiencies')
    canv.SetLogx()
    canv.SetGridx()
    canv.SetGridy()
    canv.Write()

    # Electrons MiniIso
    hPromptEl_MiniIso = getHistFromTree(leptree,promptElCut,'miniIso','Prompt Electrons')
    hNonPromptEl_MiniIso = getHistFromTree(leptree,nonPromptElCut,'miniIso','NonPrompt Electrons')
    hUnmatchedEl_MiniIso = getHistFromTree(leptree,unmatchedElCut,'miniIso','Unmatched Electrons')
    hFakeEl_MiniIso = hNonPromptEl_MiniIso.Clone()
    hFakeEl_MiniIso.SetName('hFakeEl_MiniIso')
    hFakeEl_MiniIso.Add(hUnmatchedEl_MiniIso)
    hFakeEl_MiniIso.SetTitle('Fake Electrons')

    El_miniIso_hists = [hPromptEl_MiniIso,hNonPromptEl_MiniIso,hUnmatchedEl_MiniIso,hFakeEl_MiniIso]
    canv = plotHists(El_miniIso_hists,'El miniIso','logiso')
    canv.SetLogy()
    canv.SetLogx()

    canv.Write()

    # El ROC
    roc_NP_El_MiniIso = makeROC(hPromptEl_MiniIso,hNonPromptEl_MiniIso,'NonPrompt (miniIso)',true)
    roc_UM_El_MiniIso = makeROC(hPromptEl_MiniIso,hUnmatchedEl_MiniIso,'UnMatched (miniIso)',true)
    roc_Fake_El_MiniIso = makeROC(hPromptEl_MiniIso,hFakeEl_MiniIso,'Fake (miniIso)',true)

    El_miniIso_rocs = [roc_NP_El_MiniIso[0],roc_UM_El_MiniIso[0],roc_Fake_El_MiniIso[0]]

    canv = plotGraphs(El_miniIso_rocs,'El MiniIso ROC curve','roc')
    canv.SetGridx()
    canv.SetGridy()

    canv.Write()

    El_miniIso_eff = roc_NP_El_MiniIso[1:]+roc_UM_El_MiniIso[2:]+roc_Fake_El_MiniIso[2:]

    canv = plotHists(El_miniIso_eff,'El MiniIso cut efficiencies')
    canv.SetLogx()
    canv.SetGridx()
    canv.SetGridy()
    canv.Write()

    # RelIso vs MiniIso
    canv  = plotGraphs(El_relIso_rocs+El_miniIso_rocs,'El ROC: RelIso vs MiniIso','roc')
    canv.SetGridx()
    canv.SetGridy()
    canv.Write()

    ############
    # MUONS
    ############

    promptMuCut = 'pdgID == 13 && passID && match && prompt'
    nonPromptMuCut = 'pdgID == 13 && passID && match && !prompt'
    unmatchedMuCut = 'pdgID == 13 && passID && !match'

    print 'Processing relIso'

    # Muons relIso
    hPromptMu_RelIso = getHistFromTree(leptree,promptMuCut,'relIso','Prompt Muons')
    hNonPromptMu_RelIso = getHistFromTree(leptree,nonPromptMuCut,'relIso','NonPrompt Muons')
    hUnmatchedMu_RelIso = getHistFromTree(leptree,unmatchedMuCut,'relIso','Unmatched Muons')
    hFakeMu_RelIso = hNonPromptMu_RelIso.Clone()
    hFakeMu_RelIso.SetName('hFakeMu_RelIso')
    hFakeMu_RelIso.Add(hUnmatchedMu_RelIso)
    hFakeMu_RelIso.SetTitle('Fake Muons')

    Mu_relIso_hists = [hPromptMu_RelIso,hNonPromptMu_RelIso,hUnmatchedMu_RelIso,hFakeMu_RelIso]
    canv = plotHists(Mu_relIso_hists,'Mu relIso','logiso')
    canv.SetLogy()
    canv.SetLogx()

    canv.Write()

    # Mu ROC
    roc_NP_Mu_RelIso = makeROC(hPromptMu_RelIso,hNonPromptMu_RelIso,'NonPrompt (relIso)',true)
    roc_UM_Mu_RelIso = makeROC(hPromptMu_RelIso,hUnmatchedMu_RelIso,'UnMatched (relIso)',true)
    roc_Fake_Mu_RelIso = makeROC(hPromptMu_RelIso,hFakeMu_RelIso,'Fake (relIso)',true)

    Mu_relIso_rocs = [roc_NP_Mu_RelIso[0],roc_UM_Mu_RelIso[0],roc_Fake_Mu_RelIso[0]]

    canv = plotGraphs(Mu_relIso_rocs,'Mu RelIso ROC curve','roc')
    canv.SetGridx()
    canv.SetGridy()

    canv.Write()

    Mu_relIso_eff = roc_NP_Mu_RelIso[1:]+roc_UM_Mu_RelIso[2:]+roc_Fake_Mu_RelIso[2:]

    canv = plotHists(Mu_relIso_eff,'Mu RelIso cut efficiencies')
    canv.SetLogx()
    canv.SetGridx()
    canv.SetGridy()
    canv.Write()

    # Muons MiniIso
    hPromptMu_MiniIso = getHistFromTree(leptree,promptMuCut,'miniIso','Prompt Muons')
    hNonPromptMu_MiniIso = getHistFromTree(leptree,nonPromptMuCut,'miniIso','NonPrompt Muons')
    hUnmatchedMu_MiniIso = getHistFromTree(leptree,unmatchedMuCut,'miniIso','Unmatched Muons')
    hFakeMu_MiniIso = hNonPromptMu_MiniIso.Clone()
    hFakeMu_MiniIso.SetName('hFakeMu_MiniIso')
    hFakeMu_MiniIso.Add(hUnmatchedMu_MiniIso)
    hFakeMu_MiniIso.SetTitle('Fake Muons')

    Mu_miniIso_hists = [hPromptMu_MiniIso,hNonPromptMu_MiniIso,hUnmatchedMu_MiniIso,hFakeMu_MiniIso]
    canv = plotHists(Mu_miniIso_hists,'Mu miniIso','logiso')
    canv.SetLogy()
    canv.SetLogx()

    canv.Write()

    # Mu ROC
    roc_NP_Mu_MiniIso = makeROC(hPromptMu_MiniIso,hNonPromptMu_MiniIso,'NonPrompt (miniIso)',true)
    roc_UM_Mu_MiniIso = makeROC(hPromptMu_MiniIso,hUnmatchedMu_MiniIso,'UnMatched (miniIso)',true)
    roc_Fake_Mu_MiniIso = makeROC(hPromptMu_MiniIso,hFakeMu_MiniIso,'Fake (miniIso)',true)

    Mu_miniIso_rocs = [roc_NP_Mu_MiniIso[0],roc_UM_Mu_MiniIso[0],roc_Fake_Mu_MiniIso[0]]

    canv = plotGraphs(Mu_miniIso_rocs,'Mu MiniIso ROC curve','roc')
    canv.SetGridx()
    canv.SetGridy()

    canv.Write()

    Mu_miniIso_eff = roc_NP_Mu_MiniIso[1:]+roc_UM_Mu_MiniIso[2:]+roc_Fake_Mu_MiniIso[2:]

    canv = plotHists(Mu_miniIso_eff,'Mu MiniIso cut efficiencies')
    canv.SetLogx()
    canv.SetGridx()
    canv.SetGridy()
    canv.Write()

    # RelIso vs MiniIso
    canv  = plotGraphs(Mu_relIso_rocs+Mu_miniIso_rocs,'Mu ROC: RelIso vs MiniIso','roc')
    canv.SetGridx()
    canv.SetGridy()
    canv.Write()

    # COMPARE MUON AND ELECTRON
    # RelIso vs MiniIso
    rocEl_relIso = roc_Fake_El_RelIso[0]
    rocEl_relIso.SetTitle('Elec relIso')

    rocEl_miniIso = roc_Fake_El_MiniIso[0]
    rocEl_miniIso.SetTitle('Elec miniIso')

    rocMu_relIso = roc_Fake_Mu_RelIso[0]
    rocMu_relIso.SetTitle('Muon relIso')

    rocMu_miniIso = roc_Fake_Mu_MiniIso[0]
    rocMu_miniIso.SetTitle('Muon miniIso')

    rocs = [rocEl_relIso,rocEl_miniIso,rocMu_relIso,rocMu_miniIso]

    canv  = plotGraphs(rocs,'El vs Mu ROC: RelIso vs MiniIso','roc')
    canv.SetGridx()
    canv.SetGridy()
    canv.Write()


    tfile.Close()
    outfile.Close()
