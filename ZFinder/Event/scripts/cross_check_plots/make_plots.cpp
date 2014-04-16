// Standard Library
#include <iostream>
#include <string>
#include <map>

// ROOT
#include <TFile.h>

// Plotting Code
#include "cross_check_plotter.h"


int main() {
    // Data
    TFile* data_tfile = new TFile("/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20140318_DoubleElectron_2012B/20140318_DoubleElectron_2012B_hadded.root", "READ");
    DataConfig data_config(
            data_tfile,
            "ZFinder/ET-ET Combined Double Reco/7 60 < M_{ee} < 120",
            "Data",
            -1.,  // -1 means area normalize all MC to match under the signal peak
            DATA
            );
    // Signal MC
    TFile* mc_tfile = new TFile("/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20140318_Summer12_DR53X_DYToEE_M-20_CT10_TuneZ2star_8TeV-powheg-pythia6/20140318_Summer12_DR53X_DYToEE_M-20_CT10_TuneZ2star_8TeV-powheg-pythia6_hadded.root ", "READ");
    DataConfig mc_config(
            mc_tfile,
            "ZFinder/ET-ET Combined Double Reco/7 60 < M_{ee} < 120",
            "Signal MC",
            500.,  // cross section in pb
            "ZFinder/All Electrons Reco/0 acc(ALL) AND acc(ALL)",  // TDir from before any cut
            SIGNAL_MC
            );
    // BG
    TFile* bg_tfile = new TFile("/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20140414_TTBar/20140414_TTBar_hadded.root", "READ");
    DataConfig bg_config(
            bg_tfile,
            "ZFinder/ET-ET Combined Double Reco/7 60 < M_{ee} < 120",
            "T-TBar",
            500.,
            "ZFinder/All Electrons Reco/0 acc(ALL) AND acc(ALL)",
            BG_MC
            );
    // BG Map
    data_config_map bg_map;
    bg_map["T-TBar"] = bg_config;

    // Setup the plotter
    CrossCheckPlotter* plotter = new CrossCheckPlotter(
            data_config,
            mc_config,
            bg_map
            );

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
    delete bg_tfile;

    return 0;
}
