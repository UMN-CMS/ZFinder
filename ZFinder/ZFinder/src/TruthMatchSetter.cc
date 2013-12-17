#include "ZFinder/ZFinder/interface/TruthMatchSetter.h"

// Standard Library
#include <vector>  // std::vector

// CMSSW
#include "DataFormats/Math/interface/deltaR.h"  // deltaR


namespace zf {
    void TruthMatchSetter::SetCuts(ZFinderEvent* zf_event) {
        is_real_data = zf_event->is_real_data; 
        zf_event_ = zf_event;

        // Loop over all electrons and call SetCut_
        std::vector<ZFinderElectron*>* zf_electrons = zf_event->FilteredElectrons();
        for (std::vector<ZFinderElectron*>::iterator i_elec = zf_electrons->begin(); i_elec != zf_electrons->end(); ++i_elec) {
            ZFinderElectron* zf_elec = *i_elec;
            SetCut_(zf_elec);
        }
    }

    void TruthMatchSetter::SetCut_(ZFinderElectron* zf_elec) {
        const double WEIGHT = 1.;
        // Now set all cuts
        zf_elec->AddCutResult("dr(0.05)", MatchTruth(*zf_elec, 0.05), WEIGHT);
        zf_elec->AddCutResult("dr(0.1)", MatchTruth(*zf_elec, 0.1), WEIGHT);
        zf_elec->AddCutResult("dr(0.2)", MatchTruth(*zf_elec, 0.2), WEIGHT);
        zf_elec->AddCutResult("dr(0.3)", MatchTruth(*zf_elec, 0.3), WEIGHT);
        zf_elec->AddCutResult("dr(0.4)", MatchTruth(*zf_elec, 0.4), WEIGHT);
        zf_elec->AddCutResult("dr(0.5)", MatchTruth(*zf_elec, 0.5), WEIGHT);

    }

    bool TruthMatchSetter::MatchTruth(const ZFinderElectron& ZF_ELEC, const double DELTA_R) {
        // Real data has no gen, so can't possibly match!    
        if (is_real_data) {
            return false;
        }
        // Try to match to one of the two Truth particles
        double dr0 = 9999.;
        double dr1 = 9999.;
        if (zf_event_->e0_truth != NULL) {
            dr0 = deltaR(ZF_ELEC.eta, ZF_ELEC.phi, zf_event_->e0->eta, zf_event_->e0->phi);
        }
        if (zf_event_->e1_truth != NULL) {
            dr1 = deltaR(ZF_ELEC.eta, ZF_ELEC.phi, zf_event_->e1->eta, zf_event_->e1->phi);
        }

        if (dr0 < DELTA_R || dr1 < DELTA_R) {
            return true;
        } else {
            return false;        
        }
    }
}  // namespace zf
