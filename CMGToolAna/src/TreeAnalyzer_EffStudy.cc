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
//Float_t XbinsPt[] = { 0, 25 , 30 , 35 , 40 , 45 , 50 , 55 , 60 , 65 , 70 , 75 , 80 , 85 , 90 , 95 , 100 , 120 , 140 , 160 , 180, 200 };
Float_t XbinsPt[] = { 0, 25 , 30 , 35 , 40 , 50 , 60 , 70 , 80 , 100 , 120 , 140 , 160 , 180, 200, 250, 300, 400, 500 };
Int_t  binnum = sizeof(XbinsPt)/sizeof(Float_t) - 1;
//TH1F* h = new TH1F("n","t", binnum, bins);

/*
  Electrons
  Same width histograms
*/

// hists for matching
TH1F* hElDrGen = new TH1F ("ElDr","Electron dR to gen particles",100,0.0,10.0);
TH1F* hElMinDrGen = new TH1F ("ElMinDr","Minumum Electron dR to gen particles",100,0.0,1.0);
TH1F* hElGenMatchPdgID = new TH1F ("ElGenMatchId","PdgID of matched gen particle",50,-24.5,24.5);

// reco electron
TH1F* hElPtMatch = new TH1F ("ElPtMatch","Pt of matched El",100,0,500);
TH1F* hElPtMatchNonPrompt = new TH1F ("ElPtMatchNonPrompt","Pt of non-prompt matched El",100,0,500);
TH1F* hElPtNonMatch = new TH1F ("ElPtNonMatch","Pt of non matched El",100,0,500);
TH1F* hElPt = new TH1F ("ElPt","Pt of all El",100,0,500);

// gen electron
TH1F* hGenElPtMatch = new TH1F ("GenElPtMatch","Pt of matched prompt gen El",100,0,500);
TH1F* hGenElPtMatchNonPrompt = new TH1F ("GenElPtMatchNonPrompt","Pt of matched with non-prompt gen El",100,0,500);
TH1F* hGenElPtNonMatch = new TH1F ("GenElPtNonMatch","Pt of non matched gen El",100,0,500);
TH1F* hGenElPt = new TH1F ("GenElPt","Pt of all gen El",100,0,500);

// efficiency
TH1F* hElPtMatchEff = new TH1F ("ElPtMatchEff","Efficiency of matched El",100,0,500);
TH1F* hElPtMatchNonPromptEff = new TH1F ("ElPtMatchNonPromptEff","Efficiency of non prompt matched El",100,0,500);
TH1F* hElPtNonMatchEff = new TH1F ("ElPtNonMatchEff","Efficiency of non matched El",100,0,500);

// miniIso
TH1F* hElMiniIsoMatch = new TH1F ("ElminiIsoMatch","Matched Electron miniIso",100,0.0,2.0);
TH1F* hElMiniIsoNonMatch = new TH1F ("ElminiIsoNonMatch","Non-Matched Electron miniIso",100,0.0,2.0);

// relIso
TH1F* hElRelIsoMatch = new TH1F ("ElrelisoMatch","Matched Electron relIso",100,0.0,2.0);
TH1F* hElRelIsoNonMatch = new TH1F ("ElrelisoNonMatch","Non-Matched Electron relIso",100,0.0,2.0);

/*
  Electrons
  Var width histograms
*/

TH1F* xhElPtMatch = new TH1F ("xhElPtMatch","Pt of matched El",binnum,XbinsPt);
TH1F* xhElPtMatchNonPrompt = new TH1F ("xhElPtMatchNonPrompt","Pt of matched to non-prompt El",binnum,XbinsPt);
TH1F* xhElPtNonMatch = new TH1F ("xhElPtNonMatch","Pt of non matched El",binnum,XbinsPt);
TH1F* xhElPt = new TH1F ("xhElPt","Pt of all El",binnum,XbinsPt);
// efficiency
TH1F* xhElPtMatchEff = new TH1F ("xhElPtMatchEff","Efficiency of matched El",binnum,XbinsPt);
TH1F* xhElPtMatchNonPromptEff = new TH1F ("xhElPtMatchNonPromptEff","Efficiency of non prompt matched El",binnum,XbinsPt);
TH1F* xhElPtNonMatchEff = new TH1F ("xhElPtNonMatchEff","Efficiency of non matched El",binnum,XbinsPt);

/*
  Muons
  Var width histograms
*/
// hists for matching
TH1F* hMuDrGen = new TH1F ("MuDr","Muon dR to gen particles",100,0.0,10.0);
TH1F* hMuMinDrGen = new TH1F ("MuMinDr","Minumum Muon dR to gen particles",100,0.0,1.0);
TH1F* hMuGenMatchPdgID = new TH1F ("MuGenMatchId","PdgID of matched gen particle",50,-24.5,24.5);

