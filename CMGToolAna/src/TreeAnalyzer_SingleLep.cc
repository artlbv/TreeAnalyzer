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
#include <iostream>

using namespace std;

// Map of Cuts. Instead of Cut Number use CutID (CutName):
// Fill Histo of cutname as Histo[GetCutID("cutname")]->Fill();

map<string, int> CutMap;
map<string, int>::iterator itCutMap;

// instance of the Objects class with functionality defined in ClassObjects.C
GetObjects Obj;
bool debug = false;
const int CutNumb = 11; // number of Cuts
const char * CutList[CutNumb] = {"noCut",
                                 "1 lep",
				 "veto l",
				 "Nj>2",
				 "Nj>6",
				 "JpT>80",
                                 "HT>500",
                                 "ST>200",
                                 "Nb>=1",
				 "dPhi<1",
				 "dPhi>1"
};
// define global hists for plots after each cut
TH1F* CutFlow= new TH1F("CutFlow","Cut Flow",CutNumb,0.5,CutNumb+0.5);

// Kin Vars
TH1F *hHT[CutNumb];
TH1F *hST[CutNumb];
TH1F *hMET[CutNumb];
TH1F *hdPhiMetLep[CutNumb];
TH1F *hdPhiWLep[CutNumb];

// JETS
TH1F *hnJet[CutNumb];
TH1F *h1JetpT[CutNumb];
TH1F *h2JetpT[CutNumb];
TH1F *h3JetpT[CutNumb];
TH1F *h4JetpT[CutNumb];

TH1F *hnBJet[CutNumb];
TH1F *h0BJetpT[CutNumb];
TH1F *h1BJetpT[CutNumb];
TH1F *h2BJetpT[CutNumb];
TH1F *h3BJetpT[CutNumb];

// Leptons
TH1F *hnLep[CutNumb];
TH1F *hLepPt[CutNumb];
TH1F *hLepEta[CutNumb];
TH1F *hnOver[CutNumb];

// Set Up histograms and Cut Flow variables
void SetupHists(int CutNumber){
    for(int cj = 0; cj < CutNumber; cj++)
    {
        CutFlow->GetXaxis()->SetBinLabel(cj+1,CutList[cj]);
        TString cutName=CutList[cj];
        TString nCut;
        nCut.Form("%d",cj);

        // Kin Vars
        hHT[cj] = new TH1F ("HT_"+nCut,"HT "+cutName,400,0.0,4000.0);        hHT[cj]->Sumw2();
        hST[cj] = new TH1F ("ST_"+nCut,"ST "+cutName,400,0.0,4000.0);        hST[cj]->Sumw2();
        hMET[cj] = new TH1F("MET_"+nCut,"MET "+cutName,200.0,0.0,4000.0);    hMET[cj]->Sumw2();

        hdPhiMetLep[cj] = new TH1F("dPhiMetLep_"+nCut,"dPhiMetLep "+cutName,80,0,3.2);         hdPhiMetLep[cj]->Sumw2();
        hdPhiWLep[cj] = new TH1F("dPhiWLep_"+nCut,"dPhiWLep "+cutName,64,0.0,3.2);        hdPhiWLep[cj]->Sumw2();

        // JETS
        hnJet[cj] = new TH1F ("nJet_"+nCut,"nJet "+cutName,20,0,20);        hnJet[cj]->Sumw2();
        hnBJet[cj] = new TH1F ("nBJet_"+nCut,"nBJet "+cutName,20,0,20);        hnBJet[cj]->Sumw2();

        h1JetpT[cj] = new TH1F ("1JetpT_"+nCut,"1JetpT "+cutName,200,0.0,2000.0);        h1JetpT[cj]->Sumw2();
        h2JetpT[cj] = new TH1F ("2JetpT_"+nCut,"2JetpT "+cutName,200,0.0,2000.0);        h2JetpT[cj]->Sumw2();
        h3JetpT[cj] = new TH1F ("3JetpT_"+nCut,"3JetpT "+cutName,200,0.0,2000.0);        h3JetpT[cj]->Sumw2();
        h4JetpT[cj] = new TH1F ("4JetpT_"+nCut,"4JetpT "+cutName,200,0.0,2000.0);        h4JetpT[cj]->Sumw2();

        h0BJetpT[cj] = new TH1F ("0BJetpT_"+nCut,"0BJetpT "+cutName,200,0.0,2000.0);        h0BJetpT[cj]->Sumw2();
        h1BJetpT[cj] = new TH1F ("1BJetpT_"+nCut,"1BJetpT "+cutName,200,0.0,2000.0);        h1BJetpT[cj]->Sumw2();
        h2BJetpT[cj] = new TH1F ("2BJetpT_"+nCut,"2BJetpT "+cutName,200,0.0,2000.0);        h2BJetpT[cj]->Sumw2();
        h3BJetpT[cj] = new TH1F ("3BJetpT_"+nCut,"3BJetpT "+cutName,200,0.0,2000.0);        h3BJetpT[cj]->Sumw2();

        // Leptons
        hnLep[cj] = new TH1F ("nLep_"+nCut,"nLep "+cutName,10,0,10);        hnLep[cj]->Sumw2();
        hLepPt[cj] = new TH1F ("LepPt_"+nCut,"Lep pT "+cutName,100,0,1000);        hLepPt[cj]->Sumw2();
        hnOver[cj] = new TH1F ("nOver_"+nCut,"nOver "+cutName,2,0,2);       hnOver[cj]->Sumw2();
        hLepEta[cj] = new TH1F ("LepEta_"+nCut,"Lep eta "+cutName,100,-2.5,2.5);        hLepEta[cj]->Sumw2();

    }
}

