// Standard Library
#include <fstream>
#include <iostream>
#include <sstream>

// ROOT
#include <TCanvas.h>

// RooFit
#include "RooAddPdf.h"
#include "RooArgSet.h"
#include "RooBinning.h"
#include "RooCategory.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooFFTConvPdf.h"
#include "RooFitter.h"
#include "RooFormulaVar.h"
#include "RooGaussian.h"
#include "RooKeysPdf.h"
#include "RooGenericPdf.h"
#include "RooHistPdf.h"
#include "RooPlot.h"
#include "RooRealVar.h"
#include "RooWorkspace.h"

using namespace RooFit;


TCanvas* get_tcanvas(const int X_DIM, const int Y_DIM) {
    TCanvas* tcan = new TCanvas("canvas", "canvas", X_DIM, Y_DIM);
    tcan->Divide(2);  // Split in two side-by-side areas
    return tcan;
}

int RooFitter(
        const std::string& DATA_FILE,
        const std::string& DATA_WS,
        const std::string& MC_FILE,
        const std::string& MC_WS,
        const std::string& OUT_DIR
        ) {
    // Open the data file
    TFile* f_data = new TFile(DATA_FILE.c_str(), "READ");
    if (f_data == NULL) {
        std::cout << "Data file is invalid" << std::endl;
        return 1;
    }
    // Open the MC file
    TFile* f_mc = new TFile(MC_FILE.c_str(), "READ");
    if (f_mc == NULL) {
        std::cout << "MC file is invalid" << std::endl;
        return 1;
    }

    // Pass the open files to the main RooFitter
    const int RET_CODE = RooFitter(f_data, DATA_WS, f_mc, MC_WS, OUT_DIR);

    // Clean up and return the exit code
    delete f_data;
    delete f_mc;

    return RET_CODE;
}

