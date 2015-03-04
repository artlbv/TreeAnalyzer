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

    // VARS FOR TTREE
    //Lepton treeLepton;
    Float_t EvWeight;

    Int_t nGoodLep;
    Int_t nVetoLep;
    Int_t nGenLep;
    Float_t lepPt;
    Float_t lepEta;
    Int_t pdgID;
    Bool_t match;
    Bool_t prompt;
    Bool_t passID;
    Bool_t passIso;

    Float_t relIso;
    Float_t miniIso;

    Int_t nJets;
    Int_t nBJets;
    Float_t HT;
    Float_t ST;
    Float_t MET;
    Float_t dPhi;
    Float_t Jet1Pt;
    Float_t Jet2Pt;

    TString rootfilename = "lepTree_"+outname+".root";
    TFile * treef = new TFile(rootfilename,"RECREATE");
    TTree *leptree = new TTree("Leptons","Leptons for efficiency studies");
    //leptree->Branch("Lepton",&treeLepton);
    leptree->Branch("EvWeight",&EvWeight,"EvWeight/F");

    leptree->Branch("nGoodLep",&nGoodLep,"nGoodLep/I");
    leptree->Branch("nVetoLep",&nVetoLep,"nVetoLep/I");
    leptree->Branch("lepPt",&lepPt,"lepPt/F");
    leptree->Branch("lepEta",&lepEta,"lepEta/F");
    leptree->Branch("pdgID",&pdgID,"pdgID/I");
    leptree->Branch("match",&match,"match/B");
    leptree->Branch("prompt",&prompt,"prompt/B");
    leptree->Branch("passID",&passID,"passID/B");
    leptree->Branch("passIso",&passIso,"passIso/B");

    leptree->Branch("relIso",&relIso,"relIso/F");
    leptree->Branch("miniIso",&miniIso,"miniIso/F");

    leptree->Branch("nJets",&nJets,"nJets/I");
    leptree->Branch("nBJets",&nBJets,"nBJets/I");

    leptree->Branch("Jet1Pt",&Jet1Pt,"Jet1Pt/F");
    leptree->Branch("Jet2Pt",&Jet2Pt,"Jet2Pt/F");

    leptree->Branch("HT",&HT,"HT/F");
    leptree->Branch("ST",&ST,"ST/F");
    leptree->Branch("MET",&MET,"MET/F");
    leptree->Branch("dPhi",&dPhi,"dPhi/F");


    int maxEvents = min(100000,Nevents);
//    int maxEvents = Nevents;

    for(int entry=0; entry < maxEvents; entry+=1){

        if (entry % 10000 == 0)
            cout << "================= Processing entry: " << entry <<  " (" << 100*entry/maxEvents <<  "% done)" << '\r' << flush;

        //lumi calcualtion done in runAnalyzer.py (fb and pb)
        Float_t fw = tree->GetEntryW(entry);
        //Float_t EvWeight = 1.0;
	EvWeight = 1.0;
        EvWeight *= fw ;

        // Pre-selection

        // Get all objects
        Obj.GetLeptons(tree,"effID","effID");

	/*
	int neff = Obj.nLepGood;
        Obj.GetLeptons(tree,"mvaPhys14","effID");
	int nmva = Obj.nLepGood;

	if (neff != nmva){
	    cout << "Found in entry\t" <<entry << endl
		 << "EffID: n good lep:" << neff << endl
		 << "MVAID: n good lep:" << nmva << endl;
	}
	continue;
	*/

        // select only with 1 lepton
//	if (Obj.nGenLep + Obj.nGenLepFromTau == 0) continue;
//        if (Obj.goodLep.size() == 0 ) continue;

	nGoodLep = Obj.nLepGood;
	nVetoLep = Obj.nLepVeto;

	// fill kinemativ vars
        Obj.GetJets(tree);
	Obj.GetMET(tree);
        Obj.GetKinVariables();
//        if ( Obj.HT40 < 1250) continue;
	HT = Obj.HT40;
	ST = Obj.ST;
	MET = Obj.MET.Pt();
	dPhi = Obj.DelPhiWLep;
	nJets = Obj.nJetGood;
	nBJets = Obj.nBJetGood;
	if (nJets > 0)
	    Jet1Pt = Obj.goodJet[0].Pt();
	else
	    Jet1Pt = -99;
	if (nJets > 1)
	    Jet2Pt = Obj.goodJet[1].Pt();
	else
	    Jet2Pt = -99;

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

	/*
	  Strategy:
	  1. Match reference object to probe
	  2. Check probe origin (prompt/non-prompt/etc)
	  3. Fill hists
	*/

	// Loop through Tag/reference objects
        for(int iref = 0; iref < refPart.size(); iref++){

	    //treeLepton = refPart[iref];
            int tagId = abs(refPart[iref].pdgId);

	    lepPt =  refPart[iref].Pt();
	    lepEta =  refPart[iref].Eta();
	    passID =  refPart[iref].passID;
	    passIso =  refPart[iref].passIso;
	    pdgID =  tagId;

	    relIso =  refPart[iref].relIso03;
	    miniIso =  refPart[iref].miniRelIso;

	    // kinematic selection
	    if (abs(refPart[iref].Eta()) > 2.4) continue;
	    if (abs(refPart[iref].Pt()) < 25) continue;

	    // find matching object
            float maxDr = 0.1;
            float minDr = 9999.;
            float matchIndx = -1;
            bool matched = false;

            // Loop through gen particles
            for (int iprobe = 0; iprobe < probe.size(); iprobe++){

                int probeId = abs(probe[iprobe].pdgId);

                // check same ID
                if (tagId != probeId) continue;

		// reject objects originating from itself (double counting gen particles)
		if (abs(probe[iprobe].motherId) == abs(probe[iprobe].pdgId)) continue;

                // relDeltaPt < 0.3
                if (abs(1 - probe[iprobe].Pt()/refPart[iref].Pt()) > 0.3) continue;

                // calc dR
                float tmpDr = refPart[iref].DeltaR((TLorentzVector) probe[iprobe]);

                // check maxDr
                if (tmpDr < maxDr && tmpDr < minDr){
//                    if (matched)
//                        cout << endl << "Double Matched in " << entry << " to indx " << matchIndx << " and " << iprobe << endl;
                    matched = true;
                    matchIndx = iprobe;
                    minDr = tmpDr;
                }
            }

	    match = matched;
	    prompt = false;

	    // Fill plots if matched reference to probe
            if( matched ){

		// get probe infromation
                int pdg = probe[matchIndx].pdgId;

		// check whether W or tau mother
		if (abs(probe[matchIndx].motherId) == 24 || abs(probe[matchIndx].motherId) == 15) prompt = true;
		else prompt = false;

//		if (prompt) cout << entry << "\t" << probe[matchIndx].motherId << endl;
	    }

	    // Fill Tree for each lepton
	    leptree->Fill();
        }
    }

    cout << endl << "Finished event loop" << endl;
/*
    cout << "Found " << hElPtMatch->GetEntries() << "\t matched prompt electrons" << endl;
    cout << "Found " << hElPtMatchNonPrompt->GetEntries() << "\t matched non prompt electrons" << endl;
    cout << "Found " << hElPtNonMatch->GetEntries() << "\t un-matched electrons" << endl;
    cout << "Found " << hMuPtMatch->GetEntries() << "\t matched muons" << endl;
    cout << "Found " << hMuPtNonMatch->GetEntries() << "\t un-matched muons" << endl;
*/
    treef->cd();
    leptree->Write();
    treef->Close();
}