void FillMainHists(int CutIndex, double EvWeight, bool FillBJets = true){
    hnJet[CutIndex]->Fill(Obj.nJetGood,EvWeight);
    hnLep[CutIndex]->Fill(Obj.nLepGood,EvWeight);

    if (Obj.nJetGood > 0) {
	h1JetpT[CutIndex]->Fill(Obj.goodJet[0].Pt(),EvWeight);
	hHT[CutIndex]->Fill(Obj.HT40,EvWeight);
    }
    if (Obj.nJetGood > 1) h2JetpT[CutIndex]->Fill(Obj.goodJet[1].Pt(),EvWeight);
    if (Obj.nJetGood > 2) h3JetpT[CutIndex]->Fill(Obj.goodJet[2].Pt(),EvWeight);
    if (Obj.nJetGood > 3) h4JetpT[CutIndex]->Fill(Obj.goodJet[3].Pt(),EvWeight);

    if(FillBJets){
        hnBJet[CutIndex]->Fill(Obj.nBJetGood,EvWeight);

        if (Obj.nBJetGood > 0) h0BJetpT[CutIndex]->Fill(Obj.goodBJet[0].Pt(),EvWeight);
        if (Obj.nBJetGood > 1) h1BJetpT[CutIndex]->Fill(Obj.goodBJet[1].Pt(),EvWeight);
        if (Obj.nBJetGood > 2) h2BJetpT[CutIndex]->Fill(Obj.goodBJet[2].Pt(),EvWeight);
        if (Obj.nBJetGood > 3) h3BJetpT[CutIndex]->Fill(Obj.goodBJet[3].Pt(),EvWeight);
    }

    if (Obj.nLepGood > 0) {
        hLepPt[CutIndex]->Fill(Obj.goodLep[0].Pt(),EvWeight);
        hLepEta[CutIndex]->Fill(Obj.goodLep[0].Eta(),EvWeight);

        hdPhiWLep[CutIndex]->Fill(Obj.DelPhiWLep,EvWeight);
        hdPhiMetLep[CutIndex]->Fill(Obj.DelPhiMetLep,EvWeight);

	hST[CutIndex]->Fill(Obj.ST,EvWeight);
    }

    hMET[CutIndex]->Fill(Obj.MET.Pt(),EvWeight);
}


