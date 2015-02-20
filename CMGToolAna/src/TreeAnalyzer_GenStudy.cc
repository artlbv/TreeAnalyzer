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

// instance of the Objects class with functionality defined in ClassObjects.C
GetObjects Obj;
bool debug = false;

// define global hists

//TH1F* hElIso = new TH1F ("ElIso","Electron isolation",100,0.0,10.0);
TH1F* hElDrGen = new TH1F ("ElDr","Electron dR to gen particles",100,0.0,10.0);
TH1F* hElMinDrGen = new TH1F ("ElMinDr","Minumum Electron dR to gen particles",100,0.0,1.0);
TH1F* hElGenMatchPdgID = new TH1F ("ElGenMatchId","PdgID of matched gen particle",50,-24.5,24.5);

TH1F* hElPtMatch = new TH1F ("hElPtMatch","Pt of matched El",100,0,500);
TH1F* hElPtNonMatch = new TH1F ("hElPtNonMatch","Pt of non matched El",100,0,500);

TH1F* hPhoPt = new TH1F ("hPhoPt","Pt of photon",100,0,500);

// miniIso
TH1F* hElMiniIsoMatch = new TH1F ("ElminiIsoMatch","Matched Electron miniIso",100,0.0,2.0);
TH1F* hElMiniIsoNonMatch = new TH1F ("ElminiIsoNonMatch","Non-Matched Electron miniIso",100,0.0,2.0);

// relIso
TH1F* hElRelIsoMatch = new TH1F ("ElrelisoMatch","Matched Electron relIso",100,0.0,2.0);
TH1F* hElRelIsoNonMatch = new TH1F ("ElrelisoNonMatch","Non-Matched Electron relIso",100,0.0,2.0);

// Muons
//TH1F* hMuIso = new TH1F ("MuIso","Muon isolation",100,0.0,10.0);
TH1F* hMuDrGen = new TH1F ("MuDr","Muon dR to gen particles",100,0.0,10.0);
TH1F* hMuMinDrGen = new TH1F ("MuMinDr","Minumum Muon dR to gen particles",100,0.0,1.0);
TH1F* hMuGenMatchPdgID = new TH1F ("MuGenMatchId","PdgID of matched gen particle",50,-24.5,24.5);

// miniIso
TH1F* hMuMiniIsoMatch = new TH1F ("MuminiIsoMatch","Matched Muon miniIso",100,0.0,2.0);
TH1F* hMuMiniIsoNonMatch = new TH1F ("MuminiIsoNonMatch","Non-Matched Muon miniIso",100,0.0,2.0);

// relIso
TH1F* hMuRelIsoMatch = new TH1F ("MurelisoMatch","Matched Muon relIso",100,0.0,2.0);
TH1F* hMuRelIsoNonMatch = new TH1F ("MurelisoNonMatch","Non-Matched Muon relIso",100,0.0,2.0);


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
    vector<float> weights;
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

    string outnameStr = (string)outname;

    cout << "Starting event loop" << endl;

    int maxEvents = min(4000000,Nevents);
