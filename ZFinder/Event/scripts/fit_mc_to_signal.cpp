#include <string>
#include <iostream>
#include <map>

#include "TH1.h"
#include "TH1I.h"
#include "TF1.h"
#include "TFile.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TObject.h"
#include "TCanvas.h"
#include <TROOT.h>
#include <TStyle.h>
#include <TGraphAsymmErrors.h>
#include <TMultiGraph.h>

int MakePlots(
         std::string input_data_file,
         std::string input_mc_file,
         std::string data_hist_name,
         std::string mc_hist_name,
         std::string output_file
        ) {
    std::cout << input_data_file << std::endl;
    std::cout << input_mc_file << std::endl;
    /* Open the root file */
    TFile* in_tfile_data = new TFile(input_data_file.c_str(), "READ");
    TFile* in_tfile_mc = new TFile(input_mc_file.c_str(), "READ");
    if (!in_tfile_data || !in_tfile_mc) {
        std::cout << "Failed to open the input file" << std::endl;
        return 1;
    }

    /* Extract the histograms */
    TH1I* data_hist;
    TH1I* mc_hist;
    in_tfile_data->GetObject(data_hist_name.c_str(), data_hist);
    in_tfile_mc->GetObject(mc_hist_name.c_str(), mc_hist);
    if (!data_hist) {
        std::cout << "Failed to get data histogram" << std::endl;
        return 1;
    }
    if (!mc_hist) {
        std::cout << "Failed to get MC histogram" << std::endl;
        return 1;
    }

    /* Pretty plots */
    gROOT->SetStyle("Plain");
    gStyle->SetOptFit();
    gStyle->SetOptStat(11111);
    gStyle->SetCanvasBorderMode(0);

    /* Make a canvas to store the results */
    //TCanvas* canvas = new TCanvas("canvas", "canvas", 1200, 400);
    TCanvas* canvas = new TCanvas("canvas", "canvas", 1000, 1200);

    /* Normalize areas */
    const double mc_area = mc_hist->Integral();
    const double data_area = data_hist->Integral();
    mc_hist->Scale(data_area / mc_area);

    /* Set up the color and markers of the Histograms */
    data_hist->SetMarkerStyle(20);
    data_hist->SetMarkerColor(kBlack);
    mc_hist->SetLineWidth(2);

    data_hist->Draw("E");
    mc_hist->Draw("SAME");
    canvas->Print(output_file.c_str(), "png");
}

int main(int argc, char* argv[]) {
    const int argcLow = 5;
    const int argcHigh = 7;

    if (argc <= argcLow) {
        std::cout<<"Not enough arguments.";
        return 1;
    } else if (argc >= argcHigh) {
        std::cout<<"Too many arguments.";
        return 1;
    } else {
        /* Read in arguments */
        std::string input_data_file(argv[1]);
        std::string data_hist_name(argv[2]);
        std::string input_mc_file(argv[3]);
        std::string mc_hist_name(argv[4]);
        std::string output_file(argv[5]);

        return MakePlots(input_data_file, input_mc_file, data_hist_name, mc_hist_name, output_file);
    }
}

/* Compile time notes:
 *    g++ -O2 -o fit_mc_to_signal.exe fit_mc_to_signal.cpp `root-config --cflags --libs`
 */
