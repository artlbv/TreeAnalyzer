#include "../interface/NtupleTools3.h"
#include "../interface/ClassObjects.h"

//#include "mt2w_bisect.h"
//#include "mt2w_bisect.cpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>

using namespace std;

// Map of Cuts. Instead of Cut Number use CutID (CutName):
// Fill Histo of cutname as Histo[GetCutID("cutname")]->Fill();

map<string, int> CutMap;
map<string, int>::iterator itCutMap;
// or vector -- but difficult to search in
/*
  vector<string> CutVect;
  vector<string>::iterator itCutVect;
*/

// Fill Cut Map
const int CutNumb = 26; // number of Cuts
const char* CutList[CutNumb] = {"noCut",
                                "== 1 Mu",
                                "6Jet",
                                "HT>500",
                                "ST>250",
                                "Nb>=1",
                                "Df<1",
                                "Df>1",
                                "Nb=1, 250<ST<350,Df<1",
                                "Nb=1, 250<ST<350,Df>1",
                                "Nb=1, 350<ST<450, Df<1",
                                "Nb=1, 350<ST<450,Df>1",
                                "Nb=1, ST>450, Df<1",
                                "Nb=1, ST>450,Df>1",
                                "Nb=2 ,250<ST<350, Df<1",
                                "Nb=2, 250<ST< 350,Df>1",
                                "Nb=2 ,350<ST<450,Df<1",
                                "Nb=2,350<ST<450,Df>1",
                                "Nb=2 ,ST>450, Df<1",
                                "Nb=2 ,ST>450,Df>1",
                                "Nb>2 250<ST<350 ,Df<1",
                                "Nb>2,250<ST<350,Df>1",
                                "Nb>2 350<ST<450,Df<1",
                                "Nb>2,350<ST<450, Df>1",
                                "Nb>2 ST>450,Df<1",
                                "Nb>2 ST>450,Df>1"
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
        //    hHT[cj]->Sumw2();
        hST[cj] = new TH1F ("ST_"+nCut,"ST "+cutName,400,0.0,4000.0);
        //  hST[cj]->Sumw2();
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
        hDfHT[cj] = new TH2F ("DfST_"+nCut,"DfST "+cutName,300,0.0,3000.0,64,0.0,3.2);
        hDfHT[cj]->Sumw2();

        hMTMET[cj] = new TH2F ("MTMET_"+nCut,"MTMET "+cutName,200,0.0,2000.0,200,0.0,2000);
        hMTMET[cj]->Sumw2();
        hHTST[cj] = new TH2F ("HTST_"+nCut,"HTST "+cutName,300,0.0,3000.0,200,0.0,2000);
        hHTST[cj]->Sumw2();

    }
}

void FillMainHists(int CutIndex, float EvWeight, bool FillBJets = true){
//    cout << "Filling " << CutIndex << endl;

    hnJet[CutIndex]->Fill(nJetGood,EvWeight);
    hnLep[CutIndex]->Fill(nLepGood,EvWeight);
    hnMu[CutIndex]->Fill(nMuGood,EvWeight);
    hnEl[CutIndex]->Fill(nElGood,EvWeight);

    if (nJetGood > 0) h0JetpT[CutIndex]->Fill(goodJet[0].Pt(),EvWeight);
    if (nJetGood > 1) h1JetpT[CutIndex]->Fill(goodJet[1].Pt(),EvWeight);
    if (nJetGood > 2) h2JetpT[CutIndex]->Fill(goodJet[2].Pt(),EvWeight);
    if (nJetGood > 3) h3JetpT[CutIndex]->Fill(goodJet[3].Pt(),EvWeight);

    if(FillBJets){
        hnBJet[CutIndex]->Fill(nBJetGood,EvWeight);

        if (nBJetGood > 0) h0BJetpT[CutIndex]->Fill(goodBJet[0].Pt(),EvWeight);
        if (nBJetGood > 1) h1BJetpT[CutIndex]->Fill(goodBJet[1].Pt(),EvWeight);
        if (nBJetGood > 2) h2BJetpT[CutIndex]->Fill(goodBJet[2].Pt(),EvWeight);
        if (nBJetGood > 3) h3BJetpT[CutIndex]->Fill(goodBJet[3].Pt(),EvWeight);
    }

    if (nLepGood > 0) hLeppt[CutIndex]->Fill(goodLep[0].Pt(),EvWeight);
    if (nMuGood > 0) hMupt[CutIndex]->Fill(goodMu[0].Pt(),EvWeight);
    if (nElGood > 0) hElpt[CutIndex]->Fill(goodEl[0].Pt(),EvWeight);

    hMET[CutIndex]->Fill(MET.Pt(),EvWeight);
    hHT[CutIndex]->Fill(HT40,EvWeight);
    hST[CutIndex]->Fill(ST,EvWeight);

    hnGenLep[CutIndex]->Fill(nGenLep,EvWeight);
    hnGenTau[CutIndex]->Fill(nGenTau,EvWeight);
    hnGenLepFromTau[CutIndex]->Fill(nGenLepFromTau,EvWeight);
}

