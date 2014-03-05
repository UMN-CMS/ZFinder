#ifndef ZFINDER_ZFINDERFITTER_H_
#define ZFINDER_ZFINDERFITTER_H_

// Standard Library
#include <vector>

// Root
#include <RooArgSet.h>
#include <RooDataSet.h>
#include <RooRealVar.h>
#include <RooWorkspace.h>

// CMSSW
#include "CommonTools/UtilAlgos/interface/TFileService.h"  // TFileDirectory

// ZFinder Code
#include "ZFinderEvent.h"  // ZFinderEvent


namespace zf {


    class ZFinderFitter{
        public:
            // Constructor
            ZFinderFitter();

            // Destructor
            ~ZFinderFitter();

            // Add events
            void FillAll(const ZFinderEvent& zf_event);
            void FillSelected(const ZFinderEvent& zf_event);
            //calculates cross-section, print fits
            void Write();

        protected:
            // DataSets
            RooArgSet* zf_arg_set;

            RooDataSet* mc_truth_dataset;
            RooDataSet* mc_reco_dataset;
            RooDataSet* data_reco_dataset;

            RooRealVar* z_mass;
            RooRealVar* z_eta ;
            RooRealVar* z_y;
            RooRealVar* z_pt;
            RooRealVar* phistar;
            RooRealVar* weight;
            RooRealVar* pass;
            RooRealVar* e0_pt;
            RooRealVar* e0_phi;
            RooRealVar* e0_eta;
            //RooRealVar* e0_charge;
            RooRealVar* e1_pt;
            RooRealVar* e1_phi;
            RooRealVar* e1_eta;
            //RooRealVar* e1_charge;
            RooRealVar* n_vert;

            std::vector<RooRealVar*> rrv_cuts;
    };
}  // namespace zf
#endif  // ZFINDER_ZFINDERFITTER_H_
