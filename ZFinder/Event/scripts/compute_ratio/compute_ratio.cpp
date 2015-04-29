// Standard Library
#include <stdexcept>
#include <iostream>

// ROOT
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1D.h>
#include <TLeaf.h>

// ZFinder
#include "../../interface/WeightID.h"


double GetWeight(
        const int& WEIGHT_SIZE,
        const double WEIGHTS[],
        const int WEIGHTIDS[]
        ) {
    double weight = 1.;

    // Loop over the weights and use it if it is one of correct
    for (int i = 0; i < WEIGHT_SIZE; ++i) {
        switch (WEIGHTIDS[i]) {
            case zf::WeightID::GEN_MC:
            case zf::WeightID::PILEUP:
            case zf::WeightID::VETO:
            case zf::WeightID::LOOSE:
            case zf::WeightID::MEDIUM:
            case zf::WeightID::TIGHT:
            case zf::WeightID::SINGLE_TRIG:
            case zf::WeightID::DOUBLE_TRIG:
            case zf::WeightID::GSF_RECO:
                weight *= WEIGHTS[i];
                break;
            default:
                break;
        }
    }

    return weight;
}

double GetOverallNormalization(const std::string NAME) {
    const double DATA_LUMI = 19712;
    const std::map<std::string, double> NORM = {
        {"Signal", 3531.89 / 30459500.},
        {"BG_Ditau", 1966.7 / 47271600.},
        {"BG_TTBar", 23.64 / 4246440},
        {"BG_single_t", 11.1 / 497658},
        {"BG_single_tbar", 11.1 / 493460.},
        {"BG_ww", 54.84 / 10000430.},
        {"BG_wz", 33.21 / 10000280.},
        {"BG_zz", 17.0 / 9799908.},
    };
    const double WEIGHT = DATA_LUMI * NORM.at(NAME);
    return WEIGHT;
}

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
    // Variable to read
    const std::string VAR_NAME = "z_y";
    // Input Files
    const std::string DATA_FILE =
        "/data/whybee0a/user/gude_2/Data/20150324_SingleElectron_2012ALL/hadded.root";
    const std::string MC_FILE =
        "/data/whybee0a/user/gude_2/MC/20150409_mc/MadGraph_hadded.root";

    // Tree
    const std::string DATA_TREE_NAME =
        "ZFinder/Combined Single Reco/Combined Single Reco";
    const std::string MC_TREE_NAME = DATA_TREE_NAME;

    // Load the tree
    TTree* data_tree = GetTTree(DATA_FILE, DATA_TREE_NAME);
    TTree* mc_tree = GetTTree(MC_FILE, MC_TREE_NAME);

    // Get the branch
    TBranch* data_reco_branch = data_tree->GetBranch("reco");
    TBranch* mc_reco_branch = mc_tree->GetBranch("reco");
    TLeaf* data_variable = data_reco_branch->GetLeaf(VAR_NAME.c_str());
    TLeaf* mc_variable = mc_reco_branch->GetLeaf(VAR_NAME.c_str());

    // Open an output file
    TFile output("ratio.root", "RECREATE");
    output.cd();

    // Fill the data histogram
    TH1D* data_histo = new TH1D("data", "data", 60, -3, 3);
    for (int i = 0; i < data_tree->GetEntries(); i++) {
        data_tree->GetEntry(i);
        data_histo->Fill(data_variable->GetValue());
    }

    // We have to grab the first entry to learn how many weights there are
    int nweights = 0;
    mc_tree->SetBranchAddress("weight_size", &nweights);
    mc_tree->GetEntry(0);

    double weights[nweights];
    int weightid[nweights];
    mc_tree->SetBranchAddress("weights", &weights);
    mc_tree->SetBranchAddress("weight_ids", &weightid);

    // Fill the MC histogram
    TH1D* mc_histo = new TH1D("mc", "mc", 60, -3, 3);
    for (int i = 0; i < mc_tree->GetEntries(); i++) {
        mc_tree->GetEntry(i);
        const double WEIGHT = GetOverallNormalization("Signal") * GetWeight(nweights, weights, weightid);
        mc_histo->Fill(mc_variable->GetValue(), WEIGHT);
    }

    // Make the ratio
    TH1D* ratio_histo = dynamic_cast<TH1D*>(data_histo->Clone("ratio"));
    ratio_histo->Divide(mc_histo);

    output.Write();
    output.Close();

    return EXIT_SUCCESS;
}
