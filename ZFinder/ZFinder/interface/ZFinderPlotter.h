#ifndef ZFINDER_ZFINDERPLOTTER_H_
#define ZFINDER_ZFINDERPLOTTER_H_

// Root
#include <TDirectory.h>  // TDirectory
#include <TH1I.h>  // TH1I

// CMSSW
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"  // reco::GsfElectron
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"  // HepMC::GenParticle

// ZFinder Code
#include "ZFinderEvent.h"  // ZFinderEvent


class ZFinderPlotter{
    public:
        // Constructor
        ZFinderPlotter(TDirectory* td);

        // Add events
        void Fill(const ZFinderEvent& zf_event, const int first_electron = 0, const int second_electron = 1);
        // Make PNGs
        void Print();

    protected:
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

        // Plotting variables
        static const int X_SIZE = 1280;
        static const int Y_SIZE = 640;
};

#endif  // ZFINDER_ZFINDERPLOTTER_H_
