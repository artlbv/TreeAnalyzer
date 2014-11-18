#!/usr/bin/python

import sys
from ROOT import *
from array import array
#gROOT.LoadMacro("tdrstyle.C")
#setTDRStyle()

#gROOT.LoadMacro("CMS_lumi_v2.C")
#dirName='FatJetNov4/'
dirName=''
njetcut=str(6)
nbjetcut=str(1)
dphicut=str(2)
f = TFile("plots_allsig_njet"+njetcut+"_bjet"+nbjetcut+"_dphi"+dphicut+"_bins.root", "recreate")

global h_ttbar,h_wjets,h_qcd,h_1200_800,h_1500_100,h_800_450,h_1300_100

def get(file,what,Lfac,lineCol,lineWid,lineSty,fillCol,reb=0):
	
	h = file.Get(what)
	h.SetLineColor(lineCol)
	h.SetLineWidth(lineWid)
	h.SetLineStyle(lineSty)
	h.SetFillColor(fillCol)
	h.Scale(Lfac)
	if reb!=0: h.Rebin(reb)
	return h.Clone()

def plot(ana,histos,h_legend,minY,maxY,minX,maxX,whatX,whatY,c1=0,all=1):
	global stack,leg,dum
	print ana
	#prepare the layou
	if c1==0:
		c1=TCanvas('c1','',800,600)
	c1.SetLogy() 
	c1.SetTicky() 
	c1.SetTickx() 
        c1.SetBottomMargin(0.1306294);
	dum = h_ttbar.Clone()
	dum.Reset()
	dum.SetMinimum(minY)
	dum.SetMaximum(maxY)
	dum.GetXaxis().SetRangeUser(minX,maxX)
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

        tex = TLatex(0.90,0.93,"13 TeV, 1 fb^{-1}, 50ns 40 PU")
        tex.SetNDC()
        tex.SetTextAlign(31)
        tex.SetTextFont(42)
        tex.SetTextSize(0.048)
        tex.SetLineWidth(2)
        tex.Draw()
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
	leg.AddEntry(h_1300_100,'T1tttt 1300 100','l')


	if all:
		# bgrd stack
		stack = THStack('stack','')
		N=len(histos)
		for i in range(N):
			h=histos[i][0]
			stack.Add(h)
		for i in range(N-1,-1,-1):
			h=histos[i][0]
			t=h_legend[h]
			leg.AddEntry(h,t,'f')
		# draw bgrds + signals
		stack.Draw('samehist')

       	h_1200_800.Draw('samehist')
	h_1500_100.Draw('samehist')                                                                               
        h_800_450.Draw('samehist')                                                                              
        h_1300_100.Draw('samehist')   

	leg.Draw()
	c1.RedrawAxis()
	c1.SetName(what+'_njet'+njetcut+'_bjet'+nbjetcut+'_dphi'+dphicut+'_'+ana)
	#c1.SaveAs(dirName+'/'+tev+'_'+pu+'_'+what+'_'+ana+'.pdf')
	f.cd()
	c1.Write()



ana='SingleS_P+DelphMET'
tev='14TeV'	
# scale factor - histos are for 300/fb
Lfac=1.0
#base of filename
base=dirName+'CMG_MC'
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
quantity=['vhMJ','vhMJ_lep','vhHT','vhST','vhNjet','vhNbjet', 'vhNFatJet','vhDPhi']
cutstart = [0,       0,       0,      0,       0,         0,      0,           0]
cutend = [  2000,    2000,    3000,   2000,    8,         6,      6,         31]
cutstep=[   100,     100,     200,    200,     1,         1,      1,         1]
binsize=[   50,      50,      50,     50,      1,         1,      1,         0.1]
cutsShort = ["one"]

