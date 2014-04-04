//#ifndef __CINT__ // Used if running in root
//#include "RooGlobalFunc.h"
//#endif

#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooFormulaVar.h"
#include "RooGenericPdf.h"
#include "RooDataSet.h"
#include "RooAbsData.h"
#include "RooArgSet.h"
#include "RooBinning.h"
#include "RooRealVar.h"
#include "RooWorkspace.h"
#include "RooDataHist.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooKeysPdf.h"
#include "RooHistPdf.h"

#include <TH2D.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace RooFit;
//using namespace RooStats;
const int PHI_SIZE = 5;
double PB[PHI_SIZE] = {0, 0.1, 0.2, 0.5, 1};
const std::vector<double> PHISTAR_BINS(&PB[0], &PB[0]+PHI_SIZE);
const int ETA_SIZE = 3;
double EB[ETA_SIZE] = {0, 2.5, 5};
const std::vector<double> ETA_BINS(&EB[0], &EB[0]+ETA_SIZE);

int RooFitter(
        const std::string& DATA_FILE,
        const std::string& DATA_WS,
        const std::string& MC_FILE,
        const std::string& MC_WS,
        const std::string& OUT_DIR
        ) {
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

    z_mass.setBins(200);

    // Open the data file
    TFile* f_data= new TFile(DATA_FILE.c_str(), "READ");
    if (f_data == NULL) {
        std::cout << "Data file is invalid" << std::endl;
        return 1;
    }
    RooWorkspace* w_data = static_cast<RooWorkspace*>(f_data->Get(DATA_WS.c_str()));
    RooDataSet* data_reco = static_cast<RooDataSet*>(w_data->data("roo_dataset"));

    // Open the mc file
    TFile* f_mc= new TFile(MC_FILE.c_str(), "READ");
    if (f_mc == NULL) {
        std::cout << "MC file is invalid" << std::endl;
        return 1;
    }
    RooWorkspace* w_mc = static_cast<RooWorkspace*>(f_mc->Get(MC_WS.c_str()));
    RooDataSet* mc_reco = static_cast<RooDataSet*>(w_mc->data("roo_dataset"));

    // // Plot
    // TCanvas* c=new TCanvas();
    // c->cd();
    // RooPlot* xframe = z_mass.frame(0, 200, 2000);
    // // Binning
    // RooBinning b(0, 200);
    // b.addUniform(8, 0, 80);
    // b.addUniform(10, 80, 100);
    // b.addUniform(10, 100, 200);
    // data_reco->plotOn(xframe, Binning(b));
    RooDataHist h_mc("h_mc", "h_mc", RooArgSet(z_mass), *mc_reco);
    RooHistPdf signalpdf("signalpdf", "signalpdf", z_mass, h_mc);
    //signalpdf.plotOn(xframe, LineColor(kRed));

    //xframe->Draw();

    // Variables for the fit
    //RooRealVar x("x", "x", 40, 150);
    RooRealVar alpha("alpha", "alpha", 60., 0.1, 1000.);
    RooRealVar gamma("gamma", "gamma", 0.01, 0.0001, 0.3);
    RooRealVar delta("delta", "delta", 10., 3., 80.);
    RooFormulaVar var1("var1", "(alpha-z_mass)/delta", RooArgSet(alpha, z_mass, delta));
    RooFormulaVar var2("var2", "-1.0*gamma*z_mass", RooArgSet(gamma, z_mass));
    RooGenericPdf MyBackgroundPdf("MyBackgroundPdf", "ROOT::Math::erfc(var1)*exp(var2)", RooArgSet(var1, var2));

    RooRealVar sigratio("sigratio", "sigratio", 0.1, 0.0, 1.0);
    RooAddPdf fitpdf("fitpdf", "fitpdf", RooArgList(MyBackgroundPdf, signalpdf), RooArgList(sigratio));

    RooDataHist h_data("h_data", "h_data", RooArgSet(z_mass), *data_reco);
    fitpdf.fitTo(h_data);

    TCanvas* c=new TCanvas();
    c->cd();
    RooPlot* fitFrame = z_mass.frame(50, 150); ///, Title(name));
    h_data.plotOn(fitFrame);
    fitpdf.plotOn(fitFrame, Components(MyBackgroundPdf), LineColor(kRed));
    fitpdf.plotOn(fitFrame, Components(signalpdf), LineColor(kBlue));
    fitFrame->Draw();

    /*
       double acceptance[ETA_BINS.size()][PHISTAR_BINS.size()];

       for (unsigned int i_eta = 0; i_eta < ETA_BINS.size(); ++i_eta){
    // Initialize the eta cut
    TString eta_cut = "abs(z_eta) >";
    eta_cut += double(ETA_BINS[i_eta]);
    eta_cut +="&& abs(z_eta) <";
    eta_cut += double(ETA_BINS[i_eta+1]);

    RooDataSet* mc_reco_etabin = static_cast<RooDataSet*>(mc_reco->reduce(argset, eta_cut));
    RooDataSet* data_reco_etabin = static_cast<RooDataSet*>(data_reco->reduce(argset, eta_cut));
    for (unsigned int i_phistar = 0; i_phistar < PHISTAR_BINS.size(); ++i_phistar){
    // Initialize the phistar cut
    TString phistar_cut = "phistar > ";
    phistar_cut += double(PHISTAR_BINS[i_phistar]);
    phistar_cut +="&& phistar < ";
    phistar_cut += double(PHISTAR_BINS[i_phistar+1]);

    RooDataSet* mc_reco_phistarbin = static_cast<RooDataSet*>(mc_reco_etabin->reduce(argset, phistar_cut));
    RooDataSet* data_reco_phistarbin = static_cast<RooDataSet*>(data_reco_etabin->reduce(argset, phistar_cut));

    // Plot
    // TCanvas* c=new TCanvas();
    // c->cd();
    // RooPlot* xframe = z_mass.frame(0, 200);
    // data_reco_phistarbin->plotOn(xframe);
    // xframe->Draw();
    //if (mc_true_all_phistarbin->sumEntries() > 0) {
    //  acceptance[i_eta][i_phistar] = mc_reco_phistarbin->sumEntries() / mc_true_all_phistarbin->sumEntries();
    //} else {
    //  acceptance[i_eta][i_phistar] = 0;
    //}

    //RooDataHist h_data("h_data", "h_data", RooArgSet(z_mass), *data_reco_phistarbin);
    //RooDataHist h_mc("h_mc", "h_mc", RooArgSet(z_mass), *mc_reco_phistarbin);
    //cout<<"going to make keyspdf"<<endl;
    //RooHistPdf signalpdf("signalpdf", "signalpdf", z_mass, h_mc);
    ////   RooKeysPdf signalpdf("signalpdf", "signalpdf", z_mass, *mc_reco_phistarbin);
    //cout<<"Pdf done"<<endl;
    //RooRealVar sigratio("sigratio", "sigratio", 0.5, 0.0, 1.0);
    ////    sigratio.setRange(0.5, 0.5);
    //RooAddPdf fitpdf("fitpdf", "fitpdf", RooArgList(MyBackgroundPdf, signalpdf), RooArgList(sigratio));

    //fitpdf.fitTo(h_data);

    //TString name="";
    //name+=double(ETA_BINS[i_eta]);
    //name+="<eta<";
    //name+=double(ETA_BINS[i_eta+1]);
    //name+=", ";
    //name+=double(PHISTAR_BINS[i_phistar]);
    //name+="<phistar<";
    //name+=double(PHISTAR_BINS[i_phistar+1]);

    //TCanvas* c=new TCanvas();
    //c->cd();
    //RooPlot* fitFrame = z_mass.frame(0, 200); ///, Title(name));
    //h_data.plotOn(fitFrame);
    //fitpdf.plotOn(fitFrame, Components(MyBackgroundPdf), LineColor(kRed));
    //fitpdf.plotOn(fitFrame, Components(signalpdf), LineColor(kBlue));
    //fitFrame->Draw();
    //break;
    }
    //break;
    } 
    */
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
