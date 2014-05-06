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
            RooArgSet* argset_;
            // Dataset
            RooDataSet* roodataset_;
            // Z
            RooRealVar* z_mass_;
            RooRealVar* z_eta_;
            RooRealVar* z_y_;
            RooRealVar* z_pt_;
            RooRealVar* phistar_;
            // Electrons
            RooRealVar* e0_pt_;
            RooRealVar* e0_eta_;
            RooRealVar* e1_pt_;
            RooRealVar* e1_eta_;
            RooCategory* e0_charge_;
            RooCategory* e1_charge_;
            // Event
            RooRealVar* n_vert_;
            RooRealVar* weight_;
            RooCategory* data_type_;
            RooCategory* numerator_;
            RooCategory* degenerate_;

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
