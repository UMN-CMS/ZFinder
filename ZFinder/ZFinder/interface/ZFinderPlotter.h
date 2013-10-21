// ZFinder Code
#include "ZFinderElectron.h"
#include "ZFinderEvent.h"

// Root
#include <TCanvas.h>
#include <TDirectory.h>
#include <TH1I.h>

// Standard Library
#include <string>

#ifndef ZFINDERPLOTS_H_INCLUDED
#define ZFINDERPLOTS_H_INCLUDED

class ZFinderPlots{
    public:
        // Constructor
        ZFinderPlots(TDirectory* td);
        
        // Add events, and make PNGs
        void fill(const ZFinderEvent* ZEvent, const short e0 = 0, const short e1 = 1);
        void print();

    private:
        TDirectory* m_tdir;

        TH1I* Z0MassCoarse;
        TH1I* Z0MassFine;
        TH1I* Z0Rapidity;
        TH1I* Z0pt;
        TH1I* e0pt;
        TH1I* e1pt;
        TH1I* e0eta;
        TH1I* e1eta;
        TH1I* e0phi;
        TH1I* e1phi;
        TH1I* phistar;
        TH1I* pileup;
};

#endif
