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
    TFile* data_tfile = new TFile(
            "/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20141010_SingleElectron_2012ALL/20141010_SingleElectron_2012ALL_hadded.root"
            , "READ");
    DataConfig data_config(
            data_tfile,
            "ZFinder/Combined Single Reco/6 60 < M_{ee} < 120",
            "Data",
            -1.,  // -1 means area normalize all MC to match under the signal peak
            DATA
            );
    // Signal MC
    TFile* mc_tfile = new TFile(
            "/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20141126_regressed_and_smeared_MC/madgraph_hadded.root"
            , "READ");
    DataConfig mc_config(
            mc_tfile,
            "ZFinder/Combined Single Reco/7 60 < M_{ee} < 120",
            "DY to ee",
            1966.7,  // cross section in pb, from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat8TeV
            "ZFinder/unweighted_counter",  // TDir from before any cut
            SIGNAL_MC
            );
    // BG
    TFile* bg_tfile_0 = new TFile(
            "/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20141110_Summer12_DR53X_TTJets_FullLeptMGDecays_8TeV-madgraph_2012ABCD_tuples/20141110_Summer12_DR53X_TTJets_FullLeptMGDecays_8TeV-madgraph_2012ABCD_tuples_hadded.root"
            , "READ");
    DataConfig bg_config_0(
            bg_tfile_0,
            "ZFinder/Combined Single Reco/7 60 < M_{ee} < 120",
            "t#bar{t}",
            23.64,  // TTBar Inclusive is 245.8 Pb, T->Wq is 0.9621, W->lnu is 0.3257
            "ZFinder/unweighted_counter",
            BG_MC
            );
    TFile* bg_tfile_1 = new TFile(
            "/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20141126_regressed_and_smeared_MC/bg_ditau_hadded.root"
            , "READ");
    DataConfig bg_config_1(
            bg_tfile_1,
            "ZFinder/Combined Single Reco/7 60 < M_{ee} < 120",
            "DY to #tau#tau",
            1966.7,
            "ZFinder/unweighted_counter",
            BG_MC
            );
    TFile* bg_tfile_2 = new TFile(
            "/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20141126_regressed_and_smeared_MC/bg_single_t_hadded.root"
            , "READ");
    DataConfig bg_config_2(
            bg_tfile_2,
            "ZFinder/Combined Single Reco/7 60 < M_{ee} < 120",
            "Single t",
            11.1,
            "ZFinder/unweighted_counter",
            BG_MC
            );
    TFile* bg_tfile_3 = new TFile(
            "/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20141126_regressed_and_smeared_MC/bg_single_tbar_hadded.root"
            , "READ");
    DataConfig bg_config_3(
            bg_tfile_3,
            "ZFinder/Combined Single Reco/7 60 < M_{ee} < 120",
            "single #bar{t}",
            11.1,
            "ZFinder/unweighted_counter",
            BG_MC
            );
    TFile* bg_tfile_4 = new TFile(
            "/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20141126_regressed_and_smeared_MC/bg_wz_hadded.root"
            , "READ");
    DataConfig bg_config_4(
            bg_tfile_4,
            "ZFinder/Combined Single Reco/7 60 < M_{ee} < 120",
            "WZ",
            33.21,
            "ZFinder/unweighted_counter",
            BG_MC
            );
    TFile* bg_tfile_5 = new TFile(
            "/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20141126_regressed_and_smeared_MC/bg_ww_hadded.root"
            , "READ");
    DataConfig bg_config_5(
            bg_tfile_5,
            "ZFinder/Combined Single Reco/7 60 < M_{ee} < 120",
            "WW",
            54.84,
            "ZFinder/unweighted_counter",
            BG_MC
            );
    TFile* bg_tfile_6 = new TFile(
            "/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20141126_regressed_and_smeared_MC/bg_zz_hadded.root"
            , "READ");
    DataConfig bg_config_6(
            bg_tfile_6,
            "ZFinder/Combined Single Reco/7 60 < M_{ee} < 120",
            "ZZ",
            17.7,
            "ZFinder/unweighted_counter",
            BG_MC
            );
    TFile* bg_tfile_7 = new TFile(
            "/local/cms/user/gude/alex_thesis/ZFinder_RooWorkspaces/20141126_regressed_and_smeared_MC/bg_w_jets_hadded.root"
            , "READ");
    DataConfig bg_config_7(
            bg_tfile_7,
            "ZFinder/Combined Single Reco/7 60 < M_{ee} < 120",
            "W + Jets",
            37509.,
            "ZFinder/unweighted_counter",
            BG_MC
            );
    // BG Map, the names are used for sort ordering alphabetically, so adding
    // numbers to the front can be used to change the order.
    data_config_map bg_map;
    bg_map["9 TTBAR"] = bg_config_0;
    bg_map["5 DY to #tau#tau"] = bg_config_1;
    bg_map["3 Single Top"] = bg_config_2;
    bg_map["4 Single TBar"] = bg_config_3;
    bg_map["8 WZ"] = bg_config_4;
    bg_map["6 WW"] = bg_config_5;
    bg_map["7 ZZ"] = bg_config_6;
    bg_map["2 W + Jets"] = bg_config_7;

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