label=['MJ (GeV)','MJ (inc. lepton) (GeV)','H_{T} (GeV)','S_{T} (GeV)','Number of jets','Number of b-jets', 'Number of FatJets pT > 120 GeV', 'DPhi']
ymax=[2000,2000,4000,2000,2000,20,20,10,10,10,1000,1,1500,3.2,700,500,500,500]

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
		reb = 5
		if what == 'vhNjet' or what == 'vhNbjet' or what == 'vhNFatJet' or what == 'vhDPhi':
			reb = 0
	
		location = "Bins/Njet_"+njetcut+"/bjet_"+nbjetcut+"-bjet_30/HT_500/"
		name = what+"_njet"+njetcut+"_bjet"+nbjetcut+"-bjet30_HT500_Dphi"+dphicut+"_ST0-ST4000" 
		hname =  location + name
		print hname

		h_ttbar = get(file_TTbar, hname,Lfac,1,1,1,kAzure-4,reb)
		h_wjets = get(file_WJets, hname,Lfac,1,1,1,kViolet+5,reb)
		h_qcd = get(file_QCD, hname,Lfac,1,1,1,kCyan-6,reb)
		h_1200_800  = get(file_1200_800, hname,Lfac,kBlack,2,1,0,reb)
		h_1500_100  = get(file_1500_100, hname,Lfac,kMagenta,2,1,0,reb)
		h_800_450  = get(file_800_450, hname,Lfac,2,2,1,0,reb)
		h_1300_100  = get(file_1300_100, hname,Lfac,4,2,1,0,reb)

		h_ent={}
		h_leg={} # entries in 
                h_ent[h_ttbar]=h_ttbar.Integral(0,5000)
                h_leg[h_ttbar]='t#bar{t}'
                h_ent[h_wjets]=h_wjets.Integral(0,5000)
                h_leg[h_wjets]='W + jets'
                h_ent[h_qcd]=h_qcd.Integral(0,5000)
                h_leg[h_qcd]='QCD'
		max = h_ttbar.GetMaximum()+h_wjets.GetMaximum()+h_qcd.GetMaximum()
                max = max + 0.2*max
		import operator
		sorted_h = sorted(h_ent.iteritems(), key=operator.itemgetter(1))
		plot(cutsShort[k]+'_'+str(k),sorted_h,h_leg,0.01,max,0,ymax[l],label[l],'evts / bin',c1,1)
		xlist=[]
		ylist_1200_800=[]
		ylist_1300_100=[]
		ylist_1500_100=[]
		ylist_800_450=[]
		for cut in range(cutstart[l],cutend[l],cutstep[l]):
			if what == 'vhDPhi':
				cut = cut/10.0
			binstart = int (cut / binsize[l] + 1)
			binend = int ((cut+cutstep[l]) / binsize[l]) # 5000
			
			n_ttbar = h_ttbar.Integral(binstart,binend)
			n_wjets = h_wjets.Integral(binstart,binend)
			n_qcd = h_qcd.Integral(binstart,binend)
			n_bkg = n_ttbar + n_wjets + n_qcd

			n_1200_800 = h_1200_800.Integral(binstart,binend)
			n_1500_100 = h_1500_100.Integral(binstart,binend)
			n_800_450 = h_800_450.Integral(binstart,binend)
			n_1300_100 = h_1300_100.Integral(binstart,binend)
			n_sig_over_sqrtbkg_1200_800 = 0
			n_sig_over_sqrtbkg_1500_100 = 0
			n_sig_over_sqrtbkg_800_450 = 0
			n_sig_over_sqrtbkg_1300_100 = 0
			if ( n_bkg != 0):
				"""n_sig_over_sqrtbkg_1200_800 = n_1200_800 / (sqrt( n_bkg ) + 1)/ 0.085641 
				n_sig_over_sqrtbkg_1500_100 = n_1500_100 / (sqrt( n_bkg ) + 1)/ 0.0141903 
				n_sig_over_sqrtbkg_800_450 = n_800_450 / (sqrt( n_bkg ) + 1) / 1.489 
				n_sig_over_sqrtbkg_1300_100 = n_1300_100 / (sqrt( n_bkg ) + 1) / 0.044"""
				n_sig_over_sqrtbkg_1200_800 = n_1200_800 / (sqrt( n_bkg + n_1200_800 + 0.1*n_1200_800*0.1*n_1200_800+ 0.15*n_bkg*0.15*n_bkg) + 3/2) 
				n_sig_over_sqrtbkg_1500_100 = n_1500_100 / (sqrt( n_bkg + n_1500_100 + 0.1*n_1500_100*0.1*n_1500_100+ 0.15*n_bkg*0.15*n_bkg) + 3/2)
				n_sig_over_sqrtbkg_800_450 = n_800_450 / (sqrt( n_bkg + n_800_450 + 0.1*n_800_450*0.1*n_800_450+ 0.15*n_bkg*0.15*n_bkg) + 3/2)
				n_sig_over_sqrtbkg_1300_100 = n_1300_100 / (sqrt( n_bkg +n_1300_100 + 0.1*n_1300_100*0.1*n_1300_100+ 0.15*n_bkg*0.15*n_bkg) + 3/2)
			print "%.2f %.2f %.2f %.2f %.2f %.2f " % (cut, n_bkg, n_1200_800,n_1500_100, n_800_450,n_1300_100)
			print "%.2f %.2f %.2f %.2f %.2f %.2f " % (cut, n_bkg, n_sig_over_sqrtbkg_1200_800,n_sig_over_sqrtbkg_1500_100, n_sig_over_sqrtbkg_800_450,n_sig_over_sqrtbkg_1300_100)
			xlist.append(cut)
			ylist_1200_800.append(n_sig_over_sqrtbkg_1200_800)
			ylist_1300_100.append(n_sig_over_sqrtbkg_1300_100)
			ylist_1500_100.append(n_sig_over_sqrtbkg_1500_100)
			ylist_800_450.append(n_sig_over_sqrtbkg_800_450)
			print "---------------------"
		
		g_1200_800 = TGraph(len(xlist), array('d', xlist),array('d', ylist_1200_800)) 
		g_1300_100 = TGraph(len(xlist), array('d', xlist),array('d', ylist_1300_100)) 
		g_1500_100 = TGraph(len(xlist), array('d', xlist),array('d', ylist_1500_100)) 
		g_800_450 = TGraph(len(xlist), array('d', xlist),array('d', ylist_800_450)) 
		f.cd()
		g_800_450.Write(name+"_800_450")
		g_1300_100.Write(name+"_1300_100")
		g_1500_100.Write(name+"_1500_100")
		g_1200_800.Write(name+"_1200_800")
#----------------------------------------------------------------------------


	l=l+1
		