void TreeAnalyzer(TString list, TString outname,bool useW=true){

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
    double CFCounter[CutNumb]; // weighted event counter
    int   iCFCounter[CutNumb]; // int event counter
    int iCut = 0; // count the cut number

    for (int i=0;i < CutNumb; i++){
        CFCounter[i] = 0;
        iCFCounter[i] = 0;
    }

    string outnameStr = (string)outname;
    string TTbarModes[2] = {"MC_TTbar_DiLep","MC_TTbar_SinLep"};

    SetupHists(CutNumb);

// Each cut name is mapped to its id (simple enumarate)
    for(int icut = 0; icut < CutNumb; icut++){
        string cutname = CutList[icut];

        CutMap[cutname] = icut;
    }

    /////////////   event loop   //////////////////////
    for(int entry=0; entry <  Nevents/*min(100000,Nevents)*/; entry+=1){
//    for(int entry=0; entry <  1000; entry+=1){


        // Reset Cut counter
        iCut = 0;
        itCutMap = CutMap.begin();

        // Read Tree
        Float_t fw = tree->GetEntryW(entry);
        progressT();

        //lumi calcualtion done in runAnalyzer.py (fb and pb)
        float EvWeight = 1;
        EvWeight *= fw ;

        // Get Objects from tree
        GetLeptons(tree);
        GetJets(tree);
        GetGenLeptons(tree);
        GetMET(tree);
        GetGenMET(tree);

        //Top decay modes
        bool TwoLep = false;

        if((nGenLep+nGenLepFromTau ==2) || (nGenLep ==1 && nGenLepFromTau ==0 && nGenTau ==1) ||(nGenLep ==0 && nGenLepFromTau ==1 && nGenTau ==2))
            TwoLep = true;

        if (outnameStr.compare(TTbarModes[0]) ==0 )
            if(!(TwoLep)) //DiLep = true;
                continue;
        if(outnameStr.compare(TTbarModes[1]) == 0)
            if(TwoLep)
                continue;

        // Define ST
        if( nMuGood > 0) ST = MET.Pt()+goodMu[0].Pt();

        // Fill main histograms
        FillMainHists(iCut, EvWeight);

        //if(nGenLepFromTau ==1)cout<<"nTau:"<<nGenTau<<endl;
        //if(nGenLepFromTau ==2)cout<<"nTau:"<<nGenTau<<endl;

        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;

        iCut++; itCutMap++;

        // 1. Cut
        ////////////////
        //Require exactly one good lepton and two jets

        if (nMuGood != 1) continue;
        if(nMuVeto !=0 || nElVeto !=0) continue;
        if (nJetGood < 2) continue;

        // Fill main histograms
        FillMainHists(iCut, EvWeight);

        // Fill special histos

        TLorentzVector WBos = MET + goodMu[0];
	float DelPhiWlep = fabs(WBos.DeltaPhi(goodLep[0]));
	float DelPhiMetMu =  fabs(MET.DeltaPhi(goodMu[0]));

/*
        float DelPhiMetMu = fabs(MET.Phi() - goodMu[0].Phi());
        if (DelPhiMetMu > acos(-1.0)) DelPhiMetMu -= 2*acos(-1.0);
        TLorentzVector WBos = MET + goodMu[0];

        float  DelPhiWlep = (WBos.Phi() - goodMu[0].Phi());
        if (DelPhiWlep > TMath::Pi())
            DelPhiWlep -= 2*TMath::Pi();
        if (DelPhiWlep <= -TMath::Pi())
            DelPhiWlep += 2*TMath::Pi();
*/

        float Wpt = WBos.Pt();
//        float Wphi = WBos.Phi();
        float MT = sqrt(pow((goodMu[0].Et()+MET.Et()),2)-pow((goodMu[0].Px()+MET.Px()),2)-pow((goodMu[0].Py()+MET.Py()),2));

//cout << nMuGood << "\t" << MET.Pt() << endl;

        hWpT[iCut]->Fill(Wpt,EvWeight);
        hdPhiWLep[iCut]->Fill(fabs(DelPhiWlep),EvWeight);
        hdPhi[iCut]->Fill(DelPhiMetMu,EvWeight);

        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;

        iCut++; itCutMap++;

        // 2. Cut
        ////////////////////////////

        if (nJetGood < 6) continue;
        //if (!(nJetGood >3 && nJetGood <6)) continue;
        //if (nJetGood < 3) continue; //control sample or new search bin?

        // Fill main histograms
        FillMainHists(iCut, EvWeight);

        // Fill special histos

        hdPhiWLep[iCut]->Fill(fabs(DelPhiWlep),EvWeight);

        hDfST[iCut]->Fill(ST,fabs(DelPhiWlep));
        hDfHT[iCut]->Fill(HT40,fabs(DelPhiWlep));
        hHTST[iCut]->Fill(HT40,ST);
        hNJST[iCut]->Fill(nJetGood,ST);
        hMTMET[iCut]->Fill(MT,MET.Pt());
        hWpT[iCut]->Fill(Wpt,EvWeight);

        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;

        iCut++; itCutMap++;

        // 3. Cut
        ////////////////////////////
        if (HT40 < 500) continue;

        // Fill main histograms
        FillMainHists(iCut, EvWeight);

        // Fill special histos
        if(MET.Pt() > 250 && MT > 120) hDfST[iCut]->Fill(ST,fabs(DelPhiWlep));
        if(ST > 250 && fabs(DelPhiWlep) >= 1) hMTMET[iCut]->Fill(MT,MET.Pt());

        hdPhiWLep[iCut]->Fill(fabs(DelPhiWlep),EvWeight);
        hdPhi[iCut]->Fill(DelPhiMetMu,EvWeight);

        hWpT[iCut]->Fill(Wpt,EvWeight);


        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;

        iCut++; itCutMap++;


        // 4. Cut
        ////////////////////////////
        if (ST < 250 ) continue;

        // Fill main histograms
        FillMainHists(iCut, EvWeight);

        // Fill special histos

        hdPhiWLep[iCut]->Fill(fabs(DelPhiWlep),EvWeight);
        hWpT[iCut]->Fill(Wpt,EvWeight);

        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;

        iCut++; itCutMap++;

        // 5. Cut
        ////////////////////////////
        if (nBJetGood < 1) continue;

        // Fill main histograms
        FillMainHists(iCut, EvWeight);

        // Fill special histos

        hnGenLep[iCut]->Fill(nGenLep,EvWeight);
        hnGenLepFromTau[iCut]->Fill(nGenLepFromTau,EvWeight);
        hWpT[iCut]->Fill(Wpt,EvWeight);
        hdPhiWLep[iCut]->Fill(fabs((DelPhiWlep)),EvWeight);
        hdPhi[iCut]->Fill(DelPhiMetMu,EvWeight);

        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;

        iCut++; itCutMap++;

// REST

        if(fabs(DelPhiWlep) < 1){
            // get cut number from cut name
            iCut = CutMap["Df<1"];
            CFCounter[iCut]+= EvWeight;
            iCFCounter[iCut]++;

	    FillMainHists(iCut, EvWeight);

            hRC[iCut]->Fill(nBJetGood,EvWeight);
            hNJRC[iCut]->Fill(nJetGood,EvWeight);
            hHTRC[iCut]->Fill(HT40,EvWeight);
            hSTRC[iCut]->Fill(ST,EvWeight);
        }
        else{
            iCut = CutMap["Df<1"];
            CFCounter[iCut]+= EvWeight;
            iCFCounter[iCut]++;

	    FillMainHists(iCut, EvWeight);

            hRS[iCut]->Fill(nBJetGood,EvWeight);
            hNJRS[iCut]->Fill(nJetGood,EvWeight);
            hHTRS[iCut]->Fill(HT40,EvWeight);
            hSTRS[iCut]->Fill(ST,EvWeight);
        }


////// HT, NJ, ST ,Nb dependency of RCS
        if (ST > 250 && ST < 350 ) {
            if(fabs(DelPhiWlep) < 1){

                iCut = CutMap["Nb=1, 250<ST<350,Df<1"];
                CFCounter[iCut]+= EvWeight;
                iCFCounter[iCut]++;

                hRC[iCut]->Fill(nBJetGood,EvWeight);
                hNJRC[iCut]->Fill(nJetGood,EvWeight);
                hHTRC[iCut]->Fill(HT40,EvWeight);
                hSTRC[iCut]->Fill(ST,EvWeight);
            }
            else{

                iCut = CutMap["Nb=1, 250<ST<350,Df>1"];
                CFCounter[iCut]+= EvWeight;
                iCFCounter[iCut]++;

                hRS[iCut]->Fill(nBJetGood,EvWeight);
                hNJRS[iCut]->Fill(nJetGood,EvWeight);
                hHTRS[iCut]->Fill(HT40,EvWeight);
                hSTRS[iCut]->Fill(ST,EvWeight);
            }
        }

/*
  if (ST > 350 && ST < 450 ) {
  if(fabs(DelPhiWlep) < 1){
  hRC[7]->Fill(nBJetGood,EvWeight);
  hNJRC[7]->Fill(nJetGood,EvWeight);
  hHTRC[7]->Fill(HT40,EvWeight);
  hSTRC[7]->Fill(ST,EvWeight);
  }
  if(fabs(DelPhiWlep) >= 1){
  hRS[7]->Fill(nBJetGood,EvWeight);
  hNJRS[7]->Fill(nJetGood,EvWeight);
  hHTRS[7]->Fill(HT40,EvWeight);
  hSTRS[7]->Fill(ST,EvWeight);
  }
  }
  if (ST > 450  ) {
  if(fabs(DelPhiWlep) < 1){
  hRC[8]->Fill(nBJetGood,EvWeight);
  hNJRC[8]->Fill(nJetGood,EvWeight);
  hHTRC[8]->Fill(HT40,EvWeight);
  hSTRC[8]->Fill(ST,EvWeight);
  }
  if(fabs(DelPhiWlep) >= 1){
  hRS[8]->Fill(nBJetGood,EvWeight);
  hNJRS[8]->Fill(nJetGood,EvWeight);
  hHTRS[8]->Fill(HT40,EvWeight);
  hSTRS[8]->Fill(ST,EvWeight);
  }
  }

*/

////// End HT, NJ, ST ,Nb dependency of RCS
/*
        if (nBJetGood == 1) {
            if (ST > 250 && ST <= 350 ) {
                hnBJet[6]->Fill(nBJetGood,EvWeight);
                hHT[6]->Fill(HT40,EvWeight);
                hST[6]->Fill(ST,EvWeight);
                hdPhiWLep[6]->Fill(fabs(DelPhiWlep),EvWeight);
                hWpT[6]->Fill(Wpt,EvWeight);
                if(fabs(DelPhiWlep) < 1.0)
                    hRC[8]->Fill(nJetGood,EvWeight);
                if(fabs(DelPhiWlep) >= 1.0)
                    hRS[8]->Fill(nJetGood,EvWeight);
                if(fabs(DelPhiWlep) < 1){
                    CFCounter[8]+= EvWeight;
                    iCFCounter[8]++;}
                if(fabs(DelPhiWlep) >= 1){
                    CFCounter[9]+= EvWeight;
                    iCFCounter[9]++;}
            }

            if (ST > 350 && ST <= 450 ) {
                hnBJet[7]->Fill(nBJetGood,EvWeight);
                hnJet[7]->Fill(nJetGood,EvWeight);
                hHT[7]->Fill(HT40,EvWeight);
                hST[7]->Fill(ST,EvWeight);
                hdPhiWLep[7]->Fill(fabs(DelPhiWlep),EvWeight);

                hWpT[7]->Fill(Wpt,EvWeight);
                if(fabs(DelPhiWlep) < 1.0)
                    hRC[9]->Fill(nJetGood,EvWeight);
                if(fabs(DelPhiWlep) >= 1.0)
                    hRS[9]->Fill(nJetGood,EvWeight);
                if(fabs(DelPhiWlep) < 1){
                    CFCounter[10]+= EvWeight;
                    iCFCounter[10]++;}
                if(fabs(DelPhiWlep) >= 1){
                    CFCounter[11]+= EvWeight;
                    iCFCounter[11]++;}
            }

            if (ST >= 450 ) {
                hnBJet[8]->Fill(nBJetGood,EvWeight);
                hnJet[8]->Fill(nJetGood,EvWeight);
                hHT[8]->Fill(HT40,EvWeight);
                hST[8]->Fill(ST,EvWeight);
                hdPhiWLep[8]->Fill(fabs(DelPhiWlep),EvWeight);

                hWpT[8]->Fill(Wpt,EvWeight);
                if(fabs(DelPhiWlep) < 0.5)
                    hRC[10]->Fill(nJetGood,EvWeight);
                if(fabs(DelPhiWlep) >= 0.5)
                    hRS[10]->Fill(nJetGood,EvWeight);

                if(fabs(DelPhiWlep) < 0.5){
                    CFCounter[12]+= EvWeight;
                    iCFCounter[12]++;}
                if(fabs(DelPhiWlep) >= 0.5){
                    CFCounter[13]+= EvWeight;
                    iCFCounter[13]++;}
            } //ST > 450 GeV
        }// Nb=1

        if (nBJetGood == 2) {

            if (ST > 250 && ST <= 350 ) {
                if(fabs(DelPhiWlep) < 1.0)
                    hRC[11]->Fill(nJetGood,EvWeight);
                if(fabs(DelPhiWlep) >= 1.0)
                    hRS[11]->Fill(nJetGood,EvWeight);
                if(fabs(DelPhiWlep) < 1.0){
                    CFCounter[14]+= EvWeight;
                    iCFCounter[14]++;}
                if(fabs(DelPhiWlep) >= 1.0){
                    CFCounter[15]+= EvWeight;
                    iCFCounter[15]++;}
            }
            if (ST > 350 && ST <= 450 ) {
                if(fabs(DelPhiWlep) < 1.0)
                    hRC[12]->Fill(nJetGood,EvWeight);
                if(fabs(DelPhiWlep) >= 1.0)
                    hRS[12]->Fill(nJetGood,EvWeight);
                if(fabs(DelPhiWlep) < 1.0){
                    CFCounter[16]+= EvWeight;
                    iCFCounter[16]++;}
                if(fabs(DelPhiWlep) >= 1.0){
                    CFCounter[17]+= EvWeight;
                    iCFCounter[17]++;}
            }
            if (ST > 450  ) {
                if(fabs(DelPhiWlep) < 1.0)
                    hRC[13]->Fill(nJetGood,EvWeight);
                if(fabs(DelPhiWlep) >= 1.0)
                    hRS[13]->Fill(nJetGood,EvWeight);
                if(fabs(DelPhiWlep) < 0.5){
                    CFCounter[18]+= EvWeight;
                    iCFCounter[18]++;}
                if(fabs(DelPhiWlep) >= 0.5){
                    CFCounter[19]+= EvWeight;
                    iCFCounter[19]++;}
            }

        }// Nb ==2

        if (nBJetGood > 2) {

            if (ST > 250 && ST <= 350 ) {
                if(fabs(DelPhiWlep) < 1.0)
                    hRC[14]->Fill(nBJetGood,EvWeight);
                if(fabs(DelPhiWlep) >= 1.0)
                    hRS[14]->Fill(nBJetGood,EvWeight);
                if(fabs(DelPhiWlep) < 1.0){
                    CFCounter[20]+= EvWeight;
                    iCFCounter[20]++;}
                if(fabs(DelPhiWlep) >= 1.0){
                    CFCounter[21]+= EvWeight;
                    iCFCounter[21]++;}
            }
            if (ST > 350 && ST <= 450 ) {
                if(fabs(DelPhiWlep) < 1.0)
                    hRC[15]->Fill(nBJetGood,EvWeight);
                if(fabs(DelPhiWlep) >= 1.0)
                    hRS[15]->Fill(nBJetGood,EvWeight);
                if(fabs(DelPhiWlep) < 1.0){
                    CFCounter[22]+= EvWeight;
                    iCFCounter[22]++;}
                if(fabs(DelPhiWlep) >= 1.0){
                    CFCounter[23]+= EvWeight;
                    iCFCounter[23]++;}
            }
            if (ST > 450  ) {
                if(fabs(DelPhiWlep) < 1.0)
                    hRC[16]->Fill(nBJetGood,EvWeight);
                if(fabs(DelPhiWlep) >= 1.0)
                    hRS[16]->Fill(nBJetGood,EvWeight);
                if(fabs(DelPhiWlep) < 0.5){
                    CFCounter[24]+= EvWeight;
                    iCFCounter[24]++;}
                if(fabs(DelPhiWlep) >= 0.5){
                    CFCounter[25]+= EvWeight;
                    iCFCounter[25]++;}
            }

        }// Nb >2

        */
            ////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////


            }/////////////////////////////////////////END HERE
// ^loop end^





    ofstream tfile;
    TString textfilename = "CMG_cutflow_"+outname+".txt";
    tfile.open(textfilename);

    tfile << "########################################" << endl;
    tfile << "Cut efficiency numbers:" << endl;
    for(int ci = 0; ci < CutNumb; ci++)
    {
        tfile << "After cut " << CutList[ci] <<  "\t\t"
              << CFCounter[ci]  << "\t events left\t"<< iCFCounter[ci] <<" cnt\t"<< endl;
        CutFlow->SetBinContent(1+ci,CFCounter[ci]);
    }
    TFile * outf;
    TString rootfilename = "CMG_"+outname+"_his.root";

    outf  = new TFile(rootfilename,"RECREATE");

    cout << "Writing histograms" << endl;

    for(int cj = 0; cj < CutNumb; cj++)
    {
        outf->cd();
        outf->mkdir(CutList[cj]);
        outf->cd(CutList[cj]);

        if(h0JetpT[cj]->GetEntries() > 0)
            h0JetpT[cj]->Write();
        if(h1JetpT[cj]->GetEntries() > 0)
            h1JetpT[cj]->Write();
        if(h2JetpT[cj]->GetEntries() > 0)
            h2JetpT[cj]->Write();
        if(h3JetpT[cj]->GetEntries() > 0)
            h3JetpT[cj]->Write();
        if(h0BJetpT[cj]->GetEntries() > 0)
            h0BJetpT[cj]->Write();
        if(h1BJetpT[cj]->GetEntries() > 0)
            h1BJetpT[cj]->Write();
        if(h2BJetpT[cj]->GetEntries() > 0)
            h2BJetpT[cj]->Write();
        if(h3BJetpT[cj]->GetEntries() > 0)
            h3BJetpT[cj]->Write();

        if(hRS[cj]->GetEntries() > 0)
            hRS[cj]->Write();
        if(hRC[cj]->GetEntries() > 0)
            hRC[cj]->Write();
        if(hHTRS[cj]->GetEntries() > 0)
            hHTRS[cj]->Write();
        if(hHTRC[cj]->GetEntries() > 0)
            hHTRC[cj]->Write();
        if(hSTRS[cj]->GetEntries() > 0)
            hSTRS[cj]->Write();
        if(hSTRC[cj]->GetEntries() > 0)
            hSTRC[cj]->Write();
        if(hNJRS[cj]->GetEntries() > 0)
            hNJRS[cj]->Write();
        if(hNJRC[cj]->GetEntries() > 0)
            hNJRC[cj]->Write();
        if(hHT[cj]->GetEntries() > 0)
            hHT[cj]->Write();
        if(hST[cj]->GetEntries() > 0)
            hST[cj]->Write();
        if(hWpT[cj]->GetEntries() > 0)
            hWpT[cj]->Write();
        if(hnJet[cj]->GetEntries() > 0)
            hnJet[cj]->Write();
        if(hnBJet[cj]->GetEntries() > 0)
            hnBJet[cj]->Write();
        if(hnMu[cj]->GetEntries() > 0)
            hnMu[cj]->Write();
        if(hnEl[cj]->GetEntries() > 0)
            hnEl[cj]->Write();
        if(hMupt[cj]->GetEntries() > 0)
            hMupt[cj]->Write();
        if(hElpt[cj]->GetEntries() > 0)
            hElpt[cj]->Write();
        if(hnLep[cj]->GetEntries() > 0)
            hnLep[cj]->Write();
        //hnGenLep[cj]->Write();
        //hnGenTau[cj]->Write();
        //hnGenLepFromTau[cj]->Write();

        if(hLeppt[cj]->GetEntries() > 0)
            hLeppt[cj]->Write();
        if(hLepeta[cj]->GetEntries() > 0)
            hLepeta[cj]->Write();
        if(hMET[cj]->GetEntries() > 0)
            hMET[cj]->Write();
        if(hdPhiWLep[cj]->GetEntries() > 0)
            hdPhiWLep[cj]->Write();
        if(hdPhi[cj]->GetEntries() > 0)
            hdPhi[cj]->Write();
        if(hDfST[cj]->GetEntries() > 0)
            hDfST[cj]->Write();
        if(hDfHT[cj]->GetEntries() > 0)
            hDfHT[cj]->Write();
        if(hHTST[cj]->GetEntries() > 0)
            hHTST[cj]->Write();
        if(hNJST[cj]->GetEntries() > 0)
            hNJST[cj]->Write();
        if(hMTMET[cj]->GetEntries() > 0)
            hMTMET[cj]->Write();

    }
}
