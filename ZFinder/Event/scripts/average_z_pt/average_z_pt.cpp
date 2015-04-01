// Standard Library
#include <stdexcept>
#include <iostream>

// ROOT
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>


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
    // Input Files
    const std::string DATA_FILE =
        "/data/whybee0a/user/gude_2/Data/20150324_SingleElectron_2012ALL/hadded.root";

    // Tree
    const std::string TREE_NAME =
        "ZFinder/Combined Single Reco/Combined Single Reco";

    // Load the tree
    TTree* tree = GetTTree(DATA_FILE, TREE_NAME);

    // Get the branch
    TBranch* reco_branch = tree->GetBranch("reco");
    TLeaf* z_pt = reco_branch->GetLeaf("z_pt");

    double sum = 0;
    int counter = 0;
    for (int i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);
        sum += z_pt->GetValue();
        counter++;
    }

    std::cout << "Sum of Pt: " << sum << std::endl;
    std::cout << "Number of Zs: " << counter << std::endl;
    std::cout << sum / double(counter) << std::endl;

    return EXIT_SUCCESS;
}
