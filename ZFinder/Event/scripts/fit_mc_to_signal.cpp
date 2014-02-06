#include <string>
#include <iostream>

#include "TH1I.h"
#include "TFile.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TCanvas.h"
#include <TROOT.h>
#include <TStyle.h>

int MakePlots(
         const std::string input_data_file,
         const std::string input_mc_file,
         const std::string data_hist_name,
         const std::string mc_hist_name,
         const std::string output_file,
         const std::string title
        ) {
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
    gStyle->SetOptStat(0);
    gStyle->SetCanvasBorderMode(0);

    gStyle->SetLineWidth(2.);
    gStyle->SetTextSize(1.1);
    gStyle->SetPadLeftMargin(0.12);  // Add space so the Y Axis title doesn't clip
    gStyle->SetPadRightMargin(0.04);
    gStyle->SetPadTopMargin(0.04);
    gStyle->SetPadBottomMargin(0.08);

    /* Make a canvas to store the results */
    //TCanvas* canvas = new TCanvas("canvas", "canvas", 1200, 400);
    TCanvas* canvas = new TCanvas("canvas", "canvas", 830, 1000);

    /* Normalize areas */
    const double mc_area = mc_hist->Integral();
    const double data_area = data_hist->Integral();
    mc_hist->Scale(data_area / mc_area);

    /* Figure out the largest Y value and set the plot accordingly */
    const double DATA_MAX = data_hist->GetMaximum();
    const double MC_MAX = mc_hist->GetMaximum();
    if (MC_MAX > DATA_MAX) {
        data_hist->SetMaximum(MC_MAX * 1.1);
    }

    /* Set up the histogram styles */
    data_hist->SetMarkerStyle(20);
    data_hist->SetMarkerColor(kBlack);
    data_hist->SetTitle(0);  // Remove title
    data_hist->GetYaxis()->SetTitleOffset(1.65);  // Move the title left

    mc_hist->SetLineWidth(2);

    // If phi*, change the xaxis label
    if (mc_hist_name.find("phi") != std::string::npos) {
        data_hist->GetXaxis()->SetTitle("#phi*");
    }
    
    /* Add Lumi */
    // ~5.288 ~= 5.3/pb
    TLatex *lumi_label = new TLatex(.78, .8, "#int L dt = 5.3 fb^{-1} at 8 TeV");
    lumi_label->SetNDC();
    lumi_label->SetTextFont(42);
    lumi_label->SetTextColor(1);
    lumi_label->SetTextSize(0.035);
    lumi_label->SetTextAlign(22);
    lumi_label->SetTextAngle(0);

    /* Add normalization method */
    TLatex *norm_label = new TLatex(.8, .75, "MC normalized to data");
    norm_label->SetNDC();
    norm_label->SetTextFont(42);
    norm_label->SetTextColor(1);
    norm_label->SetTextSize(0.035);
    norm_label->SetTextAlign(22);
    norm_label->SetTextAngle(0);
    
    /* Add title */
    TLatex *plabel = new TLatex(.4, .93, title.c_str());
    plabel->SetNDC();
    plabel->SetTextFont(42);
    plabel->SetTextColor(1);
    plabel->SetTextSize(0.06);
    plabel->SetTextAlign(22);
    plabel->SetTextAngle(0);

    /* Make a legend */
    TLegend* leg = new TLegend(0.7, 0.85, 0.959, 0.959);
    leg->SetFillColor(kWhite);
    leg->AddEntry(data_hist, "2012A/B", "p");
    leg->AddEntry(mc_hist, "Powheg", "l");

    data_hist->Draw("E");
    mc_hist->Draw("SAME");
    leg->Draw();
    plabel->Draw();
    lumi_label->Draw();
    norm_label->Draw();
    canvas->Print(output_file.c_str(), "png");
}

int main(int argc, char* argv[]) {
    const int argcLow = 6;
    const int argcHigh = 8;

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
        std::string title(argv[6]);

        return MakePlots(input_data_file, input_mc_file, data_hist_name, mc_hist_name, output_file, title);
    }
}

/* Compile time notes:
 *    g++ -O2 -o fit_mc_to_signal.exe fit_mc_to_signal.cpp `root-config --cflags --libs`
 */
