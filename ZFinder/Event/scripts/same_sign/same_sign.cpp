// Interface
#include "same_sign.h"  // branch_struct

// Fit Function
#include "FitFunction.h"

// Standard Library
#include <stdexcept>
#include <iostream>

// ROOT
#include <TFile.h>
#include <TBranch.h>
#include <TF1.h>

// ZFinder
#include "../../interface/WeightID.h"
#include "../../interface/ATLASBins.h"  // ATLAS_PHISTAR_BINNING


TTree* GetTTree(const std::string TFILE, const std::string TTREE) {
    // Open the TFiles
    TFile* mc_tfile = new TFile(TFILE.c_str(), "READ");
    if (!mc_tfile) {
        throw std::runtime_error("Could not open the file");
    }

    // Load the tree
    TTree* tree = new TTree();
    mc_tfile->GetObject(TTREE.c_str(), tree);
    if (!tree) {
        throw std::runtime_error("Could not open the TTree");
    }

    return tree;
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

    return DATA_LUMI * NORM[NAME];
}

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

histogram_map GetHistoMap() {
    // The list of files
    std::map<std::string, std::string> files_to_open = {
        {"Data", "/data/whybee0a/user/gude_2/Data/20150306_SingleElectron_2012ALL_same_sign/20150306_SingleElectron_2012ALL_same_sign.root"},
        {"Signal", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/MadGraph_hadded.root"},
        {"BG_Ditau", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/BG_Ditau_hadded.root"},
        {"BG_TTBar", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/BG_TTBar_hadded.root"},
        {"BG_single_t", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/BG_singlet_tw_hadded.root"},
        {"BG_single_tbar", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/BG_singlet_tbarw_hadded.root"},
        {"BG_ww", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/BG_ww_hadded.root"},
        {"BG_wz", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/BG_wz_hadded.root"},
        {"BG_zz", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/BG_zz_hadded.root"},
    };

    // The Tree to access
    const std::string TREE_NAME =
        "ZFinder/Combined Single Reco/Combined Single Reco";

    // Open the files and fill the file map
    histogram_map output_map;
    for (auto iter : files_to_open) {
        std::string data_type = iter.first;
        std::string file_name = iter.second;
        bool is_real_data = false;
        if (data_type == "Data") {
            is_real_data = true;
        }

        // Open the file and load the tree
        TTree* tree = GetTTree(file_name, TREE_NAME);

        branch_struct reco_branch;
        tree->SetBranchAddress("reco", &reco_branch);

        // We have to grab the first entry to learn how many weights there are
        int nweights = 0;
        if (!is_real_data) {
            tree->SetBranchAddress("weight_size", &nweights);
            tree->GetEntry(0);
        }
        double weights[nweights];
        int weightid[nweights];
        if (!is_real_data) {
            tree->SetBranchAddress("weights", &weights);
            tree->SetBranchAddress("weight_ids", &weightid);
        }

        // Pack into a hitogram
        TH1D* z_mass_histo = new TH1D("z_mass", "m_{ee};m_{ee};Counts", 300, 0, 300);
        TH1D* low_phistar_histo = new TH1D("low_phistar", "Phi*;#phi*;Counts", zf::ATLAS_PHISTAR_BINNING.size() - 1, &zf::ATLAS_PHISTAR_BINNING[0]);
        TH1D* high_phistar_histo = new TH1D("high_phistar", "Phi*;#phi*;Counts", zf::ATLAS_PHISTAR_BINNING.size() - 1, &zf::ATLAS_PHISTAR_BINNING[0]);
        for (int i = 0; i < tree->GetEntries(); i++) {
            tree->GetEntry(i);

            // We have a bug in our tuples where the charge is often set wrong.
            // Instead we selected the events from the beinging to be be same
            // sign, so we run on all of them.
            double weight = 1;
            if (!is_real_data) {
                weight = GetOverallNormalization(data_type);
                weight *= GetWeight(nweights, weights, weightid);
            }

            const double MEE = reco_branch.z_m;
            z_mass_histo->Fill(MEE, weight);
            const double PHISTAR = reco_branch.z_phistar_dressed;
            // If we have a sideband event, fill the phistar histrograms
            if (MEE < 60) {
                low_phistar_histo->Fill(PHISTAR, weight);
            }
            else if (MEE > 120) {
                high_phistar_histo->Fill(PHISTAR, weight);
            }
        }
        // Put the histogram into the map
        z_mass_histo->Sumw2();
        low_phistar_histo->Sumw2();
        high_phistar_histo->Sumw2();
        output_map[data_type] = {z_mass_histo, low_phistar_histo, high_phistar_histo};

        delete tree;
    }

    return output_map;
}

histogram_container GetTemplates(histogram_map histo_map) {
    // Clone the histograms to make new ones to fill.
    TH1D* data_mass_histo = histo_map["Data"].mass;
    TH1D* summed_mass = new TH1D();
    summed_mass = dynamic_cast<TH1D*>(data_mass_histo->Clone("template_mass"));

    TH1D* data_low_histo = histo_map["Data"].low_side_phistar;
    TH1D* data_high_histo = histo_map["Data"].high_side_phistar;
    TH1D* low_phistar = new TH1D();
    TH1D* high_phistar = new TH1D();
    low_phistar = dynamic_cast<TH1D*>(data_low_histo->Clone("low_phistar_template"));
    high_phistar = dynamic_cast<TH1D*>(data_high_histo->Clone("high_phistar_template"));
    // Add the backgrounds and signal
    for (auto iter : histo_map) {
        if (iter.first != "Data") {
            summed_mass->Add(iter.second.mass, 1.);
            low_phistar->Add(iter.second.low_side_phistar, 1.);
            high_phistar->Add(iter.second.high_side_phistar, 1.);
        }
    }

    summed_mass->Sumw2();
    low_phistar->Sumw2();
    high_phistar->Sumw2();

    return {summed_mass, low_phistar, high_phistar};
}

int main() {
    // Get a map of the histograms of the Z Masses
    histogram_map histo_map = GetHistoMap();

    // Get a templates
    histogram_container templates = GetTemplates(histo_map);
    TH1D* template_mass = templates.mass;
    //TH1D* template_low_phistar = templates.low_side_phistar;
    //TH1D* template_high_phistar = templates.high_side_phistar;

    // Get the data
    TH1D* data_mass = histo_map["Data"].mass;
    //TH1D* data_low_phistar = histo_map["Data"].low_side_phistar;
    //TH1D* data_high_phistar = histo_map["Data"].high_side_phistar;

    // Open a tfile to save our histos
    TFile output_file("output.root", "RECREATE");
    output_file.cd();

    // Take the ratio of the data and the MC
    TH1D* data_mass_histo = histo_map["Data"].mass;
    TH1D* ratio_histogram = new TH1D();
    const int REBIN = 6;
    template_mass->Rebin(REBIN);
    data_mass->Rebin(REBIN);
    ratio_histogram = dynamic_cast<TH1D*>(data_mass->Clone("ratio"));
    ratio_histogram->Divide(template_mass);

    // Write and draw the histos
    template_mass->Write();
    data_mass->Write();
    ratio_histogram->Draw();

    output_file.Write();
    output_file.Close();

    return EXIT_SUCCESS;
}
