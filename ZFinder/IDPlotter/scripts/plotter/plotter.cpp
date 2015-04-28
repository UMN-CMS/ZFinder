// Standard Library
#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>

// ROOT
#include <TCanvas.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH1.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLine.h>


void redraw_border() {
    // https://root.cern.ch/drupal/content/how-redraw-axis-and-plot-borders
    // this little macro redraws the axis tick marks and the pad border lines.
    gPad->Update();
    gPad->RedrawAxis();
}

void set_plot_style() {
    TStyle* style = new TStyle("style", "Style for P-TDR");

    // For the canvas:
    style->SetCanvasBorderMode(0);
    style->SetCanvasColor(kWhite);
    style->SetCanvasDefX(0);  //Position on screen
    style->SetCanvasDefY(0);

    // For the Pad:
    style->SetPadBorderMode(0);
    style->SetPadColor(kWhite);
    style->SetPadGridX(false);
    style->SetPadGridY(false);
    style->SetGridColor(kBlack);
    style->SetGridStyle(3);
    style->SetGridWidth(1);

    // For the frame:
    style->SetFrameBorderMode(0);
    style->SetFrameBorderSize(1);
    style->SetFrameFillColor(kWhite);
    style->SetFrameFillStyle(0);
    style->SetFrameLineColor(kBlack);
    style->SetFrameLineStyle(1);
    style->SetFrameLineWidth(1);

    // For the histo:
    // style->SetHistFillColor(1);
    style->SetHistFillStyle(0); //
    style->SetHistLineColor(kBlack);
    style->SetHistLineStyle(0);
    style->SetHistLineWidth(1);

    style->SetEndErrorSize(2);
    style->SetErrorX(0.);

    style->SetMarkerStyle(20);

    //For the fit/function:
    style->SetOptFit(1);
    style->SetFitFormat("5.4g");
    style->SetFuncColor(kRed);
    style->SetFuncStyle(1);
    style->SetFuncWidth(1);

    //For the date:
    style->SetOptDate(0);

    // For the statistics box:
    style->SetOptFile(0);
    style->SetOptStat(0);  // To display the mean and RMS: SetOptStat("mr");
    style->SetStatColor(kWhite);
    style->SetStatFont(42);
    style->SetStatFontSize(0.025);
    style->SetStatTextColor(kBlack);
    style->SetStatFormat("6.4g");
    style->SetStatBorderSize(1);
    style->SetStatH(0.1);
    style->SetStatW(0.15);

    // Margins:
    style->SetPadTopMargin(1 - 0.95);
    style->SetPadBottomMargin(0.1);
    style->SetPadLeftMargin(0.12);
    style->SetPadRightMargin(1 - 0.96);

    // For the Global title:
    style->SetOptTitle(0);
    style->SetTitleFont(42);
    style->SetTitleColor(kBlack);
    style->SetTitleTextColor(kBlack);
    style->SetTitleFillColor(kWhite);  //10 is roughly kWhite, 10% grey?
    style->SetTitleFontSize(0.05);

    // For the axis titles:
    style->SetTitleColor(kBlack, "XYZ");
    style->SetTitleFont(42, "XYZ");
    style->SetTitleSize(0.06, "XYZ");
    style->SetTitleXOffset(0.9);
    style->SetTitleYOffset(1.25);

    // For the axis labels:
    style->SetLabelColor(kBlack, "XYZ");
    style->SetLabelFont(42, "XYZ");
    style->SetLabelOffset(0.007, "XYZ");
    style->SetLabelSize(0.05, "XYZ");

    // For the axis:
    style->SetAxisColor(kBlack, "XYZ");
    style->SetStripDecimals(true);
    style->SetTickLength(0.03, "XYZ");
    style->SetNdivisions(510, "XYZ");
    style->SetPadTickX(true);  // To get tick marks on the opposite side of the frame
    style->SetPadTickY(true);

    // Change for log plots:
    style->SetOptLogx(false);
    style->SetOptLogy(false);
    style->SetOptLogz(false);

    // Set the style
    style->cd();
}

