// Standard Library
#include <iostream>
#include <string>

// ROOT
#include <TFile.h>
#include <TH1D.h>


int main() {
    // Input Files
    const std::string MC_FILE =
        "/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20141126_regressed_and_smeared_MC/madgraph_hadded.root";
    const std::string DATA_FILE =
        "/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20141010_SingleElectron_2012ALL/20141010_SingleElectron_2012ALL_hadded.root";

    // Histograms
    // The Histogram containing the full event count with no acceptance or
    // efficiency limits.
    const std::string MC_HISTO_ALL =
        "ZFinder/0 Gen Mass Only MC/0 All Events/Z0 Mass: All";
    // The Histogram containing MC events, but with the full analysis cuts
    // applied. This is used in a ratio with MC_HISTO_ALL to correct for
    // efficiency and acceptance.
    const std::string MC_HISTO_ACC =
        "ZFinder/3 Single Trigger Cuts Reco/7 60 < M_{ee} < 120/Z0 Mass: All";
    // The data after all cuts.
    const std::string DATA_HISTO =
        "ZFinder/Combined Single Reco/6 60 < M_{ee} < 120/Z0 Mass: All";

    // From src/Metadata/lumi_json/total_luminosity.md
    // These should be in 1/fb.
    const double A_LUMI = 0.889362;  // 1/fb
    const double B_LUMI = 4.429;
    const double C_LUMI = 7.152;
    const double D_LUMI = 7.318;
    const double TOTAL_LUMI = A_LUMI + B_LUMI + C_LUMI + D_LUMI;

    // The Luminosity figure to use; set it to the correct one for the data
    // you're using.
    const double LUMI = TOTAL_LUMI;

    // Mass range
    const double MASS_LOW = 60.;
    const double MASS_HIGH = 120.;

    // Open the TFiles
    TFile* mc_tfile = new TFile(MC_FILE.c_str());
    if (!mc_tfile) {
        std::cout << "Failed to read MC_FILE" << std::endl;
        return EXIT_FAILURE;
    }
    TFile* data_tfile = new TFile(DATA_FILE.c_str());
    if (!data_tfile) {
        std::cout << "Failed to read DATA_FILE" << std::endl;
        return EXIT_FAILURE;
    }

    // Load the histograms
    TH1D* mc_histo_all;
    mc_tfile->GetObject(MC_HISTO_ALL.c_str(), mc_histo_all);
    if (!mc_histo_all) {
        std::cout << "Failed to load MC_HISTO_ALL" << std::endl;
        return EXIT_FAILURE;
    }
    TH1D* mc_histo_acc;
    mc_tfile->GetObject(MC_HISTO_ACC.c_str(), mc_histo_acc);
    if (!mc_histo_acc) {
        std::cout << "Failed to load MC_HISTO_ACC" << std::endl;
        return EXIT_FAILURE;
    }
    TH1D* data_histo;
    data_tfile->GetObject(DATA_HISTO.c_str(), data_histo);
    if (!data_histo) {
        std::cout << "Failed to load DATA_HISTO" << std::endl;
        return EXIT_FAILURE;
    }

    // Integrate the histograms to get event counts
    const int DATA_LOW = data_histo->FindBin(MASS_LOW);
    const int DATA_HIGH = data_histo->FindBin(MASS_HIGH);
    const double DATA_COUNT = data_histo->Integral(DATA_LOW, DATA_HIGH);

    const int MC_ALL_LOW = mc_histo_all->FindBin(MASS_LOW);
    const int MC_ALL_HIGH = mc_histo_all->FindBin(MASS_HIGH);
    const double MC_COUNT_ALL = mc_histo_all->Integral(MC_ALL_LOW, MC_ALL_HIGH);

    const int MC_ACC_LOW = mc_histo_acc->FindBin(MASS_LOW);
    const int MC_ACC_HIGH = mc_histo_acc->FindBin(MASS_HIGH);
    const double MC_COUNT_ACC = mc_histo_acc->Integral(MC_ACC_LOW, MC_ACC_HIGH);

    // Calculate the final number
    const double CROSS_SECTION = (MC_COUNT_ALL / MC_COUNT_ACC)
                                 * (DATA_COUNT / LUMI);

    // Report, and exit
    std::cout << "(" << MC_COUNT_ALL << " / " << MC_COUNT_ACC << ") * (";
    std::cout << DATA_COUNT << " / " << LUMI << " fb^(-1) ) = ";
    std::cout << CROSS_SECTION / 1e6 << " nanobarns" << std::endl;

    return EXIT_SUCCESS;
}
