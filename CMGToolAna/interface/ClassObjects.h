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
    int tightID;
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

class GetObjects{                                                                                              
 public:                                                                       
  void GetJets(EasyChain * tree);
  void GetMET(EasyChain * tree);
  void GetGenMET(EasyChain * tree);

  void GetMETnoPU(EasyChain * tree);
  void GetLeptons(EasyChain * tree);
  void GetGenLeptons(EasyChain * tree);
  void GetGenLeptonsFromTau(EasyChain * tree);
  void GetGenTaus(EasyChain * tree);

   std::vector<Jet> goodJet;
   std::vector<Jet> goodBJet;
  
   TLorentzVector MET;
   TLorentzVector genMET;
   TLorentzVector METnoPU;
  
   std::vector<Lepton> goodLep;
   std::vector<Lepton> goodEl;
   std::vector<Lepton> goodMu;
  
   std::vector<Lepton> vetoLep;
   std::vector<Lepton> vetoEl;
   std::vector<Lepton> vetoMu;
  
   std::vector<GenLepton> genLep;
   std::vector<GenLepton> genEl;
   std::vector<GenLepton> genMu;
   std::vector<GenLepton> genTau;
  
   std::vector<GenLepton> genLepFromTau;
   std::vector<GenLepton> genElFromTau;
   std::vector<GenLepton> genMuFromTau;
  
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

};


#endif
