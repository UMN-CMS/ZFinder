#include "ZFinder/Event/interface/ZFinderPlotter.h"

// Standard Library
#include <string>  // string
#include <vector>  // vector

// Root
#include <TCanvas.h>  // TCanvas

// ZFinder Code
#include "ZFinder/Event/interface/ZFinderElectron.h"  // ZFinderElectron


namespace zf {
    // Constructor
    ZFinderPlotter::ZFinderPlotter(TFileDirectory& tdir, const bool USE_MC) : USE_MC_(USE_MC) {
        /*
         * Initialize a set of histograms and associate them with a given TDirectory.
         */
        std::vector<double> ATLAS_PHISTAR_BINNING = {
            0.000, 0.004, 0.008, 0.012, 0.016, 0.020, 0.024, 0.029, 0.034,
            0.039, 0.045, 0.052, 0.057, 0.064, 0.072, 0.081, 0.091, 0.102,
            0.114, 0.128, 0.145, 0.165, 0.189, 0.219, 0.258, 0.312, 0.391,
            0.524, 0.695, 0.918, 1.153, 1.496, 1.947, 2.522, 3.277, 10.,
        };

        // Set up histograms
        // z0_mass_coarse_
        const std::string z0_mass_all_name = "Z0 Mass: All";
        const std::string z0_mass_all_file = "z_mass_all";
        z0_mass_all_ = tdir.make<TH1D>(z0_mass_all_file.c_str(), z0_mass_all_name.c_str(), 300, 0., 300.);
        z0_mass_all_->GetXaxis()->SetTitle("m_{ee} [GeV]");
        z0_mass_all_->GetYaxis()->SetTitle("Counts / GeV");

        // z0_mass_coarse_
        const std::string z0_mass_coarse_name = "Z0 Mass: Coarse";
        const std::string z0_mass_coarse_file = "z_mass_coarse";
        z0_mass_coarse_ = tdir.make<TH1D>(z0_mass_coarse_file.c_str(), z0_mass_coarse_name.c_str(), 100, 50., 150.);
        z0_mass_coarse_->GetXaxis()->SetTitle("m_{ee} [GeV]");
        z0_mass_coarse_->GetYaxis()->SetTitle("Counts / GeV");

        // z0_mass_fine_
        const std::string z0_mass_fine_name = "Z0 Mass: Fine";
        const std::string z0_mass_fine_file = "z_mass_fine";
        z0_mass_fine_ = tdir.make<TH1D>(z0_mass_fine_file.c_str(), z0_mass_fine_name.c_str(), 80, 80., 100.);
        z0_mass_fine_->GetXaxis()->SetTitle("m_{ee} [GeV]");
        z0_mass_fine_->GetYaxis()->SetTitle("Counts / 0.25 GeV");

        // z0_rapidity_
        const std::string z0_rapidity_name = "Z0 Rapidity";
        const std::string z0_rapidity_file = "z_rapidity";
        z0_rapidity_ = tdir.make<TH1D>(z0_rapidity_file.c_str(), z0_rapidity_name.c_str(), 100, -5., 5.);
        z0_rapidity_->GetXaxis()->SetTitle("Z_{Y}");
        z0_rapidity_->GetYaxis()->SetTitle("Counts");

        // z0_pt (dressed)
        const std::string z0_pt_name = "Z0 p_{T}";
        const std::string z0_pt_file = "z_pt";
        z0_pt_ = tdir.make<TH1D>(z0_pt_file.c_str(), z0_pt_name.c_str(), 200, 0., 200.);
        z0_pt_->GetXaxis()->SetTitle("p_{T,Z}");
        z0_pt_->GetYaxis()->SetTitle("Counts / GeV");

        // e0_pt_ (dressed)
        const std::string e0_pt_name = "p_{T,e_{0}}";
        const std::string e0_pt_file = "e0_pt";
        e0_pt_ = tdir.make<TH1D>(e0_pt_file.c_str(), e0_pt_name.c_str(), 200, 0., 200.);
        e0_pt_->GetXaxis()->SetTitle("p_{T,e_{0}}");
        e0_pt_->GetYaxis()->SetTitle("Counts / GeV");

        // e0_pt_naked_
        const std::string e0n_pt_name = "p_{T,e_{0},Naked}";
        const std::string e0n_pt_file = "e0_pt_naked";
        e0_pt_naked_ = tdir.make<TH1D>(e0n_pt_file.c_str(), e0_pt_name.c_str(), 200, 0., 200.);
        e0_pt_naked_->GetXaxis()->SetTitle("Naked p_{T,e_{0}}");
        e0_pt_naked_->GetYaxis()->SetTitle("Counts / GeV");

        // e0_pt_born_
        const std::string e0b_pt_name = "p_{T,e_{0},Born}";
        const std::string e0b_pt_file = "e0_pt_born";
        e0_pt_born_ = tdir.make<TH1D>(e0b_pt_file.c_str(), e0_pt_name.c_str(), 200, 0., 200.);
        e0_pt_born_->GetXaxis()->SetTitle("Born p_{T,e_{0}}");
        e0_pt_born_->GetYaxis()->SetTitle("Counts / GeV");

        // e1_pt_ (dressed)
        const std::string e1_pt_name = "p_{T,e_{1}}";
        const std::string e1_pt_file = "e1_pt";
        e1_pt_ = tdir.make<TH1D>(e1_pt_file.c_str(), e1_pt_name.c_str(), 200, 0., 200.);
        e1_pt_->GetXaxis()->SetTitle("p_{T,e_{1}}");
        e1_pt_->GetYaxis()->SetTitle("Counts / GeV");

        // e1_pt_naked_
        const std::string e1n_pt_name = "p_{T,e_{1},Naked}";
        const std::string e1n_pt_file = "e1_pt_naked";
        e1_pt_naked_ = tdir.make<TH1D>(e1n_pt_file.c_str(), e1_pt_name.c_str(), 200, 0., 200.);
        e1_pt_naked_->GetXaxis()->SetTitle("Naked p_{T,e_{1}}");
        e1_pt_naked_->GetYaxis()->SetTitle("Counts / GeV");

        // e1_pt_born_
        const std::string e1b_pt_name = "p_{T,e_{1},Born}";
        const std::string e1b_pt_file = "e1_pt_born";
        e1_pt_born_ = tdir.make<TH1D>(e1b_pt_file.c_str(), e1_pt_name.c_str(), 200, 0., 200.);
        e1_pt_born_->GetXaxis()->SetTitle("Born p_{T,e_{1}}");
        e1_pt_born_->GetYaxis()->SetTitle("Counts / GeV");

        // e0_eta_ (dressed)
        const std::string e0_eta_name = "#eta_{e_{0}}";
        const std::string e0_eta_file = "e0_eta";
        e0_eta_ = tdir.make<TH1D>(e0_eta_file.c_str(), e0_eta_name.c_str(), 50, -5., 5.);
        e0_eta_->GetXaxis()->SetTitle("#eta_{e_{0}}");
        e0_eta_->GetYaxis()->SetTitle("Counts");

        // e0_eta_naked_
        const std::string e0n_eta_name = "#eta_{e_{0},Naked}";
        const std::string e0n_eta_file = "e0_eta_naked";
        e0_eta_naked_ = tdir.make<TH1D>(e0n_eta_file.c_str(), e0_eta_name.c_str(), 50, -5., 5.);
        e0_eta_naked_->GetXaxis()->SetTitle("Naked #eta_{e_{0}}");
        e0_eta_naked_->GetYaxis()->SetTitle("Counts");

        // e0_eta_born_
        const std::string e0b_eta_name = "#eta_{e_{0},Born}";
        const std::string e0b_eta_file = "e0_eta_born";
        e0_eta_born_ = tdir.make<TH1D>(e0b_eta_file.c_str(), e0_eta_name.c_str(), 50, -5., 5.);
        e0_eta_born_->GetXaxis()->SetTitle("Born #eta_{e_{0}}");
        e0_eta_born_->GetYaxis()->SetTitle("Counts");

        // e1_eta_ (dressed)
        const std::string e1_eta_name = "#eta_{e_{1}}";
        const std::string e1_eta_file = "e1_eta";
        e1_eta_ = tdir.make<TH1D>(e1_eta_file.c_str(), e1_eta_name.c_str(), 50, -5., 5.);
        e1_eta_->GetXaxis()->SetTitle("#eta_{e_{1}}");
        e1_eta_->GetYaxis()->SetTitle("Counts");

        // e1_eta_naked_
        const std::string e1n_eta_name = "#eta_{e_{1},Naked}";
        const std::string e1n_eta_file = "e1_eta_naked";
        e1_eta_naked_ = tdir.make<TH1D>(e1n_eta_file.c_str(), e1_eta_name.c_str(), 50, -5., 5.);
        e1_eta_naked_->GetXaxis()->SetTitle("Naked #eta_{e_{1}}");
        e1_eta_naked_->GetYaxis()->SetTitle("Counts");

        // e1_eta_born
        const std::string e1b_eta_name = "#eta_{e_{1},Born}";
        const std::string e1b_eta_file = "e1_eta_born";
        e1_eta_born_ = tdir.make<TH1D>(e1b_eta_file.c_str(), e1_eta_name.c_str(), 50, -5., 5.);
        e1_eta_born_->GetXaxis()->SetTitle("Born #eta_{e_{1}}");
        e1_eta_born_->GetYaxis()->SetTitle("Counts");

        // e0_phi (dressed)
        const std::string e0_phi_name = "#phi_{e_{0}}";
        const std::string e0_phi_file = "e0_phi";
        e0_phi_ = tdir.make<TH1D>(e0_phi_file.c_str(), e0_phi_name.c_str(), 63, -3.15, 3.15);
        e0_phi_->GetXaxis()->SetTitle("#phi_{e_{0}}");
        e0_phi_->GetYaxis()->SetTitle("Counts");

        // e0_phi_naked
        const std::string e0n_phi_name = "#phi_{e_{0},Naked}";
        const std::string e0n_phi_file = "e0_phi_naked";
        e0_phi_naked_ = tdir.make<TH1D>(e0n_phi_file.c_str(), e0_phi_name.c_str(), 63, -3.15, 3.15);
        e0_phi_naked_->GetXaxis()->SetTitle("Naked #phi_{e_{0}}");
        e0_phi_naked_->GetYaxis()->SetTitle("Counts");

        // e0_phi_born
        const std::string e0b_phi_name = "#phi_{e_{0},Born}";
        const std::string e0b_phi_file = "e0_phi_born";
        e0_phi_born_ = tdir.make<TH1D>(e0b_phi_file.c_str(), e0_phi_name.c_str(), 63, -3.15, 3.15);
        e0_phi_born_->GetXaxis()->SetTitle("Born #phi_{e_{0}}");
        e0_phi_born_->GetYaxis()->SetTitle("Counts");

        // e1_phi (dressed)
        const std::string e1_phi_name = "#phi_{e_{1}}";
        const std::string e1_phi_file = "e1_phi";
        e1_phi_ = tdir.make<TH1D>(e1_phi_file.c_str(), e1_phi_name.c_str(), 63, -3.15, 3.15);
        e1_phi_->GetXaxis()->SetTitle("#phi_{e_{1}}");
        e1_phi_->GetYaxis()->SetTitle("counts");

        // e1_phi_naked
        const std::string e1n_phi_name = "#phi_{e_{1},Naked}";
        const std::string e1n_phi_file = "e1_phi_naked";
        e1_phi_naked_ = tdir.make<TH1D>(e1n_phi_file.c_str(), e1_phi_name.c_str(), 63, -3.15, 3.15);
        e1_phi_naked_->GetXaxis()->SetTitle("Naked #phi_{e_{1}}");
        e1_phi_naked_->GetYaxis()->SetTitle("counts");

        // e1_phi_born
        const std::string e1b_phi_name = "#phi_{e_{1},Born}";
        const std::string e1b_phi_file = "e1_phi_born";
        e1_phi_born_ = tdir.make<TH1D>(e1b_phi_file.c_str(), e1_phi_name.c_str(), 63, -3.15, 3.15);
        e1_phi_born_->GetXaxis()->SetTitle("Born #phi_{e_{1}}");
        e1_phi_born_->GetYaxis()->SetTitle("counts");

        // e0_charge
        const std::string e0_charge_name = "charge_{e_{0}}";
        const std::string e0_charge_file = "e0_charge";
        e0_charge_ = tdir.make<TH1D>(e0_charge_file.c_str(), e0_charge_name.c_str(), 3, -1.5, 1.5);
        e0_charge_->GetXaxis()->SetTitle("charge_{e_{0}}");
        e0_charge_->GetYaxis()->SetTitle("Counts");

        // e1_charge
        const std::string e1_charge_name = "charge_{e_{1}}";
        const std::string e1_charge_file = "e1_charge";
        e1_charge_ = tdir.make<TH1D>(e1_charge_file.c_str(), e1_charge_name.c_str(), 3, -1.5, 1.5);
        e1_charge_->GetXaxis()->SetTitle("charge_{e_{1}}");
        e1_charge_->GetYaxis()->SetTitle("counts");

        // e0_r9
        const std::string e0_r9_name = "r9_{e_{0}}";
        const std::string e0_r9_file = "e0_r9";
        e0_r9_ = tdir.make<TH1D>(e0_r9_file.c_str(), e0_r9_name.c_str(), 100, 0., 1.);
        e0_r9_->GetXaxis()->SetTitle("r9_{e_{0}}");
        e0_r9_->GetYaxis()->SetTitle("Counts");

        // e1_r9
        const std::string e1_r9_name = "r9_{e_{1}}";
        const std::string e1_r9_file = "e1_r9";
        e1_r9_ = tdir.make<TH1D>(e1_r9_file.c_str(), e1_r9_name.c_str(), 100, 0., 1.);
        e1_r9_->GetXaxis()->SetTitle("r9_{e_{1}}");
        e1_r9_->GetYaxis()->SetTitle("counts");

        // e0_sigma_ieta_ieta
        const std::string e0_sigma_ieta_ieta_name = "sigma_{i #eta i #eta}^{e_{0}}";
        const std::string e0_sigma_ieta_ieta_file = "e0_siesie";
        e0_sigma_ieta_ieta_ = tdir.make<TH1D>(e0_sigma_ieta_ieta_file.c_str(), e0_sigma_ieta_ieta_name.c_str(), 300, 0., 0.3);
        e0_sigma_ieta_ieta_->GetXaxis()->SetTitle("sigma_{i #eta i #eta}^{e_{0}}");
        e0_sigma_ieta_ieta_->GetYaxis()->SetTitle("Counts");

        // e1_sigma_ieta_ieta
        const std::string e1_sigma_ieta_ieta_name = "sigma_{i #eta i #eta}^{e_{1}}";
        const std::string e1_sigma_ieta_ieta_file = "e1_siesie";
        e1_sigma_ieta_ieta_ = tdir.make<TH1D>(e1_sigma_ieta_ieta_file.c_str(), e1_sigma_ieta_ieta_name.c_str(), 300, 0., 0.3);
        e1_sigma_ieta_ieta_->GetXaxis()->SetTitle("sigma_{i #eta i #eta}^{e_{1}}");
        e1_sigma_ieta_ieta_->GetYaxis()->SetTitle("Counts");

        // e0_h_over_e
        const std::string e0_h_over_e_name = "(H/E)_{e_{0}}";
        const std::string e0_h_over_e_file = "e0_he";
        e0_h_over_e_ = tdir.make<TH1D>(e0_h_over_e_file.c_str(), e0_h_over_e_name.c_str(), 300, 0., 0.3);
        e0_h_over_e_->GetXaxis()->SetTitle("(H/E)_{e_{0}}");
        e0_h_over_e_->GetYaxis()->SetTitle("Counts");

        // e1_h_over_e
        const std::string e1_h_over_e_name = "(H/E)_{e_{1}}";
        const std::string e1_h_over_e_file = "e1_he";
        e1_h_over_e_ = tdir.make<TH1D>(e1_h_over_e_file.c_str(), e1_h_over_e_name.c_str(), 300, 0., 0.3);
        e1_h_over_e_->GetXaxis()->SetTitle("(H/E)_{e_{1}}");
        e1_h_over_e_->GetYaxis()->SetTitle("Counts");

        // e0_deta_in
        const std::string e0_deta_in_name = "d#eta_{e_{0}}";
        const std::string e0_deta_in_file = "e0_deta";
        e0_deta_in_ = tdir.make<TH1D>(e0_deta_in_file.c_str(), e0_deta_in_name.c_str(), 1000, 0., 0.1);
        e0_deta_in_->GetXaxis()->SetTitle("d#eta_{e_{0}}");
        e0_deta_in_->GetYaxis()->SetTitle("Counts");

        // e1_deta_in
        const std::string e1_deta_in_name = "d#eta_{e_{1}}";
        const std::string e1_deta_in_file = "e1_deta";
        e1_deta_in_ = tdir.make<TH1D>(e1_deta_in_file.c_str(), e1_deta_in_name.c_str(), 1000, 0., 0.1);
        e1_deta_in_->GetXaxis()->SetTitle("d#eta_{e_{1}}");
        e1_deta_in_->GetYaxis()->SetTitle("Counts");

        // e0_dphi_in
        const std::string e0_dphi_in_name = "d#phi_{e_{0}}";
        const std::string e0_dphi_in_file = "e0_dphi";
        e0_dphi_in_ = tdir.make<TH1D>(e0_dphi_in_file.c_str(), e0_dphi_in_name.c_str(), 1000, 0., 0.1);
        e0_dphi_in_->GetXaxis()->SetTitle("d#phi_{e_{0}}");
        e0_dphi_in_->GetYaxis()->SetTitle("Counts");

        // e1_dphi_in
        const std::string e1_dphi_in_name = "d#phi_{e_{1}}";
        const std::string e1_dphi_in_file = "e1_dphi";
        e1_dphi_in_ = tdir.make<TH1D>(e1_dphi_in_file.c_str(), e1_dphi_in_name.c_str(), 1000, 0., 0.1);
        e1_dphi_in_->GetXaxis()->SetTitle("d#phi_{e_{1}}");
        e1_dphi_in_->GetYaxis()->SetTitle("Counts");

        // e0_track_iso_in
        const std::string e0_track_isoname = "Track ISO_{e_{0}}";
        const std::string e0_track_isofile = "e0_track_iso";
        e0_track_iso_ = tdir.make<TH1D>(e0_track_isofile.c_str(), e0_track_isoname.c_str(), 300, 0., 3.);
        e0_track_iso_->GetXaxis()->SetTitle("Track ISO_{e_{0}}");
        e0_track_iso_->GetYaxis()->SetTitle("Counts");

        // e1_track_iso_in
        const std::string e1_track_isoname = "Track ISO_{e_{1}}";
        const std::string e1_track_isofile = "e1_track_iso";
        e1_track_iso_ = tdir.make<TH1D>(e1_track_isofile.c_str(), e1_track_isoname.c_str(), 300, 0., 3.);
        e1_track_iso_->GetXaxis()->SetTitle("Track ISO_{e_{1}}");
        e1_track_iso_->GetYaxis()->SetTitle("Counts");

        // e0_ecal_iso_in
        const std::string e0_ecal_isoname = "ECAL ISO_{e_{0}}";
        const std::string e0_ecal_isofile = "e0_ecal_iso";
        e0_ecal_iso_ = tdir.make<TH1D>(e0_ecal_isofile.c_str(), e0_ecal_isoname.c_str(), 300, 0., 3.);
        e0_ecal_iso_->GetXaxis()->SetTitle("ECAL ISO_{e_{0}}");
        e0_ecal_iso_->GetYaxis()->SetTitle("Counts");

        // e1_ecal_iso_in
        const std::string e1_ecal_isoname = "ECAL ISO_{e_{1}}";
        const std::string e1_ecal_isofile = "e1_ecal_iso";
        e1_ecal_iso_ = tdir.make<TH1D>(e1_ecal_isofile.c_str(), e1_ecal_isoname.c_str(), 300, 0., 3.);
        e1_ecal_iso_->GetXaxis()->SetTitle("ECAL ISO_{e_{1}}");
        e1_ecal_iso_->GetYaxis()->SetTitle("Counts");

        // e0_hcal_iso_in
        const std::string e0_hcal_isoname = "HCAL ISO_{e_{0}}";
        const std::string e0_hcal_isofile = "e0_hcal_iso";
        e0_hcal_iso_ = tdir.make<TH1D>(e0_hcal_isofile.c_str(), e0_hcal_isoname.c_str(), 300, 0., 3.);
        e0_hcal_iso_->GetXaxis()->SetTitle("HCAL ISO_{e_{0}}");
        e0_hcal_iso_->GetYaxis()->SetTitle("Counts");

        // e1_hcal_iso_in
        const std::string e1_hcal_isoname = "HCAL ISO_{e_{1}}";
        const std::string e1_hcal_isofile = "e1_hcal_iso";
        e1_hcal_iso_ = tdir.make<TH1D>(e1_hcal_isofile.c_str(), e1_hcal_isoname.c_str(), 300, 0., 1.);
        e1_hcal_iso_->GetXaxis()->SetTitle("HCAL ISO_{e_{1}}");
        e1_hcal_iso_->GetYaxis()->SetTitle("Counts");

        // e0_one_over_e_mins_one_over_p
        const std::string e0_one_over_e_mins_one_over_pname = "1/E - 1/P e_{0}";
        const std::string e0_one_over_e_mins_one_over_pfile = "e0_1oe_1op";
        e0_one_over_e_mins_one_over_p_ = tdir.make<TH1D>(e0_one_over_e_mins_one_over_pfile.c_str(), e0_one_over_e_mins_one_over_pname.c_str(), 1000, -1., 1.);
        e0_one_over_e_mins_one_over_p_->GetXaxis()->SetTitle("1/E - 1/P e_{0}");
        e0_one_over_e_mins_one_over_p_->GetYaxis()->SetTitle("Counts");

        // e1_one_over_e_mins_one_over_p
        const std::string e1_one_over_e_mins_one_over_pname = "1/E - 1/P e_{1}";
        const std::string e1_one_over_e_mins_one_over_pfile = "e1_1oe_1op";
        e1_one_over_e_mins_one_over_p_ = tdir.make<TH1D>(e1_one_over_e_mins_one_over_pfile.c_str(), e1_one_over_e_mins_one_over_pname.c_str(), 1000, -1., 1.);
        e1_one_over_e_mins_one_over_p_->GetXaxis()->SetTitle("1/E - 1/P e_{1}");
        e1_one_over_e_mins_one_over_p_->GetYaxis()->SetTitle("Counts");

        // phistar (dressed)
        const std::string phistar_name = "#phi*";
        const std::string phistar_file = "phistar";
        phistar_ = tdir.make<TH1D>(phistar_file.c_str(), phistar_name.c_str(), ATLAS_PHISTAR_BINNING.size() - 1, &ATLAS_PHISTAR_BINNING[0]);
        phistar_->GetXaxis()->SetTitle("#phi*");
        phistar_->GetYaxis()->SetTitle("Counts");

        // phistar born
        const std::string phistar_name_born = "Born #phi*";
        const std::string phistar_name_file = "phistar_born";
        phistar_born_ = tdir.make<TH1D>(phistar_name_file.c_str(), phistar_name_born.c_str(), ATLAS_PHISTAR_BINNING.size() - 1, &ATLAS_PHISTAR_BINNING[0]);
        phistar_born_->GetXaxis()->SetTitle("Born #phi*");
        phistar_born_->GetYaxis()->SetTitle("Counts");

        // phistar naked
        const std::string phistar_name_naked = "Naked #phi*";
        const std::string phistar_name_nfile = "phistar_naked";
        phistar_naked_ = tdir.make<TH1D>(phistar_name_nfile.c_str(), phistar_name_naked.c_str(), ATLAS_PHISTAR_BINNING.size() - 1, &ATLAS_PHISTAR_BINNING[0]);
        phistar_naked_->GetXaxis()->SetTitle("Naked #phi*");
        phistar_naked_->GetYaxis()->SetTitle("Counts");

        // phistar supercluster
        const std::string phistar_name_supercluster = "Supercluster #phi*";
        const std::string phistar_name_superclufile = "phistar_sc";
        phistar_supercluster_ = tdir.make<TH1D>(phistar_name_superclufile.c_str(), phistar_name_supercluster.c_str(), ATLAS_PHISTAR_BINNING.size() - 1, &ATLAS_PHISTAR_BINNING[0]);
        phistar_supercluster_->GetXaxis()->SetTitle("Supercluster #phi*");
        phistar_supercluster_->GetYaxis()->SetTitle("Counts");

        // other_phistar for gen-reco efficiencies
        const std::string other_phistar_name = "Other #phi*";
        const std::string other_phistar_file = "phistart_other";
        other_phistar_ = tdir.make<TH1D>(other_phistar_file.c_str(), other_phistar_name.c_str(), ATLAS_PHISTAR_BINNING.size() - 1, &ATLAS_PHISTAR_BINNING[0]);
        other_phistar_->GetXaxis()->SetTitle("#phi*_{other}");
        other_phistar_->GetYaxis()->SetTitle("Counts");

        // other_y for gen-reco efficiencies
        const std::string other_y_name = "Other Rapidity";
        const std::string other_y_file = "z_rapidity_other";
        other_y_ = tdir.make<TH1D>(other_y_file.c_str(), other_y_name.c_str(), 100, -5., 5.);
        other_y_->GetXaxis()->SetTitle("Z_{Y, other}");
        other_y_->GetYaxis()->SetTitle("Counts");

        // pileup
        const std::string pileup_name = "N_{Vertices}";
        const std::string pileup_file = "n_verts";
        pileup_ = tdir.make<TH1D>(pileup_file.c_str(), pileup_name.c_str(), 100, 0., 100.);
        pileup_->GetXaxis()->SetTitle("Number of Vertices");
        pileup_->GetYaxis()->SetTitle("Counts");

        // nelectrons
        const std::string nelectrons_name = "N_{e}";
        const std::string nelectrons_file = "n_electrons";
        nelectrons_ = tdir.make<TH1D>(nelectrons_file.c_str(), nelectrons_name.c_str(), 10, 0., 10.);
        nelectrons_->GetXaxis()->SetTitle("N_{e}");
        nelectrons_->GetYaxis()->SetTitle("Events");

        // baseweights
        const std::string baseweights_name = "Base Weight";
        const std::string baseweights_file = "base_weight";
        baseweights_ = tdir.make<TH1D>(baseweights_file.c_str(), baseweights_name.c_str(), 500, 0., 5.);
        baseweights_->GetXaxis()->SetTitle("Weight");
        baseweights_->GetYaxis()->SetTitle("Events");

        // fullweights
        const std::string fullweights_name = "Full Weight";
        const std::string fullweights_file = "full_weight";
        fullweights_ = tdir.make<TH1D>(fullweights_file.c_str(), fullweights_name.c_str(), 500, 0., 5.);
        fullweights_->GetXaxis()->SetTitle("Weight");
        fullweights_->GetYaxis()->SetTitle("Events");

        // e0_pt_vs_trig
        const std::string e0_pt_vs_trig_name = "p_{T,e_{0}} Vs. Trigger";
        const std::string e0_pt_vs_trig_file = "e0_pt_vs_trigger";
        e0_pt_vs_trig_ = tdir.make<TH1D>(e0_pt_vs_trig_file.c_str(), e0_pt_vs_trig_name.c_str(), 200, 0., 2.);
        e0_pt_vs_trig_->GetXaxis()->SetTitle("Ratio of p_{T,e_{0}} Reco / Trigger");
        e0_pt_vs_trig_->GetYaxis()->SetTitle("Events");

        // e1_pt_vs_trig
        const std::string e1_pt_vs_trig_name = "p_{T,e_{1}} Vs. Trigger";
        const std::string e1_pt_vs_trig_file = "e1_pt_vs_trigger";
        e1_pt_vs_trig_ = tdir.make<TH1D>(e1_pt_vs_trig_file.c_str(), e1_pt_vs_trig_name.c_str(), 200, 0., 2.);
        e1_pt_vs_trig_->GetXaxis()->SetTitle("Ratio of p_{T,e_{1}} Reco / Trigger");
        e1_pt_vs_trig_->GetYaxis()->SetTitle("Events");

        // phistar
        const std::string phistar_vs_truth_name = "#phi*: Reco Vs. Truth";
        const std::string phistar_vs_truth_file = "phistar_reco_vs_truth";
        phistar_vs_truth_ = tdir.make<TH1D>(phistar_vs_truth_file.c_str(), phistar_vs_truth_name.c_str(), 200, 0., 2.);
        phistar_vs_truth_->GetXaxis()->SetTitle("#phi* Reco MC / Truth");
        phistar_vs_truth_->GetYaxis()->SetTitle("Events");

        //deltaR
        const std::string deltaR_name = "#DeltaR";
        const std::string deltaR_file = "delta_r";
        deltaR_ = tdir.make<TH1D>(deltaR_file.c_str(), deltaR_name.c_str(), 100, 0., 10.);
        deltaR_->GetXaxis()->SetTitle("#DeltaR(e_{0},e_{1})");
        deltaR_->GetYaxis()->SetTitle("Counts");
    }