std::unique_ptr<TH1D> get_histogram(
    const std::string& FILE_NAME,
    const std::string& HISTOGRAM_NAME
) {
    // Open the file
    TFile tfile(FILE_NAME.c_str(), "READ");
    if (tfile.IsZombie()) {
        throw std::runtime_error("Could not open file " + FILE_NAME);
    }

    // Get the histogram
    std::unique_ptr<TH1D> histo(static_cast<TH1D*>(tfile.Get(HISTOGRAM_NAME.c_str())));
    if (histo.get() == nullptr || histo->IsZombie()) {
        throw std::runtime_error("Could not load histogram " + HISTOGRAM_NAME);
    }

    // Set weights
    histo->Sumw2();

    return histo;
}

void write_plot(
    const std::unique_ptr<TH1D>& HISTO,
    const std::unique_ptr<TH1D>& HISTO_MC,
    const std::string OUTPUT_NAME,
    const int REBIN = 10,
    const std::string AXIS_TITLE = "",
    const double MED_CUT = -100,
    const double TIGHT_CUT = -100,
    const double START_X = -1,
    const double END_X = -1
) {
    // Make a canvas to control writing of the pdf file
    TCanvas canvas("canvas", "canvas", 500, 500);
    canvas.cd();

    if (AXIS_TITLE != "") {
        HISTO->GetXaxis()->SetTitle(AXIS_TITLE.c_str());
    }
    HISTO->GetYaxis()->SetTitle("Normalized Counts");

    const double MARKER_SIZE = 1.;

    const double DATA_AREA = HISTO->Integral(0, HISTO->GetNbinsX()-1);
    HISTO->Scale(1. / DATA_AREA);
    const double MC_AREA = HISTO_MC->Integral(0, HISTO_MC->GetNbinsX()-1);
    HISTO_MC->Scale(1. / MC_AREA);

    //HISTO->GetYaxis()->SetNdivisions(7, 5, 0);  // Set 10 major ticks, 0 minor
    //HISTO->GetXaxis()->SetNdivisions(7, 5, 0);  // Set 10 major ticks, 0 minor
    HISTO->GetYaxis()->SetTickLength(0.02);  // Make the ticks smaller
    HISTO->GetXaxis()->SetTickLength(0.02);
    HISTO->GetYaxis()->SetTitleOffset(1.8);

    const int FORWARD_HATCH = 3004;
    const int BACKWARD_HATCH = 3005;

    HISTO->SetMarkerColor(kBlue);
    HISTO->SetLineColor(kBlue);
    HISTO->SetLineWidth(2);
    HISTO->SetFillColor(kBlue);
    HISTO->SetFillStyle(FORWARD_HATCH);
    HISTO->SetMarkerStyle(kFullTriangleDown);
    HISTO->SetMarkerSize(MARKER_SIZE);
    HISTO->Rebin(REBIN);

    HISTO_MC->SetMarkerColor(kRed);
    HISTO_MC->SetLineColor(kRed);
    HISTO_MC->SetLineWidth(2);
    HISTO_MC->SetFillColor(kRed);
    HISTO_MC->SetFillStyle(BACKWARD_HATCH);
    HISTO_MC->SetMarkerStyle(kFullTriangleUp);
    HISTO_MC->SetMarkerSize(MARKER_SIZE);
    HISTO_MC->Rebin(REBIN);
    // Set the Y max
    const double Y_MAX = std::max(HISTO->GetMaximum(), HISTO_MC->GetMaximum()) * 1.2;
    HISTO->SetMaximum(Y_MAX);
    //HISTO->SetMinimum(0.001);
    //canvas.SetLogy();

    // Set the X Axis range
    if (START_X != -1 && END_X != -1) {
        HISTO->GetXaxis()->SetRangeUser(START_X, END_X);
    }

    // Cut Line
    TLine* cut_line_0p = nullptr;
    TLine* cut_line_0m = nullptr;
    if (MED_CUT != -100) {
        const double X1 = MED_CUT;
        const double X2 = X1;
        const double Y1 = 0;
        const double Y2 = Y_MAX;
        cut_line_0p = new TLine(X1, Y1, X2, Y2);
        cut_line_0p->SetLineStyle(kDotted);
        cut_line_0p->SetLineWidth(3);
        cut_line_0m = new TLine(-X1, Y1, -X2, Y2);
        cut_line_0m->SetLineStyle(kDotted);
        cut_line_0m->SetLineWidth(3);
    }
    TLine* cut_line_1p = nullptr;
    TLine* cut_line_1m = nullptr;
    if (TIGHT_CUT != -100) {
        const double X1 = TIGHT_CUT;
        const double X2 = X1;
        const double Y1 = 0;
        const double Y2 = Y_MAX;
        cut_line_1p = new TLine(X1, Y1, X2, Y2);
        cut_line_1p->SetLineStyle(kDashed);
        cut_line_1p->SetLineWidth(2);
        cut_line_1m = new TLine(-X1, Y1, -X2, Y2);
        cut_line_1m->SetLineStyle(kDashed);
        cut_line_1m->SetLineWidth(2);
    }

    // Add CMS text inside the plot on the top left
    const std::string CMS_STRING = "CMS Preliminary";
    const double TOP_EDGE = 0.95;
    const double LEFT_EDGE = 0.12;
    TLatex* cms_latex = new TLatex(LEFT_EDGE + 0.0, TOP_EDGE + 0.01,  CMS_STRING.c_str());
    cms_latex->SetNDC(kTRUE);  // Use pad coordinates, not Axis
    cms_latex->SetTextSize(0.035);

    // Add luminosity text outside the plot on the top right
    const std::string LUMI_STRING = "19.7 fb^{-1} (8 TeV)";
    const double RIGHT_EDGE = 0.90;
    TLatex* lumi_latex = new TLatex(RIGHT_EDGE - 0.165, TOP_EDGE + 0.01,  LUMI_STRING.c_str());
    lumi_latex->SetNDC(kTRUE);  // Use pad coordinates, not Axis
    lumi_latex->SetTextSize(0.035);

    // Set up the legend using the plot edges to set its location
    const double LEG_HEIGHT = 0.15;
    const double LEG_LENGTH = 0.40;
    TLegend legend(
            RIGHT_EDGE + 0.06 - LEG_LENGTH,
            TOP_EDGE - LEG_HEIGHT,  // 0.025 offset to avoid ticks
            RIGHT_EDGE + 0.06,
            TOP_EDGE  // 0.025 offset to avoid the ticks
            );
    legend.SetFillColor(kWhite);
    legend.AddEntry(HISTO.get(), "Minimum-bias Electrons", "f");
    legend.AddEntry(HISTO_MC.get(), "Signal MC", "f");
    legend.SetBorderSize(1);  // Remove drop shadow and border
    //legend.SetFillStyle(0);  // Transparent

    HISTO->Draw("HIST");
    HISTO_MC->Draw("HIST SAME");
    HISTO->Draw("HIST SAME");
    if (cut_line_0p != nullptr) {
        legend.AddEntry(cut_line_0p, "Medium Requirement", "l");
        cut_line_0p->Draw("SAME");
        cut_line_0m->Draw("SAME");
    }
    if (cut_line_1p != nullptr) {
        // If no other cut, both are at the same place
        if (MED_CUT == -100) {
            legend.AddEntry(cut_line_1p, "Selection Requirement", "l");
        }
        else {
            legend.AddEntry(cut_line_1p, "Tight Requirement", "l");
        }
        cut_line_1p->Draw("SAME");
        cut_line_1m->Draw("SAME");
    }
    cms_latex->Draw();
    lumi_latex->Draw();
    canvas.cd();
    redraw_border();
    legend.Draw();

    const std::string FINAL_NAME = "e_reco_var_" + OUTPUT_NAME;
    canvas.Print(FINAL_NAME.c_str(), "pdf");

    // Clean up
    if (cut_line_0p != nullptr) {
        delete cut_line_0p;
        delete cut_line_0m;
    }
    if (cut_line_1p != nullptr) {
        delete cut_line_1p;
        delete cut_line_1m;
    }
}

