// Standard Library
#include <iostream>
#include <string>

// ROOT
#include <TTree.h>
#include <TFile.h>
#include <TBranch.h>
#include <TH1D.h>


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
        double n_true_pileup;
        int e_charge[2];
        int n_verts;
    } truth_branch;

    struct event_branch {
        unsigned int event_number;
        bool is_mc;
    } event_branch;

    // Input Files
    const std::string MC_FILE =
        "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL/MadGraph/hadded.root";

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

    // Open a new file and histogram
    TFile outfile("outfile.root", "RECREATE");
    outfile.cd();
    TH1D outhisto("true_pileup", "true_pileup", 100, 0, 100);

    // Get the branch
    tree->SetBranchAddress("truth", &truth_branch);
    tree->SetBranchAddress("event_info", &event_branch);
    for (int i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);
        outhisto.Fill(truth_branch.n_true_pileup);
    }

    //outhisto.Write();
    outfile.Write();
    outfile.Close();

    return EXIT_SUCCESS;
}