//    int maxEvents = Nevents;

    for(int entry=0; entry < maxEvents; entry+=1){

        if (entry % 10000 == 0)
            cout << "================= Processing entry: " << entry << "\t(" << 100*entry/maxEvents <<  "% done)" << '\r' << flush;

        //lumi calcualtion done in runAnalyzer.py (fb and pb)
        Float_t fw = tree->GetEntryW(entry);
        Float_t EvWeight = 1.0;
        EvWeight *= fw ;

	// selection
	Obj.GetJets(tree);
	Obj.GetKinVariables();

	if ( Obj.HT40 < 750) continue;

        //get all objects
        Obj.GetLeptons(tree,"genID","genID");
//        Obj.GetGenParticles(tree);
        Obj.GetGenLeptonsFromTau(tree);
        Obj.GetGenLeptons(tree);


/*
  if(Obj.nMuGood || Obj.nGenPart){
  cout << "Event\t" << entry << " has\t" << Obj.nMuGood << " muons "
  << "and\t" << Obj.nGenPart << " gen particles" << endl;
  }
*/
        // Leptons
        for(int ilep = 0; ilep < Obj.nLepGood; ilep++){
            // lepton is Obj.goodLep

            int lepId = abs(Obj.goodLep[ilep].pdgId);

            float maxDr = 0.1;
            float minDr = 9999.;
            float matchIndx = -1;

	    bool matched = false;

	    // define reference objects: genPart, genLep or genLepFromTau
	    //vector<GenParticle> refGen = Obj.genPart;
	    vector<GenLepton> refGen = Obj.genLep;
	    //vector<GenLepton> refGen = Obj.genLepFromTau;
	    // adding more collections (like genLepFromTau)
	    refGen.insert(refGen.end(), Obj.genLepFromTau.begin(), Obj.genLepFromTau.end());

//	    cout << "Number of reference objects: " << refGen.size() << endl;

            // 1. loop through gen particles
            for (int iref = 0; iref < refGen.size(); iref++){

                int genId = abs(refGen[iref].pdgId);

		// check same ID
                if (lepId != genId) continue;

		// check W or tau mother
                if (!(abs(refGen[iref].motherId) != 24 || abs(refGen[iref].motherId) != 15)) continue;

                // relDeltaPt < 0.3
                if (abs(1 - Obj.goodLep[ilep].Pt()/refGen[iref].Pt()) > 0.3) continue;

                // 2. calc dR
                float tmpDr = Obj.goodLep[ilep].DeltaR((TLorentzVector) refGen[iref]);

                if (lepId == 11) hElDrGen ->Fill(tmpDr, EvWeight);
                if (lepId == 13) hMuDrGen ->Fill(tmpDr, EvWeight);

                // 3. check maxDr
                if (tmpDr < maxDr){

		    if (matched)
			cout << "Double Matched in " << entry << " to indx " << matchIndx << " and " << iref << endl;

		    matched = true;
                    matchIndx = iref;
		    minDr = tmpDr;
		}
            }

            if( matched ){
                int pdg = refGen[matchIndx].pdgId;

                if (lepId == 11){
//                    cout << "Found El match in event " << entry << " with dR\t" << minDr <<  " and pdgID\t" << pdg << endl;
                    hElMinDrGen ->Fill(minDr, EvWeight);
                    hElGenMatchPdgID ->Fill(pdg, EvWeight);

		    hElPtMatch->Fill(Obj.goodLep[ilep].Pt());
		    if (pdg == 22) hPhoPt->Fill(refGen[matchIndx].Pt());

		    hElMiniIsoMatch->Fill(Obj.goodLep[ilep].miniRelIso,EvWeight);
		    hElRelIsoMatch->Fill(Obj.goodLep[ilep].relIso03,EvWeight);
                }


                if (lepId == 13){
//                    cout << "Found Mu match in event " << entry << " with dR\t" << minDr <<  " and pdgID\t" << pdg << endl;
                    hMuMinDrGen ->Fill(minDr, EvWeight);
                    hMuGenMatchPdgID ->Fill(pdg, EvWeight);

		    hMuMiniIsoMatch->Fill(Obj.goodLep[ilep].miniRelIso,EvWeight);
		    hMuRelIsoMatch->Fill(Obj.goodLep[ilep].relIso03,EvWeight);

                }
            }
	    else{
                if (lepId == 11){
		    hElMiniIsoNonMatch->Fill(Obj.goodLep[ilep].miniRelIso,EvWeight);
		    hElRelIsoNonMatch->Fill(Obj.goodLep[ilep].relIso03,EvWeight);

		    hElPtNonMatch->Fill(Obj.goodLep[ilep].Pt());
                }
		if (lepId == 13){
		    hMuMiniIsoNonMatch->Fill(Obj.goodLep[ilep].miniRelIso,EvWeight);
		    hMuRelIsoNonMatch->Fill(Obj.goodLep[ilep].relIso03,EvWeight);
		}
	    }
        }
    }

    cout << endl << "Finished event loop" << endl;

    cout << "Found " << hElGenMatchPdgID->GetEntries() << "\t matched ele" << endl;

    //write out histograms
    TFile * outf;
    TString rootfilename = "gen_CMG_"+outname+"_his.root";
    outf = new TFile(rootfilename,"RECREATE");

    // Write Hists out
    //first the main histograms
    cout<<"start with normal histograms"<<endl;

    hElDrGen->Write();
    hElMinDrGen->Write();
    hElGenMatchPdgID->Write();

    hElPtMatch->Write();
    hElPtNonMatch->Write();
    hPhoPt->Write();

    hElMiniIsoMatch->Write();
    hElMiniIsoNonMatch->Write();
    hElRelIsoMatch->Write();
    hElRelIsoNonMatch->Write();

    hMuDrGen->Write();
    hMuMinDrGen->Write();
    hMuGenMatchPdgID->Write();

    hMuMiniIsoMatch->Write();
    hMuMiniIsoNonMatch->Write();
    hMuRelIsoMatch->Write();
    hMuRelIsoNonMatch->Write();



    cout<<"done with normal histograms"<<endl;

}
