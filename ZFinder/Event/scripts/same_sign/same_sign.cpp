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
    std::map<std::string, double> norm = {
        {"Signal", 3531.89 / 30459500.},
        {"BG_Ditau", 1966.7 / 47271600.},
        {"BG_TTBar", 23.64 / 4246440},
        {"BG_single_t", 11.1 / 497658},
        {"BG_single_tbar", 11.1 / 493460.},
        {"BG_ww", 54.84 / 10000430.},
        {"BG_wz", 33.21 / 10000280.},
        {"BG_zz", 17.0 / 9799908.},
    };

    return DATA_LUMI * norm[NAME];
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

std::map<std::string, std::pair<TH1D*, TH1D*>> GetHistoMap() {
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
    std::map<std::string, std::pair<TH1D*, TH1D*>> output_map;
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
        TH1D* phistar_histo = new TH1D("phistar", "Phi*:#phi*:Counts", zf::ATLAS_PHISTAR_BINNING.size() - 1, &zf::ATLAS_PHISTAR_BINNING[0]);
        TH1D* z_mass_histo = new TH1D("z_mass", "m_{ee}:m_{ee}:Counts", 300, 0, 300);
        for (int i = 0; i < tree->GetEntries(); i++) {
            tree->GetEntry(i);

            // We have a bug in our tuples where the charge is often set wrong.
            // Instead we selected the events from the beinging to be be same
            // sign, so we run on all of them.
            //const int CHARGE0 = reco_branch.e_charge[0];
            //const int CHARGE1 = reco_branch.e_charge[1];
            //const bool SAME_SIGN = (CHARGE0 * CHARGE1) > 0;
            //const bool NOT_Z = (reco_branch.z_m < 60 || reco_branch.z_m > 120);
            double weight = 1;
            if (!is_real_data) {
                weight = GetOverallNormalization(data_type);
                weight *= GetWeight(nweights, weights, weightid);
            }

            const double PHISTAR = reco_branch.z_phistar_dressed;
            phistar_histo->Fill(PHISTAR, weight);
            const double MEE = reco_branch.z_m;
            z_mass_histo->Fill(MEE, weight);
        }
        // Put the histogram into the map
        output_map[data_type] = {phistar_histo, z_mass_histo};

        delete tree;
    }

    return output_map;
}

std::pair<TH1D*, TH1D*> GetTemplates(std::map<std::string, std::pair<TH1D*, TH1D*>> histo_map) {
    TH1D* data_phi_histo = histo_map["Data"].first;
    TH1D* summed_phi = new TH1D();
    summed_phi = dynamic_cast<TH1D*>(data_phi_histo->Clone("template_phi"));
    TH1D* data_mass_histo = histo_map["Data"].second;
    TH1D* summed_mass = new TH1D();
    summed_mass = dynamic_cast<TH1D*>(data_mass_histo->Clone("template_mass"));
    // Add the backgrounds and signal
    for (auto iter : histo_map) {
        if (iter.first != "Data") {
            summed_phi->Add(iter.second.first, 1.);
            summed_mass->Add(iter.second.second, 1.);
        }
    }

    return {summed_phi, summed_mass};
}

int main() {
    // Get a map of the histograms of the Z Masses
    std::map<std::string, std::pair<TH1D*, TH1D*>> histo_map = GetHistoMap();

    // Get a templates
    std::pair<TH1D*, TH1D*> template_histos = GetTemplates(histo_map);
    TH1D* template_phi = template_histos.first;
    TH1D* template_mass = template_histos.second;

    // Get the data
    TH1D* data_phi = histo_map["Data"].first;
    TH1D* data_mass = histo_map["Data"].second;

    // Make our fit function
    FitFunction ff(*template_mass);
    TF1* function = new TF1("function", ff, 0., 300., ff.nparms());

    data_mass->Fit("function", "LLEMR");

    // Get the same "scale factor"
    const double AMPLITUDE = function->GetParameter(0);

    // Subtract the template Phi* from the data
    TH1D* qcd_phistar = dynamic_cast<TH1D*>(data_phi->Clone("qcd_phistar"));
    qcd_phistar->Add(template_phi, -AMPLITUDE);

    // Open a tfile to save our histos
    TFile output_file("output.root", "RECREATE");
    output_file.cd();

    // Write and draw the histos
    template_phi->Write();
    data_phi->Write();
    template_mass->Write();
    data_mass->Write();
    qcd_phistar->Write();

    template_phi->Draw();
    data_phi->Draw("E SAME");
    template_mass->Draw();
    data_mass->Draw("E SAME");
    qcd_phistar->Draw();

    output_file.Write();
    output_file.Close();

    // Clean up
    delete function;

    return EXIT_SUCCESS;
}
