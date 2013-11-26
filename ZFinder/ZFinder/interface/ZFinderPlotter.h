#ifndef ZFINDER_ZFINDERPLOTTER_H_
#define ZFINDER_ZFINDERPLOTTER_H_

// Root
#include <TH1I.h>  // TH1I

// CMSSW
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// ZFinder Code
#include "ZFinderEvent.h"  // ZFinderEvent


namespace zf {
    class ZFinderPlotter{
        public:
            // Constructor
            ZFinderPlotter(TFileDirectory* tdir);

            // Add events
            void Fill(const ZFinderEvent& zf_event, const int first_electron = 0, const int second_electron = 1);
            // Make PNGs
            void Print(const std::string& basename);

        protected:
            // Histograms
            TH1I* z0_mass_coarse_;
            TH1I* z0_mass_fine_;
            TH1I* z0_rapidity_;
            TH1I* z0_pt_;
            TH1I* e0_pt_;
            TH1I* e1_pt_;
            TH1I* e0_eta_;
            TH1I* e1_eta_;
            TH1I* e0_phi_;
            TH1I* e1_phi_;
            TH1I* phistar_;
            TH1I* pileup_;
            TH1I* nelectrons_;

            // Plotting variables
            static const int X_SIZE = 1280;
            static const int Y_SIZE = 640;
    };
}  // namespace zf
#endif  // ZFINDER_ZFINDERPLOTTER_H_
