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
#include <TCanvas.h>
#include <TStyle.h>
#include <TMath.h>
#include <TBranch.h>
#include <TLeaf.h>

// Style
#include "PlotStyle.h"

// ZFinder
#include "../../interface/WeightID.h"
#include "../../interface/ATLASBins.h"  // ATLAS_PHISTAR_BINNING

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

histogram_map Get2DHistoMap() {
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

        TBranch* reco_branch = tree->GetBranch("reco");
        TLeaf* e0_charge = reco_branch->GetLeaf("e_charge0");
        TLeaf* e1_charge = reco_branch->GetLeaf("e_charge1");
        TLeaf* mee = reco_branch->GetLeaf("z_m");
        TLeaf* phistar = reco_branch->GetLeaf("z_phistar_dressed");

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
        TH2D* histo = new TH2D("phistar_and_mass", "Phistar Vs. Mass;#phi*;m_{ee}", zf::ATLAS_PHISTAR_BINNING.size() - 1, &zf::ATLAS_PHISTAR_BINNING[0], 50, 0, 300);
        for (int i = 0; i < tree->GetEntries(); i++) {
            tree->GetEntry(i);

            // Reject opposite sign
            if (e0_charge->GetValue() * e1_charge->GetValue() < 0) {
                continue;
            }

            // We have a bug in our tuples where the charge is often set wrong.
            // Instead we selected the events from the beinging to be be same
            // sign, so we run on all of them.
            double weight = 1;
            if (!is_real_data) {
                weight = GetOverallNormalization(data_type);
                weight *= GetWeight(nweights, weights, weightid);
            }

            const double MEE = mee->GetValue();
            const double PHISTAR = phistar->GetValue();
            histo->Fill(PHISTAR, MEE, weight);
        }
        histo->Sumw2();

        delete tree;

        output_map[data_type] = histo;
    }

    return output_map;
}

TH2D* GetTemplate(histogram_map histo_map) {
    // Clone the histograms to make new ones to fill.
    TH2D* data_mass_histo = histo_map["Data"];
    TH2D* template_2d = new TH2D();
    template_2d = dynamic_cast<TH2D*>(data_mass_histo->Clone("2d_template"));

    // Empty the histogram
    template_2d->Reset();

    // Add the backgrounds and signal
    for (auto iter : histo_map) {
        if (iter.first != "Data") {
            template_2d->Add(iter.second, 1.);
        }
    }

    return template_2d;
}

void WritePNG(TH1D* data_histo, TH1D* template_histo, const std::string FILE_NAME) {
    // Make a function to fit
    FitFunction ff(*template_histo);
    TF1* fit_function = new TF1("function", ff, 0., 300., ff.nparms());
    fit_function->SetParName(0, "Histogram Amplitude");
    fit_function->SetParName(1, "alpha");
    fit_function->SetParName(2, "gamma");
    fit_function->SetParName(3, "delta");
    fit_function->SetParName(4, "beta");
    // Set limits on the fit
    fit_function->SetParLimits(0, 0.1, 1.5);
    fit_function->SetParLimits(1, 20., 50.);
    fit_function->SetParLimits(2, 0.0001, 0.3);
    fit_function->SetParLimits(3, 3., 80.);
    fit_function->SetParLimits(4, 0.1, 1000000.);
    // Set starting value
    fit_function->SetParameter(0, 0.5);
    fit_function->SetParameter(1, 60.);
    fit_function->SetParameter(2, 0.01);
    fit_function->SetParameter(3, 10.);
    fit_function->SetParameter(4, 1000.);

    data_histo->Fit("function", "LLEMRB0");

    // Set the style
    SetPlotStyle();

    // Make a canvas
    const int EDGE_IN_PIXEL = 1000;
    TCanvas canvas("canvas", "canvas", EDGE_IN_PIXEL, EDGE_IN_PIXEL);
    canvas.cd();
    gPad->SetLogy(true);

    data_histo->SetMarkerColor(kBlack);
    data_histo->SetLineColor(kBlack);
    data_histo->SetMarkerStyle(kFullCircle);
    data_histo->Draw("E");

    fit_function->Draw("Same");

    template_histo->Scale(fit_function->GetParameter(0));
    template_histo->Draw("Hist Same");

    // Make the Haupt function
    const std::string HF = "[3] * (TMath::Erfc(([0] - x) / [2]) * TMath::Exp(-[1] * x))";
    TF1* haupt_function = new TF1("haupt", HF.c_str(), 0, 300);
    haupt_function->SetParameter(0, fit_function->GetParameter(1));
    haupt_function->SetParameter(1, fit_function->GetParameter(2));
    haupt_function->SetParameter(2, fit_function->GetParameter(3));
    haupt_function->SetParameter(3, fit_function->GetParameter(4));
    haupt_function->SetLineColor(kGreen);
    haupt_function->SetLineWidth(4.);

    haupt_function->Draw("Same");

    canvas.Print(FILE_NAME.c_str(), "png");

    delete fit_function;
    delete haupt_function;
}

TH1D* Get1DFromBin(TH2D* histo, const int BIN, const std::string PREFIX) {
    // Project the histogram
    TH1D* out_histo = new TH1D();
    const std::string NAME = PREFIX + std::to_string(BIN);
    out_histo = histo->ProjectionY(NAME.c_str(), BIN, BIN, "e");

    return out_histo;
}

int main() {
    // Get a map of the histograms of the Z Masses
    histogram_map histo2d = Get2DHistoMap();

    // Open a tfile to save our histos
    TFile output_file("output.root", "RECREATE");
    output_file.cd();

    // Write and draw the histos
    TH2D* data_histo2d = histo2d["Data"];
    TH2D* template_histo2d = GetTemplate(histo2d);

    for (int i = 1; i < zf::ATLAS_PHISTAR_BINNING.size(); ++i) {
        TH1D* data_histo = Get1DFromBin(data_histo2d, i, "data_bin_");
        TH1D* template_histo = Get1DFromBin(template_histo2d, i, "template_bin_");
        std::string OUT_NAME = "fit_from_bin_" + std::to_string(i) + ".png";
        WritePNG(data_histo, template_histo, OUT_NAME);
        delete data_histo;
        delete template_histo;
    }

    // Get the background subtracted MZ
    TH1D* data_mass = data_histo2d->ProjectionY("data_mass", 1, -1, "e");
    TH1D* mc_mass = template_histo2d->ProjectionY("mc_mass", 1, -1, "e");

    data_mass->Add(mc_mass, -1);

    data_histo2d->Write();
    template_histo2d->Write();
    data_mass->Write();

    output_file.Write();
    output_file.Close();

    return EXIT_SUCCESS;
}
