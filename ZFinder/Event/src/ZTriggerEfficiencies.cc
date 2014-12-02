#include "ZFinder/Event/interface/ZTriggerEfficiencies.h"

namespace zf {

    ZTriggerEfficiencies::ZTriggerEfficiencies() {
        // Set up the hardcoded variables
        Setup();
    }

    void ZTriggerEfficiencies::SetWeights(ZFinderEvent* zf_event) {
        /*
         * We need to consider several cases:
         *     1) No electron passes, then the efficiency should be 0
         *     2) Only one possible electron can pass, than the efficiency is p1
         *     3) Both electrons could pass, then the efficiency is (1-(1-p1)(1-p2))
         *
         *  Where:
         *      p1 is the efficiency of the first electron
         *      p2 is the efficiency of the second electron
         *
         *  For scale factors we need to divide the cases by the same number in mc.
         *  In case 2 this is simply the scale factor, but in case 3 it is:
         *      (1-(1-p1_{data})(1-p2_{data})) / (1-(1-p1_{MC})(1-p2_{MC}))
         */

        // The reco electrons
        ZFinderElectron* e0 = zf_event->e0;
        ZFinderElectron* e1 = zf_event->e1;

        // Single Electron Trigger
        int passing_electrons = 0;
        if (e0 != nullptr) {
            if (e1->pt() >= 30 && fabs(e1->eta()) <= 2.1 && e1->CutPassed("trig(single_ele)") == 1) {
                passing_electrons++;
            }
        }
        if (e1 != nullptr) {
            if (e1->pt() >= 30 && fabs(e1->eta()) <= 2.1 && e1->CutPassed("trig(single_ele)") == 1) {
                passing_electrons++;
            }
        }

        // Check the three cases
        if (passing_electrons == 0) {
            return;
        }
        else if (passing_electrons == 1) {
            SetSingleElectronWeight(zf_event);
        }
        else {
            SetDoubleElectronWeight(zf_event);
        }
    }

    void ZTriggerEfficiencies::SetSingleElectronWeight(ZFinderEvent* zf_event) {
        const std::string CUT_NAME = "trig(single_ele)";

        ZFinderElectron* electron = nullptr;
        // The first one is the right one, because we know only one passes
        if (zf_event->e0 != nullptr && zf_event->e0->CutPassed(CUT_NAME)) {
            electron = zf_event->e0;
        }
        else if (zf_event->e1 != nullptr && zf_event->e1->CutPassed(CUT_NAME)) {
            electron = zf_event->e0;
        }

        const double ETA = electron->eta();
        const double PT = electron->pt();

        const double RECO_EFF = single_electron_efficiency_reco_.GetEfficiency(PT, ETA);
        const double MC_EFF = single_electron_efficiency_mc_.GetEfficiency(PT, ETA);

        // GetEfficiency returns -1 on failure
        if (RECO_EFF < 0 || MC_EFF < 0) {
            return;
        }

        const double SCALE_FACTOR = RECO_EFF / MC_EFF;
        electron->SetCutWeight(CUT_NAME, SCALE_FACTOR);
    }

    void ZTriggerEfficiencies::SetDoubleElectronWeight(ZFinderEvent* zf_event) {
        const std::string CUT_NAME = "trig(single_ele)";

        ZFinderElectron* e0 = zf_event->e0;
        ZFinderElectron* e1 = zf_event->e1;
        if (e0 == nullptr || e1 == nullptr) {
            return;
        }

        const double E0_ETA = e0->eta();
        const double E0_PT = e0->pt();
        const double E0_RECO_EFF = single_electron_efficiency_reco_.GetEfficiency(E0_PT, E0_ETA);
        const double E0_MC_EFF = single_electron_efficiency_mc_.GetEfficiency(E0_PT, E0_ETA);

        const double E1_ETA = e1->eta();
        const double E1_PT = e1->pt();
        const double E1_RECO_EFF = single_electron_efficiency_reco_.GetEfficiency(E1_PT, E1_ETA);
        const double E1_MC_EFF = single_electron_efficiency_mc_.GetEfficiency(E1_PT, E1_ETA);

        // Formula: (1-(1-p1_{data})(1-p2_{data})) / (1-(1-p1_{MC})(1-p2_{MC}))
        const double NUMERATOR  = 1. - ((1. - E0_RECO_EFF) * (1. - E1_RECO_EFF));
        const double DENOMENATOR  = 1. - ((1. - E0_MC_EFF) * (1. - E1_MC_EFF));

        const double SCALE_FACTOR = NUMERATOR / DENOMENATOR;
        // We set both because only one will ever be used, but we don't know
        // which
        e0->SetCutWeight(CUT_NAME, SCALE_FACTOR);
        e1->SetCutWeight(CUT_NAME, SCALE_FACTOR);
    }

    void ZTriggerEfficiencies::Setup() {
        /*
         * Here we set up the hard coded variables
         */

        // FIXME: We're using dummy variables until these are remeasured.

        // Reco
        single_electron_efficiency_reco_.AddEfficiency(30, 2000, 0, 2.2, 1., 0.0, 0.0);

        // MC
        single_electron_efficiency_mc_.AddEfficiency(30, 2000, 0, 2.2, 1., 0.0, 0.0);
    }

}  // namespace zf;
