// Standard Library
#include <iostream>
#include <string>

// ROOT
#include <TTree.h>
#include <TFile.h>
#include <TBranch.h>


int main() {
    //Structure outlining the branch
    struct branch_struct {
        double z_m;
        double z_y;
        double z_phistar_born;
        double z_phistar_dressed;
        double z_phistar_naked;
        double z_phistar_sc;
        double z_pt;
        double z_eta;
        double e_pt[2];
        double e_eta[2];
        double e_phi[2];
        int e_charge[2];
        unsigned int n_verts;
    } truth_branch;

    struct event_branch {
        unsigned int event_number;
        bool is_mc;
    } event_branch;

    // Input Files
    const std::string MC_FILE =
        "/data/whybee0a/user/gude_2/MC/20150211_MC_CTEQ6LL/MadGraph_hadded.root";

    // Tree
    const std::string TREE_NAME =
        "ZFinder/Combined Gen Cuts Reco/Combined Gen Cuts Reco";

    // Open the TFiles
    TFile* mc_tfile = new TFile(MC_FILE.c_str(), "READ");
    if (!mc_tfile) {
        std::cout << "Failed to read TFile: " << MC_FILE << std::endl;
        return EXIT_FAILURE;
    }

    // Load the tree
    TTree* tree;
    mc_tfile->GetObject(TREE_NAME.c_str(), tree);
    if (!tree) {
        std::cout << "Failed to load the tree: " << TREE_NAME << std::endl;
        return EXIT_FAILURE;
    }

    // Get the branch
    tree->SetBranchAddress("truth", &truth_branch);
    tree->SetBranchAddress("event_info", &event_branch);

    // Text file
    std::cout << "#event_number" << ", ";
    std::cout << "e_pt[0]" << ", ";
    std::cout << "e_eta[0]" << ", ";
    std::cout << "e_phi[0]" << ", ";
    std::cout << "e_charge[0]" << ", ";
    std::cout << "e_pt[1]" << ", ";
    std::cout << "e_eta[1]" << ", ";
    std::cout << "e_phi[1]" << ", ";
    std::cout << "e_charge[1]" << ", ";
    std::cout << "z_m" << ", ";
    std::cout << "z_y" << ", ";
    std::cout << "z_phistar_dressed";
    std::cout << std::endl;
    for (int i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);
        // Make the text file
        std::cout << event_branch.event_number << ", ";
        std::cout << truth_branch.e_pt[0] << ", ";
        std::cout << truth_branch.e_eta[0] << ", ";
        std::cout << truth_branch.e_phi[0] << ", ";
        std::cout << truth_branch.e_charge[0] << ", ";
        std::cout << truth_branch.e_pt[1] << ", ";
        std::cout << truth_branch.e_eta[1] << ", ";
        std::cout << truth_branch.e_phi[1] << ", ";
        std::cout << truth_branch.e_charge[1] << ", ";
        std::cout << truth_branch.z_m << ", ";
        std::cout << truth_branch.z_y << ", ";
        std::cout << truth_branch.z_phistar_dressed;
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
