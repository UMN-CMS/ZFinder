#include <iostream>
#include <string>

// ROOT
#include <TCanvas.h>
#include <TH1D.h>
#include <TLatex.h>
#include <TLegend.h>

// Plotting
#include "cross_check_plotter.h"

CrossCheckPlotter::CrossCheckPlotter(
        TFile* data_tfile,
        TFile* mc_tfile,
        std::string dir
        ) {
    // Use the same directory name for both MC and data
    setup(data_tfile, mc_tfile, dir, dir);
}

CrossCheckPlotter::CrossCheckPlotter(
        TFile* data_tfile,
        TFile* mc_tfile,
        std::string data_dir,
        std::string mc_dir
        ) {
    setup(data_tfile, mc_tfile, data_dir, mc_dir);
}

CrossCheckPlotter::~CrossCheckPlotter() {
    /* Clean up our heap variables */
    delete style_;
}

void CrossCheckPlotter::setup(
        TFile* data_tfile,
        TFile* mc_tfile,
        std::string data_dir,
        std::string mc_dir
        ) {
    // Save the pointers to the files
    data_tfile_ = data_tfile;
    mc_tfile_ = mc_tfile;

    // Save the directories
    data_dir_name_ = data_dir;
    mc_dir_name_ = mc_dir;

    // Set up the config map
    init_config_map();

    // Set up the style
    set_plot_style();
}

double CrossCheckPlotter::get_maximum(
        const TH1* const data_histo,
        const TH1* const mc_histo
        ) {
    /* Figure out the largest Y value */
    const double DATA_MAX = data_histo->GetMaximum();
    const double MC_MAX = mc_histo->GetMaximum();
    if (MC_MAX > DATA_MAX) {
        return MC_MAX;
    } else {
        return DATA_MAX;
    }
}

void CrossCheckPlotter::plot(
        const PlotType PLOT_TYPE,
        const std::string FILE_NAME
        ) {
    // Get our config_pair and some of the values from it
    config_map::iterator i_config_pair = conf_map_.find(PLOT_TYPE);
    if (i_config_pair == conf_map_.end()) {
        std::cout << "Missing PLOT_TYPE in conf_map_!" << std::endl;
        return;
    }
    PlotConfig plot_config = i_config_pair->second;
    const std::string HISTO_NAME = plot_config.histo_name;

    // Open the histograms
    const std::string DATA_HISTO_NAME = data_dir_name_ + "/" + HISTO_NAME;
    TH1D* data_histo;
    data_tfile_->GetObject(DATA_HISTO_NAME.c_str(), data_histo);
    if (!data_histo) {
        std::cout << "Can not open the Data Histogram!" << std::endl;
        return;
    }

    const std::string MC_HISTO_NAME = mc_dir_name_ + "/" + HISTO_NAME;
    TH1D* mc_histo;
    mc_tfile_->GetObject(MC_HISTO_NAME.c_str(), mc_histo);
    if (!mc_histo) {
        std::cout << "Can not open the MC Histogram!" << std::endl;
        return;
    }

    // Normalize areas
    const double MC_AREA = mc_histo->Integral();
    const double DATA_AREA = data_histo->Integral();
    mc_histo->Scale(DATA_AREA / MC_AREA);
    // Update uncertainties after rescaling
    data_histo->Sumw2();
    mc_histo->Sumw2();

    // Make a canvas to hold it
    TCanvas canvas("canvas", "canvas", X_VAL_, Y_VAL_);
    canvas.cd();

    // Set up the styles of the histograms
    style_->cd();
    // Title
    data_histo->SetTitle(0);  // Remove the title, we'll place it by hand
    mc_histo->SetTitle(0);
    // Axis labels
    data_histo->GetXaxis()->SetTitle(plot_config.x_label.c_str());
    mc_histo->GetXaxis()->SetTitle(plot_config.x_label.c_str());
    data_histo->GetYaxis()->SetTitle(plot_config.y_label.c_str());
    mc_histo->GetYaxis()->SetTitle(plot_config.y_label.c_str());
    // Position of axis labels
    data_histo->GetYaxis()->SetTitleOffset(1.4);
    data_histo->GetXaxis()->SetTitleOffset(1.2);
    // Marker, line, and fill style
    data_histo->SetMarkerStyle(kFullCircle);
    data_histo->SetMarkerColor(kBlack);
    data_histo->SetLineColor(kBlack);
    mc_histo->SetLineColor(kBlue);
    mc_histo->SetFillColor(kBlue);
    const int FORWARD_HATCH = 3004;
    //const int BACK_HATCH = 3005;
    mc_histo->SetFillStyle(FORWARD_HATCH);

    // Set the plot range maximum based on the highest peak in either histo
    const double NEW_MAX = 1.05 * get_maximum(data_histo, mc_histo);
    data_histo->SetMaximum(NEW_MAX);

    // Set up the legend using the plot edges to set its location
    const double LEG_HEIGHT = 0.15;
    const double LEG_LENGTH = 0.15;
    TLegend legend(RIGHT_EDGE_ - LEG_LENGTH, TOP_EDGE_ - LEG_HEIGHT, RIGHT_EDGE_, TOP_EDGE_);
    legend.SetFillColor(kWhite);
    legend.AddEntry(data_histo, "Data", "p");
    legend.AddEntry(mc_histo, "MC", "f");
    legend.SetBorderSize(1);  // Remove drop shadow

    // Add title
    TLatex *plot_title = NULL;
    if (plot_config.title != "") {
        const std::string TITLE = plot_config.title;
        plot_title = new TLatex(0.18, 0.93, TITLE.c_str());
        plot_title->SetNDC();
        plot_title->SetTextFont(42);
        plot_title->SetTextColor(1);
        plot_title->SetTextSize(0.06);
        plot_title->SetTextAlign(22);
        plot_title->SetTextAngle(0);
    }

    // Draw the histograms
    mc_histo->Draw("HIST");
    data_histo->Draw("E SAME");
    legend.Draw();
    if (plot_title != NULL) { plot_title->Draw(); }

    // Save the plot as a png
    canvas.Print(FILE_NAME.c_str(), "png");

    // Clean up
    delete plot_title;
}

