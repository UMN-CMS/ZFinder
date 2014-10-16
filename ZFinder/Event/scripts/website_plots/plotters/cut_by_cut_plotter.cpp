// Standard Library
#include <algorithm>  //std::equal_range, std::sort, std::unique
#include <cmath>  // std::abs
#include <iostream>
#include <map>
#include <string>
#include <vector>

// ROOT
#include <TCanvas.h>
#include <TFile.h>
#include <TLegend.h>
#include <TH1D.h>

// Plotters
#include "lib/style.h"  // set_plot_style

int Plotter(
        const std::string& INPUT_FILE,
        const std::string& OUTPUT_FILE,
        const std::string& RATIO_OUTPUT_FILE
        ) {
    // Constants
    const double RIGHT_EDGE_ = 0.96;
    const double TOP_EDGE_ = 0.95;
    const int X_SIZE = 1200;
    const int Y_SIZE = 800;
    const std::vector<double> DESIRED_BINNING = {
        0.001, 0.004, 0.008, 0.012, 0.016, 0.020, 0.024, 0.029, 0.034, 0.039,
        0.045, 0.052, 0.057, 0.064, 0.072, 0.081, 0.091, 0.102, 0.114, 0.128,
        0.145, 0.165, 0.189, 0.219, 0.258, 0.312, 0.391, 0.524, 0.695, 0.918,
        1.153, 1.496, 1.947, 2.522, 3.277, 10.
    };
    const int COLORS[] = {kRed, kBlue, kGreen + 2, kMagenta + 2, kOrange + 7, kOrange};

    set_plot_style();

    // Open the TFILE
    TFile* input_tfile = new TFile(INPUT_FILE.c_str());

    const int NUMBER_OF_HISTOGRAMS = 5;
    TH1D* histograms[NUMBER_OF_HISTOGRAMS];
    input_tfile->GetObject("ZFinder/0 Gen Mass Only Reco/0 All Events/Naked #phi*", histograms[0]);
    input_tfile->GetObject("ZFinder/0 Gen Mass Only Reco/2 60 < GEN M_{ee} < 120/Naked #phi*", histograms[1]);
    input_tfile->GetObject("ZFinder/1 Acceptance Cuts Reco/4 60 < GEN M_{ee} < 120/Naked #phi*", histograms[2]);
    input_tfile->GetObject("ZFinder/2 ID Cuts Reco/6 60 < M_{ee} < 120/Naked #phi*", histograms[3]);
    input_tfile->GetObject("ZFinder/3 Single Trigger Cuts Reco/7 60 < M_{ee} < 120/Naked #phi*", histograms[4]);

    for (int i = 0; i < NUMBER_OF_HISTOGRAMS; ++i) {
        if (!histograms[i]) {
            std::cout << "Histogram " << i << " failed to load!" << std::endl;
            return 1;
        }
    }

    // set up the canvas
    TCanvas canvas("canvas", "canvas", X_SIZE, Y_SIZE);
    gPad->SetLogy(true);
    gPad->SetLogx(true);

    // Add all the plots
    //const double SCALE_FACTOR = 19.7e15 / 1.13e9 / histograms[0]->Integral();
    //histograms[0]->SetMaximum(30 * histograms[0]->GetMaximum());  // rescale to avoid hitting the ledgend
    histograms[0]->GetYaxis()->SetTitle("Events / Unit #phi*");
    for (int i = 0; i < NUMBER_OF_HISTOGRAMS; ++i) {
        std::string opts_str = "SAME";
        if (i == 0) {
            opts_str = "";
        }
        // Set the line color
        histograms[i]->Scale(1, "width");
        histograms[i]->Sumw2();
        histograms[i]->SetMarkerStyle(kFullCircle);
        histograms[i]->SetMarkerColor(COLORS[i]);
        histograms[i]->SetLineColor(COLORS[i]);
        histograms[i]->SetLineWidth(2);
        histograms[i]->Draw(opts_str.c_str());
    }

    // Set up the legend
    const double LEG_HEIGHT = 0.30;
    const double LEG_LENGTH = 0.25;
    TLegend legend(
        // Hardcoded offset is to avoid ticks
        (RIGHT_EDGE_ - 0.025) - LEG_LENGTH,
        (TOP_EDGE_ - 0.025) - LEG_HEIGHT,
        RIGHT_EDGE_ - 0.025,
        TOP_EDGE_ - 0.025
    );
    legend.SetFillColor(kWhite);
    const std::string LED_TYPE = "l";
    legend.AddEntry(histograms[0], "All Z Like Events", LED_TYPE.c_str());
    legend.AddEntry(histograms[1], "GSF + 60 < Gen M_{ee} < 120", LED_TYPE.c_str());
    legend.AddEntry(histograms[2], "|#eta| < 2.1 (2.4), p_{T} > 30 (20)", LED_TYPE.c_str());
    legend.AddEntry(histograms[3], "ID Tight (Medium)", LED_TYPE.c_str());
    legend.AddEntry(histograms[4], "Single E Trigger", LED_TYPE.c_str());
    legend.SetBorderSize(0);  // Remove drop shadow and border
    legend.SetFillStyle(0);  // Transparent
    legend.Draw();

    // Save the output file
    canvas.Print(OUTPUT_FILE.c_str(), "png");

    // Make ratio plots
    TCanvas canvas2("canvas2", "canvas2", X_SIZE, Y_SIZE);
    canvas2.cd();
    gPad->SetLogx(true);

    TH1D* total_histo = dynamic_cast<TH1D*>(histograms[1]->Clone());
    TH1D* acceptance_histo = dynamic_cast<TH1D*>(histograms[2]->Clone());

    total_histo->Divide(histograms[4], total_histo, 1, 1, "B");
    acceptance_histo->Divide(histograms[4], acceptance_histo, 1, 1, "B");

    total_histo->GetYaxis()->SetTitle("Ratio");

    total_histo->SetMaximum(1.);
    total_histo->SetMinimum(0.);

    total_histo->Draw("E");
    acceptance_histo->Draw("E SAME");

    // Set up the legend
    TLegend legend2(
        // Hardcoded offset is to avoid ticks
        (RIGHT_EDGE_ - 0.025) - .4,
        (TOP_EDGE_ - 0.025) - .2,
        RIGHT_EDGE_ - 0.025,
        TOP_EDGE_ - 0.025
    );
    legend2.SetFillColor(kWhite);
    legend2.AddEntry(acceptance_histo, "All Cuts / Mass, GSF, Acceptance Cuts", LED_TYPE.c_str());
    legend2.AddEntry(total_histo, "All Cuts / Mass and GSF Cut", LED_TYPE.c_str());
    legend2.SetBorderSize(0);  // Remove drop shadow and border
    legend2.SetFillStyle(0);  // Transparent
    legend2.Draw();

    canvas2.Print(RATIO_OUTPUT_FILE.c_str(), "png");

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
        std::string output_file(argv[2]);
        std::string ratio_output_file(argv[3]);

        return Plotter(input_file, output_file, ratio_output_file);
    }
}
