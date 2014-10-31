#include "../interface/ClassObjects.h"

using namespace std;

/*
// global Objects
vector<Jet> goodJet;
vector<Jet> goodBJet;

TLorentzVector MET;
TLorentzVector genMET;
TLorentzVector METnoPU;

vector<Lepton> goodLep;
vector<Lepton> goodEl;
vector<Lepton> goodMu;

vector<Lepton> vetoLep;
vector<Lepton> vetoEl;
vector<Lepton> vetoMu;

vector<GenLepton> genLep;
vector<GenLepton> genEl;
vector<GenLepton> genMu;
vector<GenLepton> genTau;

vector<GenLepton> genLepFromTau;
vector<GenLepton> genElFromTau;
vector<GenLepton> genMuFromTau;
*/

///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
// Object cuts
Float_t goodEta = 2.4;

//leptons
Float_t goodElPt = 20.0;
Float_t goodMuPt = 20.0;
Float_t goodLepPt = 20.0;
Float_t vetoLepPt = 10.0;

Float_t goodEl_relIso03 = 0.14;
Float_t goodMu_relIso03 = 0.12;
Float_t goodLep_relIso03 = 0.15;

Bool_t goodMu_tightID = true;
Bool_t goodEl_tightID = true;

//jets
Float_t goodJetPt = 40.0;
Float_t goodJetBtagCSV = 0.679;

///////////////////////////////////
///////////////////////////////////
///////////////////////////////////

// variables for fTree
const int arrayN = 50;
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
Int_t   LepGood_pdgId[arrayN];
Bool_t  LepGood_tightID[arrayN];

// Gen particles
Float_t genLep_pt[2]; //[ngenLep]
Float_t genLep_mass[2]; //[ngenLep]
Float_t genLep_eta[2]; //[ngenLep]
Float_t genLep_phi[2]; //[ngenLep]
Int_t genLep_pdgId[2]; //[ngenLep]
//Float_t genLep_charge[2]; //[ngenLep]

// MET
Float_t met_eta;
Float_t met_phi;
Float_t met_pt;
Float_t met_mass;

