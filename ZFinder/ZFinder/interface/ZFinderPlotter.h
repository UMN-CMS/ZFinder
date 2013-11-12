#ifndef ZFINDER_ZFINDERPLOTTER_H_
#define ZFINDER_ZFINDERPLOTTER_H_

// Root
#include <TDirectory.h>  // TDirectory
#include <TH1I.h>  // TH1I

// ZFinder Code
#include "ZFinderEvent.h"  // ZFinderEvent

class ZFinderPlots{
    public:
        // Constructor
        ZFinderPlots(TDirectory* td);

        // Add events
        void Fill(ZFinderEvent const * const z_event, const short first_electron = 0, const short second_electron = 1);
        // Make PNGs
        void Print();

    private:
        // The TDirectory to store plots in
        TDirectory* tdir_;

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
};

#endif  // ZFINDER_ZFINDERPLOTTER_H_
