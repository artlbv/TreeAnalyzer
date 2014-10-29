#include "../interface/NtupleTools3.h"
#include "../interface/Objects.h" 
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TFile.h>
#include "fastjet/ClusterSequence.hh"
#include <iostream>
using namespace fastjet;
using namespace std;


int main (){
  TString list = "/afs/desy.de/user/s/safarzad/dust/13TeV/ISOTrck/T1tttt/SMS_T1tttt_2J_mGl1300_mLSP100_PU_S14_POSTLS170/treeProducerSusySingleLepton/ 2 ";
  TString outname = "T1tttt_1300_100";
  TObjArray* arr = list.Tokenize(" ");
  int size=arr->GetEntries();
  if(size%2!=0) {
    cout<<"unbalance file/weight list: "<<list<<endl;
    exit(0);
  }
  vector<TString> files;
  vector<double> weights;
  for(int i=0;i<size;i+=2){
    files.push_back( arr->At(i)->GetName() );
    weights.push_back( atof( arr->At(i+1)->GetName() ) );
  }
  


  EasyChain* tree = new EasyChain("treeProducerSusySingleLepton");
  for(unsigned i=0;i<files.size();i++){
    tree->AddSmartW(files[i],weights[i]);
    cout<<"add: "<<files[i]<<" "<<weights[i]<<endl;
  }
  int Nevents=tree->GetEntries();
  cout<<">>>>>>>>>>>>>>>>>>>>>>> total number of events:\t" << Nevents <<endl;

  // CutFlow variables
  const int CutNumb = 2;
  const char * CutList[CutNumb] = {"noCut",
				   "== 1 lepton, >= 2 jets",
  };
  //top
  double CFCounter[CutNumb];
  int iCFCounter[CutNumb];
  for (int i=0;i < CutNumb; i++){
    CFCounter[i] = 0;
    iCFCounter[i] = 0;
  }
  TH1F* CutFlow= new TH1F("CutFlow","Cut Flow",CutNumb,0.5,CutNumb+0.5);
  TH1F *hHT[CutNumb];
  TH1F *hST[CutNumb];
  TH1F *hMJ[CutNumb];
  TH1F *h0JetpT[CutNumb];
  TH1F *h1JetpT[CutNumb];
  TH1F *h2JetpT[CutNumb];
  TH1F *h3JetpT[CutNumb];
  TH1I *hnJet[CutNumb];
  TH1F *hnBJet[CutNumb];
  TH1F *hnMu[CutNumb];
  TH1F *hnEl[CutNumb];
  TH1F *hMupt[CutNumb];
  TH1F *hElpt[CutNumb];
  TH1F *hnLep[CutNumb];
  TH1F *hLeppt[CutNumb];
  TH1F *hLepeta[CutNumb];
  TH1F *hCentral[CutNumb];
  TH1F *hMET[CutNumb];
  TH1F *hdPhi[CutNumb];
  TH1F *hmT[CutNumb];
  TH1F *hmT2W[CutNumb];
  for(int cj = 0; cj < CutNumb; cj++)
    {
      CutFlow->GetXaxis()->SetBinLabel(cj+1,CutList[cj]);
      TString cutName=CutList[cj];
      TString nCut; nCut.Form("%d",cj);
      hHT[cj] = new TH1F ("HT_"+nCut,"HT "+cutName,200,0.0,4000.0);
      hHT[cj]->Sumw2();
      hST[cj] = new TH1F ("ST_"+nCut,"ST "+cutName,200,0.0,4000.0);
      hST[cj]->Sumw2();
      hMJ[cj] = new TH1F ("MJ_"+nCut,"MJ "+cutName,200,0.0,4000.0);
      hMJ[cj]->Sumw2();
      h0JetpT[cj] = new TH1F ("0JetpT_"+nCut,"0JetpT "+cutName,200,0.0,2000.0);
      h0JetpT[cj]->Sumw2();
      h1JetpT[cj] = new TH1F ("1JetpT_"+nCut,"1JetpT "+cutName,200,0.0,2000.0);
      h1JetpT[cj]->Sumw2();
      h2JetpT[cj] = new TH1F ("2JetpT_"+nCut,"2JetpT "+cutName,200,0.0,2000.0);
      h2JetpT[cj]->Sumw2();
      h3JetpT[cj] = new TH1F ("3JetpT_"+nCut,"3JetpT "+cutName,200,0.0,2000.0);
      h3JetpT[cj]->Sumw2();
      hnJet[cj] = new TH1I ("nJet_"+nCut,"nJet "+cutName,20,0,20);
      hnJet[cj]->Sumw2();
      hnBJet[cj] = new TH1F ("nBJet_"+nCut,"nBJet "+cutName,20,0,20);
      hnBJet[cj]->Sumw2();
      hnMu[cj] = new TH1F ("nMu_"+nCut,"nMu "+cutName,10,0,10);
      hnMu[cj]->Sumw2();
      hnEl[cj] = new TH1F ("nEl_"+nCut,"nEl "+cutName,10,0,10);
      hnEl[cj]->Sumw2();
      hnLep[cj] = new TH1F ("nLep_"+nCut,"nLep "+cutName,10,0,10);
      hnLep[cj]->Sumw2();
      hLeppt[cj] = new TH1F ("LeppT_"+nCut,"Lep pT "+cutName,100,0,1000);
      hLeppt[cj]->Sumw2();
      hElpt[cj] = new TH1F ("ElpT_"+nCut,"El pT "+cutName,100,0,1000);
      hElpt[cj]->Sumw2();
      hMupt[cj] = new TH1F ("MupT_"+nCut,"Mu pT "+cutName,100,0,1000);
      hMupt[cj]->Sumw2();
      hLepeta[cj] = new TH1F ("Lepeta_"+nCut,"Lep eta "+cutName,80,-4,4);
      hLepeta[cj]->Sumw2();
      hCentral[cj] = new TH1F ("Central_"+nCut," Central "+cutName,100,0,1);
      hCentral[cj]->Sumw2();
      hMET[cj] = new TH1F("MET_"+nCut,"MET "+cutName,200.0,0.0,4000.0);
      hMET[cj]->Sumw2();
      hdPhi[cj] = new TH1F("dPhi_"+nCut,"dPhi "+cutName,80,0,3.2);
      hdPhi[cj]->Sumw2();
      hmT[cj] = new TH1F("mT_"+nCut,"mT "+cutName,100,0,1000);
      hmT[cj]->Sumw2();
      hmT2W[cj] = new TH1F("mT2W_"+nCut,"mT2W "+cutName,70,0,700);
      hmT2W[cj]->Sumw2();
    }

  bool Debug = false;
  Float_t goodElPt = 20.0;
  Float_t goodMuPt = 20.0;
  Float_t goodLepPt = 20.0;
  Float_t vetoLepPt = 10.0;
  Float_t goodJetPt = 40.0;
  Float_t goodEta = 2.4;
  int arrayN = 500;
  Float_t Jet_pt[arrayN];
  Float_t Jet_eta[arrayN];
  Float_t Jet_phi[arrayN];
  Float_t Jet_mass[arrayN];
  Float_t Jet_btagCSV[arrayN];
  Float_t LepGood_pt[arrayN];
  Float_t LepGood_eta[arrayN];
  Float_t LepGood_phi[arrayN];
  Float_t LepGood_mass[arrayN];
  Float_t LepGood_relIso03[arrayN];
  Int_t LepGood_pdgId[arrayN];
  vector<TLorentzVector> goodJet;
  vector<TLorentzVector> goodLep;
  vector<TLorentzVector> goodEl;
  vector<TLorentzVector> goodMu;
  vector<TLorentzVector> vetoLep;

  for(int entry=0; entry < Nevents/*min(1000000,Nevents)*/; entry+=1){
    goodJet.clear();
    goodLep.clear(); vetoLep.clear();
    goodEl.clear(); goodMu.clear();
  Float_t fw = tree->GetEntryW(entry);
  progressT();
  //lumi calcualtion done in runAnalyzer.py (fb and pb)
  float EvWeight = 1;
  EvWeight *= fw ;


    int nJet = tree->Get(nJet,"nJet");
    tree->Get(Jet_pt[0],"Jet_pt");
    tree->Get(Jet_eta[0],"Jet_eta");
    tree->Get(Jet_phi[0],"Jet_phi");
    tree->Get(Jet_mass[0],"Jet_mass");

    //GetLeptons(tree);

    /*    int nLep = tree->Get(nLep,"nLepGood");
    int nLepGood = 0;
    int nMuGood = 0;
    int nElGood = 0;
    tree->Get(LepGood_pt[0],"LepGood_pt");
    tree->Get(LepGood_eta[0],"LepGood_eta");
    tree->Get(LepGood_phi[0],"LepGood_phi");
    tree->Get(LepGood_mass[0],"LepGood_mass");
    tree->Get(LepGood_relIso03[0],"LepGood_relIso03");
    tree->Get(LepGood_pdgId[0],"LepGood_pdgId");
    Float_t MET = tree->Get(MET,"met_pt");
    ///at some point export this as functions
    for(int ilep=0;ilep<nLep;ilep++)
      {
	TLorentzVector dummyLep;
	dummyLep.SetPtEtaPhiM(LepGood_pt[ilep],LepGood_eta[ilep],LepGood_phi[ilep],LepGood_mass[ilep]);
	if(dummyLep.Pt() > vetoLepPt && fabs(dummyLep.Eta()) < goodEta){
	  vetoLep.push_back(dummyLep);
	  if( dummyLep.Pt() > goodLepPt && LepGood_relIso03[ilep] < 0.15){ //TODO: need to adjust isolation
	    goodLep.push_back(dummyLep);
	    if(abs(LepGood_pdgId[ilep]) == 11){
	      goodEl.push_back(dummyLep);
	      nElGood++;
	    }
	    if(abs(LepGood_pdgId[ilep]) == 13){
	      goodMu.push_back(dummyLep);
	      nMuGood++;
	    }
	    nLepGood++;
	  }
	}
      }
    */
    vector<PseudoJet> particles;
    // an event with three particles:   px    py  pz      E
    for(int ijet=0;ijet<nJet;ijet++)
      {
	if (Jet_pt[ijet] > 40 && fabs(Jet_eta[ijet]) < 2.4){
	  TLorentzVector dummyJet;
	  dummyJet.SetPtEtaPhiM(Jet_pt[ijet],Jet_eta[ijet],Jet_phi[ijet],Jet_mass[ijet]);
	  particles.push_back ( PseudoJet( dummyJet.Px(), dummyJet.Py(), dummyJet.Pz(), dummyJet.E()));
	}
      }

    
    // choose a jet definition
    double R = 1.2;
    JetDefinition jet_def(antikt_algorithm, R);
    
    // run the clustering, extract the jets
    ClusterSequence cs(particles, jet_def);
    vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets());
    
    // print out some infos
    cout << "Clustering with " << jet_def.description() << endl;
    float fMJ=0;    
    // print the jets
    cout <<   "        pt y phi" << endl;
    for (unsigned i = 0; i < jets.size(); i++) {
      fMJ = fMJ + jets[i].m(); 
      cout << "jet " << i << ": "<< jets[i].pt() << " " 
	   << jets[i].rap() << " " << jets[i].phi() << endl;
      vector<PseudoJet> constituents = jets[i].constituents();
      for (unsigned j = 0; j < constituents.size(); j++) {
	cout << "    constituent " << j << "'s pt: " << constituents[j].pt()
	     << endl;
      }
    }
    hMJ[0]->Fill(fMJ,EvWeight);
    
}

  ofstream tfile;
  TString textfilename = "CMG_cutflow_"+outname+".txt";
  tfile.open(textfilename);
  tfile << "########################################" << endl;
  tfile << "Cut efficiency numbers:" << endl;
  for(int ci = 0; ci < CutNumb; ci++)
    {
      tfile << "After cut " << CutList[ci] << "\t\t"
	    << CFCounter[ci] << "\t events left\t"<< iCFCounter[ci] <<" cnt\t"<< endl;
      CutFlow->SetBinContent(1+ci,CFCounter[ci]);
    }
  TFile * outf;
  TString rootfilename = "CMG_"+outname+"_his.root";
  outf = new TFile(rootfilename,"RECREATE");
  for(int cj = 0; cj < CutNumb; cj++)
    {
      hHT[cj]->Write();
      hST[cj]->Write();
      hMJ[cj]->Write();
      h0JetpT[cj]->Write();
      h1JetpT[cj]->Write();
      h2JetpT[cj]->Write();
      h3JetpT[cj]->Write();
      hnJet[cj]->Write();
      hnBJet[cj]->Write();
      hnMu[cj]->Write();
      hnEl[cj]->Write();
      hMupt[cj]->Write();
      hElpt[cj]->Write();
      hnLep[cj]->Write();
      hLeppt[cj]->Write();
      hLepeta[cj]->Write();
      hCentral[cj]->Write();
      hMET[cj]->Write();
      hdPhi[cj]->Write();
      hmT[cj]->Write();
      hmT2W[cj]->Write();
    }
}
