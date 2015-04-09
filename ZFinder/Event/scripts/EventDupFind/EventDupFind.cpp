// Standard Library
#include <stdexcept>
#include <iostream>
#include <sstream>

// ROOT
#include <TFile.h>
#include <TBranch.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLeaf.h>

#include <TTree.h>
#include "TSystem.h"

#include <set>

using namespace std; // yeah I am evil, get over it

TTree* GetTTree(const std::string TFILE, const std::string TTREE) {
  // Open the TFiles
  TFile* mc_tfile = new TFile(TFILE.c_str(), "READ");
  if (!mc_tfile || mc_tfile->IsZombie()) {
    const std::string ERR = "Could not open the file " + TFILE;
    throw std::runtime_error(ERR);
  }

  // Load the tree
  TTree* tree = new TTree();
  mc_tfile->GetObject(TTREE.c_str(), tree);
  if (!tree || tree->IsZombie()) {
    const std::string ERR = "Could not open the TTree " + TTREE;
    throw std::runtime_error(ERR);
  }

  return tree;
}

int main() {
    // Get a map of the histograms of the Z Masses
    const std::string Tree_HighCut = "ZFinder/Combined Single Reco/Combined Single Reco";
    const std::string Tree_LowCut = "ZFinder/Combined Single Lowered Threshold Reco/Combined Single Lowered Threshold Reco";
    const std::string file_name = "/data/whybee0a/user/gude_2/Data/20150324_SingleElectron_2012ALL/hadded.root";
    // Open the file and load the tree
    TTree* treeH = GetTTree(file_name, Tree_HighCut);
    TBranch* event_infoH = treeH->GetBranch("event_info");
    TLeaf* EVNumbH = event_infoH->GetLeaf("event_number");
    // Pack into a hitogram
    std::set<int> eventnumber;
    std::set<int> eventnumberLow;
    for (int i = 0; i < treeH->GetEntries(); i++) {
        treeH->GetEntry(i);
        if (eventnumber.find(EVNumbH->GetValue()) == eventnumber.end()) {
            eventnumber.insert(EVNumbH->GetValue());
        } else {
            cout << "multiple events numbered :" << EVNumbH->GetValue()<<" in higher cuts" << endl;
        }

    }

    TTree* TreeLow = GetTTree(file_name, Tree_LowCut);
    TBranch* event_infoL = TreeLow->GetBranch("event_info");
    TLeaf* EVNumbLow = event_infoL->GetLeaf("event_number");
    for (int i = 0; i < TreeLow->GetEntries(); i++) {
        TreeLow->GetEntry(i);
        eventnumberLow.insert(EVNumbLow->GetValue());
        if (eventnumberLow.find(EVNumbLow->GetValue()) == eventnumber.end()) {
            eventnumberLow.insert(EVNumbLow->GetValue());
        } else {
cout << "multiple events numbered :" << EVNumbLow->GetValue()<<" in lowered cuts" << endl;
        }

    }

    return EXIT_SUCCESS;
}
