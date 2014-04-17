// Standard Library
#include <algorithm>  //std::equal_range, std::sort, std::unique
#include <cmath>  // std::abs
#include <iostream>
#include <string>

// ROOT
#include <TCanvas.h>
#include <TH1D.h>
#include <THStack.h>
#include <TLatex.h>
#include <TLegend.h>

// Plotting
#include "cross_check_plotter.h"

CrossCheckPlotter::CrossCheckPlotter(
        DataConfig data_config,
        DataConfig mc_config
        ) {
    // Save DataConfigs
    data_config_ = data_config;
    mc_config_ = mc_config;
    bg_configs_ = {};

    // General Setup
    setup();
}

CrossCheckPlotter::CrossCheckPlotter(
        DataConfig data_config,
        DataConfig mc_config,
        data_config_map bg_configs
        ) {
    // Save DataConfigs
    data_config_ = data_config;
    mc_config_ = mc_config;
    bg_configs_ = bg_configs;

    // General Setup
    setup();
}

CrossCheckPlotter::~CrossCheckPlotter() {
    /* Clean up our heap variables */
    delete style_;
}

void CrossCheckPlotter::setup() {
    // Set up the config map
    init_config_map();

    // Set up the style
    set_plot_style();

    // Set up colors and hatching
    init_color_styles();

    // Rescaling
    set_mc_scale_factors();
    if (data_config_.luminosity <= 0) {
        area_rescale_factor_ = set_area_rescale_factor();
        data_config_.luminosity = 1;
    } else {
        area_rescale_factor_ = -1;
    }
}

void CrossCheckPlotter::set_mc_scale_factors() {
    /*
     * Set the "scale_factor" in the DataConfig for each MC sample. The
     * scale_factor is used to rescale the MC as compared to the signal MC
     * (which has a scale factor of 1).
     *
     * For a BG MC sample, it is:
     *     scale_factor = bg_lumi / signal_lumi
     */
    const double SIG_LUMI = mc_config_.luminosity;
    mc_config_.scale_factor = 1.;

    for (auto& i_pair : bg_configs_) {
        double new_scale = SIG_LUMI / i_pair.second.luminosity;
        i_pair.second.scale_factor = new_scale;
        std::cout << i_pair.second.scale_factor << std::endl;
    }
}

double CrossCheckPlotter::get_rescaling(
        const DataConfig& DATA,
        const DataConfig& MC
        ) {
    /*
     * Given two luminosities in inverse picobarns, calculates the correct
     * scaling so that the two distributions have the same amount of data.
     */
    return DATA.luminosity / MC.luminosity;
}

double CrossCheckPlotter::set_area_rescale_factor() {
    /*
     * If the data config has a lumi of 0, then we scale the sum of the MC such
     * that the area from 60 to 120 GeV (the area under the Z peak) is the same
     * as in data. We store the information in the luminosity key of the
     * config. We scale the MC to their relative luminosity before calculating
     * this number.
     */
    const double LOWER = 60.;
    const double UPPER = 120.;
    // Open the histograms
    const bool DO_RESCALING = true;
    HistoStore histo_store = open_histos("Z0 Mass: All", DO_RESCALING);
    TH1D* data_histo = histo_store.data_histo;
    TH1D* mc_histo = histo_store.mc_histo;
    std::vector<std::pair<std::string, TH1D*>> bg_histos = histo_store.bg_histos;
    // Check that open_histos exited successfully, otherwise end
    if (data_histo == NULL || mc_histo == NULL) {
        return -1;
    }

    // Copy the MC histogram and add all the bg histos
    TH1D* tmp_histo = dynamic_cast<TH1D*>(mc_histo->Clone());
    for (auto& i_pair : bg_histos) {
        tmp_histo->Add(i_pair.second);
    }

    // Calculate the area
    const int DATA_LOW = data_histo->FindBin(LOWER);
    const int DATA_HIGH = data_histo->FindBin(UPPER);
    const double DATA_AREA = data_histo->Integral(DATA_LOW, DATA_HIGH);

    const int MC_LOW = tmp_histo->FindBin(LOWER);
    const int MC_HIGH = tmp_histo->FindBin(UPPER);
    const double MC_AREA = tmp_histo->Integral(MC_LOW, MC_HIGH);

    return DATA_AREA / MC_AREA;
}

