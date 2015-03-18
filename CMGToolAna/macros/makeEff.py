#!/usr/bin/python

import sys
sys.argv.append( '-b' )

from ROOT import *
from array import array

def getLegend(pos = 'ne'):
#    leg = TLegend(0.63,0.525,0.87,0.875)
#    leg = TLegend(0.1,0.7,0.48,0.9)
    if pos == 'ne':
        leg = TLegend(0.4,0.7,0.9,0.9)
    elif pos == 'log':
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

def getNormHist(hist):
    # normalize a variable binned size hist
    for bin in range(1,hist.GetNbinsX()):
        hist.SetBinContent(bin, hist.GetBinContent(bin) / hist.GetBinWidth(bin))

    return hist

def makeEffPlot(hTotal, hPassed,title = 'Efficiency'):

    hEff = hPassed.Clone()
    hEff.Divide(hTotal)

    canv = TCanvas(title,title,800,600)
#    canv.SetLogy()

    hEff.Draw()

    SetOwnership( hEff, 0 )
    SetOwnership( canv, 0 )

    print 'Max Efficiency', hEff.GetMaximum()
    return canv

def getEffPlot(hTotal, hPassed,title = 'Efficiency'):

#    hEff = hPassed.Clone()
#    hEff.Divide(hTotal)
    hEff = TEfficiency(hPassed, hTotal)

    var = hPassed.GetXaxis().GetTitle()

    if var == 'lepPt':
        var = 'lepton p_{T}'

    hEff.SetTitle(title+';'+ var + ';Efficiency');
    hEff.SetName('Eff_'+hEff.GetName());

#    print 'Max Efficiency', hEff.GetMaximum()
    return hEff


def getSelection(tree,totalcut,selcut,title = 'Efficiency',varbin = false, var = 'lepPt'):

    print '#Selecting histos for ', title
    print '#Base cut:', totalcut
    print '#Pass with:', selcut

#    title = var + '_' + title
    hname = var+title.replace(' ','')

    if not varbin:
        if var == 'lepPt':
            hTotal = TH1F (hname+'_total',title+' (total)',100,0,500);
            hPassed = TH1F (hname+'_pass',title+' (pass)',100,0,500);

        if var == 'HT':
            hTotal = TH1F (hname+'_total',title+' (total)',100,0,2500);
            hPassed = TH1F (hname+'_pass',title+' (pass)',100,0,2500);

        if var == 'lepEta':
            hTotal = TH1F (hname+'_total',title+' (total)',100,-2.5,2.5);
            hPassed = TH1F (hname+'_pass',title+' (pass)',100,-2.5,2.5);

    else:
        # for Pt
        if var == 'lepPt':
            #xbins = [0, 25 , 30 , 35 , 40 , 50 , 60 , 70 , 80 , 100 , 120 , 140 , 160 , 180, 200, 250, 300, 400, 500 ]
            #xbins = [0] + range(25,40,5) + range (40,60,10) + range(60,100,20) + range(100,200,50) + range(200,500,100)
            xbins = [25,35,45,55,70,85,100,125,150,200,300,400]
        # for HT
        if var == 'HT':
            #xbins = [ 0, 50, 100, 200 ] +  range(400,2000,250) + [2500]
            xbins = [100,250,500,750,1000,1250,1500,1750,2000,2500,3000]

        hTotal = TH1F (hname+'_totalX',title+' (total)',int(len(xbins)-1),array('d', xbins))
        hPassed = TH1F(hname+'_passX',title+' (pass)',int(len(xbins)-1),array('d', xbins))

    # add event weight
#    selcut = 'EvWeight * (' + totalcut + ' && ' + selcut + ')'
#    totalcut = 'EvWeight * (' + totalcut + ')'
    selcut = totalcut + ' && ' + selcut

    # Fill histograms
    tree.Draw(var+' >> '+hTotal.GetName(),totalcut)
    tree.Draw(var+' >> '+hPassed.GetName(),selcut)

    # Compute weights
