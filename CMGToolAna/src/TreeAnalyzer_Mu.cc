#include "../interface/ClassObjects.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TFile.h>
#include <iostream>

using namespace std;
// instance of the Objects class with functionality defined in ClassObjects.C
GetObjects Obj;
bool debug = false;
const int CutNumb = 7; // number of Cuts
const char * CutList[CutNumb] = {"noCut",
                                 "== 1 Mu", "6Jet","HT>500","ST>250",
                                 "Nb>=1" , "dPhiWLep> 1.0"//,"nTopTag >= 1"
};

 static const int nbJetBins = 3;
static const int cutLonbJetsArr[] = { 1, 2, 3 };
static const int cutHinbJetsArr[] = { 2, 3, -1 };

static const double cutLoHTArr[]  = {500,500,500,500,500,750,750,750,750,750,1000,1000,1000,1000,1000,/**/ 500,500,500,500,500,750,750,750,750,750,1000,1000,1000,1000,1000,/**/500,500,500,500,500,750,750,750,750,750,1000,1000,1000,1000,1000 };
static const double cutHiHTArr[]  = {750,750,750,750,750,1000,1000,1000,1000,1000,-1,-1,-1,-1,-1,/**/ 750,750,750,750,750,1000,1000,1000,1000,1000,-1,-1,-1,-1,-1,/**/ 750,750,750,750,750,1000,1000,1000,1000,1000,-1,-1,-1,-1,-1};
static const double cutLoSTArr[]  = {200,250,350,500,600,200,250,350,500,600,200,250,350,500,600,/**/ 200,250,350,500,600,200,250,350,500,600,200,250,350,500,600,/**/ 200,250,350,500,600,200,250,350,500,600,200,250,350,500,600 };
static const double cutHiSTArr[]  = {250,350,500,600,-1,250,350,500,600,-1,250,350,500,600,-1,/**/ 250,350,500,600,-1,250,350,500,600,-1,250,350,500,600,-1,/**/ 250,350,500,600,-1,250,350,500,600,-1,250,350,500,600,-1 };
//static const double cutLoDphiArr[]  ={0,1,0,1,0,1,0,1,0,1};

//static const double cutLonJetsArr[]  = {4,6,4,6,4,6,4,6,4,6,/**/4,6,4,6,4,6,4,6,4,6,/**/ 4,6,4,6,4,6,4,6,4,6 };
//static const double cutHinJetsArr[]  = {6,-1,6,-1,6,-1,6,-1,6,-1,/**/6,-1,6,-1,6,-1,6,-1,6,-1,/**/6,-1,6,-1,6,-1,6,-1,6,-1}
static const int nStages = sizeof(cutLoHTArr)/sizeof(cutLoHTArr[0]);
vector<string> keyStrForStages;
static const int nbJetsCut = 1;

char names[200];

