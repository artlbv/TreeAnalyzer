#!/usr/bin/python

import sys
from ROOT import *

#gROOT.LoadMacro("tdrstyle.C")
#setTDRStyle()

#gROOT.LoadMacro("CMS_lumi_v2.C")
dirName='September18'
f = TFile("ProfilePlots.root", "recreate")

global h_ttbar,h_wjets,h_qcd,h_1200_800,h_1500_100,h_800_450,h_1300_100

def getProfile(file,what,Lfac,lineCol,lineWid,lineSty,fillCol,reb=0):
	h2D = file.Get(what)
	h = h2D.ProfileX()
	h.SetErrorOption("s")
	h.Rebin(7)

	h.SetLineColor(lineCol)
	h.SetLineWidth(lineWid)
	h.SetLineStyle(lineSty)
	h.SetFillColor(fillCol)
	h.Scale(Lfac)
	if reb!=0: h.Rebin(reb)
	return h.Clone()

def get2D(file,what,Lfac,lineCol,lineWid,lineSty,fillCol,reb=0):
	h2D = file.Get(what)
	h.Scale(Lfac)
	return h.Clone()

def plot(ana,histos,h_legend,minY,maxY,minX,maxX,whatX,whatY,c1=0,all=1):
	global stack,leg,dum
	print ana
	#prepare the layou
	if c1==0:
		c1=TCanvas('c1','',800,600)

        c1.SetBottomMargin(0.1306294);
	dum = h_ttbar.Clone()
	dum.Reset()

	dum.GetXaxis().SetRangeUser(minX,maxX)
	dum.GetYaxis().SetRangeUser(minX,maxX)
	dum.GetXaxis().SetTitleSize(0.06)
	dum.GetYaxis().SetTitleSize(0.06)
	dum.GetXaxis().SetLabelSize(0.05)
	dum.GetYaxis().SetLabelSize(0.05)
        dum.GetXaxis().SetTitleOffset(0.95)
        #dum.GetYaxis().SetTitleOffset(0.85)

	dum.SetXTitle(whatX)
	dum.SetYTitle(whatY)
	dum.SetTitle('')
	dum.SetStats(0)
	dum.Draw()

        tex = TLatex(0.90,0.93,"14 TeV, 3000 fb^{-1}, PU = 140")
        tex.SetNDC()
        tex.SetTextAlign(31)
        tex.SetTextFont(42)
        tex.SetTextSize(0.048)
        tex.SetLineWidth(2)
        #tex.Draw()
        #CMS_lumi_v2( c1, 14, 11 )
        tex1 = TLatex(0.15,0.89,"CMS Phase II Simulation")
        tex1.SetNDC()
        tex1.SetTextAlign(13)
        tex1.SetTextFont(61)
        tex1.SetTextSize(0.045)
        tex1.SetLineWidth(2)
        #tex1.Draw()
	# a legend

	leg = TLegend(0.63,0.525,0.87,0.875) #for 33 TeV (0.65,0.65,0.9,0.9)
        leg.SetBorderSize(1)
        leg.SetTextFont(62)
        leg.SetTextSize(0.03321678)
        leg.SetLineColor(0)
        leg.SetLineStyle(1)
        leg.SetLineWidth(1)
        leg.SetFillColor(0)
        leg.SetFillStyle(1001)


	leg.AddEntry(h_1200_800,'T1tttt 1200 800','l')
	leg.AddEntry(h_1500_100,'T1tttt 1500 100','l')
	leg.AddEntry(h_800_450,'T1tttt 800 450','l')
	leg.AddEntry(h_1300_100,'T1tttt 1300100','l')


	if all:
		# bgrd stack
		stack = THStack('stack','')
		N=len(histos)
		for i in range(N):
			h=histos[i][0]
			h.Draw("e2sameL")
			hfordraw = h.Clone()
			hfordraw.SetFillColor(kWhite)
			hfordraw.Draw("histsame")
		for i in range(N-1,-1,-1):
			h=histos[i][0]
			t=h_legend[h]
			leg.AddEntry(h,t,'f')
		# draw bgrds + signals
		stack.Draw('samehist')

       	h_1200_800.Draw('Csame')
	h_1500_100.Draw('Csame')                                                                               
        h_800_450.Draw('Csame')                                                                              
        h_1300_100.Draw('Csame')   

	leg.Draw()
	c1.RedrawAxis()
	c1.SetName(tev+'_'+pu+'_'+what+'_'+ana)
	#c1.SaveAs(dirName+'/'+tev+'_'+pu+'_'+what+'_'+ana+'.pdf')
	f.cd()
	c1.Write()