    void ZFinderPlotter::Fill(
            const ZFinderEvent& ZF_EVENT,
            const int ELECTRON_0,
            const int ELECTRON_1,
            const double EVENT_WEIGHT
            ) {
        /*
         * Given a zf_event, fills all the histograms.
         *
         * ELECTRON_0 and ELECTRON_1 can be used to assign zf_event.eN to the given
         * number in the histogram. For example, assigning ELECTRON_0 = 1 will fill
         * the e0 histograms with data from zf_event.e1.
         */
        // Z Info
        if (!USE_MC_) {
            z0_mass_all_->Fill(ZF_EVENT.reco_z.m, EVENT_WEIGHT);
            z0_mass_coarse_->Fill(ZF_EVENT.reco_z.m, EVENT_WEIGHT);
            z0_mass_fine_->Fill(ZF_EVENT.reco_z.m, EVENT_WEIGHT);
            z0_rapidity_->Fill(ZF_EVENT.reco_z.y, EVENT_WEIGHT);
            z0_pt_->Fill(ZF_EVENT.reco_z.pt, EVENT_WEIGHT);
            phistar_->Fill(ZF_EVENT.reco_z.phistar, EVENT_WEIGHT);
            deltaR_->Fill(ZF_EVENT.reco_z.deltaR, EVENT_WEIGHT);
            phistar_supercluster_->Fill(ZF_EVENT.reco_z.scPhistar, EVENT_WEIGHT);
            // We only want to plot this if corresponding gen info exists
            if(!ZF_EVENT.is_real_data) {
                other_phistar_->Fill(ZF_EVENT.reco_z.other_phistar, EVENT_WEIGHT);
                other_y_->Fill(ZF_EVENT.reco_z.other_y, EVENT_WEIGHT);
                phistar_born_->Fill(ZF_EVENT.reco_z.bornPhistar, EVENT_WEIGHT);
                phistar_naked_->Fill(ZF_EVENT.reco_z.nakedPhistar, EVENT_WEIGHT);
            }

            // Fill the histograms with the information from the approriate electron
            if (ELECTRON_0 == 0 && ELECTRON_1 == 1) {
                if (ZF_EVENT.e0 != nullptr) {
                    e0_pt_->Fill(ZF_EVENT.e0->pt(), EVENT_WEIGHT);
                    e0_eta_->Fill(ZF_EVENT.e0->eta(), EVENT_WEIGHT);
                    e0_phi_->Fill(ZF_EVENT.e0->phi(), EVENT_WEIGHT);
                    e0_charge_->Fill(ZF_EVENT.e0->charge(), EVENT_WEIGHT);
                    e0_r9_->Fill(ZF_EVENT.e0->r9(), EVENT_WEIGHT);
                    e0_sigma_ieta_ieta_->Fill(ZF_EVENT.e0->sigma_ieta_ieta(), EVENT_WEIGHT);
                    e0_h_over_e_->Fill(ZF_EVENT.e0->h_over_e(), EVENT_WEIGHT);
                    e0_deta_in_->Fill(ZF_EVENT.e0->deta_in(), EVENT_WEIGHT);
                    e0_dphi_in_->Fill(ZF_EVENT.e0->dphi_in(), EVENT_WEIGHT);
                    e0_track_iso_->Fill(ZF_EVENT.e0->track_iso(), EVENT_WEIGHT);
                    e0_ecal_iso_->Fill(ZF_EVENT.e0->ecal_iso(), EVENT_WEIGHT);
                    e0_hcal_iso_->Fill(ZF_EVENT.e0->hcal_iso(), EVENT_WEIGHT);
                    e0_one_over_e_mins_one_over_p_->Fill(ZF_EVENT.e0->one_over_e_mins_one_over_p(), EVENT_WEIGHT);
                }
                if (ZF_EVENT.e1 != nullptr) {
                    e1_pt_->Fill(ZF_EVENT.e1->pt(), EVENT_WEIGHT);
                    e1_eta_->Fill(ZF_EVENT.e1->eta(), EVENT_WEIGHT);
                    e1_phi_->Fill(ZF_EVENT.e1->phi(), EVENT_WEIGHT);
                    e1_charge_->Fill(ZF_EVENT.e1->charge(), EVENT_WEIGHT);
                    e1_r9_->Fill(ZF_EVENT.e1->r9(), EVENT_WEIGHT);
                    e1_sigma_ieta_ieta_->Fill(ZF_EVENT.e1->sigma_ieta_ieta(), EVENT_WEIGHT);
                    e1_h_over_e_->Fill(ZF_EVENT.e1->h_over_e(), EVENT_WEIGHT);
                    e1_deta_in_->Fill(ZF_EVENT.e1->deta_in(), EVENT_WEIGHT);
                    e1_dphi_in_->Fill(ZF_EVENT.e1->dphi_in(), EVENT_WEIGHT);
                    e1_track_iso_->Fill(ZF_EVENT.e1->track_iso(), EVENT_WEIGHT);
                    e1_ecal_iso_->Fill(ZF_EVENT.e1->ecal_iso(), EVENT_WEIGHT);
                    e1_hcal_iso_->Fill(ZF_EVENT.e1->hcal_iso(), EVENT_WEIGHT);
                    e1_one_over_e_mins_one_over_p_->Fill(ZF_EVENT.e1->one_over_e_mins_one_over_p(), EVENT_WEIGHT);
                }
                if (ZF_EVENT.e0_trig != nullptr && ZF_EVENT.e0 != nullptr) {
                    e0_pt_vs_trig_->Fill(
                            ZF_EVENT.e0->pt() / ZF_EVENT.e0_trig->pt(),
                            EVENT_WEIGHT
                            );
                }
                if (ZF_EVENT.e1_trig != nullptr && ZF_EVENT.e1 != nullptr) {
                    e1_pt_vs_trig_->Fill(
                            ZF_EVENT.e1->pt() / ZF_EVENT.e1_trig->pt(),
                            EVENT_WEIGHT
                            );
                }
            }
            else if (ELECTRON_0 == 1 && ELECTRON_1 == 0) {
                if (ZF_EVENT.e1 != nullptr) {
                    e0_pt_->Fill(ZF_EVENT.e1->pt(), EVENT_WEIGHT);
                    e0_eta_->Fill(ZF_EVENT.e1->eta(), EVENT_WEIGHT);
                    e0_phi_->Fill(ZF_EVENT.e1->phi(), EVENT_WEIGHT);
                    e0_charge_->Fill(ZF_EVENT.e1->charge(), EVENT_WEIGHT);
                    e0_r9_->Fill(ZF_EVENT.e1->r9(), EVENT_WEIGHT);
                    e0_sigma_ieta_ieta_->Fill(ZF_EVENT.e1->sigma_ieta_ieta(), EVENT_WEIGHT);
                    e0_h_over_e_->Fill(ZF_EVENT.e1->h_over_e(), EVENT_WEIGHT);
                    e0_deta_in_->Fill(ZF_EVENT.e1->deta_in(), EVENT_WEIGHT);
                    e0_dphi_in_->Fill(ZF_EVENT.e1->dphi_in(), EVENT_WEIGHT);
                    e0_track_iso_->Fill(ZF_EVENT.e1->track_iso(), EVENT_WEIGHT);
                    e0_ecal_iso_->Fill(ZF_EVENT.e1->ecal_iso(), EVENT_WEIGHT);
                    e0_hcal_iso_->Fill(ZF_EVENT.e1->hcal_iso(), EVENT_WEIGHT);
                    e0_one_over_e_mins_one_over_p_->Fill(ZF_EVENT.e1->one_over_e_mins_one_over_p(), EVENT_WEIGHT);
                }
                if (ZF_EVENT.e0 != nullptr) {
                    e1_pt_->Fill(ZF_EVENT.e0->pt(), EVENT_WEIGHT);
                    e1_eta_->Fill(ZF_EVENT.e0->eta(), EVENT_WEIGHT);
                    e1_phi_->Fill(ZF_EVENT.e0->phi(), EVENT_WEIGHT);
                    e1_charge_->Fill(ZF_EVENT.e0->charge(), EVENT_WEIGHT);
                    e1_r9_->Fill(ZF_EVENT.e0->r9(), EVENT_WEIGHT);
                    e1_sigma_ieta_ieta_->Fill(ZF_EVENT.e0->sigma_ieta_ieta(), EVENT_WEIGHT);
                    e1_h_over_e_->Fill(ZF_EVENT.e0->h_over_e(), EVENT_WEIGHT);
                    e1_deta_in_->Fill(ZF_EVENT.e0->deta_in(), EVENT_WEIGHT);
                    e1_dphi_in_->Fill(ZF_EVENT.e0->dphi_in(), EVENT_WEIGHT);
                    e1_track_iso_->Fill(ZF_EVENT.e0->track_iso(), EVENT_WEIGHT);
                    e1_ecal_iso_->Fill(ZF_EVENT.e0->ecal_iso(), EVENT_WEIGHT);
                    e1_hcal_iso_->Fill(ZF_EVENT.e0->hcal_iso(), EVENT_WEIGHT);
                    e1_one_over_e_mins_one_over_p_->Fill(ZF_EVENT.e0->one_over_e_mins_one_over_p(), EVENT_WEIGHT);
                }
                if (ZF_EVENT.e1_trig != nullptr && ZF_EVENT.e1 != nullptr) {
                    e0_pt_vs_trig_->Fill(
                            ZF_EVENT.e1->pt() / ZF_EVENT.e1_trig->pt(),
                            EVENT_WEIGHT
                            );
                }
                if (ZF_EVENT.e0_trig != nullptr && ZF_EVENT.e0 != nullptr) {
                    e1_pt_vs_trig_->Fill(
                            ZF_EVENT.e0->pt() / ZF_EVENT.e0_trig->pt(),
                            EVENT_WEIGHT
                            );
                }
            }
            // Event Info
            pileup_->Fill(ZF_EVENT.reco_vert.num, EVENT_WEIGHT);
            nelectrons_->Fill(ZF_EVENT.n_reco_electrons, EVENT_WEIGHT);
        }
        else if (USE_MC_ && !ZF_EVENT.is_real_data) {
            z0_mass_all_->Fill(ZF_EVENT.truth_z.m, EVENT_WEIGHT);
            z0_mass_coarse_->Fill(ZF_EVENT.truth_z.m, EVENT_WEIGHT);
            z0_mass_fine_->Fill(ZF_EVENT.truth_z.m, EVENT_WEIGHT);
            z0_rapidity_->Fill(ZF_EVENT.truth_z.y, EVENT_WEIGHT);
            z0_pt_->Fill(ZF_EVENT.truth_z.pt, EVENT_WEIGHT);
            phistar_->Fill(ZF_EVENT.truth_z.phistar, EVENT_WEIGHT);
            phistar_born_->Fill(ZF_EVENT.truth_z.bornPhistar, EVENT_WEIGHT);
            phistar_naked_->Fill(ZF_EVENT.truth_z.nakedPhistar, EVENT_WEIGHT);
            phistar_supercluster_->Fill(ZF_EVENT.truth_z.scPhistar, EVENT_WEIGHT);
            deltaR_->Fill(ZF_EVENT.truth_z.deltaR, EVENT_WEIGHT);
            other_phistar_->Fill(ZF_EVENT.truth_z.other_phistar, EVENT_WEIGHT);
            other_y_->Fill(ZF_EVENT.truth_z.other_y, EVENT_WEIGHT);

            // Fill the histograms with the information from the approriate electron
            if (ELECTRON_0 == 0 && ELECTRON_1 == 1) {
                if (ZF_EVENT.e0_truth != nullptr) {
                    e0_pt_->Fill(ZF_EVENT.e0_truth->pt(), EVENT_WEIGHT);
                    e0_pt_naked_->Fill(ZF_EVENT.e0_truth->nakedPt(), EVENT_WEIGHT);
                    e0_pt_born_->Fill(ZF_EVENT.e0_truth->bornPt(), EVENT_WEIGHT);
                    e0_eta_->Fill(ZF_EVENT.e0_truth->eta(), EVENT_WEIGHT);
                    e0_eta_naked_->Fill(ZF_EVENT.e0_truth->nakedEta(), EVENT_WEIGHT);
                    e0_eta_born_->Fill(ZF_EVENT.e0_truth->bornEta(), EVENT_WEIGHT);
                    e0_phi_->Fill(ZF_EVENT.e0_truth->phi(), EVENT_WEIGHT);
                    e0_phi_naked_->Fill(ZF_EVENT.e0_truth->nakedPhi(), EVENT_WEIGHT);
                    e0_phi_born_->Fill(ZF_EVENT.e0_truth->bornPhi(), EVENT_WEIGHT);
                    e0_charge_->Fill(ZF_EVENT.e0_truth->charge(), EVENT_WEIGHT);
                    e0_r9_->Fill(ZF_EVENT.e0_truth->r9(), EVENT_WEIGHT);
                    e0_sigma_ieta_ieta_->Fill(ZF_EVENT.e0_truth->sigma_ieta_ieta(), EVENT_WEIGHT);
                    e0_h_over_e_->Fill(ZF_EVENT.e0_truth->h_over_e(), EVENT_WEIGHT);
                    e0_deta_in_->Fill(ZF_EVENT.e0_truth->deta_in(), EVENT_WEIGHT);
                    e0_dphi_in_->Fill(ZF_EVENT.e0_truth->dphi_in(), EVENT_WEIGHT);
                    e0_track_iso_->Fill(ZF_EVENT.e0_truth->track_iso(), EVENT_WEIGHT);
                    e0_ecal_iso_->Fill(ZF_EVENT.e0_truth->ecal_iso(), EVENT_WEIGHT);
                    e0_hcal_iso_->Fill(ZF_EVENT.e0_truth->hcal_iso(), EVENT_WEIGHT);
                    e0_one_over_e_mins_one_over_p_->Fill(ZF_EVENT.e0_truth->one_over_e_mins_one_over_p(), EVENT_WEIGHT);
                }
                if (ZF_EVENT.e1_truth != nullptr) {
                    e1_pt_->Fill(ZF_EVENT.e1_truth->pt(), EVENT_WEIGHT);
                    e1_pt_naked_->Fill(ZF_EVENT.e1_truth->nakedPt(), EVENT_WEIGHT);
                    e1_pt_born_->Fill(ZF_EVENT.e1_truth->bornPt(), EVENT_WEIGHT);
                    e1_eta_->Fill(ZF_EVENT.e1_truth->eta(), EVENT_WEIGHT);
                    e1_eta_naked_->Fill(ZF_EVENT.e1_truth->nakedEta(), EVENT_WEIGHT);
                    e1_eta_born_->Fill(ZF_EVENT.e1_truth->bornEta(), EVENT_WEIGHT);
                    e1_phi_->Fill(ZF_EVENT.e1_truth->phi(), EVENT_WEIGHT);
                    e1_phi_naked_->Fill(ZF_EVENT.e1_truth->nakedPhi(), EVENT_WEIGHT);
                    e1_phi_born_->Fill(ZF_EVENT.e1_truth->bornPhi(), EVENT_WEIGHT);
                    e1_charge_->Fill(ZF_EVENT.e1_truth->charge(), EVENT_WEIGHT);
                    e1_r9_->Fill(ZF_EVENT.e1_truth->r9(), EVENT_WEIGHT);
                    e1_sigma_ieta_ieta_->Fill(ZF_EVENT.e1_truth->sigma_ieta_ieta(), EVENT_WEIGHT);
                    e1_h_over_e_->Fill(ZF_EVENT.e1_truth->h_over_e(), EVENT_WEIGHT);
                    e1_deta_in_->Fill(ZF_EVENT.e1_truth->deta_in(), EVENT_WEIGHT);
                    e1_dphi_in_->Fill(ZF_EVENT.e1_truth->dphi_in(), EVENT_WEIGHT);
                    e1_track_iso_->Fill(ZF_EVENT.e1_truth->track_iso(), EVENT_WEIGHT);
                    e1_ecal_iso_->Fill(ZF_EVENT.e1_truth->ecal_iso(), EVENT_WEIGHT);
                    e1_hcal_iso_->Fill(ZF_EVENT.e1_truth->hcal_iso(), EVENT_WEIGHT);
                    e1_one_over_e_mins_one_over_p_->Fill(ZF_EVENT.e1_truth->one_over_e_mins_one_over_p(), EVENT_WEIGHT);
                }
                if (ZF_EVENT.e0_trig != nullptr && ZF_EVENT.e0_truth != nullptr) {
                    e0_pt_vs_trig_->Fill(
                            ZF_EVENT.e0_truth->pt() / ZF_EVENT.e0_trig->pt(),
                            EVENT_WEIGHT
                            );
                }
                if (ZF_EVENT.e1_trig != nullptr && ZF_EVENT.e1_truth != nullptr) {
                    e1_pt_vs_trig_->Fill(
                            ZF_EVENT.e1_truth->pt() / ZF_EVENT.e1_trig->pt(),
                            EVENT_WEIGHT
                            );
                }
            }
            else if (ELECTRON_0 == 1 && ELECTRON_1 == 0) {
                if (ZF_EVENT.e1_truth != nullptr) {
                    e0_pt_->Fill(ZF_EVENT.e1_truth->pt(), EVENT_WEIGHT);
                    e0_pt_naked_->Fill(ZF_EVENT.e1_truth->nakedPt(), EVENT_WEIGHT);
                    e0_pt_born_->Fill(ZF_EVENT.e1_truth->bornPt(), EVENT_WEIGHT);
                    e0_eta_->Fill(ZF_EVENT.e1_truth->eta(), EVENT_WEIGHT);
                    e0_eta_naked_->Fill(ZF_EVENT.e1_truth->nakedEta(), EVENT_WEIGHT);
                    e0_eta_born_->Fill(ZF_EVENT.e1_truth->bornEta(), EVENT_WEIGHT);
                    e0_phi_->Fill(ZF_EVENT.e1_truth->phi(), EVENT_WEIGHT);
                    e0_phi_naked_->Fill(ZF_EVENT.e1_truth->nakedPhi(), EVENT_WEIGHT);
                    e0_phi_born_->Fill(ZF_EVENT.e1_truth->bornPhi(), EVENT_WEIGHT);
                    e0_charge_->Fill(ZF_EVENT.e1_truth->charge(), EVENT_WEIGHT);
                    e0_r9_->Fill(ZF_EVENT.e1_truth->r9(), EVENT_WEIGHT);
                    e0_sigma_ieta_ieta_->Fill(ZF_EVENT.e1_truth->sigma_ieta_ieta(), EVENT_WEIGHT);
                    e0_h_over_e_->Fill(ZF_EVENT.e1_truth->h_over_e(), EVENT_WEIGHT);
                    e0_deta_in_->Fill(ZF_EVENT.e1_truth->deta_in(), EVENT_WEIGHT);
                    e0_dphi_in_->Fill(ZF_EVENT.e1_truth->dphi_in(), EVENT_WEIGHT);
                    e0_track_iso_->Fill(ZF_EVENT.e1_truth->track_iso(), EVENT_WEIGHT);
                    e0_ecal_iso_->Fill(ZF_EVENT.e1_truth->ecal_iso(), EVENT_WEIGHT);
                    e0_hcal_iso_->Fill(ZF_EVENT.e1_truth->hcal_iso(), EVENT_WEIGHT);
                    e0_one_over_e_mins_one_over_p_->Fill(ZF_EVENT.e1_truth->one_over_e_mins_one_over_p(), EVENT_WEIGHT);
                }
                if (ZF_EVENT.e0_truth != nullptr) {
                    e1_pt_->Fill(ZF_EVENT.e0_truth->pt(), EVENT_WEIGHT);
                    e1_pt_naked_->Fill(ZF_EVENT.e0_truth->nakedPt(), EVENT_WEIGHT);
                    e1_pt_born_->Fill(ZF_EVENT.e0_truth->bornPt(), EVENT_WEIGHT);
                    e1_eta_->Fill(ZF_EVENT.e0_truth->eta(), EVENT_WEIGHT);
                    e1_eta_naked_->Fill(ZF_EVENT.e0_truth->nakedEta(), EVENT_WEIGHT);
                    e1_eta_born_->Fill(ZF_EVENT.e0_truth->bornEta(), EVENT_WEIGHT);
                    e1_phi_->Fill(ZF_EVENT.e0_truth->phi(), EVENT_WEIGHT);
                    e1_phi_naked_->Fill(ZF_EVENT.e0_truth->nakedPhi(), EVENT_WEIGHT);
                    e1_phi_born_->Fill(ZF_EVENT.e0_truth->bornPhi(), EVENT_WEIGHT);
                    e1_charge_->Fill(ZF_EVENT.e0_truth->charge(), EVENT_WEIGHT);
                    e1_r9_->Fill(ZF_EVENT.e0_truth->r9(), EVENT_WEIGHT);
                    e1_sigma_ieta_ieta_->Fill(ZF_EVENT.e0_truth->sigma_ieta_ieta(), EVENT_WEIGHT);
                    e1_h_over_e_->Fill(ZF_EVENT.e0_truth->h_over_e(), EVENT_WEIGHT);
                    e1_deta_in_->Fill(ZF_EVENT.e0_truth->deta_in(), EVENT_WEIGHT);
                    e1_dphi_in_->Fill(ZF_EVENT.e0_truth->dphi_in(), EVENT_WEIGHT);
                    e1_track_iso_->Fill(ZF_EVENT.e0_truth->track_iso(), EVENT_WEIGHT);
                    e1_ecal_iso_->Fill(ZF_EVENT.e0_truth->ecal_iso(), EVENT_WEIGHT);
                    e1_hcal_iso_->Fill(ZF_EVENT.e0_truth->hcal_iso(), EVENT_WEIGHT);
                    e1_one_over_e_mins_one_over_p_->Fill(ZF_EVENT.e0_truth->one_over_e_mins_one_over_p(), EVENT_WEIGHT);
                }
                if (ZF_EVENT.e1_trig != nullptr && ZF_EVENT.e1_truth != nullptr) {
                    e0_pt_vs_trig_->Fill(
                            ZF_EVENT.e1_truth->pt() / ZF_EVENT.e1_trig->pt(),
                            EVENT_WEIGHT
                            );
                }
                if (ZF_EVENT.e0_trig != nullptr && ZF_EVENT.e0_truth != nullptr) {
                    e1_pt_vs_trig_->Fill(
                            ZF_EVENT.e0_truth->pt() / ZF_EVENT.e0_trig->pt(),
                            EVENT_WEIGHT
                            );
                }
            }
            // Event Info
            pileup_->Fill(ZF_EVENT.truth_vert.num, EVENT_WEIGHT);
            nelectrons_->Fill(2, EVENT_WEIGHT);  // We only ever grab the two electrons from the Z
        }
        // Event weights, they are of course, unweighted
        baseweights_->Fill(ZF_EVENT.event_weight);
        fullweights_->Fill(EVENT_WEIGHT);

        // Phistar Reco Vs. Truth
        if (!ZF_EVENT.is_real_data
                && ZF_EVENT.e0_truth != nullptr
                && ZF_EVENT.e1_truth != nullptr
                && ZF_EVENT.e0 != nullptr
                && ZF_EVENT.e1 != nullptr
           ) {
            phistar_vs_truth_->Fill(
                    ZF_EVENT.reco_z.phistar / ZF_EVENT.truth_z.phistar,
                    EVENT_WEIGHT
                    );
        }
    }
}  // namespace zf