// define global hists for plots after each cut
TH1D* CutFlow= new TH1D("CutFlow","Cut Flow",CutNumb,0.5,CutNumb+0.5);
TH1D *hHT[CutNumb];
TH1D *hST[CutNumb];
TH1D *h0JetpT[CutNumb];
TH1D *hnJet[CutNumb];
TH1D *hnBJet[CutNumb];
TH1D *hnLep[CutNumb];
TH1D *hLeppt[CutNumb];
TH1D *hLepeta[CutNumb];
TH1D *hMET[CutNumb];
TH1D *hnOver[CutNumb];
TH1D *hdPhiWLep[CutNumb];
TH1D *hdPhiJMET[CutNumb];
TH1D *hToppT[CutNumb];
TH1D *hnTop[CutNumb];
TH1D *hnW[CutNumb];
TH1D *hWTagpT[CutNumb];
TH1D *hWmassTagpT[CutNumb];
TH1D *hWTagMass[CutNumb];
TH1D *hWmassTagMass[CutNumb];
TH1D *hnWmass[CutNumb];
TH1D *hRC[CutNumb];
TH1D *hRS[CutNumb];
TH1D *hRC_nTop[CutNumb];
TH1D *hRS_nTop[CutNumb];
TH1D *hRC_TopPt[CutNumb];
TH1D *hRS_TopPt[CutNumb];


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
        hHT[cj] = new TH1D ("HT_"+nCut,"HT "+cutName,400,0.0,4000.0);
        hHT[cj]->Sumw2();
        hST[cj] = new TH1D ("ST_"+nCut,"ST "+cutName,400,0.0,4000.0);
        hST[cj]->Sumw2();
        h0JetpT[cj] = new TH1D ("0JetpT_"+nCut,"0JetpT "+cutName,200,0.0,2000.0);
        h0JetpT[cj]->Sumw2();
        hnJet[cj] = new TH1D ("nJet_"+nCut,"nJet "+cutName,20,0,20);
        hnJet[cj]->Sumw2();
        hnBJet[cj] = new TH1D ("nBJet_"+nCut,"nBJet "+cutName,20,0,20);
        hnBJet[cj]->Sumw2();
        hnLep[cj] = new TH1D ("nLep_"+nCut,"nLep "+cutName,10,0,10);
        hnLep[cj]->Sumw2();
        hLeppt[cj] = new TH1D ("LeppT_"+nCut,"Lep pT "+cutName,100,0,1000);
        hLeppt[cj]->Sumw2();
        hnOver[cj] = new TH1D ("nOver_"+nCut,"nOver "+cutName,2,0,2);
        hLepeta[cj] = new TH1D ("Lepeta_"+nCut,"Lep eta "+cutName,100,-4,4);
        hLepeta[cj]->Sumw2();
        hMET[cj] = new TH1D("MET_"+nCut,"MET "+cutName,200.0,0.0,4000.0);
        hMET[cj]->Sumw2();
        hdPhiWLep[cj] = new TH1D("dPhiWLep_"+nCut,"dPhiWLep "+cutName,64,0.0,3.2);
        hdPhiWLep[cj]->Sumw2();
        hdPhiJMET[cj] = new TH1D("dPhiJMET_"+nCut,"dPhiJMET "+cutName,64,0.0,3.2);
        hdPhiJMET[cj]->Sumw2();
        hToppT[cj] = new TH1D ("ToppT_"+nCut,"ToppT "+cutName,200,0.0,2000.0);
        hToppT[cj]->Sumw2();
        hnTop[cj] = new TH1D ("nTop_"+nCut,"nTop "+cutName,20,0,20);
        hnTop[cj]->Sumw2();
        hnW[cj] = new TH1D ("nW_"+nCut,"nW "+cutName,20,0,20);
        hnW[cj]->Sumw2();
        hWTagpT[cj] = new TH1D ("WpT_"+nCut,"WpT "+cutName,200,0.0,2000.0);
        hWTagpT[cj]->Sumw2();
        hWmassTagpT[cj] = new TH1D ("WmasspT_"+nCut,"WmasspT "+cutName,200,0.0,2000.0);
        hWmassTagpT[cj]->Sumw2();
        hWTagMass[cj] = new TH1D ("WMass_"+nCut,"WMass "+cutName,200,0.0,2000.0);
        hWTagMass[cj]->Sumw2();
        hWmassTagMass[cj] = new TH1D ("WmassMass_"+nCut,"WmassMass "+cutName,200,0.0,2000.0);
        hWmassTagMass[cj]->Sumw2();
        hnWmass[cj] = new TH1D ("nWmass_"+nCut,"nWmass "+cutName,20,0,20);
        hnWmass[cj]->Sumw2();
        hRS[cj] = new TH1D ("RS_"+nCut,"RS "+cutName,20,0,20);
        hRS[cj]->Sumw2();
        hRC[cj] = new TH1D ("RC_"+nCut,"RC "+cutName,20,0,20);
        hRC[cj]->Sumw2();
        hRS_nTop[cj] = new TH1D ("RS_nTop_"+nCut,"RS_nTop "+cutName,10,0,10);
        hRS_nTop[cj]->Sumw2();
        hRC_nTop[cj] = new TH1D ("RC_nTop_"+nCut,"RC_nTop "+cutName,10,0,10);
        hRC_nTop[cj]->Sumw2();
        hRS_TopPt[cj] = new TH1D ("RS_TopPt_"+nCut,"RS_nTop "+cutName,38,100,2000);
        hRS_TopPt[cj]->Sumw2();
        hRC_TopPt[cj] = new TH1D ("RC_TopPt_"+nCut,"RC_nTop "+cutName,38,100,2000);
        hRC_TopPt[cj]->Sumw2();

            
    }
}

