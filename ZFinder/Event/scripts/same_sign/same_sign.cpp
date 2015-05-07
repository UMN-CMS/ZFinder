// Interface
#include "same_sign.h"  // branch_struct

// Standard Library
#include <stdexcept>
#include <iostream>
#include <sstream>

// ROOT
#include <TFile.h>
#include <TBranch.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TLeaf.h>
#include <TDirectory.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>

// RooFit
#include "RooAddPdf.h"
#include "RooArgSet.h"
#include "RooDataHist.h"
#include "RooFitResult.h"
#include "RooFormulaVar.h"
#include "RooGenericPdf.h"
#include "RooHistPdf.h"
#include "RooPlot.h"
#include "RooRealVar.h"

// Style
#include "PlotStyle.h"

// ZFinder
#include "../../interface/WeightID.h"
#include "../../interface/ATLASBins.h"  // ATLAS_PHISTAR_BINNING

TTree* GetTTree(const std::string TFILE, const std::string TTREE) {
    // Open the TFiles
    TFile* mc_tfile = new TFile(TFILE.c_str(), "READ");
    if (!mc_tfile || mc_tfile->IsZombie()) {
        const std::string ERR = "Could not open the file " + TFILE;
        throw std::runtime_error(ERR);
    }

    // Load the tree
    TTree* tree = new TTree();
    mc_tfile->GetObject(TTREE.c_str(), tree);
    if (!tree || tree->IsZombie()) {
        const std::string ERR = "Could not open the TTree " + TTREE;
        throw std::runtime_error(ERR);
    }

    return tree;
}

double GetOverallNormalization(const std::string NAME) {
    const double DATA_LUMI = 19712;
    const std::map<std::string, double> NORM = {
        {"Signal", 3531.89 / 30459500.},
        {"BG_Ditau", 1966.7 / 47271600.},
        {"BG_TTBar", 23.64 / 4246440},
        {"BG_single_t", 11.1 / 497658},
        {"BG_single_tbar", 11.1 / 493460.},
        {"BG_ww", 54.84 / 10000430.},
        {"BG_wz", 33.21 / 10000280.},
        {"BG_zz", 17.0 / 9799908.},
    };
    const double WEIGHT = DATA_LUMI * NORM.at(NAME);
    return WEIGHT;
}

double GetWeight(
        const int& WEIGHT_SIZE,
        const double WEIGHTS[],
        const int WEIGHTIDS[]
        ) {
    double weight = 1.;

    // Loop over the weights and use it if it is one of correct
    for (int i = 0; i < WEIGHT_SIZE; ++i) {
        switch (WEIGHTIDS[i]) {
            case zf::WeightID::GEN_MC:
            case zf::WeightID::PILEUP:
            case zf::WeightID::VETO:
            case zf::WeightID::LOOSE:
            case zf::WeightID::MEDIUM:
            case zf::WeightID::TIGHT:
            case zf::WeightID::SINGLE_TRIG:
            case zf::WeightID::DOUBLE_TRIG:
            case zf::WeightID::GSF_RECO:
                weight *= WEIGHTS[i];
                break;
            default:
                break;
        }
    }

    return weight;
}

