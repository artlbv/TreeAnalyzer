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
const int CutNumb = 6; // number of Cuts
const char * CutList[CutNumb] = {"noCut",
                                 "== 1 Mu", "6Jet","HT>500","ST>250",
                                 "Nb>=1"
};
// define global hists for plots after each cut
TH1F* CutFlow= new TH1F("CutFlow","Cut Flow",CutNumb,0.5,CutNumb+0.5);
TH1F *hHT[CutNumb];
TH1F *hST[CutNumb];
TH1F *h0JetpT[CutNumb];
TH1F *hnJet[CutNumb];
TH1F *hnBJet[CutNumb];
TH1F *hnLep[CutNumb];
TH1F *hLeppt[CutNumb];
TH1F *hLepeta[CutNumb];
TH1F *hMET[CutNumb];
TH1F *hnOver[CutNumb];

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
        hHT[cj]->Sumw2();
        hST[cj] = new TH1F ("ST_"+nCut,"ST "+cutName,400,0.0,4000.0);
        hST[cj]->Sumw2();
        h0JetpT[cj] = new TH1F ("0JetpT_"+nCut,"0JetpT "+cutName,200,0.0,2000.0);
        h0JetpT[cj]->Sumw2();
        hnJet[cj] = new TH1F ("nJet_"+nCut,"nJet "+cutName,20,0,20);
        hnJet[cj]->Sumw2();
        hnBJet[cj] = new TH1F ("nBJet_"+nCut,"nBJet "+cutName,20,0,20);
        hnBJet[cj]->Sumw2();
        hnLep[cj] = new TH1F ("nLep_"+nCut,"nLep "+cutName,10,0,10);
        hnLep[cj]->Sumw2();
        hLeppt[cj] = new TH1F ("LeppT_"+nCut,"Lep pT "+cutName,100,0,1000);
        hLeppt[cj]->Sumw2();
        hnOver[cj] = new TH1F ("nOver_"+nCut,"nOver "+cutName,2,0,2);
        hLepeta[cj] = new TH1F ("Lepeta_"+nCut,"Lep eta "+cutName,100,-4,4);
        hLepeta[cj]->Sumw2();
        hMET[cj] = new TH1F("MET_"+nCut,"MET "+cutName,200.0,0.0,4000.0);
        hMET[cj]->Sumw2();
    }
}

void FillMainHists(int CutIndex, double EvWeight, bool FillBJets = true){
    hnJet[CutIndex]->Fill(Obj.nJetGood,EvWeight);
    hnLep[CutIndex]->Fill(Obj.nLepGood,EvWeight);
    if (Obj.nJetGood > 0) h0JetpT[CutIndex]->Fill(Obj.goodJet[0].Pt(),EvWeight);
    if(FillBJets){
        hnBJet[CutIndex]->Fill(Obj.nBJetGood,EvWeight);
    }
    if (Obj.nLepGood > 0) hLeppt[CutIndex]->Fill(Obj.goodLep[0].Pt(),EvWeight);
    hMET[CutIndex]->Fill(Obj.MET.Pt(),EvWeight);
    hHT[CutIndex]->Fill(Obj.HT40,EvWeight);
    hST[CutIndex]->Fill(Obj.ST,EvWeight);
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
    for(int icut = 0; icut < CutNumb; icut++){
        string cutname = CutList[icut];

        CutMap[cutname] = icut;

	cout << "Mapping cuts" << endl;
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

    for(int entry=0; entry < min(10000,Nevents); entry+=1){

        if (entry % 1000 == 0)
            cout << "================= Processing entry: " << entry << '\r' << flush;

        //lumi calcualtion done in runAnalyzer.py (fb and pb)
        Double_t fw = tree->GetEntryW(entry);
        Double_t EvWeight = 1.0;
        EvWeight *= fw ;

        //get all objects
        if(debug) cout<<"GetLeptons" <<endl;
        Obj.GetLeptons(tree);
        if(debug) cout<<" GetJets"<<endl;
        Obj.GetJets(tree);
        if(debug) cout<<" GetFatJets"<<endl;
        Obj.GetFatJets(tree);
        if(debug) cout<<" GetGenLeptons"<<endl;
        Obj.GetGenLeptons(tree);
        if(debug) cout<<" GetMET"<<endl;
        Obj.GetMET(tree);
        if(debug) cout<<" GetGenMET"<<endl;
        Obj.GetGenMET(tree);
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

        // 1. Cut
        //////////////////Require exactly one good Muon
        if (Obj.nLepGood != 1) continue;
        if ( Obj.nElGood != 1) continue;
        if (Obj.nMuVeto !=0 || Obj.nElVeto !=0) continue;
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

        // 2. Cut
        ////////////////////////////
        if (Obj.nJetGood < 6) continue;
        // Fill main histograms
        FillMainHists(iCut, EvWeight);
        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;

        // 3. Cut
        ////////////////////////////
        if (Obj.HT40 < 500) continue;
        FillMainHists(iCut, EvWeight);
        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;

        // 4. Cut
        ////////////////////////////
        if (Obj.ST < 250 ) continue;
        FillMainHists(iCut, EvWeight);
        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;

        // 5. Cut
        ////////////////////////////
        if (Obj.nBJetGood < 1) continue;
        FillMainHists(iCut, EvWeight);
        //cout<<Obj.MTbMet<<endl;

        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;
    }

    cout << endl << "Finished event loop" << endl;

    //write out cutflow
    ofstream tfile;
    TString textfilename = "CMG_cutflow_"+outname+".txt";
    tfile.open(textfilename);
    tfile << "########################################" << endl;
    tfile << "Cut efficiency numbers:" << endl;

    for(int ci = 0; ci < CutNumb; ci++)
    {
	tfile << "After cut\t"
	      << CutList[ci] << "\t\t"
	      << CFCounter[ci] << "\tevents left\t"<< iCFCounter[ci] <<"\tcnt"<< endl;
        CutFlow->SetBinContent(1+ci,CFCounter[ci]);
    }

    //write out histograms
    TFile * outf;
    TString rootfilename = "CMG_"+outname+"_his.root";
    outf = new TFile(rootfilename,"RECREATE");
    char FOLDER[100];
    //first the main histograms
    cout<<"start with normal histograms"<<endl;
    for(int cj = 0; cj < CutNumb; cj++)
    {
        outf->cd();
        //outf->mkdir(CutList[cj]);
        //outf->cd(CutList[cj]);
        h0JetpT[cj]->Write();
        hnJet[cj]->Write();
        hnOver[cj]->Write();
        hnBJet[cj]->Write();
        hnLep[cj]->Write();
        hLeppt[cj]->Write();
        hLepeta[cj]->Write();
        hMET[cj]->Write();

    }
    cout<<"done with normal histograms"<<endl;

}
