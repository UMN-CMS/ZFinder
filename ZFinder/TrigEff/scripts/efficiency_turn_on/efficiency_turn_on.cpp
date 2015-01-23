// Standard Library
#include <iostream>
#include <string>

// ROOT
#include <TCanvas.h>
#include <TFile.h>
#include <TH2D.h>

int main() {
    const std::string INPUT_FILE =
        "/local/cms/user/gude/alex_thesis/trigger_efficiency/20141203_SingleElectron_trigger_eff_2012All/20141203_SingleElectron_trigger_eff_2012All_hadded.root";
    const int X_SIZE = 900;
    const int Y_SIZE = 1200;
    const std::string OUTPUT_FILE = "eff.png";
    TFile* input_tfile = new TFile(INPUT_FILE.c_str());

    const int NUMBER_OF_HISTOGRAMS = 2;
    TH2D* histograms[NUMBER_OF_HISTOGRAMS];
    input_tfile->GetObject("trigeff/numerator", histograms[0]);
    input_tfile->GetObject("trigeff/denominator", histograms[1]);

    for (int i = 0; i < NUMBER_OF_HISTOGRAMS; ++i) {
        if (!histograms[i]) {
            std::cout << "Histogram " << i << " failed to load!" << std::endl;
            return 1;
        }
        else {
            histograms[i]->Sumw2();
        }
    }

    // Open an output root file for saving
    TFile* out_file = new TFile("output.root", "RECREATE");
    out_file->cd();

    // set up the canvas
    TCanvas canvas("canvas", "canvas", X_SIZE, Y_SIZE);
    gPad->SetLogx(true);
    TH2D* final_histo = dynamic_cast<TH2D*>(histograms[0]->Clone());
    final_histo->Divide(histograms[0], histograms[1], 1, 1, "B");
    final_histo->Draw("TEXT");
    // Save the output file
    canvas.Print(OUTPUT_FILE.c_str(), "png");

    final_histo->Draw("TEXT");
    out_file->Write();

    // Print bins and errors
    for (int bin_number = 0; bin_number <= 60; ++bin_number) {
        std::cout << "Bin: " << bin_number << " SF: " << final_histo->GetBinContent(bin_number) << " +- " << final_histo->GetBinError(bin_number) << std::endl;
    }

    out_file->Close();

    return 0;
}