#    hTotal.Sumw2()
#    hPassed.Sumw2()

    # normalize to bin widths
#    hTotal = getNormHist(hTotal)
#    hPassed = getNormHist(hPassed)

    hTotal.SetStats(0)
    hPassed.SetStats(0)

    hTotal.GetXaxis().SetTitle(var)
    hPassed.GetXaxis().SetTitle(var)

    print 'Got', hTotal.GetEntries(), 'entries in total selection'
    print 'Got', hPassed.GetEntries(), 'entries in passed selection'

    return [hTotal,hPassed]

def plotHists(histList, title = '', legpos = 'ne'):

    drawOpt = ''

    if title == '':
        title = histList[0].GetTitle()

    # clone hists
    histList = [h.Clone() for h in histList]

    canv = TCanvas('c'+title.replace(' ',''),title,800,600)

    leg = getLegend(legpos)

    for indx,hist in enumerate(histList):
        hist.Draw(drawOpt+'hist')

        leg.AddEntry(hist,hist.GetTitle(),'l')
        SetOwnership( hist, 0 )

        hist.SetLineColor(indx+1)
        hist.SetLineWidth(2)

        if drawOpt == '': drawOpt = 'same'

    # set Y axis range
    ymin = max(0.001,min([h.GetMinimum(0) for h in histList]))
    ymax = max([h.GetMaximum() for h in histList])

    if 'log' in legpos:
        ymax *= 2
    else:
        ymax *= 1.5

    histList[0].GetYaxis().SetRangeUser(ymin,ymax)

    # set pad title
    histList[0].SetTitle(title)

    # draw legend
    leg.Draw()

    SetOwnership( canv, 0 )
    SetOwnership( leg, 0 )

    return canv

def plotEffs(histList, title = '', legpos = 'ne'):

    drawOpt = 'AP'

    if title == '':
        title = histList[0].GetTitle()

    # create canvas
    canv = TCanvas(title.replace(' ',''),title,800,600)
    leg = getLegend(legpos)

    '''
    canv.Clear()
    gPad.Clear()

    # draw hBase
    hBase0 = histList[0].GetTotalHistogram().Clone()
    hBase = hBase0.Clone()
    hBase.SetName('dummy')
    hBase.DrawNormalized()
    hBase.GetYaxis().SetRangeUser(0,1.5)
    hBase.SetMaximum(10)
    hBase.SetFillColor(3)

    gPad.Modify()
    gPad.Update()

    print hBase
    '''

    # clone hists
    histList = [h.Clone() for h in histList]

    for indx,hist in enumerate(histList):

        hist.Draw(drawOpt)

        if drawOpt == 'AP':
            drawOpt = 'Psame'

        leg.AddEntry(hist,hist.GetTitle(),'l')
        SetOwnership( hist, 0 )

        hist.SetLineColor(indx+1)
        hist.SetLineWidth(2)

    leg.Draw()

#    hBase.SetTitle(title)
    histList[0].SetTitle(title)

    SetOwnership( canv, 0 )
    SetOwnership( leg, 0 )

    return canv

# first argument is '-b' == for batch mode
#    print sys.argv

if __name__ == "__main__":

    if len(sys.argv) > 2:
        infile = sys.argv[1]

    tfile  = TFile(infile, "READ")
    outfile = TFile('Eff_'+infile, "RECREATE")

    if not tfile:
        print "Couldn't open the file"
        exit(0)

    # get the TTree
    leptree = tfile.Get('Leptons')
    print 'Found', leptree.GetEntries(), 'entries in Lepton tree'

    ##########
    # ELECTRON
    ##########

    promptCut = 'pdgID == 11 && passID && match && prompt'
    promptCut += '&& nGoodLep == 1'
    nonpromptCut = 'pdgID == 11 && passID && match && !prompt'
    nonpromptCut += '&& nGoodLep == 1'
    unmatchedCut = 'pdgID == 11 && passID && !match'
    unmatchedCut += '&& nGoodLep == 1'
    fakeCut = '('+ unmatchedCut +') || (' + nonpromptCut +')'

