//#include "../interface/NtupleTools3.h"
#include "../interface/ClassObjects.h" 
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include "fastjet/ClusterSequence.hh"
#include <iostream>
using namespace fastjet;
using namespace std;
  GetObjects Obj;   
const int CutNumb = 26; // number of Cuts
const char * CutList[CutNumb] = {"noCut",
				 "== 1 Mu", "6Jet","HT>500","ST>250",
				 "Nb>=1","Df<1","Df>1",
				 "Nb=1, 250<ST<350,Df<1","Nb =1, 250<ST<350,Df>1",
				 "Nb=1, 350<ST<450, Df<1","Nb =1, 350<ST<450,Df>1",
				 "Nb=1, ST>450, Df<1","Nb =1, ST>450,Df>1",
				 "Nb=2 ,250<ST<350, Df<1","Nb=2, 250<ST< 350,Df>1",
				 "Nb=2 ,350<ST<450,Df<1","Nb=2,350<ST<450,Df>1",
				 "Nb=2 ,ST>450, Df<1","Nb=2 ,ST>450,Df>1",
				 "Nb>2 250<ST<350 ,Df<1","Nb>2,250<ST<350,Df>1",
				 "Nb>2 350<ST<450,Df<1","Nb>2,350<ST<450, Df>1",
				 "Nb>2 ST>450,Df<1","Nb>2 ST>450,Df>1",
};
// define global hists
TH1F* CutFlow= new TH1F("CutFlow","Cut Flow",CutNumb,0.5,CutNumb+0.5);
TH1F *hHT[CutNumb];
TH1F *hST[CutNumb];
TH1F *hWpT[CutNumb];
TH1F *h0JetpT[CutNumb];
TH1F *h1JetpT[CutNumb];
TH1F *h2JetpT[CutNumb];
TH1F *h3JetpT[CutNumb];
TH1F *hnJet[CutNumb];
TH1F *hnBJet[CutNumb];
TH1F *hnMu[CutNumb];
TH1F *hnEl[CutNumb];
TH1F *hMupt[CutNumb];
TH1F *hnGenLep[CutNumb];
TH1F *hnGenTau[CutNumb];
TH1F *hnGenLepFromTau[CutNumb];
TH1F *hElpt[CutNumb];
TH1F *hnLep[CutNumb];
TH1F *hLeppt[CutNumb];
TH1F *hLepeta[CutNumb];
TH1F *hCentral[CutNumb];
TH1F *hMET[CutNumb];
TH1F *hdPhi[CutNumb];
TH1F *hdPhiWLep[CutNumb];
TH1F *hdPhiWgenLepC[CutNumb];
TH1F *hdPhiWgenLepS[CutNumb];
TH1F *hmT[CutNumb];
TH1F *hmT2W[CutNumb];
TH1F *h0BJetpT[CutNumb];
TH1F *h1BJetpT[CutNumb];
TH1F *h2BJetpT[CutNumb];
TH1F *h3BJetpT[CutNumb];
TH1F *hRS[CutNumb];
TH1F *hRC[CutNumb];
TH1F *hHTRS[CutNumb];
TH1F *hHTRC[CutNumb];
TH1F *hSTRS[CutNumb];
TH1F *hSTRC[CutNumb];
TH1F *hNJRS[CutNumb];
TH1F *hNJRC[CutNumb];
TH2F *hDfST[CutNumb];
TH2F *hDfHT[CutNumb];
TH2F *hHTST[CutNumb];
TH2F *hNJST[CutNumb];
TH2F *hMTMET[CutNumb];
TH2F *hDfDfGen[CutNumb];
// Set Up histograms and Cut Flow variables
void SetupHists(int CutNumber){
  double xbinNJ[5] ={3,5,6,7,20};
  double xbinNJn[3] ={3,6,12};
  double xbinHT[4] ={500,750,1000,3000};
  double xbinST[4] ={250,350,450,2000};
  for(int cj = 0; cj < CutNumber; cj++)
    {
      CutFlow->GetXaxis()->SetBinLabel(cj+1,CutList[cj]);
      TString cutName=CutList[cj];
      TString nCut;
      nCut.Form("%d",cj);
      hHT[cj] = new TH1F ("HT_"+nCut,"HT "+cutName,400,0.0,4000.0);
      // hHT[cj]->Sumw2();
      hST[cj] = new TH1F ("ST_"+nCut,"ST "+cutName,400,0.0,4000.0);
      // hST[cj]->Sumw2();
      h0JetpT[cj] = new TH1F ("0JetpT_"+nCut,"0JetpT "+cutName,200,0.0,2000.0);
      //h0JetpT[cj]->Sumw2();
      h1JetpT[cj] = new TH1F ("1JetpT_"+nCut,"1JetpT "+cutName,200,0.0,2000.0);
      //h1JetpT[cj]->Sumw2();
      h2JetpT[cj] = new TH1F ("2JetpT_"+nCut,"2JetpT "+cutName,200,0.0,2000.0);
      //h2JetpT[cj]->Sumw2();
      h3JetpT[cj] = new TH1F ("3JetpT_"+nCut,"3JetpT "+cutName,200,0.0,2000.0);
      //h3JetpT[cj]->Sumw2();
      h0BJetpT[cj] = new TH1F ("0BJetpT_"+nCut,"0BJetpT "+cutName,200,0.0,2000.0);
      //h0BJetpT[cj]->Sumw2();
      h1BJetpT[cj] = new TH1F ("1BJetpT_"+nCut,"1BJetpT "+cutName,200,0.0,2000.0);
      //h1BJetpT[cj]->Sumw2();
      h2BJetpT[cj] = new TH1F ("2BJetpT_"+nCut,"2BJetpT "+cutName,200,0.0,2000.0);
      //h2BJetpT[cj]->Sumw2();
      h3BJetpT[cj] = new TH1F ("3BJetpT_"+nCut,"3BJetpT "+cutName,200,0.0,2000.0);
      //h3BJetpT[cj]->Sumw2();
      hnJet[cj] = new TH1F ("nJet_"+nCut,"nJet "+cutName,20,0,20);
      //hnJet[cj]->Sumw2();
      hnBJet[cj] = new TH1F ("nBJet_"+nCut,"nBJet "+cutName,20,0,20);
      //hnBJet[cj]->Sumw2();
      hnMu[cj] = new TH1F ("nMu_"+nCut,"nMu "+cutName,10,0,10);
      //hnMu[cj]->Sumw2();
      hnEl[cj] = new TH1F ("nEl_"+nCut,"nEl "+cutName,10,0,10);
      //hnEl[cj]->Sumw2();
      hnLep[cj] = new TH1F ("nLep_"+nCut,"nLep "+cutName,10,0,10);
      //hnLep[cj]->Sumw2();
      hnGenLep[cj] = new TH1F ("nGenLep_"+nCut,"nGenLep "+cutName,10,0,10);
      //hnGenLep[cj]->Sumw2();
      hnGenTau[cj] = new TH1F ("nGenTau_"+nCut,"nGenTau "+cutName,10,0,10);
      //hnGenTau[cj]->Sumw2();
      hnGenLepFromTau[cj] = new TH1F ("nGenLepFromTau_"+nCut,"nGenLepFromTau "+cutName,10,0,10);
      //hnGenLepFromTau[cj]->Sumw2();
      hRS[cj] = new TH1F ("RS_"+nCut,"RS "+cutName,20,0,20);
      hRS[cj]->Sumw2();
      hRC[cj] = new TH1F ("RC_"+nCut,"RC "+cutName,20,0,20);
      hRC[cj]->Sumw2();
      hNJRS[cj] = new TH1F ("NJRS_"+nCut,"NJRS "+cutName,4,xbinNJ);
      hNJRS[cj]->Sumw2();
      hNJRC[cj] = new TH1F ("NJRC_"+nCut,"NJRC "+cutName,4,xbinNJ);
      hNJRC[cj]->Sumw2();
      hHTRS[cj] = new TH1F ("HTRS_"+nCut,"HTRS "+cutName,3,xbinHT);
      hHTRS[cj]->Sumw2();
      hHTRC[cj] = new TH1F ("HTRC_"+nCut,"HTRC "+cutName,3,xbinHT);
      hHTRC[cj]->Sumw2();
      hSTRS[cj] = new TH1F ("STRS_"+nCut,"STRS "+cutName,3,xbinST);
      hSTRS[cj]->Sumw2();
      hSTRC[cj] = new TH1F ("STRC_"+nCut,"STRC "+cutName,3,xbinST);
      hSTRC[cj]->Sumw2();
      hWpT[cj] = new TH1F ("WpT_"+nCut,"WpT "+cutName,200,0.0,2000.0);
      //hWpT[cj]->Sumw2();
      hLeppt[cj] = new TH1F ("LeppT_"+nCut,"Lep pT "+cutName,100,0,1000);
      //hLeppt[cj]->Sumw2();
      hElpt[cj] = new TH1F ("ElpT_"+nCut,"El pT "+cutName,100,0,1000);
      //hElpt[cj]->Sumw2();
      hMupt[cj] = new TH1F ("MupT_"+nCut,"Mu pT "+cutName,100,0,1000);
      //hMupt[cj]->Sumw2();
      hLepeta[cj] = new TH1F ("Lepeta_"+nCut,"Lep eta "+cutName,80,-4,4);
      //hLepeta[cj]->Sumw2();
      hCentral[cj] = new TH1F ("Central_"+nCut," Central "+cutName,100,0,1);
      hCentral[cj]->Sumw2();
      hMET[cj] = new TH1F("MET_"+nCut,"MET "+cutName,200.0,0.0,4000.0);
      //hMET[cj]->Sumw2();
      hdPhi[cj] = new TH1F("dPhi_"+nCut,"dPhi "+cutName,80,0,3.2);
      //hdPhi[cj]->Sumw2();
      hdPhiWLep[cj] = new TH1F("dPhiWLep_"+nCut,"dPhiWLep "+cutName,64,0.0,3.2);
      //hdPhiWLep[cj]->Sumw2();
      hdPhiWgenLepC[cj] = new TH1F("dPhiWgenLepC_"+nCut,"dPhiWGenLepC "+cutName,64,0.0,3.2);
      //hdPhiWgenLepC[cj]->Sumw2();
      hdPhiWgenLepS[cj] = new TH1F("dPhiWgenLepS_"+nCut,"dPhiWGenLepS "+cutName,64,0.0,3.2);
      //hdPhiWgenLepS[cj]->Sumw2();
      hmT[cj] = new TH1F("mT_"+nCut,"mT "+cutName,100,0,1000);
      //hmT[cj]->Sumw2();
      hmT2W[cj] = new TH1F("mT2W_"+nCut,"mT2W "+cutName,70,0,700);
      //hmT2W[cj]->Sumw2();
      hDfST[cj] = new TH2F ("DfST_"+nCut,"DfST "+cutName,200,0.0,2000.0,64,0.0,3.2);
      hDfST[cj]->Sumw2();
      hDfDfGen[cj] = new TH2F ("DfDfGen_"+nCut,"DfDfGen "+cutName,64,0.0,3.2,64,0.0,3.2);
      hDfDfGen[cj]->Sumw2();
      hNJST[cj] = new TH2F ("NJST_"+nCut,"NJST "+cutName,20,0,20,200,0.0,2000.0);
      hNJST[cj]->Sumw2();
      hDfHT[cj] = new TH2F ("DfHT_"+nCut,"DfHT "+cutName,300,0.0,3000.0,64,0.0,3.2);
      hDfHT[cj]->Sumw2();
      hMTMET[cj] = new TH2F ("MTMET_"+nCut,"MTMET "+cutName,200,0.0,2000.0,200,0.0,2000);
      hMTMET[cj]->Sumw2();
      hHTST[cj] = new TH2F ("HTST_"+nCut,"HTST "+cutName,300,0.0,3000.0,200,0.0,2000);
      hHTST[cj]->Sumw2();
    }
}

