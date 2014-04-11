#ifndef ZFINDER_CROSS_CHECK_PLOTTER_H_
#define ZFINDER_CROSS_CHECK_PLOTTER_H_


// Standard Library
#include <map>
#include <string>
#include <utility>  // std::pair
#include <vector>

// Root
#include <TFile.h>
#include <TH1.h>
#include <TStyle.h>

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
    N_E
};

struct PlotConfig{
    // Constructor
    PlotConfig(
        std::string x_label,
        std::string y_label,
        std::string title,
        std::string histo_name,
        bool logy,
        std::vector<double> binning
            ) :
        x_label(x_label),
        y_label(y_label),
        title(title),
        histo_name(histo_name),
        logy(logy),
        binning(binning)
    { // Everything needed is done by the initializer list
    }

    // Variables used in plotting
    std::string x_label;
    std::string y_label;
    std::string title;
    std::string histo_name;
    bool logy;
    std::vector<double> binning;
};

// Typedefs of our custom types
typedef std::map<PlotType, PlotConfig> config_map;
typedef std::pair<PlotType, PlotConfig> config_pair;

class CrossCheckPlotter{
    public:
        // Constructors
        CrossCheckPlotter() {}
        CrossCheckPlotter(
                TFile* data_tfile,
                TFile* mc_tfile,
                std::string data_dir,
                std::string mc_dir
                );
        CrossCheckPlotter(
                TFile* data_tfile,
                TFile* mc_tfile,
                std::string dir
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
        void setup(
                TFile* data_tfile,
                TFile* mc_tfile,
                std::string data_dir,
                std::string mc_dir
             );

        // Set the plotting style
        void set_plot_style();
        TStyle* style_;

        // Input TFiles
        TFile* data_tfile_;
        TFile* mc_tfile_;

        // Various helper functions
        double get_maximum(const TH1* const DATA_HISTO, const TH1* const MC_HISTO);

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

};
#endif  // ZFINDER_CROSS_CHECK_PLOTTER_H_