std::vector<double> CrossCheckPlotter::get_rebinning(
        std::vector<double> desired_bins,
        const TH1* const HISTO
        ) {
    /* Given a desired set of bin edges, and a histogram, finds the bin edges
     * in the histogram that most closely approximate the desired edges. This
     * is done by, for each desired edge, choosing the bin edge from the
     * histogram gram that is closest in terms of linear distance.
     */
    std::vector<double> out_vec;
    // Fill the old bins
    std::vector<double> old_bins;
    const int N_BINS = HISTO->GetXaxis()->GetNbins();
    for (int i_bin = 1; i_bin <= N_BINS; ++i_bin) {
        double bin_edge = HISTO->GetXaxis()->GetBinLowEdge(i_bin);
        old_bins.push_back(bin_edge);
    }
    // Add the high edge, which isn't included but is needed
    old_bins.push_back(HISTO->GetXaxis()->GetBinUpEdge(N_BINS));
    std::sort(old_bins.begin(), old_bins.end());

    // Loop through the desired bins.
    //
    // We use a binary search to find the actual
    // bins on either side of the desired bin, we then compare the distance and
    // pick the closest. At the end we remove duplicate entries. equal_range
    // will return a pointer to the first entry that is not less than our
    // desired bin, and one that is strictly greater than our test bin. This
    // means we might need to move one of the pointers back, and of course we
    // need to check that they don't run off the edge of the vector.
    std::sort(desired_bins.begin(), desired_bins.end());
    for (auto& i : desired_bins) {
        const double DESIRED_BIN = i;
        auto bounds = std::equal_range(old_bins.begin(), old_bins.end(), DESIRED_BIN);
        // Move the first pointer back if needed
        if (       bounds.first == bounds.second
                && bounds.first != old_bins.begin()
                && *bounds.first != DESIRED_BIN
           ) {
            --bounds.first;
        }
        // Check distance, pick the closest (or the first for a tie)
        const double FIRST_BIN = *bounds.first;
        const double SECOND_BIN = *bounds.second;
        const double FIRST_DIST = std::abs(FIRST_BIN - DESIRED_BIN);
        const double SECOND_DIST = std::abs(SECOND_BIN - DESIRED_BIN);
        if (SECOND_DIST >= FIRST_DIST) {
            out_vec.push_back(FIRST_BIN);
        } else {
            out_vec.push_back(SECOND_BIN);
        }
    }

    // Remove the non-unique entries. Unique moves the non-uniques to the end
    // and returns a pointer to the first non-unique item, erase then removes
    // them.
    std::sort(out_vec.begin(), out_vec.end());
    out_vec.erase(std::unique(out_vec.begin(), out_vec.end()), out_vec.end());

    // Return our vector
    return out_vec;
}

