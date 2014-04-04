#ifndef ROOFITTER_H_
#define ROOFITTER_H_

// Standard Library
#include <string>
#include <vector>

// ROOT
#include <TFile.h>

//using namespace RooStats;
const int PHI_SIZE = 5;
double PB[PHI_SIZE] = {0, 0.1, 0.2, 0.5, 1};
const std::vector<double> PHISTAR_BINS(&PB[0], &PB[0]+PHI_SIZE);
const int ETA_SIZE = 3;
double EB[ETA_SIZE] = {0, 2.5, 5};
const std::vector<double> ETA_BINS(&EB[0], &EB[0]+ETA_SIZE);

// Set the binning based on the Z Electron locations
enum Z_TYPES{
    ETET,
    ETNT,
    ETHF
};

RooBinning* get_roobinning(const Z_TYPES& Z_TYPE);

// Set up the TCanvas
TCanvas* get_tcanvas(const int X_DIM = 1280, const int Y_DIM = 640);

// Multiple ways to call the function in ROOT
int RooFitter(
        const std::string& DATA_FILE,
        const std::string& DATA_WS,
        const std::string& MC_FILE,
        const std::string& MC_WS,
        const std::string& OUT_DIR
        );

int RooFitter(
        TFile* const DATA_FILE,
        const std::string& DATA_WS,
        TFile* const MC_FILE,
        const std::string& MC_WS,
        const std::string& OUT_DIR
        );

// Main, used only on the command line
int main(int argc, char* argv[]);
#endif  // ROOFITTER_H_