int main (int argc, char* argv[]){
    if (argc < 4) {
        cout<<"usage ./TreeAnalyzer_example [INPUTFOLDER] [XSEC * LUMI] [SAMPLENAME]"<<endl;
        return 1;
    }
    cout<<argc<<" "<<argv[1]<<" "<<argv[2]<<endl;
    TString list = argv[1];
    for (int i = 2; i<argc-1;i++){
        list.Append(" ");
        list.Append(argv[i]);
    }
    TString outname = argv[argc-1];

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

    EasyChain* tree = new EasyChain("tree");
    for(unsigned i=0;i<files.size();i++){
        tree->AddSmartW(files[i],weights[i]);
        cout<<"add: "<<files[i]<<" "<<weights[i]<<endl;
    }
    int Nevents=tree->GetEntries();
    cout<<">>>>>>>>>>>>>>>>>>>>>>> total number of events:\t" << Nevents <<endl;

    SetupHists(CutNumb);

    // Each cut name is mapped to its id (simple enumarate)
    cout << "Mapping cuts" << endl;

    for(int icut = 0; icut < CutNumb; icut++){
        string cutname = CutList[icut];

        CutMap[cutname] = icut;
        cout << "Cut name\t" << cutname << "\tcut number\t" << icut << endl;
    }

    // CutFlow variables
    int iCut = 0;
    double CFCounter[CutNumb];
    int iCFCounter[CutNumb];
    for (int i=0;i < CutNumb; i++){
        CFCounter[i] = 0;
        iCFCounter[i] = 0;
    }

    string outnameStr = (string)outname;
    string TTbarModes[2] = {"MC_TTbar_DiLep","MC_TTbar_SinLep"};

    cout << "Starting event loop" << endl;

//    for(int entry=0; entry < min(10000,Nevents); entry+=1){
    for(int entry=0; entry < Nevents; entry+=1){

        if (entry % 1000 == 0)
            cout << "================= Processing entry: " << entry << '\r' << flush;

        //lumi calcualtion done in runAnalyzer.py (fb and pb)
        Double_t fw = tree->GetEntryW(entry);
        Double_t EvWeight = 1.0;
        EvWeight *= fw ;

        //get all objects
        if(debug) cout<<"GetLeptons" <<endl;
        Obj.GetLeptons(tree,"mvaPhys14","CristinaID");
        if(debug) cout<<" GetJets"<<endl;
        Obj.GetJets(tree);
        if(debug) cout<<" GetFatJets"<<endl;
//        Obj.GetFatJets(tree);
        if(debug) cout<<" GetGenLeptons"<<endl;
//        Obj.GetGenLeptons(tree);
        if(debug) cout<<" GetMET"<<endl;
        Obj.GetMET(tree);
        if(debug) cout<<" GetGenMET"<<endl;
//        Obj.GetGenMET(tree);

        //check src/ClassObjects.C for what is available and implement new variables in there
        if(debug) cout<<" GetKinVariables"<<endl;
        Obj.GetKinVariables();

        bool SoftHard_over =false;

        ///////////////////////////////////////////////////////
        ////////////START CUTFLOW//////////////////////////////
        ///////////////////////////////////////////////////////

        // 0. No Cuts (i.e. CMG tuple skim only)
        // Fill main histograms
        iCut = 0;
        FillMainHists(iCut, EvWeight);

        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;

        // 1. Cut (Require exactly one good Electron)
        //////////////////
        if (Obj.nLepGood != 1) continue;
//        if ( Obj.nMuGood != 1) continue;
        if ( Obj.nElGood != 1) continue;

        // replace LepGood collection with SoftLepGood if you want to do the soft lep analysis
        /*
          if (Obj.nSoftLepGood != 1) continue;
          if( Obj.nSoftElGood != 1) continue;
          if(Obj.nSoftMuVeto !=0 || Obj.nSoftElVeto !=0) continue;
        */

        FillMainHists(iCut, EvWeight);

        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;

        // 2. Cut (Lepton veto)
        ////////////////////////////
        if (Obj.nMuVeto !=0 || Obj.nElVeto !=0) continue;
        // Fill main histograms
        FillMainHists(iCut, EvWeight);
        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;

        // 3. Cut (Njets >= 2)
        ////////////////////////////
        if (Obj.nJetGood < 2) continue;
        // Fill main histograms
        FillMainHists(iCut, EvWeight);
        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;

        // 4. Cut (Njets >= 6)
        ////////////////////////////
        if (Obj.nJetGood < 6) continue;
        // Fill main histograms
        FillMainHists(iCut, EvWeight);
        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;

        // 5. Cut (leading jets pT > 80)
        ////////////////////////////
	if(!(Obj.goodJet[0].Pt()> 80 && Obj.goodJet[1].Pt()> 80)) continue;
        // Fill main histograms
        FillMainHists(iCut, EvWeight);
        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;

        // 6. Cut
        ////////////////////////////
        if (Obj.HT40 < 500) continue;
        // Fill main histograms
        FillMainHists(iCut, EvWeight);
        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;

        // 7. Cut
        ////////////////////////////
        if (Obj.ST < 200 ) continue;
        // Fill main histograms
        FillMainHists(iCut, EvWeight);
        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;

        // 8. Cut
        ////////////////////////////
        if (Obj.nBJetGood < 1) continue;
        // Fill main histograms
        FillMainHists(iCut, EvWeight);
        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;

	// BINS

        if(Obj.DelPhiWLep < 1)
            // get cut number from cut name
            iCut = CutMap["dPhi<1"];

        else if (Obj.DelPhiWLep > 1)
            iCut = CutMap["dPhi>1"];

	CFCounter[iCut]+= EvWeight;
	iCFCounter[iCut]++;

        // Fill main histograms
	FillMainHists(iCut, EvWeight);


    }

    cout << endl << "Finished event loop" << endl;

    //write out cutflow
    ofstream tfile;
    TString textfilename = "CMG_cutflow_"+outname+"_El.txt";
    tfile.open(textfilename);
    tfile << "########################################" << endl;
    tfile << "Cut efficiency numbers:" << endl;

    for(int ci = 0; ci < CutNumb; ci++)
    {
        tfile << "After cut\t"
              << CutList[ci] << "\t\t"
              << CFCounter[ci] << "\tevents left\t"<< iCFCounter[ci] <<"\tcnt"<< endl;

        cout << "After cut\t"
	     << CutList[ci] << "\t\t"
	     << CFCounter[ci] << "\tevents left\t"<< iCFCounter[ci] <<"\tcnt"<< endl;


        CutFlow->SetBinContent(1+ci,CFCounter[ci]);
    }

    //write out histograms
    TFile * outf;
    TString rootfilename = "CMG_"+outname+"_El_his.root";
    outf = new TFile(rootfilename,"RECREATE");

    //first the main histograms
    cout<<"start with normal histograms"<<endl;

    CutFlow->Write();

    for(int cj = 0; cj < CutNumb; cj++)
    {
        outf->cd();

        // mkdir of CutName
        outf->mkdir(CutList[cj]);
        outf->cd(CutList[cj]);

	// Kin Vars
        if(hHT[cj]->GetEntries() > 0) hHT[cj]->Write();
        if(hST[cj]->GetEntries() > 0) hST[cj]->Write();
        if(hMET[cj]->GetEntries() > 0) hMET[cj]->Write();

	if(hdPhiMetLep[cj]->GetEntries() > 0) hdPhiMetLep[cj]->Write();
	if(hdPhiWLep[cj]->GetEntries() > 0) hdPhiWLep[cj]->Write();

	// JETS
        if(hnJet[cj]->GetEntries() > 0) hnJet[cj]->Write();
        if(h1JetpT[cj]->GetEntries() > 0) h1JetpT[cj]->Write();
        if(h2JetpT[cj]->GetEntries() > 0) h2JetpT[cj]->Write();
        if(h3JetpT[cj]->GetEntries() > 0) h3JetpT[cj]->Write();
        if(h4JetpT[cj]->GetEntries() > 0) h4JetpT[cj]->Write();

        if(hnBJet[cj]->GetEntries() > 0) hnBJet[cj]->Write();
        if(h0BJetpT[cj]->GetEntries() > 0) h0BJetpT[cj]->Write();
        if(h1BJetpT[cj]->GetEntries() > 0) h1BJetpT[cj]->Write();
        if(h2BJetpT[cj]->GetEntries() > 0) h2BJetpT[cj]->Write();
        if(h3BJetpT[cj]->GetEntries() > 0) h3BJetpT[cj]->Write();

	// Leptons
        if(hnLep[cj]->GetEntries() > 0) hnLep[cj]->Write();
        if(hLepPt[cj]->GetEntries() > 0) hLepPt[cj]->Write();
        if(hLepEta[cj]->GetEntries() > 0) hLepEta[cj]->Write();
        if(hnOver[cj]->GetEntries() > 0) hnOver[cj]->Write();

/*
        h1JetpT[cj]->Write();
        hnJet[cj]->Write();
        hnOver[cj]->Write();
        hnBJet[cj]->Write();
        hnLep[cj]->Write();
        hLepPt[cj]->Write();
        hLepEta[cj]->Write();
        hMET[cj]->Write();
*/
    }
    cout<<"done with normal histograms"<<endl;

}
