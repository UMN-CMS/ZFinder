// Standard Library
#include <iostream>
#include <string>
#include <map>

// ROOT
#include <TFile.h>

// Plotting Code
#include "cross_check_plotter.h"


int main() {

    const std::string CUTLEVEL = "ZFinder/Combined Single Reco/7 60 < M_{ee} < 120";
    //const std::string CUTLEVEL = "ZFinder/Combined Single Reco/3 acc(MUON_TIGHT) AND acc(MUON_LOOSE)";

    // Data
    TFile* data_tfile = new TFile(
            "/data/whybee0a/user/gude_2/Data/20150324_SingleElectron_2012ALL/hadded.root"
            , "READ");
    DataConfig data_config(
            data_tfile,
            CUTLEVEL,
            "Data",
            -1.,  // -1 means area normalize all MC to match under the signal peak
            DATA
            );
    // Signal MC
    TFile* mc_tfile = new TFile(
            "/data/whybee0a/user/gude_2/MC/20150318_MADGRAPH/MadGraph_DRESSED_hadded.root"
            , "READ");
    DataConfig mc_config(
            mc_tfile,
            CUTLEVEL,
            "DY to ee",
            // * 3 because 1177.3 is to a single flavor, but the MC has all 3
            1177.3 * 3.,  // cross section in pb, from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat8TeV
            //1966.7, // Powheg
            "ZFinder/weighted_counter",  // TDir from before any cut
            SIGNAL_MC
            );
    // BG
    TFile* bg_tfile_0 = new TFile(
            "/data/whybee0a/user/gude_2/MC/20150318_BGs/BG_TTBar_hadded.root"
            , "READ");
    DataConfig bg_config_0(
            bg_tfile_0,
            CUTLEVEL,
            "t#bar{t}",
            23.64,  // TTBar Inclusive is 245.8 Pb, T->Wq is 0.9621, W->lnu is 0.3257
            "ZFinder/weighted_counter",
            BG_MC
            );
    TFile* bg_tfile_1 = new TFile(
            "/data/whybee0a/user/gude_2/MC/20150318_BGs/BG_Ditau_hadded.root"
            , "READ");
    DataConfig bg_config_1(
            bg_tfile_1,
            CUTLEVEL,
            "DY to #tau#tau",
            1966.7,
            "ZFinder/weighted_counter",
            BG_MC
            );
    TFile* bg_tfile_2 = new TFile(
            "/data/whybee0a/user/gude_2/MC/20150318_BGs/BG_singlet_tw_hadded.root"
            , "READ");
    DataConfig bg_config_2(
            bg_tfile_2,
            CUTLEVEL,
            "Single t",
            11.1,
            "ZFinder/weighted_counter",
            BG_MC
            );
    TFile* bg_tfile_3 = new TFile(
            "/data/whybee0a/user/gude_2/MC/20150318_BGs/BG_singlet_tbarw_hadded.root"
            , "READ");
    DataConfig bg_config_3(
            bg_tfile_3,
            CUTLEVEL,
            "Single #bar{t}",
            11.1,
            "ZFinder/weighted_counter",
            BG_MC
            );
    TFile* bg_tfile_4 = new TFile(
            "/data/whybee0a/user/gude_2/MC/20150318_BGs/BG_wz_hadded.root"
            , "READ");
    DataConfig bg_config_4(
            bg_tfile_4,
            CUTLEVEL,
            "WZ",
            33.21,
            "ZFinder/weighted_counter",
            BG_MC
            );
    TFile* bg_tfile_5 = new TFile(
            "/data/whybee0a/user/gude_2/MC/20150318_BGs/BG_ww_hadded.root"
            , "READ");
    DataConfig bg_config_5(
            bg_tfile_5,
            CUTLEVEL,
            "WW",
            54.84,
            "ZFinder/weighted_counter",
            BG_MC
            );
    TFile* bg_tfile_6 = new TFile(
            "/data/whybee0a/user/gude_2/MC/20150318_BGs/BG_zz_hadded.root"
            , "READ");
    DataConfig bg_config_6(
            bg_tfile_6,
            CUTLEVEL,
            "ZZ",
            17.7,
            "ZFinder/weighted_counter",
            BG_MC
            );
    TFile* bg_tfile_7 = new TFile(
            "/home/user1/gude/CMS/src/CMSSW_5_3_13_UNSTABLE/src/ZFinder/Event/scripts/same_sign/output.root"
            , "READ");
    DataConfig bg_config_7(
            bg_tfile_7,
            "qcd/",
            "QCD",
            19712,
            BG_MC
            );
    // BG Map, the names are used for sort ordering alphabetically, so adding
    // numbers to the front can be used to change the order.
    data_config_map bg_map;
    bg_map["9 TTBAR"] = bg_config_0;
    bg_map["6 DY to #tau#tau"] = bg_config_1;
    bg_map["2 Single Top"] = bg_config_2;
    bg_map["3 Single TBar"] = bg_config_3;
    bg_map["7 WZ"] = bg_config_4;
    bg_map["5 WW"] = bg_config_5;
    bg_map["8 ZZ"] = bg_config_6;
    //bg_map["4 QCD"] = bg_config_7;

    // Setup the plotter
    CrossCheckPlotter* plotter = new CrossCheckPlotter(
            data_config,
            mc_config,
            bg_map
            );

    // Make a plot
    //plotter->plot(Z_MASS_ALL, "z_mass_all.pdf");
    //plotter->plot(Z_MASS_COARSE, "z_mass_coarse.pdf");
    plotter->plot(Z_MASS_FINE, "z_mass_fine.pdf");
    plotter->plot(Z_RAPIDITY, "z_rapidity.pdf");
    plotter->plot(Z_PT, "z_pt.pdf");
    plotter->plot(E0_PT, "e0_pt.pdf");
    plotter->plot(E0_ETA, "e0_eta.pdf");
    //plotter->plot(E0_PHI, "e0_phi.pdf");
    //plotter->plot(E0_CHARGE, "e0_charge.pdf");
    plotter->plot(E1_PT, "e1_pt.pdf");
    plotter->plot(E1_ETA, "e1_eta.pdf");
    //plotter->plot(E1_PHI, "e1_phi.pdf");
    //plotter->plot(E1_CHARGE, "e1_charge.pdf");
    plotter->plot(PHISTAR, "phistar.pdf");
    plotter->plot(N_VERTS, "n_verts.pdf");
    //plotter->plot(N_E, "n_e.pdf");
    //plotter->plot(E0_R9, "e0_r9.pdf");
    //plotter->plot(E1_R9, "e1_r9.pdf");
    //plotter->plot(E0_SIGMAIETA, "e0_sigmaieta.pdf");
    //plotter->plot(E1_SIGMAIETA, "e1_sigmaieta.pdf");
    //plotter->plot(E0_HE, "e0_HE.pdf");
    //plotter->plot(E1_HE, "e1_HE.pdf");
    //plotter->plot(E0_DETA, "e0_deta.pdf");
    //plotter->plot(E1_DETA, "e1_deta.pdf");
    //plotter->plot(E0_DPHI, "e0_dphi.pdf");
    //plotter->plot(E1_DPHI, "e1_dphi.pdf");
    //plotter->plot(E0_TRACKISO, "e0_track_iso.pdf");
    //plotter->plot(E1_TRACKISO, "e1_track_iso.pdf");
    //plotter->plot(E0_ECALISO, "e0_ecal_iso.pdf");
    //plotter->plot(E1_ECALISO, "e1_ecal_iso.pdf");
    //plotter->plot(E0_HCALISO, "e0_hcal_iso.pdf");
    //plotter->plot(E1_HCALISO, "e1_hcal_iso.pdf");
    //plotter->plot(E0_1OEM1OP, "e0_1oe_1op_iso.pdf");
    //plotter->plot(E1_1OEM1OP, "e1_1oe_1op_iso.pdf");

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

    return 0;
}
