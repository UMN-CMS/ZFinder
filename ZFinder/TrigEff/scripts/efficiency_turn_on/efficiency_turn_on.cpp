// Standard Library
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// ROOT
#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include <TH1D.h>
#include <TH2D.h>


int main() {
    const std::string INPUT_FILE =
        "/data/whybee0a/user/gude_2/trigeff/20150123_madgraph_hadded.root";
    TFile* input_tfile = new TFile(INPUT_FILE.c_str());

    const int NUMBER_OF_HISTOGRAMS = 2;
    TH2D* histograms[NUMBER_OF_HISTOGRAMS];
    input_tfile->GetObject("trigeff/numerator", histograms[0]);
    input_tfile->GetObject("trigeff/denominator", histograms[1]);

    // Check the histograms
    for (int i = 0; i < NUMBER_OF_HISTOGRAMS; ++i) {
        if (!histograms[i]) {
            std::cout << "Histogram " << i << " failed to load!" << std::endl;
            return 1;
        }
        else {
            histograms[i]->Sumw2();
        }
    }

    // Make TGraphs of every projection (We project so that we have a single
    // Eta bin, but multiple PT bins)
    std::vector<std::pair<TGraphAsymmErrors*, std::string>> ratios;
    for (int i = 0; i < histograms[0]->GetNbinsY(); ++i) {
        // Make the TGraphAsymmErrors from projections of the TH2D
        TH1D* num = histograms[0]->ProjectionX(Form("num_bin%d", i + 1), i + 1, i + 2);
        TH1D* den = histograms[1]->ProjectionX(Form("den_bin%d", i + 1), i + 1, i + 2);
        // cl=0.683 b(1,1) mode is the option needed for Bayes divide
        TGraphAsymmErrors* tg = new TGraphAsymmErrors(num, den, "cl=0.683 b(1,1) mode");

        // Get the eta bin
        const double low_edge = histograms[0]->GetYaxis()->GetBinLowEdge(i + 1);
        const double high_edge = low_edge + histograms[0]->GetYaxis()->GetBinWidth(i + 1);
        std::stringstream s;
        s << low_edge << ", " << high_edge;
        const std::string eta = s.str();

        std::pair<TGraphAsymmErrors*, std::string> p = make_pair(tg, eta);
        ratios.push_back(p);
    }

    // Open an output root file for saving
    TFile* out_file = new TFile("output.root", "RECREATE");
    out_file->cd();

    for (auto& i_pair : ratios) {
        TGraphAsymmErrors* tgraph = i_pair.first;
        const std::string eta = i_pair.second;
        std::cout << "Eta Bin Edges: " << eta << std::endl;
        double x = 0;
        double y = 0;
        for (int i = 0; i < tgraph->GetN(); ++i) {
            tgraph->GetPoint(i, x, y);
            double low_edge = x - tgraph->GetErrorXlow(i);
            double high_edge = x + tgraph->GetErrorXhigh(i);
            std::cout << "Pt Bin Edges: " << low_edge << ", " << high_edge << std::endl;
            std::cout << y << " + " << tgraph->GetErrorYhigh(i);
            std::cout << " - " << tgraph->GetErrorYlow(i) << std::endl;
        }
    }

    return 0;
}
