#!/usr/bin/python

import sys
sys.argv.append( '-b' )

from ROOT import *
from array import array

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

def makeEffPlot(hTotal, hPassed,title = 'Efficiency'):

    hEff = hPassed.Clone()
    hEff.Divide(hTotal)

    canv = TCanvas(title,title,800,600)
#    canv.SetLogy()

    hEff.Draw()

    SetOwnership( hEff, 0 )
    SetOwnership( canv, 0 )

    print 'Efficiency', hEff.GetMaximum()
    return canv

def getEffPlot(hTotal, hPassed,title = 'Efficiency'):

    hEff = hPassed.Clone()
    hEff.Divide(hTotal)
#    hEff = TEfficiency(hPassed, hTotal)

    hEff.SetTitle(title);
    hEff.SetName('Eff_'+hEff.GetName());

    print 'Efficiency', hEff.GetMaximum()
    return hEff


def getSelection(tree,totalcut,selcut,title = 'Efficiency',varbin = false, var = 'lepPt'):

    print '#Selecting histos from', tree.GetName(), 'with base cut:', totalcut
    print '#Pass with:', selcut

#    title = var + '_' + title

    if not varbin:
        if var == 'lepPt':
            hTotal = TH1F (title+'_total',title+'_total',100,0,500);
            hPassed = TH1F (title+'_pass',title+'_pass',100,0,500);

        if var == 'HT':
            hTotal = TH1F (title+'_total',title+'_total',100,0,2500);
            hPassed = TH1F (title+'_pass',title+'_pass',100,0,2500);
    else:
        # for Pt
        if var == 'lepPt':
        #xbins = [0, 25 , 30 , 35 , 40 , 50 , 60 , 70 , 80 , 100 , 120 , 140 , 160 , 180, 200, 250, 300, 400, 500 ]
            xbins = [0] + range(25,40,5) + range (40,60,10) + range(60,100,20) + range(100,200,50) + range(200,500,100)
        # for HT
        if var == 'HT':
            xbins = [ 0, 50, 100, 200 ] +  range(400,2000,250) + [2500]

        hTotal = TH1F (var+title+'_totalX',title+'_total',int(len(xbins)-1),array('d', xbins))
        hPassed = TH1F(var+title+'_passX',title+'_pass',int(len(xbins)-1),array('d', xbins))

    # add event weight
    totalcut = 'EvWeight * (' + totalcut + ')'
    selcut = 'EvWeight * (' + totalcut + ' && ' + selcut + ')'

    hTotal.Sumw2()
    hPassed.Sumw2()

    tree.Draw(var+' >> '+hTotal.GetName(),totalcut)
    tree.Draw(var+' >> '+hPassed.GetName(),selcut)

    hTotal.SetStats(0)
    hPassed.SetStats(0)

    hTotal.GetXaxis().SetTitle(var)
    hPassed.GetXaxis().SetTitle(var)

    print 'Got', hTotal.GetEntries(), 'entries in total selection'
    print 'Got', hPassed.GetEntries(), 'entries in passed selection'

    return [hTotal,hPassed]

def plotHists(histList, title = ''):

    dosame = ''

    if title == '':
        title = histList[0].GetTitle()

#    histList[0].SetMaximum(histList[0].GetMaximum()*1.5)

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

# first argument is '-b' == for batch mode
#    print sys.argv

if __name__ == "__main__":

    if len(sys.argv) > 2:
        infile = sys.argv[1]

    tfile  = TFile(infile, "READ")
    outfile = TFile('plot_'+infile, "RECREATE")

    if not tfile:
        print "Couldn't open the file"
        exit(0)

    # get the TTree
    leptree = tfile.Get('Leptons')
    print 'Found', leptree.GetEntries(), 'entries in Lepton tree'

    promptcut = 'nGoodLep ==1 && pdgID == 11 && match && prompt && passMVA'
    promptcut += ' && HT > 150'# && nJets >=6 '
    nonpromptcut = 'nGoodLep ==1 && pdgID == 11 && match && !prompt && passMVA'
    nonpromptcut += '&& HT > 150'# && nJets >=6 '

