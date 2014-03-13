#ifndef ZFINDER_ZDEFINITIONWORKSPACE_H_
#define ZFINDER_ZDEFINITIONWORKSPACE_H_

// Standard Library
#include <map>  // std::map
#include <string>  // string
#include <vector>  // std::vector

// Root
#include <RooArgSet.h>  // RooArgSet
#include <RooCategory.h>  // RooCategory
#include <RooDataSet.h>  // RooDataSet
#include <RooRealVar.h>  // RooRealVar
#include <RooWorkspace.h>  // RooWorkspace
#include <TDirectory.h>  // TDirectory

// CMSSW
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// ZFinder Code
#include "ZFinderEvent.h"  // ZFinderEvent, CutLevel, ZData
#include "ZDefinition.h"  // ZDefinition


namespace zf {

    class ZDefinitionWorkspace{
        public:
            // Constructor
            ZDefinitionWorkspace(
                    const ZDefinition& zdef,
                    TFileDirectory& tdir,
                    const bool USE_TRUTH = false,
                    const bool PROBE_FIRST = true
                    );

            // Destructor
            ~ZDefinitionWorkspace();

            // Add events
            void Fill(const ZFinderEvent& zf_event);

            // Save output
            void Write();

        protected:
            // The Roo Variables
            RooArgSet* argset;
            // Dataset
            RooDataSet* roodataset;
            // Z
            RooRealVar* z_mass;
            RooRealVar* z_eta;
            RooRealVar* z_y;
            RooRealVar* z_pt;
            RooRealVar* phistar;
            // Electrons
            RooRealVar* e0_pt;
            RooRealVar* e0_eta;
            RooRealVar* e1_pt;
            RooRealVar* e1_eta;
            RooCategory* e0_charge;
            RooCategory* e1_charge;
            // Event
            RooRealVar* n_vert;
            RooRealVar* weight;
            RooCategory* data_type;
            RooCategory* numerator;
            RooCategory* degenerate;

            void InitVariables();

            // Name
            std::string zdef_name_;

            // Internal directory to save to
            TDirectory* write_dir_;

            // Set the 0th electron to the one that passed the 0th cuts
            const bool PROBE_FIRST_;
            // Use the truth data, not the reco
            const bool USE_TRUTH_;
    };
}  // namespace zf
#endif  // ZFINDER_ZDEFINITIONWORKSPACE_H_