histogram_map Get2DHistoMap() {
    // The list of files
    std::map<std::string, std::string> files_to_open = {
        {"Data", "/data/whybee0a/user/gude_2/Data/20150306_SingleElectron_2012ALL_same_sign/20150306_SingleElectron_2012ALL_same_sign.root"},
        {"Signal", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/MadGraph_hadded.root"},
        {"BG_Ditau", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/BG_Ditau_hadded.root"},
        {"BG_TTBar", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/BG_TTBar_hadded.root"},
        {"BG_single_t", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/BG_singlet_tw_hadded.root"},
        {"BG_single_tbar", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/BG_singlet_tbarw_hadded.root"},
        {"BG_ww", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/BG_ww_hadded.root"},
        {"BG_wz", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/BG_wz_hadded.root"},
        {"BG_zz", "/data/whybee0a/user/gude_2/MC/20150219_MC_CTEQ6LL_same_sign_no_mass_cut/BG_zz_hadded.root"},
    };

    // The Tree to access
    const std::string TREE_NAME =
        "ZFinder/Combined Single Reco/Combined Single Reco";

    // Open the files and fill the file map
    histogram_map output_map;
    for (auto iter : files_to_open) {
        std::string data_type = iter.first;
        std::string file_name = iter.second;
        bool is_real_data = false;
        if (data_type == "Data") {
            is_real_data = true;
        }

        // Open the file and load the tree
        TTree* tree = GetTTree(file_name, TREE_NAME);

        TBranch* reco_branch = tree->GetBranch("reco");
        TLeaf* e0_charge = reco_branch->GetLeaf("e_charge0");
        TLeaf* e1_charge = reco_branch->GetLeaf("e_charge1");
        TLeaf* mee = reco_branch->GetLeaf("z_m");
        TLeaf* phistar = reco_branch->GetLeaf("z_phistar_dressed");

        // We have to grab the first entry to learn how many weights there are
        int nweights = 0;
        if (!is_real_data) {
            tree->SetBranchAddress("weight_size", &nweights);
            tree->GetEntry(0);
        }
        double weights[nweights];
        int weightid[nweights];
        if (!is_real_data) {
            tree->SetBranchAddress("weights", &weights);
            tree->SetBranchAddress("weight_ids", &weightid);
        }

        // Pack into a hitogram
        TH2D* histo = new TH2D("phistar_and_mass", "Phistar Vs. Mass;#phi*;m_{ee}", zf::ATLAS_PHISTAR_BINNING.size() - 1, &zf::ATLAS_PHISTAR_BINNING[0], 50, 0, 300);
        for (int i = 0; i < tree->GetEntries(); i++) {
            tree->GetEntry(i);

            // Reject opposite sign
            if (e0_charge->GetValue() * e1_charge->GetValue() < 0) {
                continue;
            }

            // We have a bug in our tuples where the charge is often set wrong.
            // Instead we selected the events from the beinging to be be same
            // sign, so we run on all of them.
            double weight = 1;
            if (!is_real_data) {
                weight = GetOverallNormalization(data_type);
                weight *= GetWeight(nweights, weights, weightid);
            }

            const double MEE = mee->GetValue();
            const double PHISTAR = phistar->GetValue();
            histo->Fill(PHISTAR, MEE, weight);
        }
        histo->Sumw2();

        delete tree;

        output_map[data_type] = histo;
    }

    return output_map;
}

TH2D* GetTemplate(histogram_map histo_map) {
    // Clone the histograms to make new ones to fill.
    TH2D* data_mass_histo = histo_map["Data"];
    TH2D* template_2d = new TH2D();
    template_2d = dynamic_cast<TH2D*>(data_mass_histo->Clone("2d_template"));

    // Empty the histogram
    template_2d->Reset();

    // Add the backgrounds and signal
    for (auto iter : histo_map) {
        if (iter.first != "Data") {
            template_2d->Add(iter.second, 1.);
        }
    }

    return template_2d;
}

TH1D* Get1DFromBin(TH2D* histo, const int BIN, const std::string PREFIX) {
    // Project the histogram
    TH1D* out_histo = new TH1D();
    const std::string NAME = PREFIX + std::to_string(BIN);
    out_histo = histo->ProjectionY(NAME.c_str(), BIN, BIN, "e");

    return out_histo;
}

std::pair<double, double> FitForQCD(TH1D* data_histo, TH1D* template_histo, const std::string BIN, const std::string PHISTAR_RANGE) {
    using namespace RooFit;
    // The X value of the histogram
    RooRealVar z_mass("z_mass", "m_{ee}" , 0, 300, "GeV");
    z_mass.setRange("signal", 60., 120.);
    // Data and MC histogram
    RooDataHist h_data("h_data", "h_data", RooArgSet(z_mass), data_histo);
    RooDataHist h_mc("h_mc", "h_mc", RooArgSet(z_mass), template_histo);
    RooHistPdf signalpdf("signalpdf", "signalpdf", z_mass, h_mc);
    // Background pdf
    // alpha controls the peak position
    RooRealVar alpha("alpha", "alpha", 50., 10., 90.);
    // gamma controls the slope of the exponential
    RooRealVar gamma("gamma", "gamma", 0.01, 0.0001, 0.03);
    // Delta controls the ratio of the slope of the two sides, as well as the
    // smoothness of the peak
    RooRealVar delta("delta", "delta", 40., 10, 80.);
    RooFormulaVar var1("var1", "(alpha-z_mass)/delta", RooArgSet(alpha, z_mass, delta));
    RooFormulaVar var2("var2", "-1.0*gamma*z_mass", RooArgSet(gamma, z_mass));
    RooGenericPdf MyBackgroundPdf("MyBackgroundPdf", "ROOT::Math::erfc(var1)*exp(var2)", RooArgSet(var1, var2));
    // Make the sum of the histogram and the Haupt function
    RooRealVar sigratio("sigratio", "sigratio", 0.9, 0.6, 1.0);
    RooAddPdf combined_pdf("combined_pdf", "combined_pdf", RooArgList(signalpdf, MyBackgroundPdf), RooArgList(sigratio));

    // Fit, but without yelling please
    std::cout << std::endl;
    std::cout << "Running on Bin: " << BIN << std::endl;
    std::cout << std::endl;
    RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING) ;
    gErrorIgnoreLevel = kWarning;
    RooFitResult* fit_result = combined_pdf.fitTo(h_data, Verbose(false), PrintLevel(-1), Save(true));

    // Set up the legend using the plot edges to set its location
    const double RIGHT_EDGE = 0.90;
    const double TOP_EDGE = 0.92;
    const double LEG_HEIGHT = 0.20;
    const double LEG_LENGTH = 0.40;
    TLegend legend(
            RIGHT_EDGE - LEG_LENGTH,
            (TOP_EDGE - 0.025) - LEG_HEIGHT,  // 0.025 offset to avoid ticks
            RIGHT_EDGE,
            TOP_EDGE - 0.025  // 0.025 offset to avoid the ticks
            );
    legend.SetFillColor(kWhite);
    legend.SetBorderSize(0);  // Remove drop shadow and border
    legend.SetFillStyle(0);  // Transparent

    // Plot
    TCanvas canvas("canvas", "canvas", 600, 600);
    canvas.cd();
    gPad->SetLogy();
    const std::string TITLE = "QCD Fit: " + PHISTAR_RANGE;
    RooPlot* fitFrame = z_mass.frame(Title(TITLE.c_str()));
    fitFrame->SetTitleOffset(1.25, "Y");

    // Plot components and add them to the legend
    h_data.plotOn(fitFrame, Name("data"));
    legend.AddEntry(fitFrame->findObject("data"), "Data", "p");

    combined_pdf.plotOn(fitFrame, Components(signalpdf), LineColor(kBlack), LineStyle(kDashed), Name("template"));
    legend.AddEntry(fitFrame->findObject("template"), "MC Template", "l");

    combined_pdf.plotOn(fitFrame, Components(MyBackgroundPdf), LineColor(kRed), LineStyle(kDashed), Name("qcd"));
    legend.AddEntry(fitFrame->findObject("qcd"), "Analytic Background", "l");

    combined_pdf.plotOn(fitFrame, LineColor(kBlue), Name("fit"));
    legend.AddEntry(fitFrame->findObject("fit"), "Sum of Fit Components", "l");

    h_data.plotOn(fitFrame);
    fitFrame->Draw();
    legend.Draw();
    const std::string FILE_TYPE = "pdf";
    const std::string OUT_NAME = "qcd_fit_plot_for_" + BIN + "." + FILE_TYPE;
    const std::string OUT_NAME_C = "qcd_fit_plot_for_" + BIN + "." + FILE_TYPE + ".C";
    canvas.Print(OUT_NAME.c_str(), FILE_TYPE.c_str());
    canvas.Print(OUT_NAME_C.c_str(), "cxx");

    // Find the integral of the background
    //std::cout << "Computing Integral" << std::endl;
    RooAbsReal* fracInt = MyBackgroundPdf.createIntegral(z_mass, Range("signal"));

    delete fitFrame;

    //return {sigratio.getVal() * fracInt->getVal(), fracInt->getPropagatedError(*fit_result)};
    return {fracInt->getVal(), fracInt->getPropagatedError(*fit_result)};
}

void MakePhistarPlot(TH1D* histo) {
    SetPlotStyle();
    TCanvas canvas("canvas", "canvas", 800, 800);
    canvas.cd();
    //gPad->SetLogy();
    gPad->SetLogx();

    histo->GetYaxis()->SetTitle("Estimated QCD and W+Jet Events");
    histo->SetTitleOffset(1.3, "Y");
    histo->SetMinimum(0);
    histo->SetMarkerStyle(kFullCircle);
    histo->SetMarkerColor(kBlack);
    histo->SetLineColor(kBlack);

    histo->Draw("E");

    // Add CMS text inside the plot on the top left
    const std::string CMS_STRING = "CMS Preliminary";
    const double LEFT_EDGE_ = 0.10;
    const double TOP_EDGE_ = 0.95;
    TLatex* cms_latex = new TLatex(LEFT_EDGE_ + 0.035, TOP_EDGE_ - 0.055,  CMS_STRING.c_str());
    cms_latex->SetNDC(kTRUE);  // Use pad coordinates, not Axis
    cms_latex->SetTextSize(0.035);

    canvas.Print("qcd_phistar.pdf", "pdf");
    canvas.Print("qcd_phistar.C", "cxx");

    // Print bins for the latex table
    for (int i = 1; i <= histo->GetNbinsX(); ++i) {
        const double LOW_EDGE = histo->GetBinLowEdge(i);
        const double HIGH_EDGE = LOW_EDGE + histo->GetBinWidth(i);
        const double VALUE = histo->GetBinContent(i);
        //const double UP_ERR = histo->GetBinErrorUp(i);
        //const double DOWN_ERR = histo->GetBinErrorLow(i);
        const double ERR = histo->GetBinError(i);
        std::cout << '$' << LOW_EDGE << "--" << HIGH_EDGE << "$ ";
        std::cout << '&' << " $" << VALUE;
        std::cout << " \\pm " << ERR << "$ \\\\" << std::endl;
    }
}

int main() {
    // Get a map of the histograms of the Z Masses
    histogram_map histo2d = Get2DHistoMap();

    // Write and draw the histos
    TH2D* data_histo2d = histo2d["Data"];
    TH2D* template_histo2d = GetTemplate(histo2d);

    // Fit the mass peak from each phistar bin using an MC template and
    // analytic QCD function
    TH1D* qcd_phistar_histo = new TH1D("phistar", "QCD Phistar;#phi*;counts", zf::ATLAS_PHISTAR_BINNING.size() - 1, &zf::ATLAS_PHISTAR_BINNING[0]);
    TH1D* fake_zmass = new TH1D("z_mass_all", "z_mass_all", 60, 60., 120.);
    qcd_phistar_histo->Sumw2();
    for (unsigned int i = 1; i < zf::ATLAS_PHISTAR_BINNING.size(); ++i) {
        // Get histograms for each phistar bin
        TH1D* data_histo = Get1DFromBin(data_histo2d, i, "data_bin_");
        TH1D* template_histo = Get1DFromBin(template_histo2d, i, "template_bin_");

        // Set up the title for the plot
        const double MIN_PHISTAR = zf::ATLAS_PHISTAR_BINNING.at(i-1);
        const double MAX_PHISTAR = zf::ATLAS_PHISTAR_BINNING.at(i);
        std::ostringstream convert;
        convert << std::setprecision(3);
        convert << std::fixed;
        convert << MIN_PHISTAR;
        convert << " < #phi* < ";
        convert << MAX_PHISTAR;
        const std::string PHISTART_BINNING = convert.str();

        // Run the fit and make the plot
        std::ostringstream convert2;
        convert2 << std::setw(2) << std::setfill('0') << i;
        std::pair<double, double> fit_result = FitForQCD(data_histo, template_histo, convert2.str(), PHISTART_BINNING);

        // Fill the background histogram
        qcd_phistar_histo->SetBinContent(i, fit_result.first);
        qcd_phistar_histo->SetBinError(i, fit_result.second);
        fake_zmass->Fill(91, fit_result.first);

        // Clean up
        delete data_histo;
        delete template_histo;
    }

    // Divide by bin width and scale for the fact that we cosider only same
    // sign here, but in the analysis have same sign and opposite.
    const int SAME_SIGN_CORRECTION = 2;
    qcd_phistar_histo->Scale(SAME_SIGN_CORRECTION);
    fake_zmass->Scale(SAME_SIGN_CORRECTION);

    // Get the background subtracted MZ
    const int FIRST_BIN = 1;
    const int LAST_BIN = -1; // -1 means "ALL"
    TH1D* data_mass = data_histo2d->ProjectionY("data_mass", FIRST_BIN, LAST_BIN, "e");
    TH1D* mc_mass = template_histo2d->ProjectionY("mc_mass", FIRST_BIN, LAST_BIN, "e");

    data_mass->Add(mc_mass, -1);

    // Open a tfile to save our histos
    TFile output_file("output.root", "RECREATE");
    output_file.cd();

    data_histo2d->Write();
    template_histo2d->Write();
    data_mass->Write();

    // Make a folder
    output_file.mkdir("qcd");
    output_file.cd("qcd");
    qcd_phistar_histo->Write();
    fake_zmass->Write();

    MakePhistarPlot(qcd_phistar_histo);

    output_file.Write();
    output_file.Close();

    return EXIT_SUCCESS;
}