void FillMainHists(int CutIndex, float EvWeight, bool FillBJets = true){
  // cout << "Filling " << CutIndex << endl;
  hnJet[CutIndex]->Fill(Obj.nJetGood,EvWeight);
  hnLep[CutIndex]->Fill(Obj.nLepGood,EvWeight);
  hnMu[CutIndex]->Fill(Obj.nMuGood,EvWeight);
  hnEl[CutIndex]->Fill(Obj.nElGood,EvWeight);
  if (Obj.nJetGood > 0) h0JetpT[CutIndex]->Fill(Obj.goodJet[0].Pt(),EvWeight);
  if (Obj.nJetGood > 1) h1JetpT[CutIndex]->Fill(Obj.goodJet[1].Pt(),EvWeight);
  if (Obj.nJetGood > 2) h2JetpT[CutIndex]->Fill(Obj.goodJet[2].Pt(),EvWeight);
  if (Obj.nJetGood > 3) h3JetpT[CutIndex]->Fill(Obj.goodJet[3].Pt(),EvWeight);
  if(FillBJets){
    hnBJet[CutIndex]->Fill(Obj.nBJetGood,EvWeight);
    if (Obj.nBJetGood > 0) h0BJetpT[CutIndex]->Fill(Obj.goodBJet[0].Pt(),EvWeight);
    if (Obj.nBJetGood > 1) h1BJetpT[CutIndex]->Fill(Obj.goodBJet[1].Pt(),EvWeight);
    if (Obj.nBJetGood > 2) h2BJetpT[CutIndex]->Fill(Obj.goodBJet[2].Pt(),EvWeight);
    if (Obj.nBJetGood > 3) h3BJetpT[CutIndex]->Fill(Obj.goodBJet[3].Pt(),EvWeight);
  }
  if (Obj.nLepGood > 0) hLeppt[CutIndex]->Fill(Obj.goodLep[0].Pt(),EvWeight);
  if (Obj.nMuGood > 0) hMupt[CutIndex]->Fill(Obj.goodMu[0].Pt(),EvWeight);
  if (Obj.nElGood > 0) hElpt[CutIndex]->Fill(Obj.goodEl[0].Pt(),EvWeight);
  hMET[CutIndex]->Fill(Obj.MET.Pt(),EvWeight);
  hHT[CutIndex]->Fill(Obj.HT40,EvWeight);
  hST[CutIndex]->Fill(Obj.ST,EvWeight);
  hnGenLep[CutIndex]->Fill(Obj.nGenLep,EvWeight);
  hnGenTau[CutIndex]->Fill(Obj.nGenTau,EvWeight);
  hnGenLepFromTau[CutIndex]->Fill(Obj.nGenLepFromTau,EvWeight);
}



