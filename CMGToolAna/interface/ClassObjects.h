#ifndef CLOBJECTS_H
#define CLOBJECTS_H

#include "NtupleTools3.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TFile.h>

// shouldn't be using 'using namespace' in header files
//using namespace std;

// Extend the EasyChain class with getObjects functions
class ObjectChain: EasyChain {

};

// Object classes

// common object variable class ~ simple TLorentzVector -> inherit from TLV??
class ParticleObject: public TLorentzVector{
//private:
//    ParticleObject() {}

public:
// inherit baseclass constructors (C++11)
    using TLorentzVector::TLorentzVector;
    ParticleObject() {}
// conflict with default constructor?
    ParticleObject(Double_t pt, Double_t eta, Double_t phi, Double_t mass){
	SetPtEtaPhiM(pt,eta,phi,mass);
    }

// common variables for all obejcts, except TLV
};

/*
class ParticleObject{

private:
// constructor
    ParticleObject() {}

public:
    TLorentzVector vect;

    void SetVect(Double_t pt, Double_t eta, Double_t phi, Double_t mass){
	vect.SetPtEtaPhiM(pt,eta,phi,mass);
    }

// for fast access to kinematic variables
    Double_t Pt(){ return vect.Pt() }
    Double_t Eta(){ return vect.Eta() }
    Double_t Phi(){ return vect.Phi() }

};

// constuctor as TLV SetPtEtaPhiM
ParticleObject::ParticleObject(Double_t pt, Double_t eta, Double_t phi, Double_t mass){
    SetVect(pt,eta,phi,mass);
}

*/

// Lepton
class Lepton: public ParticleObject{
public:
    using ParticleObject::ParticleObject;

    int pdgID;
    Double_t relIso03;
    bool tightID;
    int charge;
};


// Generator Lepton
class GenLepton: public ParticleObject{
public:
    using ParticleObject::ParticleObject;

    int status;
};

// Jet
class Jet: public ParticleObject{
private:
// determine btag in constructor?
//    Jet() { if(btagCSV > 0.676) btag = true; }

public:
    bool btag;
    Double_t btagCSV;
};


// MET
class MET: public ParticleObject{
public:

//    Double_t met(){ return TLorentzVector::Pt() }
    Double_t met(){ return Pt(); }
};

// non class objects

/* Definition of objects:
 * leptons, muons, electrons
 * jets
 */

void GetJets(EasyChain * tree);
void GetMET(EasyChain * tree);
void GetGenMET(EasyChain * tree);

void GetLeptons(EasyChain * tree);
void GetGenLeptons(EasyChain * tree);
void GetGenLeptonsFromTau(EasyChain * tree);
void GetGenTaus(EasyChain * tree);

// to be written ?
/*
  void GetElectrons(EasyChain * tree);
  void GetMuons(EasyChain * tree);
  void GetGoodLeptons(EasyChain * tree);
  void GetGoodElectrons(EasyChain * tree);
  void GetGoodMuons(EasyChain * tree);
*/

// global object variables
extern std::vector<Jet> goodJet;
extern std::vector<Jet> goodBJet;

extern TLorentzVector MET;
extern TLorentzVector genMET;
extern TLorentzVector METnoPU;

extern std::vector<Lepton> goodLep;
extern std::vector<Lepton> goodEl;
extern std::vector<Lepton> goodMu;

extern std::vector<Lepton> vetoLep;
extern std::vector<Lepton> vetoEl;
extern std::vector<Lepton> vetoMu;

extern std::vector<GenLepton> genLep;
extern std::vector<GenLepton> genEl;
extern std::vector<GenLepton> genMu;
extern std::vector<GenLepton> genTau;

extern std::vector<GenLepton> genLepFromTau;
extern std::vector<GenLepton> genElFromTau;
extern std::vector<GenLepton> genMuFromTau;

// objects number can be aslo detemined as object.size()

Int_t nLepGood;
Int_t nMuGood;
Int_t nElGood;

Int_t nLepVeto;
Int_t nElVeto;
Int_t nMuVeto;

Int_t nJetGood;
Int_t nBJetGood;

Int_t nGenTau;
Int_t nGenLep;
Int_t nGenLepFromTau;

// global variables

Float_t HT40;
Float_t ST;

#endif
