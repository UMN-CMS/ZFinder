// Interface
#include "same_sign.h"  // branch_struct

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

std::map<std::string, TH1D*> GetHistoMap() {
    // The list of files
    std::map<std::string, std::string> files_to_open = {
        {"Data", "/data/whybee0a/user/gude_2/Data/20140220_SingleElectron_2012_same_sign_ALL/hadded.root"},
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
    std::map<std::string, TH1D*> output_map;
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
        for (int i = 0; i < tree->GetEntries(); i++) {
            tree->GetEntry(i);

            // We have a bug in our tuples where the charge is often set wrong.
            // Instead we selected the events from the beinging to be be same
            // sign, so we run on all of them.
            //const int CHARGE0 = reco_branch.e_charge[0];
            //const int CHARGE1 = reco_branch.e_charge[1];
            //const bool SAME_SIGN = (CHARGE0 * CHARGE1) > 0;
            //const bool NOT_Z = (reco_branch.z_m < 60 || reco_branch.z_m > 120);
            const bool NOT_Z = (reco_branch.z_m < 60);
            if (NOT_Z) {
                double weight = 1;
                if (!is_real_data) {
                    weight = GetOverallNormalization(data_type);
                    weight *= GetWeight(nweights, weights, weightid);
                }

                const double PHISTAR = reco_branch.z_phistar_dressed;
                phistar_histo->Fill(PHISTAR, weight);
            }
        }
        // Put the histogram into the map
        output_map[data_type] = phistar_histo;

        delete tree;
    }

    return output_map;
}

TH1D* GetSubstractedHistogram(std::map<std::string, TH1D*> histo_map) {
    TH1D* data_histo = histo_map["Data"];
    TH1D* qcd_histo = new TH1D();
    qcd_histo = dynamic_cast<TH1D*>(data_histo->Clone("qcd_histo"));
    // Subtract the backgrounds (but not that data) from our cloned data
    // histogram to get the histogram of just the events unaccounted for in our
    // MC
    for (auto iter : histo_map) {
        if (iter.first != "Data") {
            qcd_histo->Add(iter.second, -1.);
        }
    }

    return qcd_histo;
}

TH1D* GetRatioHistogram(std::map<std::string, TH1D*> histo_map) {
    // Add all the backgrounds together
    TH1D* signal_histo = histo_map["Signal"];
    TH1D* backgrounds_histos = dynamic_cast<TH1D*>(signal_histo->Clone("bgs"));
    for (auto iter : histo_map) {
        if (iter.first != "Data" && iter.first != "Signal") {
            backgrounds_histos->Add(iter.second, 1.);
        }
    }
    // Take the ratio
    TH1D* data_histo = histo_map["Data"];
    TH1D* ratio_histo = new TH1D();
    ratio_histo = dynamic_cast<TH1D*>(data_histo->Clone("ratio_histo"));

    ratio_histo->Divide(data_histo, backgrounds_histos);

    return ratio_histo;

}

TF1* FitGaussAndLine(TH1D* qcd_histo) {
    TF1* gaus_and_line = new TF1("gaus_and_line", "gaus(0) + pol1(3)", 60, 120);

    gaus_and_line->SetParameter(0, 300.); // Gauss amplitude
    gaus_and_line->SetParameter(1, 91.);  // Gauss center
    gaus_and_line->SetParameter(2, 4.);   // Gauss width

    qcd_histo->Fit("gaus_and_line", "LLEMR");
    return gaus_and_line;
}

int main() {
    // Get a map of the histograms of the Z Masses
    std::map<std::string, TH1D*> histo_map = GetHistoMap();

    // Get a background subtracted histogram
    TH1D* qcd_histo = GetSubstractedHistogram(histo_map);

    // Get the ratio of data to background
    //TH1D* ratio_histo = GetRatioHistogram(histo_map);

    // Fit the qcd histogram
    //TF1* gaus_and_line = FitGaussAndLine(qcd_histo);

    // Open a tfile to save our histos
    TFile output_file("output.root", "RECREATE");
    output_file.cd();

    //for (auto iter : histo_map) {
    //    iter.second->Write();
    //}
    qcd_histo->Write();
    //ratio_histo->Write();

    // Draw the fit
    qcd_histo->Draw();
    //gaus_and_line->Draw("same");

    output_file.Write();
    output_file.Close();

    return EXIT_SUCCESS;
}
