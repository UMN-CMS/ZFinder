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
    TFile* data_tfile = new TFile("/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20140611_DoubleElectron_2012B/20140611_DoubleElectron_2012B_hadded.root", "READ");
    DataConfig data_config(
            data_tfile,
            "ZFinder/Combined Double Reco/5 60 < M_{ee} < 120",
            "Data",
            -1.,  // -1 means area normalize all MC to match under the signal peak
            DATA
            );
    // Signal MC
    TFile* mc_tfile = new TFile("/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20140611_Summer12_DR53X_DYToEE_M-20_CT10_TuneZ2star_8TeV-powheg-pythia6/20140611_Summer12_DR53X_DYToEE_M-20_CT10_TuneZ2star_8TeV-powheg-pythia6_hadded.root", "READ");
    DataConfig mc_config(
            mc_tfile,
            "ZFinder/Combined Double Reco/5 60 < M_{ee} < 120",
            "Signal MC",
            12234.4,  // cross section in pb, from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat8TeV
            "ZFinder/All Electrons Reco/0 acc(ALL) AND acc(ALL)/Z0 Mass: All",  // TDir from before any cut
            SIGNAL_MC
            );
    // BG
    TFile* bg_tfile_0 = new TFile("/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20140612_Summer12_DR53X_TTJets_FullLeptMGDecays_8TeV-madgraph/20140612_Summer12_DR53X_TTJets_FullLeptMGDecays_8TeV-madgraph_hadded.root", "READ");
    DataConfig bg_config_0(
            bg_tfile_0,
            "ZFinder/Combined Double Reco/5 60 < M_{ee} < 120",
            "T-TBar",
            23.64,  // TTBar Inclusive is 245.8 Pb, T->Wq is 0.9621, W->lnu is 0.3257
            "ZFinder/All Electrons Reco/0 acc(ALL) AND acc(ALL)/Z0 Mass: All",
            BG_MC
            );
    TFile* bg_tfile_1 = new TFile("/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20140619_bgs_run_at_fnal/BG_Ditau/res/BG_Ditau_hadded.root", "READ");
    DataConfig bg_config_1(
            bg_tfile_1,
            "ZFinder/Combined Double Reco/5 60 < M_{ee} < 120",
            "Z To TauTau",
            1966.7,
            "ZFinder/unweighted_counter",
            BG_MC
            );
    TFile* bg_tfile_2 = new TFile("/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20140619_bgs_run_at_fnal/BG_singlet_tw/res/BG_singlet_tw_hadded.root", "READ");
    DataConfig bg_config_2(
            bg_tfile_2,
            "ZFinder/Combined Double Reco/5 60 < M_{ee} < 120",
            "single_t",
            11.1,
            "ZFinder/unweighted_counter",
            BG_MC
            );
    TFile* bg_tfile_3 = new TFile("/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20140619_bgs_run_at_fnal/BG_singlet_tbarw/res/BG_singletbar_tw_hadded.root", "READ");
    DataConfig bg_config_3(
            bg_tfile_3,
            "ZFinder/Combined Double Reco/5 60 < M_{ee} < 120",
            "single_t",
            11.1,
            "ZFinder/unweighted_counter",
            BG_MC
            );
    TFile* bg_tfile_4 = new TFile("/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20140619_bgs_run_at_fnal/BG_wz/res/BG_wz_hadded.root", "READ");
    DataConfig bg_config_4(
            bg_tfile_4,
            "ZFinder/Combined Double Reco/5 60 < M_{ee} < 120",
            "wz",
            33.21,
            "ZFinder/unweighted_counter",
            BG_MC
            );
    TFile* bg_tfile_5 = new TFile("/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20140619_bgs_run_at_fnal/BG_ww/res/BG_ww_hadded.root", "READ");
    DataConfig bg_config_5(
            bg_tfile_5,
            "ZFinder/Combined Double Reco/5 60 < M_{ee} < 120",
            "ww",
            54.84,
            "ZFinder/unweighted_counter",
            BG_MC
            );
    TFile* bg_tfile_6 = new TFile("/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20140619_bgs_run_at_fnal/BG_zz/res/BG_zz_hadded.root", "READ");
    DataConfig bg_config_6(
            bg_tfile_6,
            "ZFinder/Combined Double Reco/5 60 < M_{ee} < 120",
            "w",
            17.7,
            "ZFinder/unweighted_counter",
            BG_MC
            );
    TFile* bg_tfile_7 = new TFile("/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20140619_bgs_run_at_fnal/BG_w_jets/res/BG_w_jets.root", "READ");
    DataConfig bg_config_7(
            bg_tfile_7,
            "ZFinder/Combined Double Reco/5 60 < M_{ee} < 120",
            "w_jets",
            37509.,
            "ZFinder/unweighted_counter",
            BG_MC
            );
    // BG Map
    data_config_map bg_map;
    bg_map["TTBar"] = bg_config_0;
    bg_map["Z to #tau #tau"] = bg_config_1;
    bg_map["Single Top"] = bg_config_2;
    bg_map["Single TBar"] = bg_config_3;
    bg_map["WZ"] = bg_config_4;
    bg_map["WW"] = bg_config_5;
    bg_map["ZZ"] = bg_config_6;
    //bg_map["W + Jets"] = bg_config_7;

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
    //plotter->plot(E0_PHI, "e0_phi.png");
    //plotter->plot(E0_CHARGE, "e0_charge.png");
    plotter->plot(E1_PT, "e1_pt.png");
    plotter->plot(E1_ETA, "e1_eta.png");
    //plotter->plot(E1_PHI, "e1_phi.png");
    //plotter->plot(E1_CHARGE, "e1_charge.png");
    plotter->plot(PHISTAR, "phistar.png");
    plotter->plot(N_VERTS, "n_verts.png");
    plotter->plot(N_E, "n_e.png");

    // Clean up
    delete plotter;
    delete data_tfile;
    delete mc_tfile;
    delete bg_tfile_0;
    delete bg_tfile_1;
    delete bg_tfile_2;
    delete bg_tfile_3;
    delete bg_tfile_4;
    delete bg_tfile_5;
    delete bg_tfile_6;
    delete bg_tfile_7;

    return 0;
}