int main() {
    // Don't associate histograms with TFiles so we can delete them
    TH1::AddDirectory(false);

    // Set our style
    set_plot_style();

    // Get the histograms
    const std::string INPUT_FILE =
        "/data/whybee0a/user/gude_2/IDPlots/20150416/2012_ALL_hadded.root";
    const std::string INPUT_MC_FILE =
        "/data/whybee0a/user/gude_2/IDPlots/20150416/MadGraph_hadded.root";

    // Load the histograms
    // Data
    std::unique_ptr<TH1D> h_r9 = get_histogram(INPUT_FILE, "IDPlotter/r9");
    std::unique_ptr<TH1D> h_sigma_ieta_ieta_eb = get_histogram(INPUT_FILE, "IDPlotter/sieie_eb");
    std::unique_ptr<TH1D> h_sigma_ieta_ieta_ee = get_histogram(INPUT_FILE, "IDPlotter/sieie_ee");
    std::unique_ptr<TH1D> h_he = get_histogram(INPUT_FILE, "IDPlotter/he");
    std::unique_ptr<TH1D> h_deta = get_histogram(INPUT_FILE, "IDPlotter/deta");
    std::unique_ptr<TH1D> h_dphi = get_histogram(INPUT_FILE, "IDPlotter/dphi");
    std::unique_ptr<TH1D> h_track_iso = get_histogram(INPUT_FILE, "IDPlotter/track_iso");
    std::unique_ptr<TH1D> h_ecal_iso = get_histogram(INPUT_FILE, "IDPlotter/ecal_iso");
    std::unique_ptr<TH1D> h_hcal_iso = get_histogram(INPUT_FILE, "IDPlotter/hcal_iso");
    std::unique_ptr<TH1D> h_1oe_1op = get_histogram(INPUT_FILE, "IDPlotter/1oe_1op");
    std::unique_ptr<TH1D> h_d0 = get_histogram(INPUT_FILE, "IDPlotter/d0");
    std::unique_ptr<TH1D> h_dz = get_histogram(INPUT_FILE, "IDPlotter/dz");
    std::unique_ptr<TH1D> h_mhits = get_histogram(INPUT_FILE, "IDPlotter/mhits");
    std::unique_ptr<TH1D> h_iso = get_histogram(INPUT_FILE, "IDPlotter/iso");
    std::unique_ptr<TH1D> h_nmiss = get_histogram(INPUT_FILE, "IDPlotter/mhits");
    // MC
    std::unique_ptr<TH1D> h_r9_mc = get_histogram(INPUT_MC_FILE, "IDPlotter/r9");
    std::unique_ptr<TH1D> h_sigma_ieta_ieta_eb_mc = get_histogram(INPUT_MC_FILE, "IDPlotter/sieie_eb");
    std::unique_ptr<TH1D> h_sigma_ieta_ieta_ee_mc = get_histogram(INPUT_MC_FILE, "IDPlotter/sieie_ee");
    std::unique_ptr<TH1D> h_he_mc = get_histogram(INPUT_MC_FILE, "IDPlotter/he");
    std::unique_ptr<TH1D> h_deta_mc = get_histogram(INPUT_MC_FILE, "IDPlotter/deta");
    std::unique_ptr<TH1D> h_dphi_mc = get_histogram(INPUT_MC_FILE, "IDPlotter/dphi");
    std::unique_ptr<TH1D> h_track_iso_mc = get_histogram(INPUT_MC_FILE, "IDPlotter/track_iso");
    std::unique_ptr<TH1D> h_ecal_iso_mc = get_histogram(INPUT_MC_FILE, "IDPlotter/ecal_iso");
    std::unique_ptr<TH1D> h_hcal_iso_mc = get_histogram(INPUT_MC_FILE, "IDPlotter/hcal_iso");
    std::unique_ptr<TH1D> h_1oe_1op_mc = get_histogram(INPUT_MC_FILE, "IDPlotter/1oe_1op");
    std::unique_ptr<TH1D> h_d0_mc = get_histogram(INPUT_MC_FILE, "IDPlotter/d0");
    std::unique_ptr<TH1D> h_dz_mc = get_histogram(INPUT_MC_FILE, "IDPlotter/dz");
    std::unique_ptr<TH1D> h_mhits_mc = get_histogram(INPUT_MC_FILE, "IDPlotter/mhits");
    std::unique_ptr<TH1D> h_iso_mc = get_histogram(INPUT_MC_FILE, "IDPlotter/iso");
    std::unique_ptr<TH1D> h_nmiss_mc = get_histogram(INPUT_MC_FILE, "IDPlotter/mhits");

    // Write pdfs
    const std::string NO_CHANGE = "";
    const double NO_LINE = -100;
    write_plot(h_r9, h_r9_mc, "r9.pdf", 50, "R9", NO_LINE, 0.94, 0, 1);
    write_plot(h_sigma_ieta_ieta_eb, h_sigma_ieta_ieta_eb_mc, "sigma_ieta_ieta_eb.pdf", 20, "#sigma_{i #eta i #eta} in EB", NO_LINE, 0.01, 0, 0.03);
    write_plot(h_sigma_ieta_ieta_ee, h_sigma_ieta_ieta_ee_mc, "sigma_ieta_ieta_ee.pdf", 50, "#sigma_{i #eta i #eta} in EB", NO_LINE, 0.03, 0, 0.09);
    write_plot(h_he, h_he_mc, "he.pdf", 10, "H / E", NO_LINE, 0.12, 0, 0.14);
    write_plot(h_deta, h_deta_mc, "deta.pdf", 20, NO_CHANGE, 0.007, 0.005, -0.018, 0.018);
    write_plot(h_dphi, h_dphi_mc, "dphi.pdf", 20, NO_CHANGE, 0.06, 0.03, -0.14, 0.14);
    //write_plot(h_track_iso, h_track_iso_mc, "track_iso.pdf", 200);
    write_plot(h_ecal_iso, h_ecal_iso_mc, "ecal_iso.pdf", 50, NO_CHANGE, NO_LINE, 0.15, 0, 1);
    write_plot(h_hcal_iso, h_hcal_iso_mc, "hcal_iso.pdf", 50, NO_CHANGE, NO_LINE, 0.1, 0, 0.2);
    write_plot(h_1oe_1op, h_1oe_1op_mc, "1oe_1op.pdf", 50, NO_CHANGE, NO_LINE, 0.05, -0.15, 0.15);
    write_plot(h_d0, h_d0_mc, "d0.pdf", 400, NO_CHANGE, NO_LINE, 0.02, -0.06, 0.06);
    write_plot(h_dz, h_dz_mc, "dz.pdf", 20, NO_CHANGE, NO_LINE, 0.1, -0.3, 0.3);
    write_plot(h_iso, h_iso_mc, "iso.pdf", 100, NO_CHANGE, 0.15, 0.10, 0, 0.5);
    write_plot(h_nmiss, h_nmiss_mc, "nmiss.pdf", 1, NO_CHANGE, 2, 1, 0, 3);

    return EXIT_SUCCESS;
}
