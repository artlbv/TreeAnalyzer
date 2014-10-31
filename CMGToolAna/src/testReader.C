//#include "../interface/NtupleTools3.h"

//#ifndef NtupleTools3_h //__NtupleTools3__
#include "../interface/ClassObjects.h"
//#endif

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TFile.h>

//#include "fastjet/ClusterSequence.hh"

using namespace std;

//void TreeAnalyzer_ext(TString list, TString outname,bool useW=true){
int main(int argc, char* argv[]){

    // read input arguments
    if (argc < 2 ) {
        cout << "Not enough arguments." << endl
             << "Usage:" << endl
             << "main lost outname [useW]" << endl;

        return 1;
    }

    TString list = argv[1];
    TString outname = argv[2];
    bool useW = true;

    if ( argc == 3) {
        useW = argv[3];
    }

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


    ObjectChain* tree = new ObjectChain("treeProducerSusySingleLepton");

    for(unsigned int i=0; i < files.size(); i++){
        tree->AddSmartW(files[i], weights[i]);
        cout << "add: " << files[i] << " " << weights[i] << endl;
    }

    int Nevents=tree->GetEntries();
    cout<<">>>>>>>>>>>>>>>>>>>>>>> total number of events:\t" << Nevents <<endl;


    // object variables;
    vector<Jet> goodJets;
    vector<Jet> goodBJets;

    /////////////   event loop   //////////////////////
//      for(int entry=0; entry <  Nevents/*min(1000000,Nevents)*/; entry+=1){
    for(int entry=0; entry <  1000/*min(1000000,Nevents)*/; entry+=1){

        //tree->GetEntry(entry); // <<<<<<<<<<< dk
        Float_t fw = tree->GetEntryW(entry);
        progressT();

	tree->GetJets(goodJets, goodBJets);

    }

    return 0;
}
