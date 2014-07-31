#include "ZFinder/Event/interface/ZFinderTree.h"

// Standard Library
#include <string>  // string

// Root
#include <TCanvas.h>  // TCanvas

// ZFinder Code
#include "ZFinder/Event/interface/ZFinderElectron.h"  // ZFinderElectron


namespace zf {
    // Constructor
    ZFinderTree::ZFinderTree(TFileDirectory& tdir, const bool IS_MC) : IS_MC_(IS_MC) {

        tree_ = tdir.make<TTree>("ZFinderTree", "ZFinderTree");

        // The branch code is an "magic" string that tells the TTree how to deal with an input struct
        branch_code_ = "z_m/D:z_y:z_phistar:z_pt:z_eta:z_phi:e_pt0:e_pt1:e_eta0:e_eta1:e_phi0:e_phi1:e_charge0/I:e_charge1:event_weight/D";

        // Set up the branches; we only set up truth if running on MC
        reco_branch_ = tree_->Branch("reco", &reco_, branch_code_.c_str());

        if (IS_MC_) {
            truth_branch_ = tree_->Branch("truth", &truth_, branch_code_.c_str());
        }

        // Set default values for the branches
        Clear(reco_);
        if (IS_MC_) {
            Clear(truth_);
        }
    }

    void ZFinderTree::Fill(
            const ZFinderEvent& ZF_EVENT,
            const int ELECTRON_0,
            const int ELECTRON_1,
            const double RECO_EVENT_WEIGHT,
            const double GEN_EVENT_WEIGHT
            ) {
        /*
         * Given a zf_event, fills all the trees.
         *
         * ELECTRON_0 and ELECTRON_1 can be used to assign zf_event.eN to the
         * given number in the tree. For example, assigning ELECTRON_0 = 1 will
         * fill the e0 tree with data from zf_event.e1.
         */

        // Set default values for the branches
        Clear(reco_);
        if (IS_MC_) {
            Clear(truth_);
        }

        // The Z
        reco_.z_m = ZF_EVENT.reco_z.m;
        reco_.z_y = ZF_EVENT.reco_z.y;
        reco_.z_phistar = ZF_EVENT.reco_z.phistar;
        reco_.z_pt = ZF_EVENT.reco_z.pt;
        reco_.z_eta = ZF_EVENT.reco_z.eta;

        if (IS_MC_) {
            truth_.z_m = ZF_EVENT.truth_z.m;
            truth_.z_y = ZF_EVENT.truth_z.y;
            truth_.z_phistar = ZF_EVENT.truth_z.phistar;
            truth_.z_pt = ZF_EVENT.truth_z.pt;
            truth_.z_eta = ZF_EVENT.truth_z.eta;
        }

        // The Electrons
        ZFinderElectron* our_e0 = NULL;
        ZFinderElectron* our_e1 = NULL;
        ZFinderElectron* our_e0_truth = NULL;
        ZFinderElectron* our_e1_truth = NULL;
        if (ELECTRON_0 == 0 && ELECTRON_1 == 1) {
            our_e0 = ZF_EVENT.e0;
            our_e1 = ZF_EVENT.e1;
            our_e0_truth = ZF_EVENT.e0_truth;
            our_e1_truth = ZF_EVENT.e1_truth;
        }
        else if (ELECTRON_0 == 1 && ELECTRON_1 == 0) {
            our_e1 = ZF_EVENT.e0;
            our_e0 = ZF_EVENT.e1;
            our_e1_truth = ZF_EVENT.e0_truth;
            our_e0_truth = ZF_EVENT.e1_truth;
        }

        if (our_e0 != NULL) {
            reco_.e_pt[0] = our_e0->pt;
            reco_.e_eta[0] = our_e0->eta;
            reco_.e_phi[0] = our_e0->phi;
            reco_.e_charge[0] = our_e0->charge;
        }
        if (our_e1 != NULL) {
            reco_.e_pt[1] = our_e1->pt;
            reco_.e_eta[1] = our_e1->eta;
            reco_.e_phi[1] = our_e1->phi;
            reco_.e_charge[1] = our_e1->charge;
        }

        if (IS_MC_) {
            if (our_e0_truth != NULL) {
                reco_.e_pt[0] = our_e0_truth->pt;
                reco_.e_eta[0] = our_e0_truth->eta;
                reco_.e_phi[0] = our_e0_truth->phi;
                reco_.e_charge[0] = our_e0_truth->charge;
            }
            if (our_e1_truth != NULL) {
                reco_.e_pt[1] = our_e1_truth->pt;
                reco_.e_eta[1] = our_e1_truth->eta;
                reco_.e_phi[1] = our_e1_truth->phi;
                reco_.e_charge[1] = our_e1_truth->charge;
            }
        }

        // Event weight
        reco_.event_weight = RECO_EVENT_WEIGHT;
        if (IS_MC_) {
            truth_.event_weight = GEN_EVENT_WEIGHT;
        }

        tree_->Fill();
    }

    void ZFinderTree::Clear(branch_struct& branch) {
        branch.z_m = -1;
        branch.z_y = -10;
        branch.z_phistar = -1;
        branch.z_pt = -1;
        branch.z_eta = -10;
        branch.z_phi = -10;
        branch.e_pt[0] = -1;
        branch.e_eta[0] = -10;
        branch.e_phi[0] = -10;
        branch.e_charge[0] = -2;
        branch.e_pt[1] = -1;
        branch.e_eta[1] = -10;
        branch.e_phi[1] = -10;
        branch.e_charge[1] = -2;
        branch.event_weight = 0;
    }
}  // namespace zf
