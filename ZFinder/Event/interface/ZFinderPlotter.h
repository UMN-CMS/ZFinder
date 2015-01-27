#ifndef ZFINDER_ZFINDERPLOTTER_H_
#define ZFINDER_ZFINDERPLOTTER_H_

// Root
#include <TH1D.h>  // TH1D
#include <TH2D.h>  // TH2D

// CMSSW
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// ZFinder Code
#include "ZFinderEvent.h"  // ZFinderEvent


namespace zf {
    class ZFinderPlotter {
        public:
            // Constructor
            ZFinderPlotter(TFileDirectory& tdir, const bool USE_MC = false);

            // Add events
            void Fill(
                    const ZFinderEvent& ZF_EVENT,
                    const int FIRST_ELECTRON = 0,
                    const int SECOND_ELECTRON = 1,
                    const double EVENT_WEIGHT = 1.
                    );

        protected:
            // Histograms
            TH1D* z0_mass_all_;
            TH1D* z0_mass_coarse_;
            TH1D* z0_mass_fine_;
            TH1D* z0_rapidity_;
            TH1D* z0_pt_;
            TH1D* e0_pt_;
            TH1D* e0_pt_naked_;
            TH1D* e0_pt_born_;
            TH1D* e1_pt_;
            TH1D* e1_pt_naked_;
            TH1D* e1_pt_born_;
            TH1D* e0_eta_;
            TH1D* e0_eta_naked_;
            TH1D* e0_eta_born_;
            TH1D* e1_eta_;
            TH1D* e1_eta_naked_;
            TH1D* e1_eta_born_;
            TH1D* e0_phi_;
            TH1D* e0_phi_naked_;
            TH1D* e0_phi_born_;
            TH1D* e1_phi_;
            TH1D* e1_phi_naked_;
            TH1D* e1_phi_born_;
            TH1D* e0_charge_;
            TH1D* e1_charge_;
            TH1D* e0_r9_;
            TH1D* e1_r9_;
            TH1D* e0_sigma_ieta_ieta_;
            TH1D* e1_sigma_ieta_ieta_;
            TH1D* e0_h_over_e_;
            TH1D* e1_h_over_e_;
            TH1D* e0_deta_in_;
            TH1D* e1_deta_in_;
            TH1D* e0_dphi_in_;
            TH1D* e1_dphi_in_;
            TH1D* e0_track_iso_;
            TH1D* e1_track_iso_;
            TH1D* e0_ecal_iso_;
            TH1D* e1_ecal_iso_;
            TH1D* e0_hcal_iso_;
            TH1D* e1_hcal_iso_;
            TH1D* e0_one_over_e_mins_one_over_p_;
            TH1D* e1_one_over_e_mins_one_over_p_;
            TH1D* phistar_;
            TH1D* phistar_born_;
            TH1D* phistar_naked_;
            TH1D* phistar_supercluster_;
            TH1D* pileup_;
            TH1D* nelectrons_;
            TH1D* baseweights_;
            TH1D* fullweights_;
            TH1D* e0_pt_vs_trig_;
            TH1D* e1_pt_vs_trig_;
            TH1D* phistar_vs_truth_;
            TH1D* deltaR_;
            TH1D* other_phistar_;
            TH1D* other_y_;
            TH2D* phistar_vs_sc_phistar_;

            // Use the MC or reco data
            const bool USE_MC_;
    };
}  // namespace zf
#endif  // ZFINDER_ZFINDERPLOTTER_H_