/*
void ObjectChain::GetLeptons(vector<Lepton> lepton, vector<Lepton> electron, vector<Lepton> muon){

    // clearing objects
    goodLep.clear();
    goodEl.clear();
    goodMu.clear();

    vetoLep.clear();
    vetoEl.clear();
    vetoMu.clear();

    nLepGood = 0;
    nMuGood = 0;
    nElGood = 0;

    nLepVeto = 0;
    nElVeto = 0;
    nMuVeto = 0;

    // filling objects from fTree
    int nLep = this->Get(nLep,"nLepGood");
    this->Get(LepGood_pt[0],"LepGood_pt");
    this->Get(LepGood_eta[0],"LepGood_eta");
    this->Get(LepGood_phi[0],"LepGood_phi");
    this->Get(LepGood_mass[0],"LepGood_mass");
    this->Get(LepGood_relIso03[0],"LepGood_relIso03");
    this->Get(LepGood_pdgId[0],"LepGood_pdgId");
    this->Get(LepGood_tightID[0],"LepGood_tightId");

    for(int ilep = 0; ilep < nLep; ilep++){

	Lepton dummyLep;
	dummyLep.SetPtEtaPhiM(LepGood_pt[ilep],LepGood_eta[ilep],LepGood_phi[ilep],LepGood_mass[ilep]);
	dummyLep.pdgID = LepGood_pdgId[ilep];
	dummyLep.tightID = LepGood_tightID[ilep];
	dummyLep.relIso03 = LepGood_relIso03[ilep];

	bool isVetoMu = false;
	bool isVetoEl = false;

	// common cuts for all leptons (good and veto leps pass)
	if(dummyLep.Pt() <= vetoLepPt || fabs(dummyLep.Eta()) > goodEta)
	    continue;
	// Muon cuts
	if(abs(LepGood_pdgId[ilep]) == 13){
	    if( dummyLep.Pt() > goodMuPt && LepGood_tightID[ilep] && LepGood_relIso03[ilep] < goodMu_relIso03){
			goodLep.push_back(dummyLep);
			goodMu.push_back(dummyLep);
			nMuGood++;
			nLepGood++;

			//continue;
		    }
		    else{
			isVetoMu = true;
			nMuVeto++;
		    }
		}

	// Electron cuts
	if(abs(LepGood_pdgId[ilep]) == 11){
	    if( dummyLep.Pt() > goodElPt && LepGood_tightID[ilep] && LepGood_relIso03[ilep] < goodEl_relIso03){
//                    isGoodEl = true;
			goodLep.push_back(dummyLep);
			goodEl.push_back(dummyLep);
			nElGood++;
			nLepGood++;

		       // continue;
		    }
		    else{
			isVetoEl = true;
			nElVeto++;
		    }

	    }

	// Only non-good El or Mu will pass => veto leptons
	if(isVetoEl || isVetoMu){
	    vetoLep.push_back(dummyLep);
	    nLepVeto++;
	}
    }


// cout << "Get leptons summary: total number of Leptons = \t" << nLep << endl;
//  cout << "Number of good Muons = \t" << nMuGood << " and veto Mu = \t" << nMuVeto << endl;
//  cout << "Number of good Electrons = \t" << nElGood  << " and veto El = \t" << nElVeto << endl;
//  cout << "Number of veto leptons = \t" << nLepVeto << endl;

}

void ObjectChain::GetGenLeptons(vector<GenLepton> lepton, vector<GenLepton> electron, vector<GenLepton> muon){

    // clearing objects
    genLep.clear();
    nGenLep = 0;

    // filling objects from this
    this->Get(nGenLep,"ngenLep"); //n prompt Lep
    this->Get(genLep_pt[0],"genLep_pt");
    this->Get(genLep_mass[0],"genLep_mass");
    this->Get(genLep_eta[0],"genLep_eta");
    this->Get(genLep_phi[0],"genLep_phi");
    this->Get(genLep_pdgId[0],"genLep_pdgId");

    for(int ilep = 0; ilep < nGenLep; ilep++){

	GenLepton dummyLep;
	dummyLep.SetPtEtaPhiM(genLep_pt[ilep],genLep_eta[ilep],genLep_phi[ilep],genLep_mass[ilep]);

	genLep.push_back(dummyLep);
//      nGenLep++;

	// Muon cuts
	if(abs(genLep_pdgId[ilep]) == 13){

	    genMu.push_back(dummyLep);
//            nGenMu++;

	    continue;
	}

	// Electron cuts
	if(abs(genLep_pdgId[ilep]) == 11){

	    genEl.push_back(dummyLep);
//            nGenEl++;

	    continue;
	}
    }
}


void ObjectChain::GetGenLeptonsFromTau(vector<GenLepton> lepton, vector<GenLepton> electron, vector<GenLepton> muon){

    // clearing objects
    genLepFromTau.clear();
    genMuFromTau.clear();
    genElFromTau.clear();

    nGenLepFromTau = 0;

    // filling objects from this
    this->Get(nGenLepFromTau,"ngenLepFromTau");// Lep from Tau decay

    this->Get(genLep_pt[0],"genLepFromTau_pt");
    this->Get(genLep_mass[0],"genLepFromTau_mass");
    this->Get(genLep_eta[0],"genLepFromTau_eta");
    this->Get(genLep_phi[0],"genLepFromTau_phi");
    this->Get(genLep_pdgId[0],"genLepFromTau_pdgId");

    for(int ilep = 0; ilep < nGenLepFromTau; ilep++){

	GenLepton dummyLep;
	dummyLep.SetPtEtaPhiM(genLep_pt[ilep],genLep_eta[ilep],genLep_phi[ilep],genLep_mass[ilep]);

	genLepFromTau.push_back(dummyLep);
//      nGenLepFromTau++;

// TO BE enhanced

	if(abs(genLep_pdgId[ilep]) == 13){
	    genMuFromTau.push_back(dummyLep);
//            nGenMu++;
	    continue;
	}

	if(abs(genLep_pdgId[ilep]) == 11){
	    genElFromTau.push_back(dummyLep);
//            nGenEl++;
	    continue;
	}
    }
}

void ObjectChain::GetGenTaus(vector<GenLepton> lepton){

    // clearing objects
    genTau.clear();
    nGenTau = 0;

    // filling objects from this
    this->Get(nGenTau,"ngenTau");// gen Tau
    this->Get(genLep_pt[0],"genTau_pt");
    this->Get(genLep_eta[0],"genTau_eta");
    this->Get(genLep_phi[0],"genTau_phi");
    this->Get(genLep_pdgId[0],"genTau_pdgId");
    this->Get(genLep_mass[0],"genTau_mass");

    for(int ilep = 0; ilep < nGenTau; ilep++){

	GenLepton dummyLep;
	dummyLep.SetPtEtaPhiM(genLep_pt[ilep],genLep_eta[ilep],genLep_phi[ilep],genLep_mass[ilep]);

	if(abs(genLep_pdgId[ilep]) == 15){
	    genTau.push_back(dummyLep);
//            nGenTau++;
	}
    }
}
*/

