#!/usr/bin/python

import sys
sys.argv.append( '-b' )

from ROOT import *

def getLegend():
    leg = TLegend(0.63,0.525,0.87,0.875)
    leg.SetBorderSize(1)
    leg.SetTextFont(62)
    leg.SetTextSize(0.03321678)
    leg.SetLineColor(1)
    leg.SetLineStyle(1)
    leg.SetLineWidth(1)
    leg.SetFillColor(0)
    leg.SetFillStyle(1001)

    return leg

def makeEffPlot(hBase, hSelect,title = 'Efficiency'):

    hEff = hSelect.Clone()
    hEff.Divide(hBase)

    canv = TCanvas(title,title,800,600)
#    canv.SetLogy()

    hEff.Draw()

    SetOwnership( hEff, 0 )
    SetOwnership( canv, 0 )

    print 'Efficiency', hEff.GetEntries()
    return canv


def getSelection(tree,basecut,selcut,title = 'Efficiency'):

    hBase = TH1F (title+'_base',title+'_base',100,0,500);
    hSelect = TH1F (title+'_sel',title+'_sel',100,0,500);

    tree.Draw('lepPt >> '+hBase.GetName(),basecut)
    tree.Draw('lepPt >> '+hSelect.GetName(),basecut+' && '+selcut)

    hBase.SetStats(0)
    hSelect.SetStats(0)

    print hBase.GetEntries(), hSelect.GetEntries()
    '''
#    print hBase.GetName(), hSelect.GetName()
#    print hBase.GetTitle(), hSelect.GetTitle()
'''

    return [hBase,hSelect]

def plotHists(histList):

    dosame = ''

    canv = TCanvas('c'+histList[0].GetName(),histList[0].GetTitle(),800,600)

    leg = getLegend()

    for indx,hist in enumerate(histList):
        hist.Draw(dosame)

        leg.AddEntry(hist,hist.GetTitle(),'l')
        SetOwnership( hist, 0 )

        hist.SetLineColor(indx+1)
        hist.SetLineWidth(2)

        if dosame == '': dosame = 'same'

    leg.Draw()

    SetOwnership( canv, 0 )
    SetOwnership( leg, 0 )

    return canv

# first argument is '-b' == for batch mode
#    print sys.argv

if len(sys.argv) > 2:
    infile = sys.argv[1]

tfile  = TFile(infile, "READ")
outfile = TFile('plot_'+infile, "RECREATE")

if not tfile:
    exit(0)

#histList = [ 'GenElPtMatchEff','GenElPtNonMatchEff' ]

leptree = tfile.Get('Leptons')
print leptree.GetEntries()

#canv = makeEffPlot(leptree,'pdgID == 11 && match && prompt && passMVA','passIso')

# Iso efficiency
hists = getSelection(leptree,'pdgID == 11 && match && prompt && passMVA','passIso','Iso')
canv = makeEffPlot(hists[0],hists[1],'Iso Efficiency')
canv.Write()

canv = plotHists(hists)
canv.Write()


tfile.Close()
outfile.Close()
