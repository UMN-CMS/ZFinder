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

// ZFinder
#include "../../interface/WeightID.h"
#include "../../interface/ATLASBins.h"  // ATLAS_PHISTAR_BINNING

void set_plot_style() {
    TStyle* style = new TStyle("style","Style for P-TDR");

    // For the canvas:
    style->SetCanvasBorderMode(0);
    style->SetCanvasColor(kWhite);
    style->SetCanvasDefX(0);  //Position on screen
    style->SetCanvasDefY(0);

    // For the Pad:
    style->SetPadBorderMode(0);
    style->SetPadColor(kWhite);
    style->SetPadGridX(false);
    style->SetPadGridY(false);
    style->SetGridColor(kBlack);
    style->SetGridStyle(3);
    style->SetGridWidth(1);

    // For the frame:
    style->SetFrameBorderMode(0);
    style->SetFrameBorderSize(1);
    style->SetFrameFillColor(kWhite);
    style->SetFrameFillStyle(0);
    style->SetFrameLineColor(kBlack);
    style->SetFrameLineStyle(1);
    style->SetFrameLineWidth(1);

    // For the histo:
    // style->SetHistFillColor(1);
    style->SetHistFillStyle(0); //
    style->SetHistLineColor(kBlack);
    style->SetHistLineStyle(0);
    style->SetHistLineWidth(1);

    style->SetEndErrorSize(2);
    style->SetErrorX(0.);

    style->SetMarkerStyle(20);

    //For the fit/function:
    style->SetOptFit(1);
    style->SetFitFormat("5.4g");
    style->SetFuncColor(kRed);
    style->SetFuncStyle(1);
    style->SetFuncWidth(1);

    //For the date:
    style->SetOptDate(0);

    // For the statistics box:
    style->SetOptFile(0);
    style->SetOptStat(0);  // To display the mean and RMS: SetOptStat("mr");
    style->SetStatColor(kWhite);
    style->SetStatFont(42);
    style->SetStatFontSize(0.025);
    style->SetStatTextColor(kBlack);
    style->SetStatFormat("6.4g");
    style->SetStatBorderSize(1);
    style->SetStatH(0.1);
    style->SetStatW(0.15);

    // For the Global title:
    style->SetOptTitle(0);
    style->SetTitleFont(42);
    style->SetTitleColor(kBlack);
    style->SetTitleTextColor(kBlack);
    style->SetTitleFillColor(kWhite);  //10 is roughly kWhite, 10% grey?
    style->SetTitleFontSize(0.05);

    // For the axis titles:
    style->SetTitleColor(kBlack, "XYZ");
    style->SetTitleFont(42, "XYZ");
    style->SetTitleSize(0.06, "XYZ");
    style->SetTitleXOffset(0.9);
    style->SetTitleYOffset(1.25);

    // For the axis labels:
    style->SetLabelColor(kBlack, "XYZ");
    style->SetLabelFont(42, "XYZ");
    style->SetLabelOffset(0.007, "XYZ");
    style->SetLabelSize(0.05, "XYZ");

    // For the axis:
    style->SetAxisColor(kBlack, "XYZ");
    style->SetStripDecimals(true);
    style->SetTickLength(0.03, "XYZ");
    style->SetNdivisions(510, "XYZ");
    style->SetPadTickX(true);  // To get tick marks on the opposite side of the frame
    style->SetPadTickY(true);

    // Change for log plots:
    style->SetOptLogx(false);
    style->SetOptLogy(false);
    style->SetOptLogz(false);

    // Set the style
    style->cd();
}


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

    return DATA_LUMI * NORM.at(NAME);
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
        TH1D* z_mass_histo = new TH1D("z_mass", "m_{ee};m_{ee};Counts", 50, 0, 300);
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

