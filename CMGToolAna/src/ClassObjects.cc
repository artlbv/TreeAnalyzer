#include "../interface/ClassObjects.h"

using namespace std;
// global object variables


Double_t goodEta = 2.4;

//leptons
Double_t goodElPt = 25.0;
Double_t goodMuPt = 25.0;
Double_t goodLepPt = 25.0;
Double_t vetoLepPt = 10.0;
// SoftLeptons
Double_t softElPt = 7.0;
Double_t softLepPtUp =25.0;
Double_t softMuPt = 5.0;
Double_t softLepPt = 5.0;
Double_t softvetoLepPt = 5.0;

Double_t goodEl_relIso03 = 0.14;
Double_t goodMu_relIso03 = 0.12;
Double_t goodLep_relIso03 = 0.15;
Double_t softLep_relIso03 = 0.4;

//jets
Double_t goodJetPt = 30.0;
//btagging medium working points
Double_t goodJetBtagCSV = 0.679;
Double_t goodJetBtagCMVA = 0.732;

//fatjet
Double_t goodFatJetPt = 100.0;

// variables for tree
const int arrayN = 50;
Double_t Jet_pt[arrayN];
Double_t Jet_eta[arrayN];
Double_t Jet_phi[arrayN];
Double_t Jet_mass[arrayN];
Double_t Jet_btagCSV[arrayN];
Double_t Jet_btagCMVA[arrayN];

Double_t FatJet_pt[arrayN];
Double_t FatJet_eta[arrayN];
Double_t FatJet_phi[arrayN];
Double_t FatJet_mass[arrayN];
Double_t FatJet_prunedMass[arrayN];
Double_t FatJet_trimmedMass[arrayN];
Double_t FatJet_filteredMass[arrayN];

Double_t FatJet_tau1[arrayN];
Double_t FatJet_tau2[arrayN];
Double_t FatJet_tau3[arrayN];
Double_t FatJet_topMass[arrayN];
Double_t FatJet_minMass[arrayN];
Double_t FatJet_nSubJets[arrayN];



Double_t LepGood_pt[arrayN];
Double_t LepGood_eta[arrayN];
Double_t LepGood_phi[arrayN];
Double_t LepGood_mass[arrayN];
Double_t LepGood_relIso03[arrayN];
Int_t   LepGood_pdgId[arrayN];
Int_t  LepGood_tightID[arrayN];

// Gen particles
Double_t genLep_pt[2]; //[ngenLep]
Double_t genLep_mass[2]; //[ngenLep]
Double_t genLep_eta[2]; //[ngenLep]
Double_t genLep_phi[2]; //[ngenLep]
Int_t genLep_pdgId[2]; //[ngenLep]

Double_t genPart_pt[arrayN];
Double_t genPart_mass[arrayN];
Double_t genPart_eta[arrayN];
Double_t genPart_phi[arrayN];
Int_t genPart_pdgId[arrayN];
Int_t genPart_motherId[arrayN];
Int_t genPart_grandmaId[arrayN];


// MET
Double_t met_eta;
Double_t met_phi;
Double_t met_pt;
Double_t met_mass;

