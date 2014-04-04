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
        plotter->plot(Z_MASS_COARSE, "TEST.png");

        // Clean up
        delete plotter;
        delete data_tfile;
        delete mc_tfile;

        return 0;
    }
}
