#include <string>
#include <iostream>
#include <vector>
#include <utility>  // std::pair

#include "TH1.h"
#include "TH1F.h"
#include "TH1I.h"
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
    static const int COLOR[3] = {kRed, kRed, kBlue};

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

    /* Set up the three regions we care about */
    static const int TMPSIZE = 3;
    static const string TMP[TMPSIZE] = {"ET-ET", "ET-NT", "ET-HF"};
    const vector<string> e_pairs(TMP, TMP+TMPSIZE);

    /* Loop over each region, and then each plot in the region */
    for (vector<string>::const_iterator i_pair = e_pairs.begin(); i_pair != e_pairs.end(); ++i_pair) {
        const string BASE_DIR_NAME = "ZFinder/" + (*i_pair) + " ";
        const string LOSE_DIR = BASE_DIR_NAME + "158 Cut Test Reco/4 60 < M_{ee} < 120/";
        const string MEDIUM_DIR = BASE_DIR_NAME + "2020 Cut Test Reco/4 60 < M_{ee} < 120/";
        const string TIGHT_DIR = BASE_DIR_NAME + "3020 Cut Test Reco/4 60 < M_{ee} < 120/";

        vector<pair<string, string> > histo_to_name;
        histo_to_name.push_back(std::make_pair("Z0 Rapidity", "Z_{Y}"));
        histo_to_name.push_back(std::make_pair("#phi*", "#phi*"));

        for (vector<pair<string, string> >::const_iterator i_h2n = histo_to_name.begin(); i_h2n != histo_to_name.end(); ++i_h2n) {
            /* Extract the histograms */
            string hist_names[3];
            hist_names[0] = LOSE_DIR + (*i_h2n).first;
            hist_names[1] = MEDIUM_DIR + (*i_h2n).first;
            hist_names[2] = TIGHT_DIR + (*i_h2n).first;
            TH1I* in_hist_tmp[3];

            /* Try to load the histograms from the file */
            for (int i = 0; i <= 2; ++i) {
                in_tfile->GetObject(hist_names[i].c_str(), in_hist_tmp[i]);
                if (!in_hist_tmp[i]){
                    std::cout << "Failed to get Histogram " << i << std::endl;
                    return 1;
                }
            }

            // The Histograms are TH1Is, so we need to deep copy them as TH1Fs for the divide to work...
            TH1F* in_hist[3];
            in_hist[0] = new TH1F("0", "0", in_hist_tmp[0]->GetNbinsX(), in_hist_tmp[0]->GetBinLowEdge(1), in_hist_tmp[0]->GetBinLowEdge(in_hist_tmp[0]->GetNbinsX()) + in_hist_tmp[0]->GetBinWidth(in_hist_tmp[0]->GetNbinsX()));
            in_hist[1] = new TH1F("1", "1", in_hist_tmp[1]->GetNbinsX(), in_hist_tmp[1]->GetBinLowEdge(1), in_hist_tmp[1]->GetBinLowEdge(in_hist_tmp[1]->GetNbinsX()) + in_hist_tmp[1]->GetBinWidth(in_hist_tmp[1]->GetNbinsX()));
            in_hist[2] = new TH1F("2", "2", in_hist_tmp[2]->GetNbinsX(), in_hist_tmp[2]->GetBinLowEdge(1), in_hist_tmp[2]->GetBinLowEdge(in_hist_tmp[2]->GetNbinsX()) + in_hist_tmp[2]->GetBinWidth(in_hist_tmp[2]->GetNbinsX()));

            for (int i = 0; i <= 2; ++i) {
                in_hist[i]->GetXaxis()->SetTitle((*i_h2n).second.c_str());
                in_hist[i]->GetYaxis()->SetTitle(in_hist_tmp[i]->GetYaxis()->GetTitle());
                for (int nbin = 0; nbin < in_hist_tmp[i]->GetNbinsX(); ++nbin) {
                    in_hist[i]->SetBinContent(nbin, in_hist_tmp[i]->GetBinContent(nbin));
                }
            }

            /* Make a canvas to store the results */
            //TCanvas* canvas = new TCanvas("canvas", "canvas", 1200, 400);
            TCanvas* canvas = new TCanvas("canvas", "canvas", 830, 1000);

            /* Set up the histogram styles */
            for (int i = 0; i <= 2; ++i) {
                in_hist[i]->SetMarkerStyle(19+i);
                in_hist[i]->SetMarkerColor(COLOR[i]);
                in_hist[i]->SetTitle(0);  // Remove title
                in_hist[i]->GetYaxis()->SetTitleOffset(1.3);  // Move the title left
                //in_hist[i]->SetLineWidth(2);
                in_hist[i]->SetLineColor(COLOR[i]);
                in_hist[i]->Sumw2();
                in_hist[i]->SetMaximum(1.5);
            }

            /* Add title */
            const string TITLE = (*i_pair) + " " + (*i_h2n).second;
            TLatex *plabel = new TLatex(.4, .93, TITLE.c_str());
            plabel->SetNDC();
            plabel->SetTextFont(42);
            plabel->SetTextColor(1);
            plabel->SetTextSize(0.06);
            plabel->SetTextAlign(22);
            plabel->SetTextAngle(0);

            /* Add normalization method */
            TLatex *norm_label = new TLatex(.8, .80, "#splitline{Points normalized to}{gpt > 15,8 acceptance}");
            norm_label->SetNDC();
            norm_label->SetTextFont(42);
            norm_label->SetTextColor(1);
            norm_label->SetTextSize(0.035);
            norm_label->SetTextAlign(22);
            norm_label->SetTextAngle(0);

            /* Make a legend */
            TLegend* leg = new TLegend(0.7, 0.85, 0.959, 0.959);
            leg->SetFillColor(kWhite);
            //leg->AddEntry(in_hist[0], "gpt>15, gpt>8", "l");
            leg->AddEntry(in_hist[1], "gpt>20,20 / gpt>15,8", "p");
            leg->AddEntry(in_hist[2], "gpt>30,20 / gpt>15,8", "p");

            /* Save the plot as a png  */
            in_hist[1]->Divide(in_hist[0]);
            in_hist[2]->Divide(in_hist[0]);
            //in_hist[0]->Draw();
            in_hist[1]->Draw();
            in_hist[2]->Draw("SAME");
            leg->Draw();
            plabel->Draw();
            //norm_label->Draw();
            const string OUT_PNG = output_dir + "/" + (*i_pair) + "_" + (*i_h2n).second + ".png";
            canvas->Print(OUT_PNG.c_str(), "png");
        }
    }

    ///* Figure out the largest Y value and set the plot accordingly */
    //const double DATA_MAX = data_hist->GetMaximum();
    //const double MC_MAX = mc_hist->GetMaximum();
    //if (MC_MAX > DATA_MAX) {
    //    data_hist->SetMaximum(MC_MAX * 1.1);
    //}

    // /* Add Lumi */
    // // ~5.288 ~= 5.3/pb
    // TLatex *lumi_label = new TLatex(.78, .8, "#int L dt = 5.3 fb^{-1} at 8 TeV");
    // lumi_label->SetNDC();
    // lumi_label->SetTextFont(42);
    // lumi_label->SetTextColor(1);
    // lumi_label->SetTextSize(0.035);
    // lumi_label->SetTextAlign(22);
    // lumi_label->SetTextAngle(0);



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