int RooFitter(
        TFile* const DATA_FILE,
        const std::string& DATA_WS,
        TFile* const MC_FILE,
        const std::string& MC_WS,
        const std::string& OUT_DIR
        ) {
    // Constants
    const int N_CPU = 12;

    // Set up the variables we're going to read in from the files
    RooRealVar z_mass("z_mass", "m_{ee}", -1, 1000, "GeV");
    RooRealVar z_eta("z_eta", "Z_{#eta}", -6, 6);
    RooRealVar z_y("z_y", "Z_{Y}", -6, 6);
    RooRealVar z_pt("z_pt", "Z_{p_{T}}", 0, 1000, "GeV");
    RooRealVar phistar("phistar", "#phi*", -0.1, 2);
    // Electrons
    RooRealVar e0_pt("e0_pt", "p_{T}^{e_{0}}", 0, 1000, "GeV");
    RooRealVar e0_eta("e0_eta", "#eta_{e_{0}}", -6, 6);
    RooCategory e0_charge("e0_charge", "q_{e_{0}}");
    e0_charge.defineType("Unmeasured", 0);
    e0_charge.defineType("Positive", 1);
    e0_charge.defineType("Negative", -1);
    RooRealVar e1_pt("e1_pt", "p_{T}^{e_{1}}", 0, 1000, "GeV");
    RooRealVar e1_eta("e1_eta", "#eta_{e_{1}}", -6, 6);
    RooCategory e1_charge("e1_charge", "q_{e_{1}}");
    e1_charge.defineType("Unmeasured", 0);
    e1_charge.defineType("Positive", 1);
    e1_charge.defineType("Negative", -1);
    // Event
    RooRealVar n_vert("n_vert", "Number of Vertices", 0, 100);
    RooRealVar weight("weight", "Event weight", 0, 1);
    RooCategory data_type("data_type", "Is the event from Data, or MC?");
    data_type.defineType("Truth MC");
    data_type.defineType("Reco MC");
    data_type.defineType("Data");
    RooCategory numerator("numerator", "Is the event in the efficiency numerator?");
    numerator.defineType("True", 1);
    numerator.defineType("False", 0);
    RooCategory degenerate("degenerate", "Could either event be the tag, and the other the probe?");
    degenerate.defineType("True", 1);
    degenerate.defineType("False", 0);

    RooArgSet argset(z_mass, phistar, z_pt, z_eta, z_y);
    argset.add(e0_pt);
    argset.add(e0_eta);
    argset.add(e0_charge);
    argset.add(e1_pt);
    argset.add(e1_eta);
    argset.add(e1_charge);
    argset.add(weight);
    argset.add(n_vert);
    argset.add(data_type);
    argset.add(numerator);
    argset.add(degenerate);

    // Set binning
    RooBinning* binning = new RooBinning(0, 1000);
    binning->addUniform(500, 0, 1000);
    z_mass.setBinning(*binning);

    // Add named ranges
    z_mass.setRange("signal", 80, 100);
    z_mass.setRange("analysis", 60, 120);
    z_mass.setRange("plot range", 50, 150);

    // Load the workspaces from the already open TFiles
    RooWorkspace* w_data = static_cast<RooWorkspace*>(DATA_FILE->Get(DATA_WS.c_str()));
    if (w_data == NULL) {
        std::cout << "Data Workspace is invalid" << std::endl;
        return 1;
    }
    RooDataSet* data_reco = static_cast<RooDataSet*>(w_data->data("roo_dataset"));
    RooDataHist precut_data_hist("precut_data_hist", "Data before applying cuts", z_mass, *data_reco);

    RooWorkspace* w_mc = static_cast<RooWorkspace*>(MC_FILE->Get(MC_WS.c_str()));
    if (w_mc == NULL) {
        std::cout << "MC Workspace is invalid" << std::endl;
        return 1;
    }
    RooDataSet* mc_reco = static_cast<RooDataSet*>(w_mc->data("roo_dataset"));

    // Select events from before and after the last cut
    RooDataSet* precut_mc_reco = mc_reco;
    RooDataSet* postcut_mc_reco = static_cast<RooDataSet*>(mc_reco->reduce("numerator==1"));
    RooDataHist precut_mc_histo("precut_mc_histo", "precut_mc_histo", RooArgSet(z_mass), *precut_mc_reco);
    RooDataHist postcut_mc_histo("postcut_mc_histo", "postcut_mc_histo", RooArgSet(z_mass), *postcut_mc_reco);
    RooHistPdf precut_signalpdf("precut_signalpdf", "Signal PDF from MC before apply the last cut", z_mass, precut_mc_histo);
    RooHistPdf postcut_signalpdf("postcut_signalpdf", "Signal PDF from MC after applying all cuts", z_mass, postcut_mc_histo);

    RooDataSet* postcut_data_reco = static_cast<RooDataSet*>(data_reco->reduce("numerator==1"));
    RooDataHist postcut_data_hist("postcut_data_hist", "Data after applying cuts", z_mass, *postcut_data_reco);

    // Set up a Gaussian to smear the Z Peak
    RooRealVar gaussmean("gaussmean", "Mean of the smearing Gaussian", 2., 0., 3.);
    RooRealVar gausssigma("gausssigma", "Width of the smearing Gaussian", 1., 0.1, 3.);
    RooGaussian smear_gauss("smear_gauss", "Gaussian used to smear the Z Peak", z_mass, gaussmean, gausssigma);

    RooFFTConvPdf smeared_precut_signalpdf("smeared_precut_signalpdf", "Smeared Signal Before Cuts", z_mass, precut_signalpdf, smear_gauss);
    RooFFTConvPdf smeared_postcut_signalpdf("smeared_postcut_signalpdf", "Smeared Signal Before Cuts", z_mass, postcut_signalpdf, smear_gauss);

    // Set up the background
    RooRealVar alpha("alpha", "alpha", 60., 0.1, 100.);
    RooRealVar gamma("gamma", "gamma", 0.02, 0.01, 0.1);
    RooRealVar delta("delta", "delta", 20., 10., 30.);
    RooFormulaVar var1("var1", "(alpha-z_mass)/delta", RooArgSet(alpha, z_mass, delta));
    RooFormulaVar var2("var2", "-1.0*gamma*z_mass", RooArgSet(gamma, z_mass));
    RooGenericPdf bg_pdf("MyBackgroundPdf", "ROOT::Math::erfc(var1)*exp(var2)", RooArgSet(var1, var2));
    RooRealVar sigratio("sigratio", "sigratio", 0.9, 0.0, 1.0);

    RooAddPdf precut_fitpdf("precut_fitpdf", "precut_fitpdf", RooArgList(bg_pdf, smeared_precut_signalpdf), RooArgList(sigratio));
    RooAddPdf postcut_fitpdf("postcut_fitpdf", "postcut_fitpdf", RooArgList(bg_pdf, smeared_postcut_signalpdf), RooArgList(sigratio));

    TCanvas* const canvas = get_tcanvas(1500, 750);

    // Plot the left side
    precut_fitpdf.fitTo(precut_data_hist, Range("plot range"), NumCPU(N_CPU), Verbose(false));
    canvas->cd(1);
    gPad->SetLogy();
    RooPlot* precut_fitframe = z_mass.frame(40, 160);
    precut_fitframe->SetName(0);  // Unset title
    precut_data_hist.plotOn(precut_fitframe);
    precut_fitpdf.plotOn(precut_fitframe, Components(bg_pdf), LineColor(kRed), NumCPU(N_CPU));
    precut_fitpdf.plotOn(precut_fitframe, Components(smeared_precut_signalpdf), LineColor(kGreen), NumCPU(N_CPU));
    precut_fitpdf.plotOn(precut_fitframe, LineColor(kBlue), NumCPU(N_CPU));
    precut_data_hist.plotOn(precut_fitframe);

    precut_fitframe->Draw();

    // Plot the right side
    postcut_fitpdf.fitTo(postcut_data_hist, Range("plot range"), NumCPU(N_CPU), Verbose(false));
    canvas->cd(2);
    gPad->SetLogy();
    RooPlot* postcut_fitframe = z_mass.frame(40, 160);
    postcut_fitframe->SetName(0);  // Unset title
    postcut_data_hist.plotOn(postcut_fitframe);
    postcut_fitpdf.plotOn(postcut_fitframe, Components(bg_pdf), LineColor(kRed), NumCPU(N_CPU));
    postcut_fitpdf.plotOn(postcut_fitframe, Components(smeared_postcut_signalpdf), LineColor(kGreen), NumCPU(N_CPU));
    postcut_fitpdf.plotOn(postcut_fitframe, LineColor(kBlue), NumCPU(N_CPU));
    postcut_data_hist.plotOn(postcut_fitframe);

    postcut_fitframe->Draw();

    canvas->Print("Test.png", "png");

    return 0;
}

int main(int argc, char* argv[]) {
    const int ARGC = 6;
    if (argc < ARGC) {
        std::cout << "Not enough arguments.";
        return 1;
    } else if (argc > ARGC) {
        std::cout << "Too many arguments.";
        return 1;
    } else {
        /* Read in arguments */
        const std::string DATA_FILE(argv[1]);
        const std::string DATA_WS(argv[2]);
        const std::string MC_FILE(argv[3]);
        const std::string MC_WS(argv[4]);
        const std::string OUT_DIR(argv[5]);

        return RooFitter(
                DATA_FILE,
                DATA_WS,
                MC_FILE,
                MC_WS,
                OUT_DIR
                );
    }
}