HistoStore CrossCheckPlotter::open_histos(
        const std::string HISTO_NAME,
        const bool DO_RESCALE
        ) {
    /*
     * Open the histograms for a given PlotType and return them in a HistoStore
     * object. If DO_RESCALE is true, then use the DataConfig.scale_factor to
     * rescale the histogram. The Data is NEVER rescaled, only the signal MC
     * and BG MC are
     */
    TH1D* tmp_histo;
    const std::string DATA_HISTO_NAME = data_config_.tdir_name + "/" + HISTO_NAME;
    data_config_.tfile->GetObject(DATA_HISTO_NAME.c_str(), tmp_histo);
    if (!tmp_histo) {
        std::cout << "Can not open the Data Histogram!" << std::endl;
        return HistoStore(NULL, NULL, {});
    }
    TH1D* data_histo = dynamic_cast<TH1D*>(tmp_histo->Clone());

    const std::string MC_HISTO_NAME = mc_config_.tdir_name + "/" + HISTO_NAME;
    mc_config_.tfile->GetObject(MC_HISTO_NAME.c_str(), tmp_histo);
    if (!tmp_histo) {
        std::cout << "Can not open the MC Histogram!" << std::endl;
        return HistoStore(NULL, NULL, {});
    }
    TH1D* mc_histo = dynamic_cast<TH1D*>(tmp_histo->Clone());
    if (DO_RESCALE) {
        mc_histo->Scale(mc_config_.scale_factor);
    }

    std::vector<std::pair<std::string, TH1D*>> bg_histos = {};
    if (bg_configs_.size() != 0) {
        // Open each BG and save to a vector with its name
        for (auto& i_pair : bg_configs_) {
            const std::string BG_HISTO_NAME = i_pair.second.tdir_name + "/" + HISTO_NAME;
            TH1D* bg_histo;
            i_pair.second.tfile->GetObject(BG_HISTO_NAME.c_str(), bg_histo);
            if (!bg_histo) {
                std::cout << "Can not open the BG Histogram for ";
                std::cout << i_pair.first;
                std::cout << "!" << std::endl;
                return HistoStore(NULL, NULL, {});
            }
            // Clone incase, for some unknown reason (perhaps testing) we want
            // to use the same histogram twice
            TH1D* bg_clone = dynamic_cast<TH1D*>(bg_histo->Clone());
            if (DO_RESCALE) {
                bg_clone->Scale(i_pair.second.scale_factor);
            }
            bg_histos.push_back(std::make_pair(i_pair.first, bg_clone));
        }
    }

    // Set up and return HistoStore
    return HistoStore(data_histo, mc_histo, bg_histos);
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
    const bool DO_RESCALING = true;
    HistoStore histo_store = open_histos(HISTO_NAME, DO_RESCALING);
    TH1D* data_histo = histo_store.data_histo;
    TH1D* mc_histo = histo_store.mc_histo;
    std::vector<std::pair<std::string, TH1D*>> bg_histos = histo_store.bg_histos;
    // Check that open_histos exited successfully, otherwise end
    if (data_histo == NULL || mc_histo == NULL) {
        return;
    }

    // Rebin if the binning is greater than 0 in size. If it is size one assume
    // we want a simple rebinning (where N bins are combined to 1), otherwise
    // the vector is the edges of the bins.
    if (plot_config.binning.size() == 1) {
        mc_histo->Rebin(static_cast<int>(plot_config.binning[0]));
        data_histo->Rebin(static_cast<int>(plot_config.binning[0]));
        for (auto& i_pair : bg_histos) {
            i_pair.second->Rebin(static_cast<int>(plot_config.binning[0]));
        }
    } else if (plot_config.binning.size() > 1) {
        std::vector<double> new_binning = get_rebinning(
                plot_config.binning,
                data_histo
                );
        mc_histo = dynamic_cast<TH1D*>(
                mc_histo->Rebin(
                    new_binning.size() - 1,
                    "mc_rebinned_histo",
                    &new_binning[0]  // double*
                    )
                );
        data_histo = dynamic_cast<TH1D*>(
                data_histo->Rebin(
                    new_binning.size() - 1,
                    "data_rebinned_histo",
                    &new_binning[0]  // double*
                    )
                );
        for (auto& i_pair : bg_histos) {
            std::string new_name = i_pair.first + "_rebinned_histo";
            i_pair.second = dynamic_cast<TH1D*>(
                    i_pair.second->Rebin(
                        new_binning.size() - 1,
                        new_name.c_str(),
                        &new_binning[0]  // double*
                        )
                    );
        }
    }

    // Normalize the plots
    // Signal MC
    if (area_rescale_factor_ > 0) {
        // Normalize by area
        mc_histo->Scale(area_rescale_factor_);
    } else {
        // Normalize by luminosity
        mc_histo->Scale(get_rescaling(data_config_, mc_config_));
    }
    // BG MC
    for (auto& i_pair : bg_histos) {
        // Locate the DataConfig by name
        auto it = bg_configs_.find(i_pair.first);
        if (it != bg_configs_.end()){
            if (area_rescale_factor_ > 0) {
                // Normalize by area
                i_pair.second->Scale(area_rescale_factor_);
            } else {
                // Normalize by luminosity
                const double RESCALING = get_rescaling(data_config_, it->second);
                i_pair.second->Scale(RESCALING);
            }
        } else {
            std::cout << "Failed to normalize " << i_pair.first;
            std::cout << ". Scaling to 0!!" << std::endl;
            i_pair.second->Scale(0);
        }
    }

    // Update uncertainties after rescaling
    data_histo->Sumw2();
    mc_histo->Sumw2();
    for (auto& i_pair : bg_histos) {
        i_pair.second->Sumw2();
    }


    // Make a stack to store the MC
    THStack* histo_stack = new THStack("hs", "Monte Carlo histrogram stack");

    // Title
    data_histo->SetTitle(0);  // Remove the title, we'll place it by hand
    //histo_stack->SetTitle(0);
    // Axis labels
    data_histo->GetXaxis()->SetTitle(plot_config.x_label.c_str());
    //histo_stack->GetXaxis()->SetTitle(plot_config.x_label.c_str());
    data_histo->GetYaxis()->SetTitle(plot_config.y_label.c_str());
    //histo_stack->GetYaxis()->SetTitle(plot_config.y_label.c_str());

    // Set up the legend using the plot edges to set its location
    const double LEG_HEIGHT = 0.15;
    const double LEG_LENGTH = 0.15;
    TLegend legend(RIGHT_EDGE_ - LEG_LENGTH, TOP_EDGE_ - LEG_HEIGHT, RIGHT_EDGE_, TOP_EDGE_);
    legend.SetFillColor(kWhite);
    legend.AddEntry(data_histo, data_config_.name.c_str(), "p");
    legend.AddEntry(mc_histo, mc_config_.name.c_str(), "f");
    legend.SetBorderSize(0);  // Remove drop shadow and border
    legend.SetFillStyle(0);  // Transparent

    // Set up the styles of the histograms
    style_->cd();
    // Position of axis labels
    mc_histo->GetYaxis()->SetTitleOffset(1.25);
    mc_histo->GetXaxis()->SetTitleOffset(1.1);
    // Marker, line, and fill style
    data_histo->SetMarkerStyle(kFullCircle);
    data_histo->SetMarkerColor(kBlack);
    data_histo->SetLineColor(kBlack);
    mc_histo->SetLineColor(kBlue);
    mc_histo->SetFillColor(kBlue);
    mc_histo->SetFillStyle(SPARSEDOT_FILL);

    // Set up the background histograms' style, coloring, and legend entry. We
    // have to do this almost at the end because we need the legend to exist.
    unsigned int i_style = 0;
    for (auto& i_pair : bg_histos) {
        // Reset if we run beyond the array
        if (i_style >=  color_styles_.size()) {
            i_style = 0;
        }
        std::pair<RootFill, int> style_pair = color_styles_[i_style];
        ++i_style;
        // Set the color and fill
        i_pair.second->SetFillStyle(style_pair.first);
        i_pair.second->SetLineColor(style_pair.second);
        i_pair.second->SetFillColor(style_pair.second);
        // Add to the stack and legend
        histo_stack->Add(i_pair.second);
        legend.AddEntry(i_pair.second, i_pair.first.c_str() ,"f");
    }
    histo_stack->Add(mc_histo);

    // Set the plot range maximum and minimum based on the highest peak in
    // either histo
    // Set the maximum
    const double DATA_MAX = data_histo->GetMaximum();
    const double STACK_MAX = histo_stack->GetMaximum();

    const double MAX_CONST = 1.2;
    if (DATA_MAX > STACK_MAX) {
        data_histo->SetMaximum(DATA_MAX * MAX_CONST);
    } else {
        data_histo->SetMaximum(STACK_MAX * MAX_CONST);
    }
    // Set the minimum
    double bg_min_max = -1;  // The smallest maximum
    for (auto& i_pair : bg_histos) {
        const double BG_MAX = i_pair.second->GetMaximum();
        if (BG_MAX < bg_min_max || bg_min_max < 0) {
            bg_min_max = BG_MAX;
        }
    };
    // Set the minimum if sane, otherwise trust root to do it
    if (bg_min_max > 0) {
        data_histo->SetMinimum(bg_min_max * 0.1);
    }

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

    // Make a canvas to hold the plot
    TCanvas canvas("canvas", "canvas", X_VAL_, Y_VAL_);
    canvas.cd();
    canvas.SetLogy(plot_config.logy);

    // Draw the histograms
    data_histo->Draw("E");  // Set axis titles
    histo_stack->Draw("HIST SAME");
    data_histo->Draw("E SAME");
    legend.Draw();
    if (plot_title != NULL) { plot_title->Draw(); }

    // Save the plot as a png
    canvas.Print(FILE_NAME.c_str(), "png");

    // Clean up
    delete plot_title;
    delete histo_stack;
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

void CrossCheckPlotter::init_color_styles() {
    /*
     * Set up the color and hatching pairs used for backgrounds
     */
    color_styles_ = {
        {FORWARD_HATCH, kRed},
        {BACKWARD_HATCH, kGreen+2},
        {VERT_HATCH, kMagenta+2},
        {HOR_HATCH, kOrange},
        {CROSS_HATCH, kOrange+7}
    };
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
                    "m_{ee} [GeV]",  // x_label
                    "Events",        // y_label
                    "",              // title
                    "Z0 Mass: All",  // histogram name (for reading in)
                    true,            // log Y axis
                    {}               // Desired new binning
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
                    "Z0 Mass: Coarse",
                    true,
                    {}
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
                    "Z0 Mass: Fine",
                    true,
                    {}
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
                    "Z0 Rapidity",
                    true,
                    {}
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
                    "Z0 p_{T}",
                    true,
                    {5}  // with one entry, just calls histo->Rebin(5);
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
                    "p_{T,e_{0}}",
                    true,
                    {5}
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
                    "p_{T,e_{1}}",
                    true,
                    {5}
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
                    "#eta_{e_{0}}",
                    true,
                    {}
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
                    "#eta_{e_{1}}",
                    true,
                    {}
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
                    "#phi_{e_{0}}",
                    false,
                    {}
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
                    "#phi_{e_{1}}",
                    false,
                    {}
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
                    "charge_{e_{0}}",
                    false,
                    {}
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
                    "charge_{e_{1}}",
                    false,
                    {}
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
                    "#phi*",
                    true,
                    // multiple entries means these are new bin edges
                    {0.0, 0.05, 0.1, 0.2, 0.3, 0.4, 0.6, 1.0}
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
                    "N_{Vertices}",
                    true,
                    {}
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
                    "N_{e}",
                    true,
                    {}
                    )
                )
            );
}