// reco electron
TH1F* hMuPtMatch = new TH1F ("MuPtMatch","Pt of matched Mu",100,0,500);
TH1F* hMuPtNonMatch = new TH1F ("MuPtNonMatch","Pt of non matched Mu",100,0,500);
TH1F* hMuPt = new TH1F ("MuPt","Pt of all Mu",100,0,500);

// gen electron
TH1F* hGenMuPtMatch = new TH1F ("GenMuPtMatch","Pt of matched gen Mu",100,0,500);
TH1F* hGenMuPtNonMatch = new TH1F ("GenMuPtNonMatch","Pt of non matched gen Mu",100,0,500);
TH1F* hGenMuPt = new TH1F ("GenMuPt","Pt of all gen Mu",100,0,500);

// efficiency
TH1F* hMuPtMatchEff = new TH1F ("MuPtMatchEff","Efficiency of matched Mu",100,0,500);
TH1F* hMuPtNonMatchEff = new TH1F ("MuPtNonMatchEff","Efficiency of non matched Mu",100,0,500);

// miniIso
TH1F* hMuMiniIsoMatch = new TH1F ("MuminiIsoMatch","Matched Muon miniIso",100,0.0,2.0);
TH1F* hMuMiniIsoNonMatch = new TH1F ("MuminiIsoNonMatch","Non-Matched Muon miniIso",100,0.0,2.0);

// relIso
TH1F* hMuRelIsoMatch = new TH1F ("MurelisoMatch","Matched Muon relIso",100,0.0,2.0);
TH1F* hMuRelIsoNonMatch = new TH1F ("MurelisoNonMatch","Non-Matched Muon relIso",100,0.0,2.0);

/*
  Muons
  Var width histograms
*/

TH1F* xhMuPtMatch = new TH1F ("xhMuPtMatch","Pt of matched Mu",binnum,XbinsPt);
TH1F* xhMuPtNonMatch = new TH1F ("xhMuPtNonMatch","Pt of non matched Mu",binnum,XbinsPt);
TH1F* xhMuPt = new TH1F ("xhMuPt","Pt of all Mu",binnum,XbinsPt);
// efficiency
TH1F* xhMuPtMatchEff = new TH1F ("xhMuPtMatchEff","Efficiency of matched Mu",binnum,XbinsPt);
TH1F* xhMuPtNonMatchEff = new TH1F ("xhMuPtNonMatchEff","Efficiency of non matched Mu",binnum,XbinsPt);


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

    int maxEvents = min(1000000,Nevents);
