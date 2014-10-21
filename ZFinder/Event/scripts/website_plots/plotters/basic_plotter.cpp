// Standard Library
#include <iostream>
#include <sstream>
#include <string>

// ROOT
#include <TCanvas.h>
#include <TFile.h>
#include <TH1D.h>

// Plotters
#include "lib/style.h"  // set_plot_style

int Plotter(
        const std::string& INPUT_FILE,
        const std::string& OUTPUT_FILE,
        const std::string& HISTOGRAM_NAME,
        const int& PLOT_COLOR
        ) {
    // Constants
    const int X_SIZE = 1200;
    const int Y_SIZE = 800;

    set_plot_style();

    // Open the TFILE
    TFile* input_tfile = new TFile(INPUT_FILE.c_str());

    // Loop over all the histograms
    // Open the histogram
    TH1D* histo;
    input_tfile->GetObject(HISTOGRAM_NAME.c_str(), histo);
    if (!histo) {
        std::cout << "Histogram " << HISTOGRAM_NAME << " failed to load!" << std::endl;
        return 1;
    }

    // Skip if there are no events
    if (histo->GetEntries() <= 0.) {
        return 2;
    }

    // set up the canvas
    TCanvas canvas("canvas", "canvas", X_SIZE, Y_SIZE);
    gPad->SetLogy(true);

    // Set the line color and style
    histo->Scale(1, "width");
    histo->SetLineColor(PLOT_COLOR);
    histo->SetLineWidth(2);
    const int FORWARD_HATCH = 3004;
    histo->SetFillStyle(FORWARD_HATCH);
    histo->SetFillColor(PLOT_COLOR);
    histo->Draw();

    // Save the output file
    canvas.Print(OUTPUT_FILE.c_str(), "png");

    return 0;
}

int main(int argc, char* argv[]) {
    const int ARGN = 5;

    if (argc < ARGN) {
        std::cout<<"Not enough arguments.";
        return 1;
    } else if (argc > ARGN) {
        std::cout<<"Too many arguments.";
        return 1;
    } else {
        /* Read in arguments */
        const std::string INPUT_FILE(argv[1]);
        const std::string OUTPUT_DIR(argv[2]);
        const std::string HISTOGRAM_NAME(argv[3]);
        int plot_color = 0;
        std::istringstream ss(argv[4]);
        if (!(ss >> plot_color)) {
            std::cout << "Invalid plot color: " << argv[4] << std::endl;
            return 1;
        }

        return Plotter(INPUT_FILE, OUTPUT_DIR, HISTOGRAM_NAME, plot_color);
    }
}