void CrossCheckPlotter::set_plot_style() {
    style_ = new TStyle("style_","Style for P-TDR");

    // For the canvas:
    style_->SetCanvasBorderMode(0);
    style_->SetCanvasColor(kWhite);
    style_->SetCanvasDefX(0);  //Position on screen
    style_->SetCanvasDefY(0);

    // For the Pad:
    style_->SetPadBorderMode(0);
    style_->SetPadColor(kWhite);
    style_->SetPadGridX(false);
    style_->SetPadGridY(false);
    style_->SetGridColor(kBlack);
    style_->SetGridStyle(3);
    style_->SetGridWidth(1);

    // For the frame:
    style_->SetFrameBorderMode(0);
    style_->SetFrameBorderSize(1);
    style_->SetFrameFillColor(kWhite);
    style_->SetFrameFillStyle(0);
    style_->SetFrameLineColor(kBlack);
    style_->SetFrameLineStyle(1);
    style_->SetFrameLineWidth(1);

    // For the histo:
    // style_->SetHistFillColor(1);
    style_->SetHistFillStyle(0); //
    style_->SetHistLineColor(kBlack);
    style_->SetHistLineStyle(0);
    style_->SetHistLineWidth(1);

    style_->SetEndErrorSize(2);
    style_->SetErrorX(0.);

    style_->SetMarkerStyle(20);

    //For the fit/function:
    style_->SetOptFit(1);
    style_->SetFitFormat("5.4g");
    style_->SetFuncColor(kRed);
    style_->SetFuncStyle(1);
    style_->SetFuncWidth(1);

    //For the date:
    style_->SetOptDate(0);

    // For the statistics box:
    style_->SetOptFile(0);
    style_->SetOptStat(0);  // To display the mean and RMS: SetOptStat("mr");
    style_->SetStatColor(kWhite);
    style_->SetStatFont(42);
    style_->SetStatFontSize(0.025);
    style_->SetStatTextColor(kBlack);
    style_->SetStatFormat("6.4g");
    style_->SetStatBorderSize(1);
    style_->SetStatH(0.1);
    style_->SetStatW(0.15);

    // Margins:
    style_->SetPadTopMargin(1 - TOP_EDGE_);
    style_->SetPadBottomMargin(BOTTOM_EDGE_);
    style_->SetPadLeftMargin(LEFT_EDGE_);
    style_->SetPadRightMargin(1 - RIGHT_EDGE_);

    // For the Global title:
    style_->SetOptTitle(0);
    style_->SetTitleFont(42);
    style_->SetTitleColor(kBlack);
    style_->SetTitleTextColor(kBlack);
    style_->SetTitleFillColor(kWhite);  //10 is roughly kWhite, 10% grey?
    style_->SetTitleFontSize(0.05);

    // For the axis titles:
    style_->SetTitleColor(kBlack, "XYZ");
    style_->SetTitleFont(42, "XYZ");
    style_->SetTitleSize(0.06, "XYZ");
    style_->SetTitleXOffset(0.9);
    style_->SetTitleYOffset(1.25);

    // For the axis labels:
    style_->SetLabelColor(kBlack, "XYZ");
    style_->SetLabelFont(42, "XYZ");
    style_->SetLabelOffset(0.007, "XYZ");
    style_->SetLabelSize(0.05, "XYZ");

    // For the axis:
    style_->SetAxisColor(kBlack, "XYZ");
    style_->SetStripDecimals(true);
    style_->SetTickLength(0.03, "XYZ");
    //style_->SetNdivisions(510, "XYZ");
    style_->SetPadTickX(true);  // To get tick marks on the opposite side of the frame
    style_->SetPadTickY(true);

    // Change for log plots:
    style_->SetOptLogx(false);
    style_->SetOptLogy(false);
    style_->SetOptLogz(false);

    // Set the style
    style_->cd();
}

