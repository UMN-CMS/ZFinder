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
            if (PassSingleElectronTriggerRequirements(e0)) {
                passing_electrons++;
            }
        }
        if (e1 != nullptr) {
            if (PassSingleElectronTriggerRequirements(e1)) {
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
        ZFinderElectron* electron = nullptr;
        // The first one is the right one, because we know only one passes
        if (zf_event->e0 != nullptr) {
            if (PassSingleElectronTriggerRequirements(zf_event->e0)) {
                electron = zf_event->e0;
            }
        }
        else if (zf_event->e1 != nullptr) {
            if (PassSingleElectronTriggerRequirements(zf_event->e1)) {
                electron = zf_event->e1;
            }
        }

        if (electron != nullptr) {
            const double ETA = electron->eta();
            const double PT = electron->pt();

            const double RECO_EFF = single_electron_efficiency_reco_.GetEfficiency(PT, ETA);
            const double MC_EFF = single_electron_efficiency_mc_.GetEfficiency(PT, ETA);

            // GetEfficiency returns -1 on failure
            if (RECO_EFF < 0 || MC_EFF < 0) {
                return;
            }

            const double SCALE_FACTOR = RECO_EFF / MC_EFF;
            const std::string CUT_NAME = "trig(single_ele)";
            electron->SetCutWeight(CUT_NAME, SCALE_FACTOR);
        }
    }

    void ZTriggerEfficiencies::SetDoubleElectronWeight(ZFinderEvent* zf_event) {
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
        const std::string CUT_NAME = "trig(single_ele)";
        // We set both because only one will ever be used, but we don't know
        // which
        e0->SetCutWeight(CUT_NAME, SCALE_FACTOR);
        e1->SetCutWeight(CUT_NAME, SCALE_FACTOR);
    }

    bool ZTriggerEfficiencies::PassSingleElectronTriggerRequirements(ZFinderElectron const * const electron) const {
        return (electron->pt() >= 30
                && fabs(electron->eta()) <= 2.1
                && electron->CutPassed("eg_tight") == 1
                && electron->CutPassed("trig(single_ele)") == 1);
    }

    void ZTriggerEfficiencies::Setup() {
        /*
         * Here we set up the hard coded variables
         */

        // For the single electron trigger, ZFinder/TrigEff is used to
        // calculate them, and convert.py is used to write the code to insure
        // accurate transcription.

        // Reco Efficiency, uncertainties are statistical only
        single_electron_efficiency_reco_.AddEfficiency(30, 40, -0.8, 0, 0.9193, 0.0003, 0.0003);
        single_electron_efficiency_reco_.AddEfficiency(30, 40, -1.442, -0.8, 0.9106, 0.0003, 0.0003);
        single_electron_efficiency_reco_.AddEfficiency(30, 40, -1.556, -1.442, 0.8838, 0.0005, 0.0005);
        single_electron_efficiency_reco_.AddEfficiency(30, 40, -2, -1.556, 0.733, 0.001, 0.001);
        single_electron_efficiency_reco_.AddEfficiency(30, 40, -2.1, -2, 0.735, 0.001, 0.001);
        single_electron_efficiency_reco_.AddEfficiency(30, 40, 0, 0.8, 0.9095, 0.0003, 0.0003);
        single_electron_efficiency_reco_.AddEfficiency(30, 40, 0.8, 1.442, 0.8866, 0.0005, 0.0005);
        single_electron_efficiency_reco_.AddEfficiency(30, 40, 1.442, 1.556, 0.752, 0.001, 0.001);
        single_electron_efficiency_reco_.AddEfficiency(30, 40, 1.556, 2, 0.759, 0.001, 0.001);
        single_electron_efficiency_reco_.AddEfficiency(30, 40, 2, 2.1, 0.764, 0.003, 0.003);
        single_electron_efficiency_reco_.AddEfficiency(40, 50, -0.8, 0, 0.9500, 0.0002, 0.0002);
        single_electron_efficiency_reco_.AddEfficiency(40, 50, -1.442, -0.8, 0.9478, 0.0002, 0.0002);
        single_electron_efficiency_reco_.AddEfficiency(40, 50, -1.556, -1.442, 0.9332, 0.0004, 0.0004);
        single_electron_efficiency_reco_.AddEfficiency(40, 50, -2, -1.556, 0.7793, 0.0010, 0.0010);
        single_electron_efficiency_reco_.AddEfficiency(40, 50, -2.1, -2, 0.772, 0.001, 0.001);
        single_electron_efficiency_reco_.AddEfficiency(40, 50, 0, 0.8, 0.9455, 0.0002, 0.0002);
        single_electron_efficiency_reco_.AddEfficiency(40, 50, 0.8, 1.442, 0.9360, 0.0003, 0.0004);
        single_electron_efficiency_reco_.AddEfficiency(40, 50, 1.442, 1.556, 0.8034, 0.0009, 0.0009);
        single_electron_efficiency_reco_.AddEfficiency(40, 50, 1.556, 2, 0.7987, 0.0009, 0.0010);
        single_electron_efficiency_reco_.AddEfficiency(40, 50, 2, 2.1, 0.792, 0.002, 0.002);
        single_electron_efficiency_reco_.AddEfficiency(50, 70, -0.8, 0, 0.9581, 0.0003, 0.0003);
        single_electron_efficiency_reco_.AddEfficiency(50, 70, -1.442, -0.8, 0.9566, 0.0003, 0.0004);
        single_electron_efficiency_reco_.AddEfficiency(50, 70, -1.556, -1.442, 0.9411, 0.0006, 0.0006);
        single_electron_efficiency_reco_.AddEfficiency(50, 70, -2, -1.556, 0.790, 0.002, 0.002);
        single_electron_efficiency_reco_.AddEfficiency(50, 70, -2.1, -2, 0.785, 0.002, 0.002);
        single_electron_efficiency_reco_.AddEfficiency(50, 70, 0, 0.8, 0.9550, 0.0004, 0.0004);
        single_electron_efficiency_reco_.AddEfficiency(50, 70, 0.8, 1.442, 0.9449, 0.0006, 0.0006);
        single_electron_efficiency_reco_.AddEfficiency(50, 70, 1.442, 1.556, 0.814, 0.002, 0.002);
        single_electron_efficiency_reco_.AddEfficiency(50, 70, 1.556, 2, 0.809, 0.002, 0.002);
        single_electron_efficiency_reco_.AddEfficiency(50, 70, 2, 2.1, 0.797, 0.005, 0.005);
        single_electron_efficiency_reco_.AddEfficiency(70, 250, -0.8, 0, 0.9660, 0.0007, 0.0007);
        single_electron_efficiency_reco_.AddEfficiency(70, 250, -1.442, -0.8, 0.9679, 0.0008, 0.0008);
        single_electron_efficiency_reco_.AddEfficiency(70, 250, -1.556, -1.442, 0.958, 0.001, 0.001);
        single_electron_efficiency_reco_.AddEfficiency(70, 250, -2, -1.556, 0.802, 0.005, 0.005);
        single_electron_efficiency_reco_.AddEfficiency(70, 250, -2.1, -2, 0.792, 0.005, 0.005);
        single_electron_efficiency_reco_.AddEfficiency(70, 250, 0, 0.8, 0.9641, 0.0008, 0.0008);
        single_electron_efficiency_reco_.AddEfficiency(70, 250, 0.8, 1.442, 0.959, 0.001, 0.001);
        single_electron_efficiency_reco_.AddEfficiency(70, 250, 1.442, 1.556, 0.830, 0.004, 0.004);
        single_electron_efficiency_reco_.AddEfficiency(70, 250, 1.556, 2, 0.822, 0.004, 0.005);
        single_electron_efficiency_reco_.AddEfficiency(70, 250, 2, 2.1, 0.82, 0.01, 0.01);

        // Madgraph is used for the MC
        // MC Efficiency, uncertainties are statistical only
        single_electron_efficiency_mc_.AddEfficiency(30, 40, -0.8, 0, 0.9467, 0.0003, 0.0003);
        single_electron_efficiency_mc_.AddEfficiency(30, 40, -1.442, -0.8, 0.9441, 0.0004, 0.0004);
        single_electron_efficiency_mc_.AddEfficiency(30, 40, -1.556, -1.442, 0.9306, 0.0007, 0.0007);
        single_electron_efficiency_mc_.AddEfficiency(30, 40, -2, -1.556, 0.744, 0.002, 0.002);
        single_electron_efficiency_mc_.AddEfficiency(30, 40, -2.1, -2, 0.736, 0.002, 0.002);
        single_electron_efficiency_mc_.AddEfficiency(30, 40, 0, 0.8, 0.9428, 0.0004, 0.0004);
        single_electron_efficiency_mc_.AddEfficiency(30, 40, 0.8, 1.442, 0.9272, 0.0007, 0.0007);
        single_electron_efficiency_mc_.AddEfficiency(30, 40, 1.442, 1.556, 0.753, 0.002, 0.002);
        single_electron_efficiency_mc_.AddEfficiency(30, 40, 1.556, 2, 0.747, 0.002, 0.002);
        single_electron_efficiency_mc_.AddEfficiency(30, 40, 2, 2.1, 0.737, 0.004, 0.004);
        single_electron_efficiency_mc_.AddEfficiency(40, 50, -0.8, 0, 0.9667, 0.0002, 0.0002);
        single_electron_efficiency_mc_.AddEfficiency(40, 50, -1.442, -0.8, 0.9648, 0.0003, 0.0003);
        single_electron_efficiency_mc_.AddEfficiency(40, 50, -1.556, -1.442, 0.9548, 0.0004, 0.0005);
        single_electron_efficiency_mc_.AddEfficiency(40, 50, -2, -1.556, 0.780, 0.001, 0.001);
        single_electron_efficiency_mc_.AddEfficiency(40, 50, -2.1, -2, 0.768, 0.001, 0.001);
        single_electron_efficiency_mc_.AddEfficiency(40, 50, 0, 0.8, 0.9639, 0.0003, 0.0003);
        single_electron_efficiency_mc_.AddEfficiency(40, 50, 0.8, 1.442, 0.9531, 0.0005, 0.0005);
        single_electron_efficiency_mc_.AddEfficiency(40, 50, 1.442, 1.556, 0.794, 0.001, 0.001);
        single_electron_efficiency_mc_.AddEfficiency(40, 50, 1.556, 2, 0.783, 0.001, 0.001);
        single_electron_efficiency_mc_.AddEfficiency(40, 50, 2, 2.1, 0.769, 0.004, 0.004);
        single_electron_efficiency_mc_.AddEfficiency(50, 70, -0.8, 0, 0.9742, 0.0004, 0.0004);
        single_electron_efficiency_mc_.AddEfficiency(50, 70, -1.442, -0.8, 0.9723, 0.0004, 0.0004);
        single_electron_efficiency_mc_.AddEfficiency(50, 70, -1.556, -1.442, 0.9614, 0.0008, 0.0008);
        single_electron_efficiency_mc_.AddEfficiency(50, 70, -2, -1.556, 0.791, 0.003, 0.003);
        single_electron_efficiency_mc_.AddEfficiency(50, 70, -2.1, -2, 0.778, 0.003, 0.003);
        single_electron_efficiency_mc_.AddEfficiency(50, 70, 0, 0.8, 0.9711, 0.0004, 0.0005);
        single_electron_efficiency_mc_.AddEfficiency(50, 70, 0.8, 1.442, 0.9594, 0.0008, 0.0008);
        single_electron_efficiency_mc_.AddEfficiency(50, 70, 1.442, 1.556, 0.805, 0.003, 0.003);
        single_electron_efficiency_mc_.AddEfficiency(50, 70, 1.556, 2, 0.795, 0.003, 0.003);
        single_electron_efficiency_mc_.AddEfficiency(50, 70, 2, 2.1, 0.779, 0.007, 0.008);
        single_electron_efficiency_mc_.AddEfficiency(70, 250, -0.8, 0, 0.9820, 0.0008, 0.0009);
        single_electron_efficiency_mc_.AddEfficiency(70, 250, -1.442, -0.8, 0.9814, 0.0009, 0.0010);
        single_electron_efficiency_mc_.AddEfficiency(70, 250, -1.556, -1.442, 0.974, 0.002, 0.002);
        single_electron_efficiency_mc_.AddEfficiency(70, 250, -2, -1.556, 0.802, 0.007, 0.007);
        single_electron_efficiency_mc_.AddEfficiency(70, 250, -2.1, -2, 0.784, 0.007, 0.008);
        single_electron_efficiency_mc_.AddEfficiency(70, 250, 0, 0.8, 0.9806, 0.0010, 0.0010);
        single_electron_efficiency_mc_.AddEfficiency(70, 250, 0.8, 1.442, 0.971, 0.002, 0.002);
        single_electron_efficiency_mc_.AddEfficiency(70, 250, 1.442, 1.556, 0.817, 0.007, 0.007);
        single_electron_efficiency_mc_.AddEfficiency(70, 250, 1.556, 2, 0.811, 0.007, 0.007);
        single_electron_efficiency_mc_.AddEfficiency(70, 250, 2, 2.1, 0.82, 0.02, 0.02);
    }

}  // namespace zf;