void FillMainHists(int CutIndex, Double_t EvWeight, bool FillBJets = true){
    hnJet[CutIndex]->Fill(Obj.nJetGood,EvWeight);
    hnLep[CutIndex]->Fill(Obj.nLepGood,EvWeight);
    if (Obj.nJetGood > 0) h0JetpT[CutIndex]->Fill(Obj.goodJet[0].Pt(),EvWeight);
    if(FillBJets){
        hnBJet[CutIndex]->Fill(Obj.nBJetGood,EvWeight);
    }
    if (Obj.nLepGood > 0) 
    {
     hLeppt[CutIndex]->Fill(Obj.goodLep[0].Pt(),EvWeight);}
     else if(Obj.nSoftLepGood > 0){
     hLeppt[CutIndex]->Fill(Obj.softLep[0].Pt(),EvWeight);
    }
     hdPhiWLep[CutIndex]->Fill(Obj.DelPhiWLep,EvWeight);

    hMET[CutIndex]->Fill(Obj.MET.Pt(),EvWeight);
    hHT[CutIndex]->Fill(Obj.HT40,EvWeight);
    hST[CutIndex]->Fill(Obj.ST,EvWeight);
    hnTop[CutIndex]->Fill(Obj.nTopTagJetGood,EvWeight);
    hdPhiJMET[CutIndex]->Fill(Obj.DelPhiJMet,EvWeight);
    if(Obj.nTopTagJetGood>0)hToppT[CutIndex]->Fill(Obj.goodTopTagJet[0].Pt(),EvWeight);
    hnW[CutIndex]->Fill(Obj.nWTagJetGood,EvWeight);
    if(Obj.nWTagJetGood>0){ 
    hWTagpT[CutIndex]->Fill(Obj.goodWTagJet[0].Pt(),EvWeight);
    hWTagMass[CutIndex]->Fill(Obj.goodWTagJet[0].M(),EvWeight);

    }
    hnWmass[CutIndex]->Fill(Obj.nWmassTagJetGood,EvWeight);
    if(Obj.nWmassTagJetGood>0){
     hWmassTagpT[CutIndex]->Fill(Obj.goodWmassTagJet[0].Pt(),EvWeight);
    hWmassTagMass[CutIndex]->Fill(Obj.goodWmassTagJet[0].M(),EvWeight);
   }
}

//example for nested histograms for different cut combinations
std::vector <std::vector <std::vector <std::vector <std::vector<TH1D* > > > > > test;
//define binning and how many steps for each cut
const int max_n = 8;
const int max_b = 6;
int Bmin[max_b] = {0,  1, 1, 2, 3};
int Bmax[max_b] = {30,30, 2, 3, 30};
const int max_h = 4;
int HTmin[max_h] = {0,500,750,1000};
const int max_m = 3;
const int max_s = 6;
int STmin[max_s] = {0,    250,    0, 250, 350, 450};
int STmax[max_s] = {4000, 4000, 250, 350, 450,4000};
Double_t MJmin[max_s] = {0,1,0};
Double_t MJmax[max_s] = {1,3.5,3,5};