void CrossCheckPlotter::init_config_map() {
    /*
     * Here we fill the PlotConfigs. Unfortunately these must be set by hand,
     * and there should be one for every value in the PlotType enum.
     */
    // Z_MASS
    conf_map_.insert(
            config_pair(
                Z_MASS_ALL,
                PlotConfig(
                    "m_{ee} [GeV]",
                    "Events",
                    "",
                    "Z0 Mass: All"
                    )
                )
            );
    conf_map_.insert(
            config_pair(
                Z_MASS_COARSE,
                PlotConfig(
                    "m_{ee} [GeV]",
                    "Events",
                    "",
                    "Z0 Mass: Coarse"
                    )
                )
            );
    conf_map_.insert(
            config_pair(
                Z_MASS_FINE,
                PlotConfig(
                    "m_{ee} [GeV]",
                    "Events",
                    "",
                    "Z0 Mass: Fine"
                    )
                )
            );
    // Z Rapidity
    conf_map_.insert(
            config_pair(
                Z_RAPIDITY,
                PlotConfig(
                    "Y_{Z}",
                    "Events",
                    "",
                    "Z0 Rapidity"
                    )
                )
            );
    // PT
    conf_map_.insert(
            config_pair(
                Z_PT,
                PlotConfig(
                    "Z p_{T} [GeV]",
                    "Events",
                    "",
                    "Z0 p_{T}"
                    )
                )
            );
    conf_map_.insert(
            config_pair(
                E0_PT,
                PlotConfig(
                    "e_{0} p_{T} [GeV]",
                    "Events",
                    "",
                    "p_{T,e_{0}}"
                    )
                )
            );
    conf_map_.insert(
            config_pair(
                E1_PT,
                PlotConfig(
                    "e_{1} p_{T} [GeV]",
                    "Events",
                    "",
                    "p_{T,e_{1}}"
                    )
                )
            );
    // Eta
    conf_map_.insert(
            config_pair(
                E0_ETA,
                PlotConfig(
                    "#eta_{e_{0}}",
                    "Events",
                    "",
                    "#eta_{e_{0}}"
                    )
                )
            );
    conf_map_.insert(
            config_pair(
                E1_ETA,
                PlotConfig(
                    "#eta_{e_{1}}",
                    "Events",
                    "",
                    "#eta_{e_{1}}"
                    )
                )
            );
    // Phi
    conf_map_.insert(
            config_pair(
                E0_PHI,
                PlotConfig(
                    "#phi_{e_{0}}",
                    "Events",
                    "",
                    "#phi_{e_{0}}"
                    )
                )
            );
    conf_map_.insert(
            config_pair(
                E1_PHI,
                PlotConfig(
                    "#phi_{e_{1}}",
                    "Events",
                    "",
                    "#phi_{e_{1}}"
                    )
                )
            );
    // Charge
    conf_map_.insert(
            config_pair(
                E0_CHARGE,
                PlotConfig(
                    "q_{e_{0}}",
                    "Events",
                    "",
                    "charge_{e_{0}}"
                    )
                )
            );
    conf_map_.insert(
            config_pair(
                E1_CHARGE,
                PlotConfig(
                    "q_{e_{1}}",
                    "Events",
                    "",
                    "charge_{e_{1}}"
                    )
                )
            );
    // Phi*
    conf_map_.insert(
            config_pair(
                PHISTAR,
                PlotConfig(
                    "#phi*",
                    "Events",
                    "",
                    "#phi*"
                    )
                )
            );
    // Vertexes
    conf_map_.insert(
            config_pair(
                N_VERTS,
                PlotConfig(
                    "Number of Vertexes",
                    "Events",
                    "",
                    "N_{Vertices}"
                    )
                )
            );
    // Electrons
    conf_map_.insert(
            config_pair(
                N_E,
                PlotConfig(
                    "Number of Electrons",
                    "Events",
                    "",
                    "N_{e}"
                    )
                )
            );
}

//
//     //plabel->Draw();
//     //norm_label->Draw();
// }

