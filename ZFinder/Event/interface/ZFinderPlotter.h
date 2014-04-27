#ifndef ZFINDER_ZFINDERPLOTTER_H_
#define ZFINDER_ZFINDERPLOTTER_H_

// Root
#include <TH1D.h>  // TH1D

// CMSSW
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// ZFinder Code
#include "ZFinderEvent.h"  // ZFinderEvent


namespace zf {
    class ZFinderPlotter{
        public:
            // Constructor
            ZFinderPlotter(TFileDirectory& tdir, const bool USE_MC = false);

            // Add events
            void Fill(
                    const ZFinderEvent& zf_event,
                    const int first_electron = 0,
                    const int second_electron = 1,
                    const double EVENT_WEIGHT = 1.
                    );
            // Make PNGs
            void Print(const std::string& basename);

        protected:
            // Histograms
            TH1D* z0_mass_all_;
            TH1D* z0_mass_coarse_;
            TH1D* z0_mass_fine_;
            TH1D* z0_rapidity_;
            TH1D* z0_pt_;
            TH1D* e0_pt_;
            TH1D* e1_pt_;
            TH1D* e0_eta_;
            TH1D* e1_eta_;
            TH1D* e0_phi_;
            TH1D* e1_phi_;
            TH1D* e0_charge_;
            TH1D* e1_charge_;
            TH1D* phistar_;
            TH1D* pileup_;
            TH1D* nelectrons_;

            // Use the MC or reco data
            const bool USE_MC_;

            // Plotting variables
            static const int X_SIZE = 1280;
            static const int Y_SIZE = 640;
    };
}  // namespace zf
#endif  // ZFINDER_ZFINDERPLOTTER_H_
