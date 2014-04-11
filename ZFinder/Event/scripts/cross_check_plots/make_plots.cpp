// Standard Library
#include <iostream>
#include <string>

// ROOT
#include <TFile.h>

// Plotting Code
#include "cross_check_plotter.h"


int main(int argc, char* argv[]) {
    const int argcLow = 4;
    const int argcHigh = 7;

    if (argc <= argcLow) {
        std::cout << "Not enough arguments.";
        return 1;
    } else if (argc >= argcHigh) {
        std::cout << "Too many arguments.";
        return 1;
    } else {
        /* Read in arguments */
        std::string input_data_file(argv[1]);
        std::string input_mc_file(argv[2]);
        std::string first_tdir(argv[3]);
        std::string second_tdir;
        std::string output_dir;
        if (argc == 5) {
            second_tdir = "";
            output_dir = argv[4];
        } else {
            second_tdir = argv[4];
            output_dir = argv[5];
        }

        // Open the TFiles
        TFile* data_tfile = new TFile(input_data_file.c_str(), "READ");
        if (!data_tfile) {
            std::cout << "Failed to open the Data File!" << std::endl;
            return 1;
        }
        TFile* mc_tfile = new TFile(input_mc_file.c_str(), "READ");
        if (!mc_tfile) {
            std::cout << "Failed to open the MC File!" << std::endl;
            return 1;
        }
        
        // Make the plotter
        CrossCheckPlotter* plotter;
        if (argc == 5) {
            plotter = new CrossCheckPlotter(
                    data_tfile, 
                    mc_tfile, 
                    first_tdir
                    );
        } else {
            plotter = new CrossCheckPlotter(
                    data_tfile, 
                    mc_tfile, 
                    first_tdir,
                    second_tdir
                    );
        }

        // Make a plot
        plotter->plot(Z_MASS_ALL, "z_mass_all.png");
        plotter->plot(Z_MASS_COARSE, "z_mass_coarse.png");
        plotter->plot(Z_MASS_FINE, "z_mass_fine.png");
        plotter->plot(Z_RAPIDITY, "z_rapidity.png");
        plotter->plot(Z_PT, "z_pt.png");
        plotter->plot(E0_PT, "e0_pt.png");
        plotter->plot(E0_ETA, "e0_eta.png");
        plotter->plot(E0_PHI, "e0_phi.png");
        plotter->plot(E0_CHARGE, "e0_charge.png");
        plotter->plot(E1_PT, "e1_pt.png");
        plotter->plot(E1_ETA, "e1_eta.png");
        plotter->plot(E1_PHI, "e1_phi.png");
        plotter->plot(E1_CHARGE, "e1_charge.png");
        plotter->plot(PHISTAR, "phistar.png");
        plotter->plot(N_VERTS, "n_verts.png");
        plotter->plot(N_E, "n_e.png");

        // Clean up
        delete plotter;
        delete data_tfile;
        delete mc_tfile;

        return 0;
    }
}
