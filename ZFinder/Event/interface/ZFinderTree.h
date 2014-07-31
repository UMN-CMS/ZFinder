#ifndef ZFINDER_ZFINDERTREE_H_
#define ZFINDER_ZFINDERTREE_H_

// Root
#include <TBranch.h>  // TBranch
#include <TTree.h>  // TTree

// CMSSW
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// ZFinder Code
#include "ZFinderEvent.h"  // ZFinderEvent


namespace zf {
    class ZFinderTree {
        public:
            // Constructor
            ZFinderTree(TFileDirectory& tdir, const bool IS_MC = false);

            // Add events
            void Fill(
                    const ZFinderEvent& ZF_EVENT,
                    const int FIRST_ELECTRON = 0,
                    const int SECOND_ELECTRON = 1,
                    const double RECO_EVENT_WEIGHT = 1.,
                    const double GEN_EVENT_WEIGHT = 1.
                    );

        protected:
            // Structs that map to the branches
            struct branch_struct {
                double z_m;
                double z_y;
                double z_phistar;
                double z_pt;
                double z_eta;
                double z_phi;
                double e_pt[2];
                double e_eta[2];
                double e_phi[2];
                int e_charge[2];
                double event_weight;
            } truth_, reco_;

            // Clear the branch structs
            void Clear(branch_struct& branch);

            // The branch codes
            std::string branch_code_;

            // The Tree and branches
            TTree* tree_;
            TBranch* truth_branch_;
            TBranch* reco_branch_;

            // Use the MC or reco data
            const bool IS_MC_;
    };
}  // namespace zf
#endif  // ZFINDER_ZFINDERTREE_H_