void ObjectChain::GetJets(vector<Jet> goodJet, vector<Jet> goodBJet){
    goodJet.clear();
    goodBJet.clear();

    double ST = 0;
    double HT40 = 0;

    double nJetGood = 0;
    double nBJetGood = 0;

    int nJet = this->Get(nJet,"nJet");
    this->Get(Jet_pt[0],"Jet_pt");
    this->Get(Jet_eta[0],"Jet_eta");
    this->Get(Jet_phi[0],"Jet_phi");
    this->Get(Jet_mass[0],"Jet_mass");
    this->Get(Jet_btagCSV[0],"Jet_btagCSV");

    for(int ijet = 0; ijet < nJet; ijet++)
    {
	Jet dummyJet;
	dummyJet.SetPtEtaPhiM(Jet_pt[ijet],Jet_eta[ijet],Jet_phi[ijet],Jet_mass[ijet]);
	//put pt, eta, cuts and other stuff
	//jet are already cleaned from all loose leptons that are in LepGood
	if(dummyJet.Pt() > goodJetPt && fabs(dummyJet.Eta()) < goodEta){
	    goodJet.push_back(dummyJet);
	    nJetGood++;
	    HT40 = HT40 + dummyJet.Pt();

	    // filling B jets
	    if(Jet_btagCSV[ijet] > goodJetBtagCSV){
		goodBJet.push_back(dummyJet);
		nBJetGood++;
	    }
	}
    }

/*
  cout << "Get jets summary: total number of jets = \t" << nJet << endl;
  cout << "Number of good jets = \t" << nJetGood  << " and b jets = \t" << nBJetGood << endl;
*/
}

void ObjectChain::GetMET(TLorentzVector MET){
    MET.SetPtEtaPhiM(0,0,0,0);

    this->Get(met_pt,"met_pt");
    this->Get(met_eta,"met_eta");
    this->Get(met_phi,"met_phi");
    this->Get(met_mass,"met_mass");

    MET.SetPtEtaPhiM(met_pt,met_eta,met_phi,met_mass);
}

void ObjectChain::GetGenMET(TLorentzVector genMET){
    genMET.SetPtEtaPhiM(0,0,0,0);

    this->Get(met_pt,"met_genPt");
    this->Get(met_eta,"met_genEta");
    this->Get(met_phi,"met_genPhi");
    this->Get(met_mass,"met_sumEt");

    genMET.SetPtEtaPhiM(met_pt,met_eta,met_phi,met_mass);
}

void ObjectChain::GetMETnoPU(TLorentzVector METnoPU){
    METnoPU.SetPtEtaPhiM(0,0,0,0);

    this->Get(met_pt,"metNoPU_pt");
    this->Get(met_eta,"metNoPU_eta");
    this->Get(met_phi,"metNoPU_phi");
    this->Get(met_mass,"metNoPU_mass");

    METnoPU.SetPtEtaPhiM(met_pt,met_eta,met_phi,met_mass);
}