//    int maxEvents = Nevents;

    for(int entry=0; entry < maxEvents; entry+=1){

        if (entry % 10000 == 0)
            cout << "================= Processing entry: " << entry <<  " (" << 100*entry/maxEvents <<  "% done)" << '\r' << flush;

        //lumi calcualtion done in runAnalyzer.py (fb and pb)
        Float_t fw = tree->GetEntryW(entry);
        Float_t EvWeight = 1.0;
        EvWeight *= fw ;

        // Pre-selection
	/*
        Obj.GetJets(tree);
        Obj.GetKinVariables();
        if ( Obj.HT40 < 1250) continue;
	*/

        // Get all objects
        Obj.GetLeptons(tree,"genID","genID");
//        Obj.GetLeptons(tree);

        // select only with 1 lepton
//	if (Obj.nGenLep + Obj.nGenLepFromTau == 0) continue;
        if (Obj.nLepGood == 0 ) continue;

	// Define 'tag'(reference) and 'probe' collections
        // define reference objects: genPart, genLep or genLepFromTau
        //vector<GenParticle> refPart = Obj.genPart;
        vector<Lepton> refPart = Obj.goodLep;
        //vector<GenLepton> refPart = Obj.genLep;
        // adding more collections (like genLepFromTau)
        //refPart.insert(refPart.end(), Obj.genLepFromTau.begin(), Obj.genLepFromTau.end());

	// define probe collection
//        vector<Lepton> probe = Obj.goodLep;
	// GenParticles as probe
        Obj.GetGenParticles(tree);
	vector<GenParticle> probe = Obj.genPart;

	// GenLeptons as probe
//        Obj.GetGenLeptonsFromTau(tree);
//        Obj.GetGenLeptons(tree);
//	vector<GenLepton> probe = Obj.genLep;
//	probe.insert(probe.end(), Obj.genLepFromTau.begin(), Obj.genLepFromTau.end());
//        vector<GenParticle> probe = Obj.genPart;

//        cout << "Number of reference objects: " << refPart.size() << endl;
//        cout << "Number of probe objects: " << probe.size() << endl;

	// Loop through Tag/reference objects (gen particles)
        for(int iref = 0; iref < refPart.size(); iref++){

            int tagId = abs(refPart[iref].pdgId);

	    // kinematic selection
	    if (abs(refPart[iref].Eta()) > 2.4) continue;
	    if (abs(refPart[iref].Pt()) < 25) continue;

            // fill reference lepton Pt
	    if (tagId == 11){
		hElPt->Fill(refPart[iref].Pt());
		xhElPt->Fill(refPart[iref].Pt());
	    }
	    if (tagId == 13){
		hMuPt->Fill(refPart[iref].Pt());
		xhMuPt->Fill(refPart[iref].Pt());
	    }

            float maxDr = 0.1;
            float minDr = 9999.;
            float matchIndx = -1;
            bool matched = false;

            // Loop through gen particles
            for (int iprobe = 0; iprobe < probe.size(); iprobe++){

                int probeId = abs(probe[iprobe].pdgId);

                // check same ID
                if (tagId != probeId) continue;

                // check W or tau mother
                //if (!(abs(refPart[iref].motherId) != 24 || abs(refPart[iref].motherId) != 15)) continue;
		//if (!(abs(probe[iprobe].motherId) != 24 || abs(probe[iprobe].motherId) != 15)) continue;

                // relDeltaPt < 0.3
                if (abs(1 - probe[iprobe].Pt()/refPart[iref].Pt()) > 0.3) continue;

                // calc dR
                float tmpDr = refPart[iref].DeltaR((TLorentzVector) probe[iprobe]);

                if (tagId == 11) hElDrGen ->Fill(tmpDr, EvWeight);
                if (tagId == 13) hMuDrGen ->Fill(tmpDr, EvWeight);

                // check maxDr
                if (tmpDr < maxDr && tmpDr < minDr){

                    if (matched && false)
                        cout << endl << "Double Matched in " << entry << " to indx " << matchIndx << " and " << iprobe << endl;

                    matched = true;
                    matchIndx = iprobe;
                    minDr = tmpDr;
                }
            }

	    // Fill plots if matched reference to probe
            if( matched ){

		// get probe infromation
                int pdg = probe[matchIndx].pdgId;
		bool prompt = false;
		if (abs(probe[matchIndx].motherId) == 24 || abs(probe[matchIndx].motherId) == 15) prompt = true;

                if (tagId == 11){
//                    cout << "Found El match in event " << entry << " with dR\t" << minDr <<  " and pdgID\t" << pdg << endl;
                    hElMinDrGen ->Fill(minDr, EvWeight);
                    hElGenMatchPdgID ->Fill(pdg, EvWeight);

		    if (prompt){
                    hElPtMatch->Fill(refPart[iref].Pt());
                    xhElPtMatch->Fill(refPart[iref].Pt());
                    hGenElPtMatch->Fill(probe[matchIndx].Pt());
		    }
		    else{
			hElPtMatchNonPrompt->Fill(refPart[iref].Pt());
			xhElPtMatchNonPrompt->Fill(refPart[iref].Pt());
			hGenElPtMatchNonPrompt->Fill(refPart[iref].Pt());
		    }
//                  if (pdg == 22) hPhoPt->Fill(refPart[matchIndx].Pt());

                    hElMiniIsoMatch->Fill(refPart[iref].miniRelIso,EvWeight);
                    hElRelIsoMatch->Fill(refPart[iref].relIso03,EvWeight);
                }


                if (tagId == 13 && prompt){
//                    cout << "Found Mu match in event " << entry << " with dR\t" << minDr <<  " and pdgID\t" << pdg << endl;
                    hMuMinDrGen ->Fill(minDr, EvWeight);
                    hMuGenMatchPdgID ->Fill(pdg, EvWeight);

                    hMuPtMatch->Fill(refPart[iref].Pt());
                    xhMuPtMatch->Fill(refPart[iref].Pt());
                    hGenMuPtMatch->Fill(probe[matchIndx].Pt());

                    hMuMiniIsoMatch->Fill(refPart[iref].miniRelIso,EvWeight);
                    hMuRelIsoMatch->Fill(refPart[iref].relIso03,EvWeight);

                }
            }
	    // non matched object
            else{
                if (tagId == 11){

//                    hGenElPtNonMatch->Fill(refPart[iref].Pt());

                    hElMiniIsoNonMatch->Fill(refPart[iref].miniRelIso,EvWeight);
                    hElRelIsoNonMatch->Fill(refPart[iref].relIso03,EvWeight);

                    hElPtNonMatch->Fill(refPart[iref].Pt());
                    xhElPtNonMatch->Fill(refPart[iref].Pt());

                }
                if (tagId == 13){
//                    hGenMuPtNonMatch->Fill(refPart[iref].Pt());

                    hMuMiniIsoNonMatch->Fill(refPart[iref].miniRelIso,EvWeight);
                    hMuRelIsoNonMatch->Fill(refPart[iref].relIso03,EvWeight);

                    hMuPtNonMatch->Fill(refPart[iref].Pt());
                    xhMuPtNonMatch->Fill(refPart[iref].Pt());
                }
            }
        }
    }

    cout << endl << "Finished event loop" << endl;

    cout << "Found " << hElPtMatch->GetEntries() << "\t matched prompt electrons" << endl;
    cout << "Found " << hElPtMatchNonPrompt->GetEntries() << "\t matched non prompt electrons" << endl;
    cout << "Found " << hElPtNonMatch->GetEntries() << "\t un-matched electrons" << endl;
    cout << "Found " << hMuPtMatch->GetEntries() << "\t matched muons" << endl;
    cout << "Found " << hMuPtNonMatch->GetEntries() << "\t un-matched muons" << endl;

    //write out histograms
    TFile * outf;
    TString rootfilename = "eff_CMG_"+outname+"_his.root";
    outf = new TFile(rootfilename,"RECREATE");

    // calc efficiencies
    hElPtMatchEff->Add(hElPtMatch);
    hElPtMatchEff->Divide(hElPt);

    hElPtMatchNonPromptEff->Add(hElPtMatchNonPrompt);
    hElPtMatchNonPromptEff->Divide(hElPt);

    hElPtNonMatchEff->Add(hElPtNonMatch);
    hElPtNonMatchEff->Divide(hElPt);

    hMuPtMatchEff->Add(hMuPtMatch);
    hMuPtMatchEff->Divide(hMuPt);

    hMuPtNonMatchEff->Add(hMuPtNonMatch);
    hMuPtNonMatchEff->Divide(hMuPt);

    // calc efficiencies
    xhElPtMatchEff->Add(xhElPtMatch);
    xhElPtMatchEff->Divide(xhElPt);

    xhElPtMatchNonPromptEff->Add(xhElPtMatchNonPrompt);
    xhElPtMatchNonPromptEff->Divide(xhElPt);

    xhElPtNonMatchEff->Add(xhElPtNonMatch);
    xhElPtNonMatchEff->Divide(xhElPt);

    xhMuPtMatchEff->Add(xhMuPtMatch);
    xhMuPtMatchEff->Divide(xhMuPt);

    xhMuPtNonMatchEff->Add(xhMuPtNonMatch);
    xhMuPtNonMatchEff->Divide(xhMuPt);


    // Write Hists out
    //first the main histograms
    cout<<"start with normal histograms"<<endl;

    hElDrGen->Write();
    hElMinDrGen->Write();
    hElGenMatchPdgID->Write();

    hElPt->Write();
    hElPtMatch->Write();
    hElPtMatchNonPrompt->Write();
    hElPtNonMatch->Write();

    xhElPt->Write();
    xhElPtMatch->Write();
    xhElPtMatchNonPrompt->Write();
    xhElPtNonMatch->Write();

    hGenElPtMatch->Write();
    hGenElPtNonMatch->Write();
    hGenElPt->Write();

    hElPtMatchEff->Write();
    hElPtMatchNonPromptEff->Write();
    hElPtNonMatchEff->Write();

    xhElPtMatchEff->Write();
    xhElPtMatchNonPromptEff->Write();
    xhElPtNonMatchEff->Write();

//    hPhoPt->Write();

    hElMiniIsoMatch->Write();
    hElMiniIsoNonMatch->Write();
    hElRelIsoMatch->Write();
    hElRelIsoNonMatch->Write();

    hMuPt->Write();
    hMuPtMatch->Write();
    hMuPtNonMatch->Write();

    xhMuPt->Write();
    xhMuPtMatch->Write();
    xhMuPtNonMatch->Write();

    hGenMuPtMatch->Write();
    hGenMuPtNonMatch->Write();
    hGenMuPt->Write();

    hMuPtMatchEff->Write();
    hMuPtNonMatchEff->Write();

    xhMuPtMatchEff->Write();
    xhMuPtNonMatchEff->Write();

    hMuDrGen->Write();
    hMuMinDrGen->Write();
    hMuGenMatchPdgID->Write();

    hMuMiniIsoMatch->Write();
    hMuMiniIsoNonMatch->Write();
    hMuRelIsoMatch->Write();
    hMuRelIsoNonMatch->Write();



    cout<<"done with normal histograms"<<endl;

}