void WritePNG(
        const std::string OUTPUT_NAME,
        TH1D* data_histo,
        TH1D* template_histo,
        TF1* fit_function,
        const bool LOGY = true,
        const bool LOGX = false
        ) {
    // Make some plots
    set_plot_style();
    const int X_SIZE = 1000;
    const int Y_SIZE = 1000;
    TCanvas canvas("canvas", "canvas", X_SIZE, Y_SIZE);
    canvas.cd();
    gPad->SetLogy(LOGY);
    gPad->SetLogx(LOGX);

    data_histo->SetMarkerColor(kBlack);
    data_histo->SetLineColor(kBlack);
    data_histo->SetMarkerStyle(kFullCircle);

    //data_histo->Rebin(5);
    //data_histo->Scale(1/5.);
    data_histo->Draw("E");
    //fit_function->Draw("SAME");

    template_histo->Scale(fit_function->GetParameter(0));
    //template_histo->Rebin(5);
    //template_histo->Scale(1/5.);
    template_histo->SetLineColor(kBlue);
    template_histo->SetLineStyle(kDashed);
    template_histo->SetLineWidth(2.);
    template_histo->Draw("hist SAME");

    // Draw a line for the QCD background
    TF1 line("line", "pol0", template_histo->GetBinLowEdge(1), template_histo->GetBinLowEdge(template_histo->GetNbinsX()) + template_histo->GetBinWidth(template_histo->GetNbinsX()));
    line.SetParameter(0, fit_function->GetParameter(1));
    line.SetLineColor(kRed);
    line.SetLineStyle(kDashed);
    line.Draw("SAME");

    // Combine the line and the histogram
    TH1D* combined_histo = new TH1D();
    combined_histo = dynamic_cast<TH1D*>(template_histo->Clone("combined_histo"));
    for (int bin_num = 1; bin_num <= combined_histo->GetNbinsX(); ++bin_num) {
        const double OLD_CONT = combined_histo->GetBinContent(bin_num);
        combined_histo->SetBinContent(bin_num, OLD_CONT + fit_function->GetParameter(1));
    }
    combined_histo->SetLineColor(kBlack);
    combined_histo->SetLineWidth(3.);
    combined_histo->SetLineStyle(kSolid);
    combined_histo->Draw("HIST SAME");

    data_histo->Draw("SAME");

    canvas.Print(OUTPUT_NAME.c_str(), "png");

    delete combined_histo;
}

int main() {
    // Get a map of the histograms of the Z Masses
    histogram_map histo_map = GetHistoMap();

    // Get a templates
    histogram_container templates = GetTemplates(histo_map);
    TH1D* template_mass = templates.mass;
    TH1D* template_low_phistar = templates.low_side_phistar;
    TH1D* template_high_phistar = templates.high_side_phistar;

    // Get the data
    TH1D* data_mass = histo_map["Data"].mass;
    TH1D* data_low_phistar = histo_map["Data"].low_side_phistar;
    TH1D* data_high_phistar = histo_map["Data"].high_side_phistar;

    // Make our fit function
    FitFunction ff(*template_mass);
    TF1* fit_function = new TF1("function", ff, 0., 300., ff.nparms());
    data_mass->Fit("function", "LLEMR0");

    FitFunction low_ff(*template_low_phistar);
    TF1* low_fit_function = new TF1("low_function", low_ff, 0., 10., low_ff.nparms());
    data_low_phistar->Fit("low_function", "LLEMR0");

    FitFunction high_ff(*template_high_phistar);
    TF1* high_fit_function = new TF1("high_function", high_ff, 0., 10., high_ff.nparms());
    data_high_phistar->Fit("high_function", "LLEMR0");

    // Get the same "scale factor"
    //const double AMPLITUDE = fit_function->GetParameter(0);

    // Open a tfile to save our histos
    TFile output_file("output.root", "RECREATE");
    output_file.cd();

    // Take the ratio of the data and the MC
    //TH1D* data_mass_histo = histo_map["Data"].mass;
    //TH1D* ratio_histogram = new TH1D();
    //const int REBIN = 6;
    //template_mass->Rebin(REBIN);
    //data_mass->Rebin(REBIN);
    //ratio_histogram = dynamic_cast<TH1D*>(data_mass->Clone("ratio"));
    //ratio_histogram->Divide(template_mass);

    // Write and draw the histos
    template_mass->Write();
    data_mass->Write();
    //ratio_histogram->Draw();
    template_low_phistar->Write();
    data_low_phistar->Write();
    template_high_phistar->Write();
    data_high_phistar->Write();

    // Make PNGs
    WritePNG("mass.png", data_mass, template_mass, fit_function);
    WritePNG("low.png", data_low_phistar, template_low_phistar, low_fit_function, true, true);
    WritePNG("high.png", data_high_phistar, template_high_phistar, high_fit_function, true, true);

    output_file.Write();
    output_file.Close();

    std::cout << "HERE" << std::endl;

    return EXIT_SUCCESS;
}