void GetObjects::GetLeptons(EasyChain * tree){

    // clearing objects
    goodLep.clear();
    goodEl.clear();
    goodMu.clear();

    SelectedLep.clear();
    softLep.clear();
    softEl.clear();
    softMu.clear();

    vetoLep.clear();
    vetoEl.clear();
    vetoMu.clear();

    SoftvetoLep.clear();
    SoftvetoEl.clear();
    SoftvetoMu.clear();

    nLepGood = 0;
    nMuGood = 0;
    nElGood = 0;

    nLepVeto = 0;
    nElVeto = 0;
    nMuVeto = 0;

    nSoftLepGood = 0;
    nSoftMuGood = 0;
    nSoftElGood = 0;

    nSoftLepVeto = 0;
    nSoftElVeto = 0;
    nSoftMuVeto = 0;
    // filling objects from tree
    int nLep = tree->Get(nLep,"nLepGood");
    tree->Get(LepGood_pt[0],"LepGood_pt");
    tree->Get(LepGood_eta[0],"LepGood_eta");
    tree->Get(LepGood_phi[0],"LepGood_phi");
    tree->Get(LepGood_mass[0],"LepGood_mass");
    tree->Get(LepGood_relIso03[0],"LepGood_relIso03");
    tree->Get(LepGood_pdgId[0],"LepGood_pdgId");
    tree->Get(LepGood_tightID[0],"LepGood_tightId");
//    cout<<"nlep "<< nLep<<endl;

    for(int ilep = 0; ilep < nLep; ilep++){
	Lepton dummyLep;
	dummyLep.SetPtEtaPhiM(LepGood_pt[ilep],LepGood_eta[ilep],LepGood_phi[ilep],LepGood_mass[ilep]);
	dummyLep.pdgID = LepGood_pdgId[ilep];
	dummyLep.tightID = LepGood_tightID[ilep];
	dummyLep.relIso03 = LepGood_relIso03[ilep];
	bool isVetoMu = false;
	bool isVetoEl = false;
	bool isSoftVetoMu = false;
	bool isSoftVetoEl = false;

       //common cuts for all soft leptons (good and veto leps pass)

	if(dummyLep.Pt() <= softvetoLepPt || fabs(dummyLep.Eta()) > goodEta)
	    continue;
	// Muon cuts
	if(abs(LepGood_pdgId[ilep]) == 13){
	    if( dummyLep.Pt() > softMuPt &&  dummyLep.Pt() < softLepPtUp && LepGood_tightID[ilep]==1 && LepGood_relIso03[ilep] < softLep_relIso03){
		softLep.push_back(dummyLep);
		softMu.push_back(dummyLep);
		nSoftMuGood++;
		nSoftLepGood++;

		//continue;
	    }
	    else{
		isSoftVetoMu = true;
		nSoftMuVeto++;
	    }
	}

	// Electron cuts
	if(abs(LepGood_pdgId[ilep]) == 11){
	    if( dummyLep.Pt() > softElPt && dummyLep.Pt() < softLepPtUp && LepGood_tightID[ilep] > 2 && LepGood_relIso03[ilep] < softLep_relIso03){
//                    isGoodEl = true;
		softLep.push_back(dummyLep);
		softEl.push_back(dummyLep);
		nSoftElGood++;
		nSoftLepGood++;

		// continue;
	    }
	    else{
		nSoftElVeto++;
		isSoftVetoEl = true;
	    }

        }
	// Only non-good El or Mu will pass => veto leptons
	if(isSoftVetoEl || isSoftVetoMu){
	    SoftvetoLep.push_back(dummyLep);
	    nSoftLepVeto++;
         }
	// common cuts for all hard leptons (good and veto leps pass)
	if(dummyLep.Pt() <= vetoLepPt || fabs(dummyLep.Eta()) > goodEta)
	    continue;
	// Muon cuts
	if(abs(LepGood_pdgId[ilep]) == 13){
	    if( dummyLep.Pt() > goodMuPt && LepGood_tightID[ilep] ==1 && LepGood_relIso03[ilep] < goodMu_relIso03){
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
	    if( dummyLep.Pt() > goodElPt && LepGood_tightID[ilep] > 2 && LepGood_relIso03[ilep] < goodEl_relIso03){
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
    /*
      cout << "Get leptons summary: total number of Leptons = \t" << nLep << endl;
      cout << "Number of good Muons = \t" << nMuGood << " and veto Mu = \t" << nMuVeto << endl;
      cout << "Number of good Electrons = \t" << nElGood  << " and veto El = \t" << nElVeto << endl;
      cout << "Number of veto leptons = \t" << nLepVeto << endl;
    */
     
    if(goodLep.size()==1) SelectedLep=goodLep;
    else if(softLep.size()==1) SelectedLep=softLep;
}

void GetObjects::GetGenLeptons(EasyChain * tree){

    // clearing objects
    genLep.clear();
    nGenLep = 0;

    // filling objects from tree
    tree->Get(nGenLep,"ngenLep"); //n prompt Lep
    tree->Get(genLep_pt[0],"genLep_pt");
    tree->Get(genLep_mass[0],"genLep_mass");
    tree->Get(genLep_eta[0],"genLep_eta");
    tree->Get(genLep_phi[0],"genLep_phi");
    tree->Get(genLep_pdgId[0],"genLep_pdgId");

/*
// why?
tree->Get(genLep_charge[0],"genLep_charge");
*/

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


void GetObjects::GetGenLeptonsFromTau(EasyChain * tree){

    // clearing objects
    genLepFromTau.clear();
    genMuFromTau.clear();
    genElFromTau.clear();

    nGenLepFromTau = 0;

    // filling objects from tree
    tree->Get(nGenLepFromTau,"ngenLepFromTau");// Lep from Tau decay

    tree->Get(genLep_pt[0],"genLepFromTau_pt");
    tree->Get(genLep_mass[0],"genLepFromTau_mass");
    tree->Get(genLep_eta[0],"genLepFromTau_eta");
    tree->Get(genLep_phi[0],"genLepFromTau_phi");
    tree->Get(genLep_pdgId[0],"genLepFromTau_pdgId");

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

void GetObjects::GetGenTaus(EasyChain * tree){

    // clearing objects
    genTau.clear();
    nGenTau = 0;

    // filling objects from tree
    tree->Get(nGenTau,"ngenTau");// gen Tau
    tree->Get(genLep_pt[0],"genTau_pt");
    tree->Get(genLep_eta[0],"genTau_eta");
    tree->Get(genLep_phi[0],"genTau_phi");
    tree->Get(genLep_pdgId[0],"genTau_pdgId");
    tree->Get(genLep_mass[0],"genTau_mass");

    for(int ilep = 0; ilep < nGenTau; ilep++){

	GenLepton dummyLep;
	dummyLep.SetPtEtaPhiM(genLep_pt[ilep],genLep_eta[ilep],genLep_phi[ilep],genLep_mass[ilep]);

	if(abs(genLep_pdgId[ilep]) == 15){
	    genTau.push_back(dummyLep);
//            nGenTau++;
	}
    }
}
void GetObjects::GetGenParticles(EasyChain * tree){
    genPart.clear();
    nGenPart = 0;
    // filling objects from tree
    tree->Get(nGenPart,"nGenPart"); //n prompt Lep
    tree->Get(genPart_pt[0],"GenPart_pt");
    tree->Get(genPart_mass[0],"GenPart_mass");
    tree->Get(genPart_eta[0],"GenPart_eta");
    tree->Get(genPart_phi[0],"GenPart_phi");
    tree->Get(genPart_pdgId[0],"GenPart_pdgId");
    tree->Get(genPart_motherId[0],"GenPart_motherId");
    tree->Get(genPart_grandmaId[0],"GenPart_grandmotherId");

    for(int ipart = 0; ipart < nGenPart; ipart++){

	GenParticle dummyPart;
	dummyPart.SetPtEtaPhiM(genPart_pt[ipart], genPart_eta[ipart], genPart_phi[ipart], genPart_mass[ipart]);
	dummyPart.pdgid = genPart_pdgId[ipart];
	dummyPart.motherid = genPart_motherId[ipart];
	dummyPart.grandmaid = genPart_grandmaId[ipart];

	genPart.push_back(dummyPart);

    }
}
void GetObjects::GetJets(EasyChain * tree){
    goodJet.clear();
    goodBJet.clear();

    ST = 0;
    HT40 = 0;

    nJetGood = 0;
    nBJetGood = 0;

    int nJet = tree->Get(nJet,"nJet");
    tree->Get(Jet_pt[0],"Jet_pt");
    tree->Get(Jet_eta[0],"Jet_eta");
    tree->Get(Jet_phi[0],"Jet_phi");
    tree->Get(Jet_mass[0],"Jet_mass");
    tree->Get(Jet_btagCSV[0],"Jet_btagCSV");
    tree->Get(Jet_btagCMVA[0],"Jet_btagCMVA");

    for(int ijet = 0; ijet < nJet; ijet++)
    {
	Jet dummyJet;
	dummyJet.SetPtEtaPhiM(Jet_pt[ijet],Jet_eta[ijet],Jet_phi[ijet],Jet_mass[ijet]);
	//put pt, eta, cuts and other stuff
	//jet are already cleaned from all loose leptons that are in LepGood
	if(dummyJet.Pt() > goodJetPt && fabs(dummyJet.Eta()) < goodEta){
	    goodJet.push_back(dummyJet);
	    nJetGood++;

	    // filling B jets
	    //if(Jet_btagCSV[ijet] > goodJetBtagCSV){
	    if(Jet_btagCMVA[ijet] > goodJetBtagCMVA){
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

void GetObjects::GetFatJets(EasyChain * tree){
    goodFatJet.clear();
    goodTopTagJet.clear();
    goodWTagJet.clear();
    goodWmassTagJet.clear();

    nFatJetGood = 0;
    nTopTagJetGood = 0;
    nWTagJetGood = 0;
    nWmassTagJetGood = 0;
    int nFatJet = tree->Get(nFatJet,"nFatJet");
    tree->Get(FatJet_pt[0],"FatJet_pt");
    tree->Get(FatJet_eta[0],"FatJet_eta");
    tree->Get(FatJet_phi[0],"FatJet_phi");
    tree->Get(FatJet_mass[0],"FatJet_mass");
    tree->Get(FatJet_prunedMass[0],"FatJet_prunedMass");
    tree->Get(FatJet_trimmedMass[0],"FatJet_trimmedMass");
    tree->Get(FatJet_filteredMass[0],"FatJet_filteredMass");

    tree->Get(FatJet_tau1[0],"FatJet_tau1");
    tree->Get(FatJet_tau2[0],"FatJet_tau2");
    tree->Get(FatJet_tau3[0],"FatJet_tau3");
    tree->Get(FatJet_topMass[0],"FatJet_topMass");
    tree->Get(FatJet_minMass[0],"FatJet_minMass");
    tree->Get(FatJet_nSubJets[0],"FatJet_nSubJets");


    for(int ijet = 0; ijet < nFatJet; ijet++)
    {
	FatJet dummyJet;
	dummyJet.SetPtEtaPhiM(FatJet_pt[ijet],FatJet_eta[ijet],FatJet_phi[ijet],FatJet_mass[ijet]);
	dummyJet.prunedMass = FatJet_prunedMass[ijet];
	dummyJet.trimmedMass = FatJet_trimmedMass[ijet];
	dummyJet.filteredMass = FatJet_filteredMass[ijet];
	dummyJet.tau1 = FatJet_tau1[ijet];
	dummyJet.tau2 = FatJet_tau2[ijet];
	dummyJet.tau3 = FatJet_tau3[ijet];
	dummyJet.topMass = FatJet_topMass[ijet];
	dummyJet.minMass = FatJet_minMass[ijet];
	dummyJet.nSubJets = FatJet_nSubJets[ijet];

	
	if(dummyJet.Pt() > goodFatJetPt && fabs(dummyJet.Eta()) < goodEta){
	  if ( ((dummyJet.tau2)/(dummyJet.tau1)) < 0.6 && dummyJet.prunedMass > 70.0 &&  dummyJet.prunedMass < 100.0 ){ //&& dummyJet.prunedMass < 100.0 ){
	    nWmassTagJetGood++;
	    goodWmassTagJet.push_back(dummyJet);	    
	  }
	  if (((dummyJet.tau2)/(dummyJet.tau1)) < 0.6 && dummyJet.prunedMass > 50.0 ){ //&& dummyJet.prunedMass < 100.0 ){
	    nWTagJetGood++;
	    goodWTagJet.push_back(dummyJet);	    
	  }
	  if ( dummyJet.nSubJets > 2 && dummyJet.minMass > 50.0 && dummyJet.topMass > 150.0 ){
	    dummyJet.topTagged = true;
	    nTopTagJetGood++;
	    goodTopTagJet.push_back(dummyJet);	    
	  }
	  else dummyJet.topTagged = false;  
	  goodFatJet.push_back(dummyJet);
	  nFatJetGood++;
	}
    }
}



void GetObjects::GetMET(EasyChain * tree){
    MET.SetPtEtaPhiM(0,0,0,0);

    tree->Get(met_pt,"met_pt");
    tree->Get(met_eta,"met_eta");
    tree->Get(met_phi,"met_phi");
    tree->Get(met_mass,"met_mass");

    MET.SetPtEtaPhiM(met_pt,met_eta,met_phi,met_mass);
}

void GetObjects::GetGenMET(EasyChain * tree){
    genMET.SetPtEtaPhiM(0,0,0,0);

    tree->Get(met_pt,"met_genPt");
    tree->Get(met_eta,"met_genEta");
    tree->Get(met_phi,"met_genPhi");
    tree->Get(met_mass,"met_sumEt");

    genMET.SetPtEtaPhiM(met_pt,met_eta,met_phi,met_mass);
}

void GetObjects::GetMETnoPU(EasyChain * tree){
    METnoPU.SetPtEtaPhiM(0,0,0,0);

    tree->Get(met_pt,"metNoPU_pt");
    tree->Get(met_eta,"metNoPU_eta");
    tree->Get(met_phi,"metNoPU_phi");
    tree->Get(met_mass,"metNoPU_mass");

    METnoPU.SetPtEtaPhiM(met_pt,met_eta,met_phi,met_mass);
}

//////////////////////////////kinematic variables/////////////////

//void GetObjects::GetKinVariables(std::vector<Lepton> goodLep, std::vector<Jet> goodJet, TLorentzVector MET){
//void GetObjects::GetKinVariables(std::vector<Lepton> SelectedLep, std::vector<Jet> goodJet, TLorentzVector MET){
void GetObjects::GetKinVariables(){

  //use leading LEPTON for everything, need to define cuts to make sure that 
  //there is only one lepton
    HT40 = 0;
    ST   = 0;
    DelPhiWLep   = -99999.9;  
    DelPhiMetLep = -99999.9;  
    minDelPhibMet   =  99999.9;  //  we want to minimize this variable  
    minDelPhiJMet   =  99999.9;  //  we want to minimize this variable 
    minDelPhibW     =  99999.9;  //  we want to minimize this variable 
    minDelPhibLep   =  99999.9;  //  we want to minimize this variable 
    minDelRJLep     =  99999.9;  //  we want to minimize this variable 
    minDelRbL       =  99999.9;  //  we want to minimize this variable 
    MTMetLep     = -99999.9;
    MTbMet       = -99999.9;
    MTbW         = -99999.9;
    MTbLep       = -99999.9;


    if(SelectedLep.size() > 0)
	ST = SelectedLep[0].Pt() + MET.Pt();

    for(int ijet = 0; ijet < goodJet.size(); ijet++){
	HT40 = HT40 + goodJet[ijet].Pt();
    }
    if(SelectedLep.size() > 0){
        TLorentzVector WBos = MET + SelectedLep[0];

	//Delta phi between W and Lep
	//standard root defintion (+ fabs)takes care of getting values between 0 and pi
	 DelPhiWLep = fabs(WBos.DeltaPhi(SelectedLep[0]));
	//alternative definiton with the same result, if you want to cross check
	Double_t DelPhiWLepAlt = (WBos.Phi() - SelectedLep[0].Phi());
	if (DelPhiWLepAlt > TMath::Pi()) DelPhiWLepAlt -= 2*TMath::Pi();
	if (DelPhiWLepAlt <= -TMath::Pi()) DelPhiWLepAlt += 2*TMath::Pi();
	DelPhiWLepAlt = fabs(DelPhiWLepAlt);
	
	// minDelta phi between b and Lep,MET and Lep
	DelPhiMetLep =  fabs(MET.DeltaPhi(SelectedLep[0]));
            int bC =-1;
            int bCW =-1;
          for(int ib =0; ib < goodBJet.size(); ib++){
              Double_t   DelPhibiMet = fabs(MET.DeltaPhi(goodBJet[ib]));
              Double_t   DelPhibiW = fabs(WBos.DeltaPhi(goodBJet[ib]));
              Double_t   DelPhibiLep = fabs(SelectedLep[0].DeltaPhi(goodBJet[ib]));
              Double_t  MTbMETMin =sqrt(pow((goodBJet[ib].Et()+MET.Et()),2)-pow((goodBJet[ib].Px()+MET.Px()),2)-pow((goodBJet[ib].Py()+MET.Py()),2));
                if ( DelPhibiLep < minDelPhibLep ) minDelPhibLep = DelPhibiLep;
                if ( DelPhibiMet < minDelPhibMet ) {minDelPhibMet = DelPhibiMet;
                         bC = ib;
                   }
                if ( DelPhibiW < minDelPhibW ) {minDelPhibW = DelPhibiW;
                         bCW = ib;
                  }
               }

	// minDelta R between b and Lep
            int bCl =-1;
          for(int ib =0; ib < goodBJet.size(); ib++){
              Double_t   DelRbiL = (SelectedLep[0].DeltaR(goodBJet[ib]));
                if ( DelRbiL< minDelRbL ) {minDelRbL = DelRbiL;
                         bCl = ib;
                   }
                  }
	
	//Transverse mass of Lep, MET
	   MTMetLep = sqrt(pow((SelectedLep[0].Et()+MET.Et()),2)-pow((SelectedLep[0].Px()+MET.Px()),2)-pow((SelectedLep[0].Py()+MET.Py()),2));
	//Transverse mass of closest b to MET (Delta phi), MET, Lep, W
           if(goodBJet.size() >0) {
                    MTbMet =sqrt(pow((goodBJet[bC].Et()+MET.Et()),2)-pow((goodBJet[bC].Px()+MET.Px()),2)-pow((goodBJet[bC].Py()+MET.Py()),2));
                    MTbW =sqrt(pow((goodBJet[bC].Et()+WBos.Et()),2)-pow((goodBJet[bC].Px()+WBos.Px()),2)-pow((goodBJet[bC].Py()+WBos.Py()),2));
                    MTbLep =sqrt(pow((goodBJet[bC].Et()+SelectedLep[0].Et()),2)-pow((goodBJet[bC].Px()+SelectedLep[0].Px()),2)-pow((goodBJet[bC].Py()+SelectedLep[0].Py()),2));
                               }

	//Min Delta Phi (J,MET) and Delta R(Jet,Lep) among three leading Jets
                   int JC =-1;
      for( int ij =0; ij < nJetGood; ij++){
              Double_t   DelPhijiMet = fabs(MET.DeltaPhi(goodJet[ij]));
              Double_t   DelRjiLep = fabs(SelectedLep[0].DeltaR(goodJet[ij]));
              if(ij >2) continue;
              if ( DelPhijiMet < minDelPhiJMet ) {minDelPhiJMet = DelPhijiMet;
              JC = ij;
              }
            
              if ( DelRjiLep < minDelRJLep ) {minDelRJLep = DelRjiLep;
             }
          }


    }

}
