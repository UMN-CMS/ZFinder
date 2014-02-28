#include <string>
#include <iostream>
#include <vector>
#include <utility>  // std::pair

#include "TH1.h"
#include "TH1D.h"
#include "TFile.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TCanvas.h"
#include <TROOT.h>
#include <TStyle.h>

int MakePlots(
        const std::string input_file,
        const std::string output_dir
        ) {

    /* Bring a selected set of std:: objects into our name space */
    using std::pair;
    using std::string;
    using std::vector;

    /* Colors */
    static const int COLOR[7] = {kRed, kRed, kBlue, kGreen+2, kCyan+1, kYellow+1, kMagenta+1};

    /* Pretty plots */
    gROOT->SetStyle("Plain");
    gStyle->SetOptFit();
    gStyle->SetOptStat(0);
    gStyle->SetCanvasBorderMode(0);

    gStyle->SetLineWidth(2);
    gStyle->SetTextSize(1.1);
    gStyle->SetPadLeftMargin(0.12);  // Add space so the Y Axis title doesn't clip
    gStyle->SetPadRightMargin(0.04);
    gStyle->SetPadTopMargin(0.04);
    gStyle->SetPadBottomMargin(0.08);

    /* Open the root file */
    TFile* in_tfile = new TFile(input_file.c_str(), "READ");
    if (!in_tfile) {
        std::cout << "Failed to open the input file" << std::endl;
        return 1;
    }

    /* Loop over each region, and then each plot in the region */
    const string BASE_DIR_NAME = "/ZFinder/";
    const string ENDING_NAME = " Reco/6 60 < M_{ee} < 120/";
    const string ALL_DIR = BASE_DIR_NAME + "All Electrons Reco/1 60 < M_{ee} < 120/";
    const string S20202124 = BASE_DIR_NAME + "S2020 2124" + ENDING_NAME;
    const string S20202424 = BASE_DIR_NAME + "S2020 2424" + ENDING_NAME;
    const string D30202124 = BASE_DIR_NAME + "D3020 2124" + ENDING_NAME;
    const string D30202424 = BASE_DIR_NAME + "D3020 2424" + ENDING_NAME;
    const string D30102124 = BASE_DIR_NAME + "D3010 2124" + ENDING_NAME;
    const string D30102424 = BASE_DIR_NAME + "D3010 2424" + ENDING_NAME;

    vector<pair<string, string> > histo_to_name;
    histo_to_name.push_back(std::make_pair("Z0 Rapidity", "Z_{Y}"));
    histo_to_name.push_back(std::make_pair("#phi*", "#phi*"));

    for (vector<pair<string, string> >::const_iterator i_h2n = histo_to_name.begin(); i_h2n != histo_to_name.end(); ++i_h2n) {
        /* Extract the histograms */
        const int HIST_LEN = 7;
        string hist_names[HIST_LEN];
        hist_names[0] = ALL_DIR + (*i_h2n).first;
        hist_names[1] = S20202124 + (*i_h2n).first;
        hist_names[2] = S20202424 + (*i_h2n).first;
        hist_names[3] = D30202124 + (*i_h2n).first;
        hist_names[4] = D30202424 + (*i_h2n).first;
        hist_names[5] = D30102124 + (*i_h2n).first;
        hist_names[6] = D30102424 + (*i_h2n).first;
        TH1D* in_hist[HIST_LEN];

        /* Try to load the histograms from the file */
        for (int i = 0; i <= HIST_LEN-1; ++i) {
            in_tfile->GetObject(hist_names[i].c_str(), in_hist[i]);
            if (!in_hist[i]){
                std::cout << "Failed to get Histogram number " << i << " named: '" << hist_names[i] << "'" << std::endl;
                return 1;
            }
        }

        /* Make a canvas to store the results */
        //TCanvas* canvas = new TCanvas("canvas", "canvas", 1200, 400);
        TCanvas* canvas = new TCanvas("canvas", "canvas", 830, 1000);

        /* Set up the histogram styles */
        for (int i = 0; i <= HIST_LEN-1; ++i) {
            in_hist[i]->SetMarkerStyle(19+i);
            in_hist[i]->SetMarkerColor(COLOR[i]);
            in_hist[i]->SetTitle(0);  // Remove title
            in_hist[i]->GetYaxis()->SetTitleOffset(1.3);  // Move the title left
            in_hist[i]->GetYaxis()->SetTitle("Ratio");  // Change title
            in_hist[i]->SetLineColor(COLOR[i]);
            in_hist[i]->Sumw2();
            in_hist[i]->SetMaximum(1.);
            in_hist[i]->SetMinimum(0.);
            if ((*i_h2n).second == "#phi*") {
                in_hist[i]->Rebin(4);
            } else {
                in_hist[i]->GetXaxis()->SetRangeUser(-4, 4);
            }
        }

        /* Add title */
        const string TITLE = (*i_h2n).second;
        TLatex *plabel = new TLatex(0.18, 0.93, TITLE.c_str());
        plabel->SetNDC();
        plabel->SetTextFont(42);
        plabel->SetTextColor(1);
        plabel->SetTextSize(0.06);
        plabel->SetTextAlign(22);
        plabel->SetTextAngle(0);

        /* Make a legend */
        TLegend* leg = new TLegend(0.4, 0.87, 0.959, 0.959);
        leg->SetFillColor(kWhite);
        leg->AddEntry(in_hist[1], "p_{T}>20,20; |Eta|< 2.1,2.4", "p");
        leg->AddEntry(in_hist[2], "p_{T}>20,20; |Eta|< 2.4,2.4", "p");
        leg->AddEntry(in_hist[3], "p_{T}>30,20; |Eta|< 2.1,2.4, Tight", "p");
        leg->AddEntry(in_hist[4], "p_{T}>30,20; |Eta|< 2.4,2.4, Tight", "p");
        leg->AddEntry(in_hist[5], "p_{T}>30,10; |Eta|< 2.1,2.4, Tight", "p");
        leg->AddEntry(in_hist[6], "p_{T}>30,10; |Eta|< 2.4,2.4, Tight", "p");

        /* Add information about the ratio */
        TLatex *norm_label = new TLatex(.69, 0.83, "#splitline{Acceptance requirements compared to}{all Z->ee events with 60 < m_{ee} < 120}");
        norm_label->SetNDC();
        norm_label->SetTextFont(42);
        norm_label->SetTextColor(1);
        norm_label->SetTextSize(0.035);
        norm_label->SetTextAlign(22);
        norm_label->SetTextAngle(0);

        /* Save the plot as a png  */
        for (int i = 1; i <= HIST_LEN-1; ++i) {
            in_hist[i]->Divide(in_hist[0]);
            if (i >= 2) {
                in_hist[i]->Draw("SAME");
            } else {
                in_hist[i]->Draw();
            }
        }
        leg->Draw();
        plabel->Draw();
        norm_label->Draw();
        const string OUT_PNG = output_dir + "/" + (*i_h2n).second + ".png";
        canvas->Print(OUT_PNG.c_str(), "png");
    }
}

int main(int argc, char* argv[]) {
    const int argcLow = 2;
    const int argcHigh = 4;

    if (argc <= argcLow) {
        std::cout<<"Not enough arguments.";
        return 1;
    } else if (argc >= argcHigh) {
        std::cout<<"Too many arguments.";
        return 1;
    } else {
        /* Read in arguments */
        std::string input_data_file(argv[1]);
        std::string output_dir(argv[2]);

        return MakePlots(input_data_file, output_dir);
    }
}
