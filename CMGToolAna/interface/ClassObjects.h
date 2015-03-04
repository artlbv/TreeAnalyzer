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
    //using TLorentzVector::TLorentzVector;
    ParticleObject() {}
// conflict with default constructor?
    ParticleObject(Float_t pt, Float_t eta, Float_t phi, Float_t mass){
        SetPtEtaPhiM(pt,eta,phi,mass);
    }

};

// Lepton
class Lepton: public ParticleObject{
public:
    //using ParticleObject::ParticleObject;

    Int_t pdgId;
    Float_t relIso03;
    Float_t miniRelIso;
    Int_t tightID;
    Float_t mvaSusy;
    Float_t mvaPhys14;
    Int_t charge;
    Int_t passID;
    Int_t passIso;
};

// a bit more generic, generator partices
class GenParticle: public ParticleObject{
public:
    //using ParticleObject::ParticleObject;

    Int_t status;
    Int_t pdgId;
    Int_t motherId;
    Int_t grandmaId;
};


// Generator Lepton
class GenLepton: public GenParticle{
public:

    Int_t charge;

};

// Jet
class FatJet: public ParticleObject{
private:
// determine btag in constructor?
//    Jet() { if(btagCSV > 0.676) btag = true; }

public:
    Float_t prunedMass;
    Float_t trimmedMass;
    Float_t filteredMass;
    Float_t  tau1;
    Float_t tau2;
    Float_t tau3;
    Float_t topMass;
    Float_t minMass;
    Float_t nSubJets;

    bool topTagged;


};


class Jet: public ParticleObject{
private:
public:
    bool btag;
    Float_t btagCSV;
};


// MET
class MET: public ParticleObject{
public:

//    Float_t met(){ return TLorentzVector::Pt() }
    Float_t met(){ return Pt(); }
};

class GetObjects{
public:
    void GetJets(EasyChain * tree);
    void GetFatJets(EasyChain * tree);
    void GetMET(EasyChain * tree);
    void GetGenMET(EasyChain * tree);

    void GetMETnoPU(EasyChain * tree);
    void GetLeptons(EasyChain * tree, string elID = "POG2012", string muID = "POG2012");
    void GetSoftLeptons(EasyChain * tree);

    void GetGenLeptons(EasyChain * tree);
    void GetGenParticles(EasyChain * tree);
    void GetGenLeptonsFromTau(EasyChain * tree);
    void GetGenTaus(EasyChain * tree);
    void GetKinVariables();
    //void GetKinVariables(std::vector<Lepton> SelectedLep, std::vector<Jet> goodJet, TLorentzVector MET);

    std::vector<Jet> goodJet;
    std::vector<Jet> goodBJet;
    std::vector<FatJet> goodFatJet;
    std::vector<FatJet> goodTopTagJet;
    std::vector<FatJet> goodWTagJet;
    std::vector<FatJet> goodWmassTagJet;

    TLorentzVector MET;
    TLorentzVector genMET;
    TLorentzVector METnoPU;

    std::vector<Lepton> goodLep;
    std::vector<Lepton> goodEl;
    std::vector<Lepton> goodMu;

    std::vector<Lepton> SelectedLep;

    std::vector<Lepton> softLep;
    std::vector<Lepton> softEl;
    std::vector<Lepton> softMu;

    std::vector<Lepton> vetoLep;
    std::vector<Lepton> vetoEl;
    std::vector<Lepton> vetoMu;

    std::vector<Lepton> SoftvetoLep;
    std::vector<Lepton> SoftvetoEl;
    std::vector<Lepton> SoftvetoMu;

    std::vector<GenLepton> genLep;
    std::vector<GenLepton> genEl;
    std::vector<GenLepton> genMu;
    std::vector<GenLepton> genTau;

    std::vector<GenLepton> genLepFromTau;
    std::vector<GenLepton> genElFromTau;
    std::vector<GenLepton> genMuFromTau;

    std::vector<GenParticle> genPart;
    // objects number can be aslo detemined as object.size()

    Int_t nLepGood;
    Int_t nMuGood;
    Int_t nElGood;

    Int_t nSoftLepGood;
    Int_t nSoftMuGood;
    Int_t nSoftElGood;

    Int_t nLepVeto;
    Int_t nElVeto;
    Int_t nMuVeto;

    Int_t nSoftLepVeto;
    Int_t nSoftElVeto;
    Int_t nSoftMuVeto;


    Int_t nJetGood;
    Int_t nFatJetGood;
    Int_t nTopTagJetGood;
    Int_t nWmassTagJetGood;
    Int_t nWTagJetGood;
    Int_t nBJetGood;

    Int_t nGenTau;
    Int_t nGenLep;
    Int_t nGenPart;
    Int_t nGenLepFromTau;

    Float_t HT40;
    Float_t ST;
    Float_t DelPhiWLep;
    Float_t DelPhiMetLep;
    Float_t minDelPhibMet;
    Float_t minDelPhiJMet;
    Float_t minDelPhibW;
    Float_t minDelPhibLep;

    Float_t MTMetLep;
    Float_t MTbMet;
    Float_t MTbLep;
    Float_t MTbW;
    Float_t  DelRJMet0;
    Float_t  DelRJMet1;
    Float_t  DelRJMet2;
    Float_t  DelRJMet01;
    Float_t  minDelRJLep;
    Float_t  minDelRbL ;


};


#endif
