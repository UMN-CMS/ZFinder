#include "ZFinder/Event/interface/ZDefinitionTree.h"

// Standard Library
#include <utility>  // std::make_pair
#include <sstream>  // std::ostringstream

// ZFinder Code
#include "ZFinder/Event/interface/CutLevel.h"  // cutlevel_vector


namespace zf {
    // Constructor
    ZDefinitionTree::ZDefinitionTree(const ZDefinition& zdef, TFile* tfile, const bool IS_MC) : IS_MC_(IS_MC) {
        // Get the name of the cut we want
        zdef_name_ = zdef.NAME;

        // Make the directory to save files to
        tdir_ = tfile->mkdir(zdef_name_.c_str());
        tdir_->cd();

        // Make the Tree to write to
        tree_ = new TTree(zdef.NAME.c_str(), zdef.NAME.c_str());
        const std::string CODE = "z_m/D:z_y:z_phistar_born:z_phistar_dressed:z_phistar_naked:z_pt:z_eta:e_pt0:e_pt1:e_eta0:e_eta1:e_phi0:e_phi1:e_charge0/I:e_charge1:n_verts/i";
        tree_->Branch("reco", &reco_, CODE.c_str());
        if (IS_MC_) {
            tree_->Branch("truth", &truth_, CODE.c_str());
        }
        const std::string EVENT_CODE = "gen_weight/D:event_weight:event_number/i:is_mc/O";
        tree_->Branch("event_info", &event_, EVENT_CODE.c_str());
    }

    ZDefinitionTree::~ZDefinitionTree() {
        // Clean up our pointer
        delete tree_;
    }

    void ZDefinitionTree::Fill(const ZFinderEvent& zf_event) {
        // If the ZDefinition doesn't pass, escape
        if (!zf_event.ZDefPassed(zdef_name_)) {
            return;
        }

        // Clear our branches
        reco_.clear_values();
        truth_.clear_values();
        event_.clear_values();

        // The weight from the generator, and the weight from the scale
        // factors, which we set to 1 but then read from the cut level vector
        const double GEN_WEIGHT = zf_event.event_weight;

        // Get the scale factors weight
        const cutlevel_vector* clv = zf_event.GetZDef(zdef_name_);
        double weight = GetCutWeight(clv);

        // Reco
        reco_.z_m = zf_event.reco_z.m;
        reco_.z_y = zf_event.reco_z.y;
        reco_.z_phistar_dressed = zf_event.reco_z.phistar;
        reco_.z_phistar_born = zf_event.reco_z.bornPhistar;
        reco_.z_phistar_naked = zf_event.reco_z.nakedPhistar;
        reco_.z_pt = zf_event.reco_z.pt;
        reco_.z_eta = zf_event.reco_z.eta;
        reco_.n_verts = zf_event.reco_vert.num;
        if (zf_event.e0 != NULL) {
            reco_.e_pt[0] = zf_event.e0->pt;
            reco_.e_eta[0] = zf_event.e0->eta;
            reco_.e_phi[0] = zf_event.e0->phi;
            reco_.e_charge[0] = zf_event.e0->charge;
        }
        if (zf_event.e1 != NULL) {
            reco_.e_pt[1] = zf_event.e1->pt;
            reco_.e_eta[1] = zf_event.e1->eta;
            reco_.e_phi[1] = zf_event.e1->phi;
            reco_.e_charge[1] = zf_event.e1->charge;
        }
        // Truth
        if (IS_MC_ && !zf_event.is_real_data) {
            truth_.z_m = zf_event.truth_z.m;
            truth_.z_y = zf_event.truth_z.y;
            truth_.z_phistar_dressed = zf_event.truth_z.phistar;
            truth_.z_phistar_born = zf_event.truth_z.bornPhistar;
            truth_.z_phistar_naked = zf_event.truth_z.nakedPhistar;
            truth_.z_pt = zf_event.truth_z.pt;
            truth_.z_eta = zf_event.truth_z.eta;
            truth_.n_verts = zf_event.truth_vert.num;
            if (zf_event.e0_truth != NULL) {
                truth_.e_pt[0] = zf_event.e0_truth->pt;
                truth_.e_eta[0] = zf_event.e0_truth->eta;
                truth_.e_phi[0] = zf_event.e0_truth->phi;
                truth_.e_charge[0] = zf_event.e0_truth->charge;
            }
            if (zf_event.e1_truth != NULL) {
                truth_.e_pt[1] = zf_event.e1_truth->pt;
                truth_.e_eta[1] = zf_event.e1_truth->eta;
                truth_.e_phi[1] = zf_event.e1_truth->phi;
                truth_.e_charge[1] = zf_event.e1_truth->charge;
            }
        }
        // General Event info
        event_.gen_weight = GEN_WEIGHT;
        event_.event_weight = weight;
        event_.is_mc = !zf_event.is_real_data;
        event_.event_number = zf_event.id.event_num;

        // Now that the right values have been set, fill the tuple, but only if
        // we have a z candidate
        if (reco_.z_m >= 0) {
            tree_->Fill();
        }
    }

    double ZDefinitionTree::GetCutWeight(cutlevel_vector const * const CUT_LEVEL_VECTOR) {
        /* Get the cut weight from the last level of cuts */
        double weight = 1.;
        if (CUT_LEVEL_VECTOR != NULL) {
            // Get the last level of cuts
            CutLevel last_cutlevel = CUT_LEVEL_VECTOR->back().second;
            // The cut can pass two ways. We check if the 0th electron is the
            // tag, and take that way if it suceeds, otherwise we check if the
            // 1st electron is the tag.
            if (last_cutlevel.t0p1_pass) {
                weight = last_cutlevel.t0p1_eff;
            }
            else if (last_cutlevel.t1p0_pass) {
                weight = last_cutlevel.t1p0_eff;
            }
        }
        return weight;
    }

    void ZDefinitionTree::Write() {
        tree_->Write();
    }
}  // namespace zf