int main (int argc, char* argv[]){
    if (argc < 4) {
        cout<<"usage ./TreeAnalyzer_Mu [INPUTFOLDER] [XSEC * LUMI] [SAMPLENAME]"<<endl;
        return 1;
    }
    cout<<argc<<" "<<argv[1]<<" "<<argv[2]<<endl;
    TString list = argv[1];
    for (int i = 2; i<argc-1;i++){
        list.Append(" ");
        list.Append(argv[i]);
    }
    TString outname = argv[argc-1];
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

    char hNAME[99];

    for(int n=0; n<max_n; n++){
        int iNjet=n+3;
        for(int b=0; b<max_b; b++){
            int iNbjetmin=Bmin[b];
            int iNbjetmax=Bmax[b];
            test.push_back(std::vector<std::vector<std::vector<std::vector<TH1D*> > > >());
            for(int h=0; h<max_h; h++){
                int iHT=HTmin[h];
                test[n].push_back(std::vector<std::vector<std::vector<TH1D*> > >());
                for(int m=0; m<max_m; m++){
                    //delphi replacement int iMJ=100*m;
                    int iMJ=m;
                    test[n][b].push_back(std::vector<std::vector<TH1D*> >());
                    for(int s=0; s<max_s; s++){
                        int iSTmin = STmin[s];
                        int iSTmax = STmax[s];
                        test[n][b][h].push_back(std::vector<TH1D*> ());
                        sprintf(hNAME, "test_njet%i_bjet%i-bjet%i_HT%i_Dphi%i_ST%i-ST%i", iNjet,iNbjetmin,iNbjetmax,iHT,iMJ,iSTmin,iSTmax);
                        test[n][b][h][m].push_back(new TH1D(hNAME,hNAME,250,0,2500));
                        test[n][b][h][m][s]->Sumw2();
                    }
                }
            }
        }
    }


    EasyChain* tree = new EasyChain("tree");
    for(unsigned i=0;i<files.size();i++){
        tree->AddSmartW(files[i],weights[i]);
        cout<<"add: "<<files[i]<<" "<<weights[i]<<endl;
    }
    int Nevents=tree->GetEntries();
    cout<<">>>>>>>>>>>>>>>>>>>>>>> total number of events:\t" << Nevents <<endl;
    // CutFlow variables
    int iCut = 0;
    double CFCounter[CutNumb];
    double statUnc[CutNumb];
    int iCFCounter[CutNumb];
  for (int i=0;i < CutNumb; i++){
          CFCounter[i] = 0;
         iCFCounter[i] = 0;
         statUnc[i] =0;
        }

   int cntJetBin = 0;
   for(int is=0; is<nStages; is++){
           sprintf(names, "%dnbJets%d_%3.0fHT%3.0f_%3.0fST%3.0f",cutLonbJetsArr[cntJetBin], cutHinbJetsArr[cntJetBin],cutLoHTArr[is], cutHiHTArr[is],cutLoSTArr[is], cutHiSTArr[is]);
                        TString namesT = names;
                              namesT.ReplaceAll(" -1", "inf");namesT.ReplaceAll(" 0", "0");
                                    std::cout<<"is : "<<is<<"  namesT : "<<namesT<<std::endl;
                                          keyStrForStages.push_back(namesT.Data());
                                                if(cutHiHTArr[is] ==-1  && cutHiSTArr[is] ==-1  ) cntJetBin++;
                                                  }

        std::vector<TH2D*> h2_NPassed;
        std::vector<TH1D*> h1_HT;
        std::vector<TH1D*> h1_HT_Inv;
       for(int is=0; is<nStages; is++){
       TString tmpKeyStrT = keyStrForStages[is];
       TString tmpKeyStrNPassedT = "NPassed_"+tmpKeyStrT;
       TH2D * h2_tmp = new TH2D(tmpKeyStrNPassedT, tmpKeyStrNPassedT, 28,100,800,16,0,800); h2_tmp->Sumw2();
       h2_NPassed.push_back((TH2D*)h2_tmp->Clone());
       TString tmpKeyStrHTT = "HT_"+tmpKeyStrT;
       TH1D * h1_tmp = new TH1D(tmpKeyStrHTT, tmpKeyStrHTT, 300,0,3000); h1_tmp->Sumw2();
       h1_HT.push_back((TH1D*)h1_tmp->Clone());
       tmpKeyStrHTT = "HT_Inv"+tmpKeyStrT;
       h1_tmp = new TH1D(tmpKeyStrHTT, tmpKeyStrHTT, 300,0,3000); h1_tmp->Sumw2();
       h1_HT_Inv.push_back((TH1D*)h1_tmp->Clone());
       TString tmpKeyStrNEvnT = "NEvn_"+tmpKeyStrT;
        }


        string outnameStr = (string)outname;
        string TTbarModes[2] = {"MC_TTbar_DiLep","MC_TTbar_SinLep"};
         double xbinNJ[5] ={3,4,6,7,12};
         double xbinNJn[3] ={3,6,12};
         double xbinHT[4] ={500,1000,3000};
         double xbinST[5] ={200,250,350,450,1000};


    cout << "Starting event loop" << endl;

    for(int entry=0; entry < Nevents/*min(1000000,Nevents)*/; entry+=1){

      if (entry % 1000 == 0)
                cout << "================= Processing entry: " << entry << '\r' << flush;

        //lumi calcualtion done in runAnalyzer.py (fb and pb)
    Double_t fw = tree->GetEntryW(entry);
    Double_t EvWeight = 1.0;
    EvWeight *= fw ;

        iCut = 0;

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
	if(debug) cout<<" GetFatJets"<<endl;
        Obj.GetFatJets(tree);
	if(debug) cout<<" GetKinVariables"<<endl;
	//check src/ClassObjects.C for what is available and implement new variables in there 
        //Obj.GetKinVariables(Obj.goodLep,Obj.goodJet,Obj.MET);
        Obj.GetKinVariables();

        // Define ST (needs to fixed for general use)


        // Fill main histograms
        FillMainHists(iCut, EvWeight);

        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;
        bool SoftHard_over =false;
        // 1. Cut
        //////////////////Require exactly one good Muon
        
        if (Obj.nLepGood != 1) continue;
        if( Obj.nMuGood != 1) continue;
        if(Obj.nMuVeto !=0 || Obj.nElVeto !=0) continue;
        // replace LepGood collection with SoftLepGood if you want to do the soft lep analysis
        /*
        if (Obj.nSoftLepGood != 1) continue;
        if( Obj.nSoftMuGood != 1) continue;
        if(Obj.nSoftMuVeto !=0 || Obj.nSoftElVeto !=0) continue;
        */
        FillMainHists(iCut, EvWeight);

        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;
/*
        //example for filling nested histograms
        for(int n=0; n<max_n; n++){
            int iNjet=n+3;
            //    if(Obj.nJetGood >= iNjet){
            for (int b=0; b<max_b; b++) {
                int iNbjetmin=Bmin[b];
                int iNbjetmax=Bmax[b];
                for (int h=0; h<max_h; h++){
                    Double_t iHT=HTmin[h];
                    for(int m=0; m<max_m; m++){
                        //Double_t iMJ=100.0*m;
                        Double_t iMJmin=MJmin[m];
                        Double_t iMJmax=MJmax[m];
                        for(int s=0; s<max_s; s++){
                            Double_t iSTmin = STmin[s];
                            Double_t iSTmax = STmax[s];
                            if(Obj.nJetGood >= iNjet){
                                if(Obj.nBJetGood >= iNbjetmin && Obj.nBJetGood < iNbjetmax){
                                    if(Obj.HT40 > iHT){
				      if(fabs(Obj.DelPhiWLep) >= iMJmin && fabs(Obj.DelPhiWLep) <= iMJmax){

                                            if(Obj.ST > iSTmin && Obj.ST < iSTmax){
                                                test[n][b][h][m][s]->Fill(Obj.ST, EvWeight);

                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
*/
        // 2. Cut
        ////////////////////////////
        if (Obj.nJetGood < 6) continue;

       if(!(Obj.goodJet[0].Pt()> 80 && Obj.goodJet[1].Pt()> 80)) continue;
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
        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;
        // 6. Cut
        ////////////////////////////
        if (Obj.DelPhiWLep < 1.0) continue; 
        FillMainHists(iCut, EvWeight);
        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;

/*
        if (Obj.nTopTagJetGood < 1) continue;
        FillMainHists(iCut, EvWeight);
        CFCounter[iCut]+= EvWeight;
        iCFCounter[iCut]++;
        iCut++;
   
*/   
        if(Obj.DelPhiWLep >1){
         hRS[6]->Fill(Obj.nJetGood);
         hRS_nTop[6]->Fill(Obj.nTopTagJetGood);
         if(Obj.nTopTagJetGood > 0)hRS_TopPt[6]->Fill(Obj.goodTopTagJet[0].Pt());
         }
        if(Obj.DelPhiWLep < 1){
         hRC[6]->Fill(Obj.nJetGood);
         hRC_nTop[6]->Fill(Obj.nTopTagJetGood);
         if(Obj.nTopTagJetGood > 0)hRC_TopPt[6]->Fill(Obj.goodTopTagJet[0].Pt());
         }



         if(Obj.nJetGood > 5){ 
          if(Obj.DelPhiWLep >1){
           
         cntJetBin =0;
         for(int is=0; is<nStages; is++){
         if(   (cutLonbJetsArr[cntJetBin] ==-1 || Obj.nBJetGood >= cutLonbJetsArr[cntJetBin])
            && (cutHinbJetsArr[cntJetBin] ==-1 || Obj.nBJetGood < cutHinbJetsArr[cntJetBin])
            && (cutLoHTArr[is] ==-1 || Obj.HT40 >= cutLoHTArr[is])
            && (cutHiHTArr[is] ==-1 || Obj.HT40 < cutHiHTArr[is])
            && (cutLoSTArr[is] ==-1 ||  Obj.ST >= cutLoSTArr[is])
            && (cutHiSTArr[is] ==-1 ||  Obj.ST < cutHiSTArr[is]) ){
             h2_NPassed[is]->Fill( Obj.ST,Obj.HT40);
             h1_HT[is]->Fill( Obj.HT40,EvWeight);
              }
         if( cutHiHTArr[is] ==-1 && cutHiSTArr[is] == -1 ) cntJetBin++;
              //cout<<cntJetBin<<endl;
            }
        }
      }


if(Obj.nJetGood > 5){
          if(Obj.DelPhiWLep <1){
         cntJetBin =0;
         for(int is=0; is<nStages; is++){
         if(   (cutLonbJetsArr[cntJetBin] ==-1 || Obj.nBJetGood >= cutLonbJetsArr[cntJetBin])
            && (cutHinbJetsArr[cntJetBin] ==-1 || Obj.nBJetGood < cutHinbJetsArr[cntJetBin])
            && (cutLoHTArr[is] ==-1 || Obj.HT40 >= cutLoHTArr[is])
            && (cutHiHTArr[is] ==-1 || Obj.HT40 < cutHiHTArr[is])
            && (cutLoSTArr[is] ==-1 ||  Obj.ST >= cutLoSTArr[is])
            && (cutHiSTArr[is] ==-1 ||  Obj.ST < cutHiSTArr[is]) ){
             h1_HT_Inv[is]->Fill( Obj.HT40,EvWeight);
              }
         if( cutHiHTArr[is] ==-1 && cutHiSTArr[is] == -1 ) cntJetBin++;
            }
        }
      }

    }//event loop

    cout << endl << "Finished event loop" << endl;
    for (int i=0;i<CutNumb;++i){
                 if (iCFCounter[i] <0.2) statUnc[i] =0;
                else statUnc[i] = CFCounter[i]/sqrt(iCFCounter[i]);
        }

    //write out cutflow
    ofstream tfile;
    TString textfilename = "CMG_cutflow_"+outname+".txt";
    tfile.open(textfilename);
    tfile << "########################################" << endl;
    tfile << "Cut efficiency numbers:" << endl;
   
       for(int ci = 0; ci < CutNumb; ci++)
        {
                tfile << "After cut " << CutList[ci] <<  "\t\t"
                      << CFCounter[ci]  << "\t events left\t"<< statUnc[ci] <<"\t SYS Unc\t"<< iCFCounter[ci] <<" cnt\t"<< endl;
                CutFlow->SetBinContent(1+ci,CFCounter[ci]);
        }

        for(int is=0; is<nStages; is++){
       TString tmpKeyStrT = keyStrForStages[is];
                tfile << "search bin " << tmpKeyStrT <<  "\t\t"
         << h1_HT[is]->Integral()<< "\t events left\t"<< h1_HT[is]->Integral()/sqrt(h1_HT[is]->GetEntries()) <<"\t SYS Unc\t"<<h1_HT[is]->GetEntries() <<" cnt\t"<< endl;
          }
/*

    for(int ci = 0; ci < CutNumb; ci++)
    {
        tfile << "After cut " << CutList[ci] << "\t\t"
              << CFCounter[ci] << "\t events left\t"<< iCFCounter[ci] <<" cnt\t"<< endl;
        CutFlow->SetBinContent(1+ci,CFCounter[ci]);
    }
*/
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
        hHT[cj]->Write();
        hST[cj]->Write();
        hToppT[cj]->Write();
        hnTop[cj]->Write();
        hWTagpT[cj]->Write();
        hWTagMass[cj]->Write();
        hnW[cj]->Write();
        hWmassTagpT[cj]->Write();
        hWmassTagMass[cj]->Write();
        hnWmass[cj]->Write();
        hdPhiWLep[cj]->Write();
        hdPhiJMET[cj]->Write();
        hRC[cj]->Write();
        hRC_nTop[cj]->Write();
        hRC_TopPt[cj]->Write();
        hRS[cj]->Write();
        hRS_nTop[cj]->Write();
        hRS_TopPt[cj]->Write();

    }
    cout<<"done with normal histograms"<<endl;
/*
    TDirectory* bins=outf->mkdir("Bins");
    outf->cd("Bins");
    for(int n=0; n<max_n; n++){
        int iNjet=n+3;
        sprintf(FOLDER, "Njet_%i", iNjet);
        TDirectory* now=bins->mkdir(FOLDER);
        bins->cd(FOLDER);
        for (int b=0; b<max_b; b++) {
            int iNbjetmin=Bmin[b];
            int iNbjetmax=Bmax[b];
            sprintf(FOLDER, "bjet_%i-bjet_%i", iNbjetmin,iNbjetmax);
            TDirectory* now2=now->mkdir(FOLDER);
            now->cd(FOLDER);
            for (int h=0; h<max_h; h++){
                int iHT=HTmin[h];
                sprintf(FOLDER,"HT_%i", iHT);
                TDirectory* now3=now2->mkdir(FOLDER);
                now2->cd(FOLDER);
                for(int m=0; m<max_m; m++){
                    for(int s=0; s<max_s; s++){

                        test[n][b][h][m][s]->Write();
                    }
                }
            }
        }
    }
    cout<<"done with nested histograms"<<endl;
*/

}