#    passcut = 'passIso'
    relIsoCut = 'relIso < 0.2'
    miniIsoCut = 'miniIso < 0.1'

    '''
    # Iso efficiency
    promptElHThists = getSelection(leptree,promptcut,relIsoCut,'Prompt El',false,'HT')
    nonPromptElHThists = getSelection(leptree,nonpromptcut,relIsoCut,'NonPrompt El',false,'HT')

    canv = plotHists(promptElHThists+nonPromptElHThists,'El HT spectra')
    canv.Write()

    promptElHThists = getSelection(leptree,promptcut,relIsoCut,'Prompt El',true,'HT')
    nonPromptElHThists = getSelection(leptree,nonpromptcut,relIsoCut,'NonPrompt El',true,'HT')

    promptElHTeffHist = getEffPlot(promptElHThists[0],promptElHThists[1],'Prompt El Iso Efficiency for HT')
    nonPromptElHTeffHist = getEffPlot(nonPromptElHThists[0],nonPromptElHThists[1],'Non Prompt El Iso Efficiency for HT')

    canv = plotHists([promptElHTeffHist,nonPromptElHTeffHist],'Electron HT Eff')
    #promptHists[0].DrawNormalized('same')
    canv.Write()
    '''
    # Electron PT plots
    promptElPThists = getSelection(leptree,promptcut,relIsoCut,'Prompt El',false,'lepPt')
    nonPromptElPThists = getSelection(leptree,nonpromptcut,relIsoCut,'NonPrompt El',false,'lepPt')

    promptElPThistsMini = getSelection(leptree,promptcut,miniIsoCut,'Prompt El MiniIso',false,'lepPt')
    nonPromptElPThistsMini = getSelection(leptree,nonpromptcut,miniIsoCut,'NonPrompt El MiniIso',false,'lepPt')

    canv = plotHists(promptElPThists+nonPromptElPThists,'El Pt spectra (relIso)')
    canv.Write()

    canv = plotHists(promptElPThistsMini+nonPromptElPThistsMini,'El Pt spectra (miniIso)')
    canv.Write()

    promptElPThists = getSelection(leptree,promptcut,relIsoCut,'Prompt El',true,'lepPt')
    nonPromptElPThists = getSelection(leptree,nonpromptcut,relIsoCut,'NonPrompt El',true,'lepPt')

    promptElPThistsMini = getSelection(leptree,promptcut,miniIsoCut,'Prompt El MiniIso',true,'lepPt')
    nonPromptElPThistsMini = getSelection(leptree,nonpromptcut,miniIsoCut,'NonPrompt El MiniIso',true,'lepPt')

    promptElPTeffHist = getEffPlot(promptElPThists[0],promptElPThists[1],'Prompt El RelIso Efficiency for Pt')
    nonPromptElPTeffHist = getEffPlot(nonPromptElPThists[0],nonPromptElPThists[1],'Non Prompt El RelIso Efficiency for Pt')

    promptElPTeffHistMini = getEffPlot(promptElPThistsMini[0],promptElPThistsMini[1],'Prompt El miniIso Efficiency for Pt')
    nonPromptElPTeffHistMini = getEffPlot(nonPromptElPThistsMini[0],nonPromptElPThistsMini[1],'Non Prompt El miniIso Efficiency for Pt')


    canv = plotHists([promptElPTeffHist,nonPromptElPTeffHist],'Electron Pt Eff RelIso')
    canv.Write()

    canv = plotHists([promptElPTeffHist,nonPromptElPTeffHist,promptElPTeffHistMini,nonPromptElPTeffHistMini],'Electron Pt Eff MiniIso')
    canv.Write()

    canv = plotHists([nonPromptElPTeffHist,nonPromptElPTeffHistMini],'Electron Pt Eff AllIso nonprompt')
    canv.Write()

    canv = plotHists([promptElPTeffHist,promptElPTeffHistMini],'Electron Pt Eff AllIso prompt')
    canv.Write()

    #canv = plotHists([promptElPTeffHistMini,nonPromptElPTeffHistMini],'Electron Pt Eff')
    #promptHists[0].DrawNormalized('same')
    #canv.Write()


    tfile.Close()
    outfile.Close()
