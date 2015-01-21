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

void TreeWriter(TString list, TString outname,bool useW=true){

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

    EasyChain* chain = new EasyChain("treeProducerSusySingleLepton");

    for(unsigned i=0;i<files.size();i++){
        chain->AddSmartW(files[i],weights[i]);
        cout<<"add: "<<files[i]<<" "<<weights[i]<<endl;
    }

    int Nevents=chain->GetEntries();
    cout<<">>>>>>>>>>>>>>>>>>>>>>> total number of events:\t" << Nevents <<endl;

    // Initialize Tree
    TTree *tree = new TTree("tree","skimmed CMG tuple");

    // tree variables
//    Event *e = new Event;
    int Nlep;
    int NlepVeto;
    int Nel;
    int Nmu;

    double LeadLepPt;
    double LeadMuPt;
    double LeadElPt;

    int NBjets;
    int Njets;

    double LeadJetPt;
    double LeadBJetPt;

    double dMET;
    double dHT;
    double dST;

    float EvWeight = 1;

    float DelPhiMetMu;
    float DelPhiWlep;
    float DelPhiMetMu2;
    float DelPhiWlep2;

    float Wpt;
    float MT;

    // tree branches
    tree->Branch("Weight",&EvWeight);

    // leptons
    tree->Branch("Nlep",&Nlep);
    tree->Branch("NlepVeto",&NlepVeto);
    tree->Branch("Nel",&Nel);
    tree->Branch("Nmu",&Nmu);

    // kinematic
    tree->Branch("LeadLepPt",&LeadLepPt);
    tree->Branch("LeadMuPt",&LeadMuPt);
    tree->Branch("LeadElPt",&LeadElPt);

    // jets
    tree->Branch("NBjets",&NBjets);
    tree->Branch("Njets",&Njets);

    tree->Branch("LeadJetPt",&LeadJetPt);
    tree->Branch("LeadBJetPt",&LeadBJetPt);

    // sums
    tree->Branch("MET",&dMET);
    tree->Branch("HT",&dHT);
    tree->Branch("ST",&dST);

    // special
    tree->Branch("DelPhiMetMu",&DelPhiMetMu);
    tree->Branch("DelPhiWlep",&DelPhiWlep);
    tree->Branch("DelPhiMetMu2",&DelPhiMetMu2);
    tree->Branch("DelPhiWlep2",&DelPhiWlep2);
    tree->Branch("Wpt",&Wpt);
    tree->Branch("MT",&MT);

    /////////////   event loop   //////////////////////
    for(int entry=0; entry <  Nevents/*min(100000,Nevents)*/; entry+=1){
//    for(int entry=0; entry <  100000; entry+=1){

        // Read Chain
        Float_t fw = chain->GetEntryW(entry);
        progressT();

        //lumi calcualtion done in runAnalyzer.py (fb and pb)
        EvWeight = fw;

        // Get Objects from chain
        GetLeptons(chain);
        GetJets(chain);

        Nlep = goodLep.size();
        NlepVeto = nLepVeto;
        Nel = goodEl.size();
        Nmu = goodMu.size();


        // skim
        if (Nlep < 1)
            continue;

        if (NlepVeto > 0)
            continue;

        if (Nlep > 0)
            LeadLepPt = goodLep[0].Pt();
        if (Nmu > 0)
            LeadMuPt = goodMu[0].Pt();
        if (Nel > 0)
            LeadElPt = goodEl[0].Pt();

        NBjets = goodBJet.size();
        Njets = goodJet.size();

        if (Njets > 0)
            LeadJetPt = goodJet[0].Pt();

        if (Njets < 2)
            continue;

        if (NBjets > 0)
            LeadBJetPt = goodBJet[0].Pt();

        GetMET(chain);

        dMET = MET.Pt();
        dHT = HT40;

        if(Nlep > 0)
            dST = dMET + LeadLepPt;

        // special

        TLorentzVector WBos = MET + goodMu[0];
	DelPhiWlep = fabs(WBos.DeltaPhi(goodLep[0]));
	DelPhiMetMu =  fabs(MET.DeltaPhi(goodMu[0]));

//        DelPhiMetMu = fabs(TVector2::Phi_mpi_pi(MET.Phi() - goodMu[0].Phi()));

/*
        DelPhiMetMu2 = fabs(MET.Phi() - goodMu[0].Phi());
        if (DelPhiMetMu2 > acos(-1.0))
            DelPhiMetMu2 -= 2*acos(-1.0);
*/

//        DelPhiWlep = fabs(TVector2::Phi_mpi_pi(WBos.Phi() - goodMu[0].Phi()));

/*
	DelPhiWlep2 = (WBos.Phi() - goodMu[0].Phi());
        if (DelPhiWlep2 > TMath::Pi())
            DelPhiWlep2 -= 2*TMath::Pi();
        if (DelPhiWlep2 <= -TMath::Pi())
            DelPhiWlep2 += 2*TMath::Pi();
*/

        Wpt = WBos.Pt();
        MT = sqrt(pow((goodMu[0].Et()+MET.Et()),2)-pow((goodMu[0].Px()+MET.Px()),2)-pow((goodMu[0].Py()+MET.Py()),2));
/*
  GetGenLeptons(chain);
  GetGenMET(chain);
*/

        tree->Fill();
    }
/////////////////////////////////////////END HERE
// ^loop end^

    TString rootfilename = "CMG_"+outname+"_tree.root";
    TFile * outf  = new TFile(rootfilename,"RECREATE");

    tree->Write();

    outf->Close();
}
