#ifndef ZFINDER_ZDEFINITIONTREE_H_
#define ZFINDER_ZDEFINITIONTREE_H_

// Standard Library
#include <string>  // string
#include <utility>  // pair

// ROOT
#include "TBranch.h"  // TBranch
#include "TDirectory.h"  // TDirectory
#include "TFile.h"  // TFile
#include "TTree.h"  // TTree

// ZFinder Code
#include "ZDefinition.h"  // ZDefinition
#include "ZFinderEvent.h"  // ZFinderEvent


namespace zf {
    class ZDefinitionTree {
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

            // Wrapper around TTree::GetCurrentFile()
            TFile* GetCurrentFile();

        protected:
            // Structs that map to the branches
            struct branch_struct {
                void clear_values() {
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
                    e_charge[0] = -2;
                    e_charge[1] = -2;
                    n_verts = -1;
                }
                // Constructor
                branch_struct() {
                    clear_values();
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
                int e_charge[2];
                unsigned int n_verts;
            } reco_, truth_;

            struct event_branch {
                void clear_values() {
                    event_number = 0;
                    is_mc = false;
                }

                // Constructor
                event_branch() {
                    clear_values();
                }
                unsigned int event_number;
                bool is_mc;
            } event_;

            // Set up a variable size branch for the weights
            int weight_size_;
            double weight_fsr_;
            int weight_cteq_size_;
            int weight_mstw_size_;
            int weight_nnpdf_size_;
            static constexpr int MAX_SIZE_ = 100;
            static constexpr int MAX_SIZE_PDF_ = 110;
            // Although vectors seem like the right solution, since TTrees need
            // the memory used for the array to be static, an array is
            // (unfortunately) the best choice
            double weights_[MAX_SIZE_];
            int weight_ids_[MAX_SIZE_];
            double weights_cteq_[MAX_SIZE_PDF_];
            double weights_mstw_[MAX_SIZE_PDF_];
            double weights_nnpdf_[MAX_SIZE_PDF_];

            // We insert the weights and the IDs into this vector, and then
            // read it out into the array before filling the tree
            std::vector<std::pair<int, double>> weight_id_vector_;

            // Name
            std::string zdef_name_;

            // File Directory to write to
            TDirectory* tdir_;

            // Use the MC or reco data
            const bool IS_MC_;

            // The tuples
            TTree* tree_;

            // Get the weight of the cuts
            void FillCutWeights(cutlevel_vector const * const CUT_LEVEL_VECTOR);
            double GetTotalWeight(cutlevel_vector const * const CUT_LEVEL_VECTOR);
    };
}  // namespace zf
#endif  // ZFINDER_ZDEFINITIONTREE_H_