int main (int argc, char* argv[]){
  if (argc < 4) {
    cout<<"usage ./TreeAnalyzer_example [INPUTFOLDER] [XSEC * LUMI] [SAMPLENAME]"<<endl;
    return 1;
  }
 cout<<argc<<" "<<argv[1]<<" "<<argv[2]<<endl;
  //TString list = "/afs/desy.de/user/s/safarzad/dust/13TeV/ISOTrck/T1tttt/SMS_T1tttt_2J_mGl1300_mLSP100_PU_S14_POSTLS170/treeProducerSusySingleLepton/ 2 ";
  TString list = argv[1];
  list.Append(" ");
  list.Append(argv[2]);
  TString outname = argv[3];
  SetupHists(CutNumb);
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
  int iCut = 0;
  double CFCounter[CutNumb];
  int iCFCounter[CutNumb];
  for (int i=0;i < CutNumb; i++){
    CFCounter[i] = 0;
    iCFCounter[i] = 0;
  }

  bool Debug = false;
  string outnameStr = (string)outname;
  string TTbarModes[2] = {"MC_TTbar_DiLep","MC_TTbar_SinLep"};

  for(int entry=0; entry < Nevents/*min(1000000,Nevents)*/; entry+=1){
  //lumi calcualtion done in runAnalyzer.py (fb and pb)
  Float_t fw = tree->GetEntryW(entry);
  float EvWeight = 1;
  EvWeight *= fw ;

  iCut = 0;
  Obj.GetLeptons(tree);
  Obj.GetJets(tree);
  Obj.GetGenLeptons(tree);
  Obj.GetMET(tree);
  Obj.GetGenMET(tree);

  bool TwoLep = false;
  if((Obj.nGenLep+Obj.nGenLepFromTau ==2) || (Obj.nGenLep ==1 && Obj.nGenLepFromTau ==0 && Obj.nGenTau ==1) ||(Obj.nGenLep ==0 && Obj.nGenLepFromTau ==1 && Obj.nGenTau ==2))
    TwoLep = true;
  //if((nGenLep+nGenLepFromTau ==2)) TwoLep = true;
  if (outnameStr.compare(TTbarModes[0]) ==0 )
    if(!(TwoLep)) //DiLep = true;
      continue;
  if(outnameStr.compare(TTbarModes[1]) == 0)
    if(TwoLep)
      continue;
  // Define ST
  if( Obj.nMuGood > 0) Obj.ST = Obj.MET.Pt()+Obj.goodMu[0].Pt();
  // Fill main histograms
  FillMainHists(iCut, EvWeight);    


  CFCounter[iCut]+= EvWeight;
  iCFCounter[iCut]++;
  iCut++;
  // 1. Cut
  //////////////////Require exactly one good lepton and two jets
  if (Obj.nMuGood != 1) continue;
  if(Obj.nMuVeto !=0 || Obj.nElVeto !=0) continue;
  if (Obj.nJetGood < 2) continue;
  // Fill main histograms
  FillMainHists(iCut, EvWeight);
  CFCounter[iCut]+= EvWeight;                                                                                                   iCFCounter[iCut]++;
    vector<PseudoJet> particles;
    // an event with three particles:   px    py  pz      E
    for(int ijet=0;ijet<Obj.nJetGood;ijet++)
      {

	  TLorentzVector dummyJet;
	  dummyJet.SetPtEtaPhiM(Obj.goodJet[ijet].Pt(),Obj.goodJet[ijet].Eta(),Obj.goodJet[ijet].Phi(),Obj.goodJet[ijet].M());
	  particles.push_back ( PseudoJet( dummyJet.Px(), dummyJet.Py(), dummyJet.Pz(), dummyJet.E()));

      }

    
    // choose a jet definition
    double R = 1.2;
    JetDefinition jet_def(antikt_algorithm, R);
    
    // run the clustering, extract the jets
    ClusterSequence cs(particles, jet_def);
    vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets());
    
    // print out some infos
    //    cout << "Clustering with " << jet_def.description() << endl;
    float fMJ=0;    
    // print the jets
    //    cout <<   "        pt y phi" << endl;
    for (unsigned i = 0; i < jets.size(); i++) {
      fMJ = fMJ + jets[i].m(); 
      //cout << "jet " << i << ": "<< jets[i].pt() << " " 
      //	   << jets[i].rap() << " " << jets[i].phi() << endl;
      vector<PseudoJet> constituents = jets[i].constituents();
      for (unsigned j = 0; j < constituents.size(); j++) {
	//	cout << "    constituent " << j << "'s pt: " << constituents[j].pt()
	//     << endl;
      }
    }
    //hMJ[0]->Fill(fMJ,EvWeight);
    
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
      outf->cd();
      outf->mkdir(CutList[cj]);
      outf->cd(CutList[cj]);
      h0JetpT[cj]->Write();
      h1JetpT[cj]->Write();
      h2JetpT[cj]->Write();
      h3JetpT[cj]->Write();
      h0BJetpT[cj]->Write();
      h1BJetpT[cj]->Write();
      h2BJetpT[cj]->Write();
      h3BJetpT[cj]->Write();
      hRS[cj]->Write();
      hRC[cj]->Write();
      hHTRS[cj]->Write();
      hHTRC[cj]->Write();
      hSTRS[cj]->Write();
      hSTRC[cj]->Write();
      hNJRS[cj]->Write();
      hNJRC[cj]->Write();
      hHT[cj]->Write();
      hST[cj]->Write();
      hWpT[cj]->Write();
      hnJet[cj]->Write();
      hnBJet[cj]->Write();
      hnMu[cj]->Write();
      hnEl[cj]->Write();
      hMupt[cj]->Write();
      hElpt[cj]->Write();
      hnLep[cj]->Write();
      //hnGenLep[cj]->Write();
      //hnGenTau[cj]->Write();
      //hnGenLepFromTau[cj]->Write();
      hLeppt[cj]->Write();
      hLepeta[cj]->Write();
      hMET[cj]->Write();
      hdPhiWLep[cj]->Write();
      hdPhi[cj]->Write();
      hDfST[cj]->Write();
      hDfHT[cj]->Write();
      hHTST[cj]->Write();
      hNJST[cj]->Write();
      hMTMET[cj]->Write();
      /*      hHT[cj]->Write();
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
      hmT2W[cj]->Write();*/
    }
}
