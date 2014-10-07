#ifndef ZFINDER_ZDEFINITIONTREE_H_
#define ZFINDER_ZDEFINITIONTREE_H_

// Standard Library
#include <string>  // string

// ROOT
#include "TBranch.h"  // TBranch
#include "TDirectory.h"  // TDirectory
#include "TFile.h"  // TFile
#include "TTree.h"  // TTree

// ZFinder Code
#include "ZDefinition.h"  // ZDefinition
#include "ZFinderEvent.h"  // ZFinderEvent


namespace zf {
    class ZDefinitionTree{
        public:
            // Constructor
            ZDefinitionTree(
                    const ZDefinition& zdef,
                    TFile* output_file,
                    const bool IS_MC = false
                    );

            // destructor
            ~ZDefinitionTree();

            // Add event
            void Fill(const ZFinderEvent& zf_event);

            // Write the Tree
            void Write();

        protected:
            // Structs that map to the branches
            struct branch_struct {
                // Constructor
                branch_struct() {
                    z_m = -1;
                    z_y = -10;
                    z_phistar_born = -1;
                    z_phistar_dressed = -1;
                    z_phistar_naked = -1;
                    z_pt = -1;
                    z_eta = -10;
                    e_pt[0] = -1;
                    e_eta[0] = -10;
                    e_phi[0] = -10;
                    e_pt[1] = -1;
                    e_eta[1] = -10;
                    e_phi[1] = -10;
                    gen_weight = -1;
                    event_weight = -1;
                    e_charge[0] = -2;
                    e_charge[1] = -2;
                    is_mc = false;
                }

                double z_m;
                double z_y;
                double z_phistar_born;
                double z_phistar_dressed;
                double z_phistar_naked;
                double z_pt;
                double z_eta;
                double e_pt[2];
                double e_eta[2];
                double e_phi[2];
                double gen_weight;
                double event_weight;
                int e_charge[2];
                bool is_mc;
            } reco_, truth_;

            // Clear the branch structs
            void Clear(branch_struct& branch);

            // Name
            std::string zdef_name_;

            // File Directory to write to
            TDirectory* tdir_;

            // Use the MC or reco data
            const bool IS_MC_;

            // The tuples
            TTree* tree_;

            // Get the weight of the cuts
            double GetCutWeight(cutlevel_vector const * const CUT_LEVEL_VECTOR);
    };
}  // namespace zf
#endif  // ZFINDER_ZDEFINITIONTREE_H_
