#ifndef ZFINDER_CROSS_CHECK_PLOTTER_H_
#define ZFINDER_CROSS_CHECK_PLOTTER_H_

// Standard Library
#include <map>
#include <string>
#include <utility>  // std::pair
#include <vector>
#include <stdlib.h>  // std::exit, EXIT_FAILURE

// Root
#include <TFile.h>
#include <TH1.h>
#include <TStyle.h>


// Various hatching paterns
enum RootFill{
    PACKEDDOT_FILL = 3001,
    DOT_FILL = 3002,
    SPARSEDOT_FILL = 3003,
    FORWARD_HATCH = 3004,
    BACKWARD_HATCH = 3005,
    VERT_HATCH = 3006,
    HOR_HATCH = 3007,
    CROSS_HATCH = 3013
};

enum PlotType{
    Z_MASS_ALL,
    Z_MASS_COARSE,
    Z_MASS_FINE,
    Z_RAPIDITY,
    Z_PT,
    E0_PT,
    E0_ETA,
    E0_PHI,
    E0_CHARGE,
    E1_PT,
    E1_ETA,
    E1_PHI,
    E1_CHARGE,
    PHISTAR,
    N_VERTS,
    N_E,
    E0_R9,
    E1_R9,
    E0_SIGMAIETA,
    E1_SIGMAIETA,
    E0_HE,
    E1_HE,
    E0_DETA,
    E1_DETA,
    E0_DPHI,
    E1_DPHI,
    E0_TRACKISO,
    E1_TRACKISO,
    E0_ECALISO,
    E1_ECALISO,
    E0_HCALISO,
    E1_HCALISO,
    E0_1OEM1OP,
    E1_1OEM1OP
};

struct PlotConfig{
    // Constructor
    PlotConfig(
        std::string x_label,
        std::string y_label,
        std::string title,
        std::string histo_name,
        bool logy,
        bool logx,
        std::vector<double> binning
            ) :
        x_label(x_label),
        y_label(y_label),
        title(title),
        histo_name(histo_name),
        logy(logy),
        logx(logx),
        binning(binning)
    { // Everything needed is done by the initializer list
    }

    // Variables used in plotting
    std::string x_label;
    std::string y_label;
    std::string title;
    std::string histo_name;
    bool logy;
    bool logx;
    std::vector<double> binning;
};

enum DataType{
    DATA,
    SIGNAL_MC,
    BG_MC
};

struct DataConfig{
    /*
     * Cross sections should be given in picobarns, and luminosities should be
     * in 1/pb.
     */
    // Empty constructor
    DataConfig() {}
    // Constructor for data
    DataConfig(
            TFile* tfile,
            std::string tdir_name,
            std::string name,
            double luminosity,
            DataType datatype
            ) :
        tfile(tfile),
        tdir_name(tdir_name),
        name(name),
        luminosity(luminosity),
        datatype(datatype),
        scale_factor(-1)
    { // Everything needed is done by the initializer list
    }
    // Constructor for MC
    DataConfig(
            TFile* tfile,
            std::string tdir_name,
            std::string name,
            double cross_section,
            std::string tdir_uncut,
            DataType datatype
            ) :
        tfile(tfile),
        tdir_name(tdir_name),
        name(name),
        datatype(datatype),
        scale_factor(-1)
    {
        // We need to open the tfile and pull out the number of events
        TH1* tmp_histo;
        const std::string TARGET_HISTO = tdir_uncut;
        tfile->GetObject(TARGET_HISTO.c_str(), tmp_histo);
        if (!tmp_histo) {
            std::cout << "Can not open the " << name;
            std::cout << " histogram to count events!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        const int N_BINS = tmp_histo->GetNbinsX();
        // Integrate from the underflow bin (0) to the overflow (N+1)
        const double EVENTS = tmp_histo->Integral(0, N_BINS + 1);
        //std::cout << name << " Events: " << EVENTS << std::endl;
        // Compute the luminosity
        luminosity = EVENTS / cross_section;
    }

    // Variables for data and MC
    TFile* tfile;
    std::string tdir_name;
    std::string name;
    double luminosity;
    DataType datatype;
    double scale_factor;
};

struct HistoStore{
    HistoStore(
        TH1D* data_histo,
        TH1D* mc_histo,
        std::vector<std::pair<std::string, TH1D*>> bg_histos
            ) :
        data_histo(data_histo),
        mc_histo(mc_histo),
        bg_histos(bg_histos)
    { // Everything needed is done by the initializer list
    }

    // Variables
    TH1D* data_histo;
    TH1D* mc_histo;
    std::vector<std::pair<std::string, TH1D*>> bg_histos;
};

// Typedefs of our custom types
typedef std::map<PlotType, PlotConfig> config_map;
typedef std::pair<PlotType, PlotConfig> config_pair;
typedef std::map<std::string, DataConfig> data_config_map;

class CrossCheckPlotter{
    public:
        // Constructors
        CrossCheckPlotter() {}
        CrossCheckPlotter(
                DataConfig data_config,
                DataConfig mc_config
                );
        CrossCheckPlotter(
                DataConfig data_config,
                DataConfig mc_config,
                data_config_map bg_configs
                );
        // Destructor
        ~CrossCheckPlotter();

        // Make various plots
        void plot(
            const PlotType PLOT_TYPE,
            const std::string FILE_NAME = "output.png"
            );

    private:
        // Used to set up the class from the multiple constructors
        void setup();

        // Store all the Data and MC information
        DataConfig data_config_;  // Data
        DataConfig mc_config_;  // Signal MC
        data_config_map bg_configs_;  // Background MC

        // Open the histograms
        HistoStore open_histos(
                const std::string HISTO_NAME,
                const bool DO_RESCALE = false
                );

        // Set the plotting style
        void set_plot_style();
        void redraw_border();
        TStyle* style_;

        // Input TFiles
        TFile* data_tfile_;
        TFile* mc_tfile_;

        // Histogram rescaling
        void set_mc_scale_factors();
        double get_rescaling(const DataConfig& DATA, const DataConfig& MC);
        double set_area_rescale_factor();
        double area_rescale_factor_;

        // Target directories
        std::string data_dir_name_;
        std::string mc_dir_name_;

        // Functions and variables related to the config_map
        void init_config_map();
        config_map conf_map_;

        // Calculate rebinning
        std::vector<double> get_rebinning(
                std::vector<double> desired_bins,
                const TH1* const HISTO
                );

        // Hardcoded variables
        static constexpr int X_VAL_ = 1000;
        static constexpr int Y_VAL_ = 1000;
        // These are measured from 0, 0 in the bottom left
        static constexpr double RIGHT_EDGE_ = 0.96;
        static constexpr double LEFT_EDGE_ = 0.10;
        static constexpr double TOP_EDGE_ = 0.95;
        static constexpr double BOTTOM_EDGE_ = 0.10;
        // Size of the ratio plot (0-1, as a fraction of the total height)
        static constexpr double RATIO_HEIGHT = 0.25;
        // Colors and styles for backgrounds
        void init_color_styles();
        std::vector<std::pair<RootFill, int>> color_styles_;
};
#endif  // ZFINDER_CROSS_CHECK_PLOTTER_H_
