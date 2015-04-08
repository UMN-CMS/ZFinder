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

#include "TH1.h"
#include <TTree.h>
#include "TSystem.h"

#include <algorithm>
#include <set>
#include <vector>
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
  for (int i = 0; i < treeH->GetEntries(); i++) {
    treeH->GetEntry(i);
    
    //if (i < 100)cout << "entry number High " << EVNumbH->GetValue() << endl;
    eventnumber.insert(EVNumbH->GetValue());

  }

  TH1 *EPt0Hist = new TH1F("E_Pt0", "PT of e0", 700, 0, 700);
  TH1 *EPt1Hist = new TH1F("E_Pt1", "PT of e1", 700, 0, 700);

  TH1 *EEta0Hist = new TH1F("E_Eta0", "Eta of e0", 200, -10, 10);
  TH1 *EEta1Hist = new TH1F("E_Eta1", "Eta of e1", 200, -10, 10);

  TTree* TreeLow = GetTTree(file_name, Tree_LowCut);
  TBranch* event_infoL = TreeLow->GetBranch("event_info");
  TLeaf* EVNumbLow = event_infoL->GetLeaf("event_number");
  TBranch* RecoLow = TreeLow->GetBranch("reco");
  TLeaf* EPt0 = RecoLow->GetLeaf("e_pt0");
  TLeaf* EPt1 = RecoLow->GetLeaf("e_pt1");
  TLeaf* EEta0 = RecoLow->GetLeaf("e_eta0");
  TLeaf* EEta1 = RecoLow->GetLeaf("e_eta1");
  int j=0;
  for (int i = 0; i < TreeLow->GetEntries(); i++) {
    TreeLow->GetEntry(i);

    if (!(i % 50000))cout << " still going :" << i << endl;
    if (eventnumber.find(EVNumbLow->GetValue()) == eventnumber.end()) {
      EPt0Hist->Fill(EPt0->GetValue());
      EPt1Hist->Fill(EPt1->GetValue());
      EEta0Hist->Fill(EEta0->GetValue());
      EEta1Hist->Fill(EEta1->GetValue());
      j++;
    } 

  }
  TFile *theFile = new TFile("Diffevents.root", "RECREATE");
  theFile->cd();
  EPt0Hist->Write();
  EPt1Hist->Write();
  EEta0Hist->Write();
  EEta1Hist->Write();

  EPt0Hist->Delete();
  EPt1Hist->Delete();
  EEta0Hist->Delete();
  EEta1Hist->Delete();
  cout<<" NUMBER IN THE HIGH :"<<treeH->GetEntries()<<endl;
  cout<<"NUmber in Low :"<<TreeLow->GetEntries()<<endl;
  cout<<"NUMBER THAT WE SHOULD HAVE :"<<TreeLow->GetEntries()-treeH->GetEntries()<<endl;
  cout<<"HOW MANY DO WE HAVE :"<<j<<endl;
  return EXIT_SUCCESS;
}