#    passCut = 'passIso'
    relIsoCut = 'relIso < 0.2'
    miniIsoCut = 'miniIso < 0.1'

    #########
    # RELISO
    #########

    # Electron PT plots
    El_prompt_Pt_relIso_hist = getSelection(leptree,promptCut,relIsoCut,'Prompt El RelIso',false,'lepPt')
    El_nonprompt_Pt_relIso_hist = getSelection(leptree,nonpromptCut,relIsoCut,'Nonprompt El RelIso',false,'lepPt')
    El_unmatched_Pt_relIso_hist = getSelection(leptree,unmatchedCut,relIsoCut,'Unmatched El RelIso',false,'lepPt')
    El_fake_Pt_relIso_hist = getSelection(leptree,fakeCut,relIsoCut,'Fake El RelIso',false,'lepPt')

    canv = plotHists(El_prompt_Pt_relIso_hist+El_fake_Pt_relIso_hist,'El Pt (relIso)','log')
    canv.SetLogy()
    canv.Write()

    canv = plotHists(El_prompt_Pt_relIso_hist[:1]+El_nonprompt_Pt_relIso_hist[:1]+El_unmatched_Pt_relIso_hist[:1]+El_fake_Pt_relIso_hist[:1],'All El Pt (relIso)','log')
    canv.SetLogy()
    canv.Write()

    # calc eff
    # get plots in var bin size:
    El_prompt_Pt_relIso_hist = getSelection(leptree,promptCut,relIsoCut,'Prompt El RelIso',true,'lepPt')
    El_fake_Pt_relIso_hist = getSelection(leptree,fakeCut,relIsoCut,'Fake El RelIso',true,'lepPt')

    hEl_prompt_Pt_relIso_eff = getEffPlot(El_prompt_Pt_relIso_hist[0],El_prompt_Pt_relIso_hist[1],'Prompt El RelIso Efficiency vs Pt')
    hEl_fake_Pt_relIso_eff = getEffPlot(El_fake_Pt_relIso_hist[0],El_fake_Pt_relIso_hist[1],'Fake El RelIso Efficiency vs Pt')

    canv = plotEffs([hEl_prompt_Pt_relIso_eff,hEl_fake_Pt_relIso_eff],'Electron Pt Eff RelIso')
    canv.Write()


    # Electron HT plots
    El_prompt_HT_relIso_hist = getSelection(leptree,promptCut,relIsoCut,'Prompt El RelIso',false,'HT')
    El_fake_HT_relIso_hist = getSelection(leptree,fakeCut,relIsoCut,'Fake El RelIso',false,'HT')

    canv = plotHists(El_prompt_HT_relIso_hist+El_fake_HT_relIso_hist,'El HT (relIso)','log')
    canv.SetLogy()
    canv.Write()

    # calc eff
    # get plots in var bin size:
    El_prompt_HT_relIso_hist = getSelection(leptree,promptCut,relIsoCut,'Prompt El RelIso',true,'HT')
    El_fake_HT_relIso_hist = getSelection(leptree,fakeCut,relIsoCut,'Fake El RelIso',true,'HT')


    hEl_prompt_HT_relIso_eff = getEffPlot(El_prompt_HT_relIso_hist[0],El_prompt_HT_relIso_hist[1],'Prompt El RelIso Efficiency vs HT')
    hEl_fake_HT_relIso_eff = getEffPlot(El_fake_HT_relIso_hist[0],El_fake_HT_relIso_hist[1],'Fake El RelIso Efficiency vs HT')

    canv = plotEffs([hEl_prompt_HT_relIso_eff,hEl_fake_HT_relIso_eff],'Electron HT Eff RelIso')
    canv.Write()

    #########
    # MINIISO
    #########

    # Electron PT plots
    El_prompt_Pt_miniIso_hist = getSelection(leptree,promptCut,miniIsoCut,'Prompt El MiniIso',false,'lepPt')
    El_nonprompt_Pt_miniIso_hist = getSelection(leptree,nonpromptCut,miniIsoCut,'Nonprompt El MiniIso',false,'lepPt')
    El_unmatched_Pt_miniIso_hist = getSelection(leptree,unmatchedCut,miniIsoCut,'Unmatched El MiniIso',false,'lepPt')
    El_fake_Pt_miniIso_hist = getSelection(leptree,fakeCut,miniIsoCut,'Fake El MiniIso',false,'lepPt')

    canv = plotHists(El_prompt_Pt_miniIso_hist+El_fake_Pt_miniIso_hist,'El Pt (miniIso)','log')
    canv.SetLogy()
    canv.Write()

    canv = plotHists(El_prompt_Pt_miniIso_hist[:1]+El_nonprompt_Pt_miniIso_hist[:1]+El_unmatched_Pt_miniIso_hist[:1]+El_fake_Pt_miniIso_hist[:1],'All El Pt (miniIso)','log')
    canv.SetLogy()
    canv.Write()

    # calc eff
    # get plots in var bin size:
    El_prompt_Pt_miniIso_hist = getSelection(leptree,promptCut,miniIsoCut,'Prompt El MiniIso',true,'lepPt')
    El_fake_Pt_miniIso_hist = getSelection(leptree,fakeCut,miniIsoCut,'Fake El MiniIso',true,'lepPt')

    hEl_prompt_Pt_miniIso_eff = getEffPlot(El_prompt_Pt_miniIso_hist[0],El_prompt_Pt_miniIso_hist[1],'Prompt El MiniIso Efficiency vs Pt')
    hEl_fake_Pt_miniIso_eff = getEffPlot(El_fake_Pt_miniIso_hist[0],El_fake_Pt_miniIso_hist[1],'Fake El MiniIso Efficiency vs Pt')

    canv = plotEffs([hEl_prompt_Pt_miniIso_eff,hEl_fake_Pt_miniIso_eff],'Electron Pt Eff MiniIso')
    canv.Write()

    # Electron HT plots
    El_prompt_HT_miniIso_hist = getSelection(leptree,promptCut,miniIsoCut,'Prompt El MiniIso',false,'HT')
    El_fake_HT_miniIso_hist = getSelection(leptree,fakeCut,miniIsoCut,'Fake El MiniIso',false,'HT')

    canv = plotHists(El_prompt_HT_miniIso_hist+El_fake_HT_miniIso_hist,'El HT (miniIso)','log')
    canv.SetLogy()
    canv.Write()

    # calc eff
    # get plots in var bin size:
    El_prompt_HT_miniIso_hist = getSelection(leptree,promptCut,miniIsoCut,'Prompt El MiniIso',true,'HT')
    El_fake_HT_miniIso_hist = getSelection(leptree,fakeCut,miniIsoCut,'Fake El MiniIso',true,'HT')


    hEl_prompt_HT_miniIso_eff = getEffPlot(El_prompt_HT_miniIso_hist[0],El_prompt_HT_miniIso_hist[1],'Prompt El MiniIso Efficiency vs HT')
    hEl_fake_HT_miniIso_eff = getEffPlot(El_fake_HT_miniIso_hist[0],El_fake_HT_miniIso_hist[1],'Fake El MiniIso Efficiency vs HT')

    canv = plotEffs([hEl_prompt_HT_miniIso_eff,hEl_fake_HT_miniIso_eff],'Electron HT Eff MiniIso')
    canv.Write()

    #########
    # COMPARE
    #########

    # Pt
    hEl_prompt_Pt_allIso_eff = [hEl_prompt_Pt_relIso_eff,hEl_prompt_Pt_miniIso_eff,hEl_fake_Pt_relIso_eff,hEl_fake_Pt_miniIso_eff]
    canv = plotEffs(hEl_prompt_Pt_allIso_eff,'Electron Isolation Efficiency vs Pt')
    canv.Write()

    hEl_prompt_Pt_allIso_eff = [hEl_prompt_Pt_relIso_eff,hEl_prompt_Pt_miniIso_eff]
    canv = plotEffs(hEl_prompt_Pt_allIso_eff,'Prompt Electron Isolation Efficiency vs Pt')
    canv.Write()

    hEl_prompt_Pt_allIso_eff = [hEl_fake_Pt_relIso_eff,hEl_fake_Pt_miniIso_eff]
    canv = plotEffs(hEl_prompt_Pt_allIso_eff,'Fake Electron Isolation Efficiency vs Pt')
    canv.Write()

    # HT
    hEl_prompt_HT_allIso_eff = [hEl_prompt_HT_relIso_eff,hEl_prompt_HT_miniIso_eff,hEl_fake_HT_relIso_eff,hEl_fake_HT_miniIso_eff]
    canv = plotEffs(hEl_prompt_HT_allIso_eff,'Electron Isolation Efficiency vs HT')
    canv.Write()

    hEl_prompt_HT_allIso_eff = [hEl_prompt_HT_relIso_eff,hEl_prompt_HT_miniIso_eff]
    canv = plotEffs(hEl_prompt_HT_allIso_eff,'Prompt Electron Isolation Efficiency vs HT')
    canv.Write()

    hEl_prompt_HT_allIso_eff = [hEl_fake_HT_relIso_eff,hEl_fake_HT_miniIso_eff]
    canv = plotEffs(hEl_prompt_HT_allIso_eff,'Fake Electron Isolation Efficiency vs HT')
    canv.Write()


    ##########
    # MUON
    ##########

    promptCut = 'nGoodLep == 1 && pdgID == 13 && passID && match && prompt'
    nonpromptCut = 'nGoodLep == 1 && pdgID == 13 && passID && match && !prompt'
    unmatchedCut = 'nGoodLep == 1 && pdgID == 13 && passID && !match'
    fakeCut = '('+ unmatchedCut +') || (' + nonpromptCut +')'

    relIsoCut = 'relIso < 0.1'
    miniIsoCut = 'miniIso < 0.1'

    #########
    # RELISO
    #########

    # Muon PT plots
    Mu_prompt_Pt_relIso_hist = getSelection(leptree,promptCut,relIsoCut,'Prompt Mu RelIso',false,'lepPt')
    Mu_nonprompt_Pt_relIso_hist = getSelection(leptree,nonpromptCut,relIsoCut,'Nonprompt Mu RelIso',false,'lepPt')
    Mu_unmatched_Pt_relIso_hist = getSelection(leptree,unmatchedCut,relIsoCut,'Unmatched Mu RelIso',false,'lepPt')
    Mu_fake_Pt_relIso_hist = getSelection(leptree,fakeCut,relIsoCut,'Fake Mu RelIso',false,'lepPt')

    canv = plotHists(Mu_prompt_Pt_relIso_hist+Mu_fake_Pt_relIso_hist,'Mu Pt (relIso)','log')
    canv.SetLogy()
    canv.Write()

    canv = plotHists(Mu_prompt_Pt_relIso_hist[:1]+Mu_nonprompt_Pt_relIso_hist[:1]+Mu_unmatched_Pt_relIso_hist[:1]+Mu_fake_Pt_relIso_hist[:1],'All Mu Pt (relIso)','log')
    canv.SetLogy()
    canv.Write()

    # calc eff
    # get plots in var bin size:
    Mu_prompt_Pt_relIso_hist = getSelection(leptree,promptCut,relIsoCut,'Prompt Mu RelIso',true,'lepPt')
    Mu_fake_Pt_relIso_hist = getSelection(leptree,fakeCut,relIsoCut,'Fake Mu RelIso',true,'lepPt')

    hMu_prompt_Pt_relIso_eff = getEffPlot(Mu_prompt_Pt_relIso_hist[0],Mu_prompt_Pt_relIso_hist[1],'Prompt Mu RelIso Efficiency vs Pt')
    hMu_fake_Pt_relIso_eff = getEffPlot(Mu_fake_Pt_relIso_hist[0],Mu_fake_Pt_relIso_hist[1],'Fake Mu RelIso Efficiency vs Pt')

    canv = plotEffs([hMu_prompt_Pt_relIso_eff,hMu_fake_Pt_relIso_eff],'Muon Pt Eff RelIso')
    canv.Write()

    # Muon HT plots
    Mu_prompt_HT_relIso_hist = getSelection(leptree,promptCut,relIsoCut,'Prompt Mu RelIso',false,'HT')
    Mu_fake_HT_relIso_hist = getSelection(leptree,fakeCut,relIsoCut,'Fake Mu RelIso',false,'HT')

    canv = plotHists(Mu_prompt_HT_relIso_hist+Mu_fake_HT_relIso_hist,'Mu HT (relIso)','log')
    canv.SetLogy()
    canv.Write()

    # calc eff
    # get plots in var bin size:
    Mu_prompt_HT_relIso_hist = getSelection(leptree,promptCut,relIsoCut,'Prompt Mu RelIso',true,'HT')
    Mu_fake_HT_relIso_hist = getSelection(leptree,fakeCut,relIsoCut,'Fake Mu RelIso',true,'HT')


    hMu_prompt_HT_relIso_eff = getEffPlot(Mu_prompt_HT_relIso_hist[0],Mu_prompt_HT_relIso_hist[1],'Prompt Mu RelIso Efficiency vs HT')
    hMu_fake_HT_relIso_eff = getEffPlot(Mu_fake_HT_relIso_hist[0],Mu_fake_HT_relIso_hist[1],'Fake Mu RelIso Efficiency vs HT')

    canv = plotEffs([hMu_prompt_HT_relIso_eff,hMu_fake_HT_relIso_eff],'Muon HT Eff RelIso')
    canv.Write()

    #########
    # MINIISO
    #########

    # Muon PT plots
    Mu_prompt_Pt_miniIso_hist = getSelection(leptree,promptCut,miniIsoCut,'Prompt Mu MiniIso',false,'lepPt')
    Mu_nonprompt_Pt_miniIso_hist = getSelection(leptree,nonpromptCut,miniIsoCut,'Nonprompt Mu MiniIso',false,'lepPt')
    Mu_unmatched_Pt_miniIso_hist = getSelection(leptree,unmatchedCut,miniIsoCut,'Unmatched Mu MiniIso',false,'lepPt')
    Mu_fake_Pt_miniIso_hist = getSelection(leptree,fakeCut,miniIsoCut,'Fake Mu MiniIso',false,'lepPt')

    canv = plotHists(Mu_prompt_Pt_miniIso_hist+Mu_fake_Pt_miniIso_hist,'Mu Pt (miniIso)','log')
    canv.SetLogy()
    canv.Write()

    canv = plotHists(Mu_prompt_Pt_miniIso_hist[:1]+Mu_nonprompt_Pt_miniIso_hist[:1]+Mu_unmatched_Pt_miniIso_hist[:1]+Mu_fake_Pt_miniIso_hist[:1],'All Mu Pt (miniIso)','log')
    canv.SetLogy()
    canv.Write()

    # calc eff
    # get plots in var bin size:
    Mu_prompt_Pt_miniIso_hist = getSelection(leptree,promptCut,miniIsoCut,'Prompt Mu MiniIso',true,'lepPt')
    Mu_fake_Pt_miniIso_hist = getSelection(leptree,fakeCut,miniIsoCut,'Fake Mu MiniIso',true,'lepPt')

    hMu_prompt_Pt_miniIso_eff = getEffPlot(Mu_prompt_Pt_miniIso_hist[0],Mu_prompt_Pt_miniIso_hist[1],'Prompt Mu MiniIso Efficiency vs Pt')
    hMu_fake_Pt_miniIso_eff = getEffPlot(Mu_fake_Pt_miniIso_hist[0],Mu_fake_Pt_miniIso_hist[1],'Fake Mu MiniIso Efficiency vs Pt')

    canv = plotEffs([hMu_prompt_Pt_miniIso_eff,hMu_fake_Pt_miniIso_eff],'Muon Pt Eff MiniIso')
    canv.Write()

    # Muon HT plots
    Mu_prompt_HT_miniIso_hist = getSelection(leptree,promptCut,miniIsoCut,'Prompt Mu MiniIso',false,'HT')
    Mu_fake_HT_miniIso_hist = getSelection(leptree,fakeCut,miniIsoCut,'Fake Mu MiniIso',false,'HT')

    canv = plotHists(Mu_prompt_HT_miniIso_hist+Mu_fake_HT_miniIso_hist,'Mu HT (miniIso)','log')
    canv.SetLogy()
    canv.Write()

    # calc eff
    # get plots in var bin size:
    Mu_prompt_HT_miniIso_hist = getSelection(leptree,promptCut,miniIsoCut,'Prompt Mu MiniIso',true,'HT')
    Mu_fake_HT_miniIso_hist = getSelection(leptree,fakeCut,miniIsoCut,'Fake Mu MiniIso',true,'HT')


    hMu_prompt_HT_miniIso_eff = getEffPlot(Mu_prompt_HT_miniIso_hist[0],Mu_prompt_HT_miniIso_hist[1],'Prompt Mu MiniIso Efficiency vs HT')
    hMu_fake_HT_miniIso_eff = getEffPlot(Mu_fake_HT_miniIso_hist[0],Mu_fake_HT_miniIso_hist[1],'Fake Mu MiniIso Efficiency vs HT')

    canv = plotEffs([hMu_prompt_HT_miniIso_eff,hMu_fake_HT_miniIso_eff],'Muon HT Eff MiniIso')
    canv.Write()

    #########
    # COMPARE
    #########

    # Pt
    hMu_prompt_Pt_allIso_eff = [hMu_prompt_Pt_relIso_eff,hMu_prompt_Pt_miniIso_eff,hMu_fake_Pt_relIso_eff,hMu_fake_Pt_miniIso_eff]
    canv = plotEffs(hMu_prompt_Pt_allIso_eff,'Muon Isolation Efficiency vs Pt')
    canv.Write()

    hMu_prompt_Pt_allIso_eff = [hMu_prompt_Pt_relIso_eff,hMu_prompt_Pt_miniIso_eff]
    canv = plotEffs(hMu_prompt_Pt_allIso_eff,'Prompt Muon Isolation Efficiency vs Pt')
    canv.Write()

    hMu_prompt_Pt_allIso_eff = [hMu_fake_Pt_relIso_eff,hMu_fake_Pt_miniIso_eff]
    canv = plotEffs(hMu_prompt_Pt_allIso_eff,'Fake Muon Isolation Efficiency vs Pt')
    canv.Write()

    # HT
    hMu_prompt_HT_allIso_eff = [hMu_prompt_HT_relIso_eff,hMu_prompt_HT_miniIso_eff,hMu_fake_HT_relIso_eff,hMu_fake_HT_miniIso_eff]
    canv = plotEffs(hMu_prompt_HT_allIso_eff,'Muon Isolation Efficiency vs HT')
    canv.Write()

    hMu_prompt_HT_allIso_eff = [hMu_prompt_HT_relIso_eff,hMu_prompt_HT_miniIso_eff]
    canv = plotEffs(hMu_prompt_HT_allIso_eff,'Prompt Muon Isolation Efficiency vs HT')
    canv.Write()

    hMu_prompt_HT_allIso_eff = [hMu_fake_HT_relIso_eff,hMu_fake_HT_miniIso_eff]
    canv = plotEffs(hMu_prompt_HT_allIso_eff,'Fake Muon Isolation Efficiency vs HT')
    canv.Write()

    ###############
    # Closing files
    ###############

    tfile.Close()
    outfile.Close()
