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
            TH1D* phistar_;
            TH1D* phistar_born_;
            TH1D* phistar_naked_;
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

            // Use the MC or reco data
            const bool USE_MC_;
    };
}  // namespace zf
#endif  // ZFINDER_ZFINDERPLOTTER_H_
