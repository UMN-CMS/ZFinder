#ifndef ZFINDER_ZFINDERFITTER_H_
#define ZFINDER_ZFINDERFITTER_H_

// Root
#include <TFile.h>
#include <RooDataSet.h>
#include <RooArgSet.h>
#include <RooRealVar.h>
#include <RooWorkspace.h>

// CMSSW
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// ZFinder Code
#include "ZFinderEvent.h"  // ZFinderEvent


namespace zf {
    class ZFinderFitter{
        public:
            // Constructor
            ZFinderFitter();

            // Add events
            void FillAll(const ZFinderEvent& zf_event);
            void FillSelected(const ZFinderEvent& zf_event);
	    //calculates cross-section, print fits
            void Write(const char* filename);


        protected:
            // DataSets
	    RooArgSet* ZEventArgSet;
	    RooDataSet* MC_true_all;
	    RooDataSet* MC_reco;
	    RooDataSet* Data_reco;
	    RooRealVar* Zmass;
	    RooRealVar* Zeta ;
	    RooRealVar* Zy;
	    RooRealVar* Zpt;
	    RooRealVar* Zphistar;
	    RooRealVar* Weight;
	    RooRealVar* Pass;
	    
	    

     

    };
}  // namespace zf
#endif  // ZFINDER_ZFINDERFITTER_H_