ana='SingleS_P+DelphMET'
tev='14TeV'	
# scale factor - histos are for 300/fb
Lfac=1.0
#base of filename
base='CMG_MC'
#
###
## pileup scenario
#pu  ='NoPU' 
#pu  ='50PU' 
pu ='140PU'
W = 800
H = 600
H_ref = 600
W_ref = 800
T = 0.08*H_ref
B = 0.12*H_ref
L = 0.12*W_ref
R = 0.04*W_ref
c1 = TCanvas('c1','c1',10,10,W,H);
c1.SetFillColor(0);
c1.SetBorderMode(0);
c1.SetFrameFillStyle(0);
c1.SetFrameBorderMode(0);
c1.SetLeftMargin( L/W );
c1.SetRightMargin( R/W );
c1.SetTopMargin( T/H );
c1.SetBottomMargin( B/H );
c1.SetTickx(0);
c1.SetTicky(0);
#c1=TCanvas('c1','',800,600)

#--------------------------------------------------------------------------------------------------------------------------------------------------------

########## preselection
#quantity=['HT','0JetpT','1JetpT','2JetpT','3JetpT','nJet','nBJet','nMu','nEl','nLep','LeppT','Central','RawMET','dPhi','mT','mT2W']
quantity=['vhMJ_vs_HT','vhMJ_lep_vs_HT']
cutsShort = ["one"]

label=['MJ vs. HT (GeV)','MJ (inc. lepton) vs. HT (GeV)']
ymax=[4000,2000,2000,2000,2000,20,20,10,10,10,1000,1,1500,3.2,700,500]

l=0

file_TTbar = TFile.Open(base+'_TTbar_his.root')
file_WJets = TFile.Open(base+'_WJets_his.root')
file_QCD = TFile.Open(base+'_QCD_his.root')
file_1200_800  = TFile.Open(base+'_T1tttt_1200_800_his.root')
file_1500_100 = TFile.Open(base+'_T1tttt_1500_100_his.root')                                                          
file_800_450 = TFile.Open(base+'_T1tttt_800_450_his.root')                                                         
file_1300_100 = TFile.Open(base+'_T1tttt_1300_100_his.root')




for what in quantity:
	for k in range (0,1):
		reb = 0
		if what == 'nLep' or what == 'nJet':
			reb = 0
	
		hname = "Bins/Njet_6/bjet_1-bjet_30/HT_500/"+what+"_njet6_bjet1-bjet30_HT500_Dphi2_ST250-ST4000" 
		print hname
		h_ttbar = getProfile(file_TTbar, hname,Lfac,1,2,1,kAzure-4,reb)
		h_wjets = getProfile(file_WJets, hname,Lfac,1,2,1,kViolet+5,reb)
		h_qcd = getProfile(file_QCD, hname,Lfac,1,2,1,kCyan-6,reb)
		h_1200_800  = getProfile(file_1200_800, hname,Lfac,kBlack,1,1,0,reb)
		h_1500_100  = getProfile(file_1500_100, hname,Lfac,kSpring-5,1,1,0,reb)
		h_800_450  = getProfile(file_800_450, hname,Lfac,2,1,1,0,reb)
		h_1300_100  = getProfile(file_1300_100, hname,Lfac,4,1,1,0,reb)
		h_ent={}
		h_leg={} # entries in 
                #h_ent[h_ttbar]= 1.0
                #h_leg[h_ttbar]='t#bar{t}'
                h_ent[h_wjets]= 1.0
                h_leg[h_wjets]='W+jets'
		import operator
		sorted_h = sorted(h_ent.iteritems(), key=operator.itemgetter(1))
		plot(cutsShort[k]+'_'+str(k),sorted_h,h_leg,0.1,max,0,ymax[l],label[l],'evts / bin',c1,1)
			
	
		
	l=l+1
		
