// Standard Library
#include <iostream>
#include <sstream>
#include <string>
#include <utility>  // std::pair
#include <vector>

// ROOT
#include <TCanvas.h>
#include <TFile.h>
#include <TH1D.h>

// Plotters
#include "lib/style.h"  // set_plot_style

int Plotter(
        const std::string& INPUT_FILE,
        const std::string& OUTPUT_DIR,
        const bool is_mc
        ) {
    // Constants
    const int X_SIZE = 1200;
    const int Y_SIZE = 800;
    std::string base_directory = "ZFinder/Combined Single Reco/7 60 < M_{ee} < 120/";
    if (is_mc) {
        base_directory = "ZFinder/Combined Single MC/7 60 < M_{ee} < 120/";
    }
    // A list of all the histograms to write:
    // { Name in TFIle, Name to use for output plot }
    const std::vector<std::pair<std::string, std::string>> HISTOGRAM_NAMES = {
        {"Z0 Mass: All", "z_mass_all",},
        {"Z0 Mass: Coarse", "z_mass_coarse",},
        {"Z0 Mass: Fine", "z_mass_fine",},
        {"Z0 Rapidity", "z_rapidity",},
        {"Z0 p_{T}", "z_pt",},
        {"p_{T,e_{0}}", "e0_pt",},
        {"p_{T,e_{0},Naked}", "e0_pt_naked",},
        {"p_{T,e_{0},Born}", "e0_pt_born",},
        {"p_{T,e_{1}}", "e1_pt",},
        {"p_{T,e_{1},Naked}", "e1_pt_naked",},
        {"p_{T,e_{1},Born}", "e1_pt_born",},
        {"#eta_{e_{0}}", "e0_eta",},
        {"#eta_{e_{0},Naked}", "e0_eta_nake",},
        {"#eta_{e_{0},Born}", "e0_eta_born",},
        {"#eta_{e_{1}}", "e1_eta",},
        {"#eta_{e_{1},Naked}", "e1_eta_naked",},
        {"#eta_{e_{1},Born}", "e1_eta_born",},
        {"#phi_{e_{0}}", "e0_phi",},
        {"#phi_{e_{0},Naked}", "e0_phi_naked",},
        {"#phi_{e_{0},Born}", "e0_phi_born",},
        {"#phi_{e_{1}}", "e1_phi",},
        {"#phi_{e_{1},Naked}", "e1_phi_naked",},
        {"#phi_{e_{1},Born}", "e1_phi_born",},
        //{"charge_{e_{0}}", "e0_charge",},
        //{"charge_{e_{1}}", "e1_charge",},
        {"#phi*", "phistar",},
        {"Born #phi*", "phistar_born",},
        {"Naked #phi*", "phistar_naked",},
        //{"Other #phi*", "phistar_other",},
        //{"Other Rapidity", "z_rapidity_other",},
        {"N_{Vertices}", "vertices",},
        {"N_{e}", "number_of_electrons",},
        {"Base Weight", "base_weight",},
        {"Full Weight", "full_weight",},
        {"p_{T,e_{0}} Vs. Trigger", "e0_pt_vs_trigger",},
        {"p_{T,e_{1}} Vs. Trigger", "e1_pt_vs_trigger",},
        {"#phi*: Reco Vs. Truth", "phistar_reco_vs_truth",},
        {"#DeltaR", "delta_r",},
    };
    int plot_color = kBlue;
    if (is_mc) {
        plot_color = kRed;
    }

    set_plot_style();

    // Open the TFILE
    TFile* input_tfile = new TFile(INPUT_FILE.c_str());

    // Loop over all the histograms
    for (auto& name_pair : HISTOGRAM_NAMES) {
        // Open the histogram
        TH1D* histo;
        const std::string NAME = base_directory + name_pair.first;
        input_tfile->GetObject(NAME.c_str(), histo);
        if (!histo) {
            std::cout << "Histogram " << name_pair.first << " failed to load!" << std::endl;
            return 1;
        }

        // Skip if there are no events
        if (histo->GetEntries() <= 0.) {
            continue;
        }

        // set up the canvas
        TCanvas canvas("canvas", "canvas", X_SIZE, Y_SIZE);
        gPad->SetLogy(true);
        //gPad->SetLogx(true);

        // Set the line color and style
        histo->Scale(1, "width");
        histo->SetLineColor(plot_color);
        histo->SetLineWidth(2);
        const int FORWARD_HATCH = 3004;
        histo->SetFillStyle(FORWARD_HATCH);
        histo->SetFillColor(plot_color);
        histo->Draw();

        // Save the output file
        std::string output_file = OUTPUT_DIR + '/' + name_pair.second + ".png";
        if (is_mc) {
            output_file = OUTPUT_DIR + '/' + name_pair.second + "_mc.png";
        }
        canvas.Print(output_file.c_str(), "png");
    }

    return 0;
}

int main(int argc, char* argv[]) {
    const int ARGN = 4;

    if (argc < ARGN) {
        std::cout<<"Not enough arguments.";
        return 1;
    } else if (argc > ARGN) {
        std::cout<<"Too many arguments.";
        return 1;
    } else {
        /* Read in arguments */
        std::string input_file(argv[1]);
        std::string output_dir(argv[2]);

        std::istringstream ss(argv[3]);
        bool is_mc;
        if (!(ss >> is_mc)) {
            std::cout << "Invalid bool " << argv[1] << std::endl;
            return 1;
        }

        std::cout << is_mc << std::endl;

        return Plotter(input_file, output_dir, is_mc);
    }
}
