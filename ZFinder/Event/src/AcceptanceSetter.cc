#include "ZFinder/Event/interface/AcceptanceSetter.h"

// Standard Library
#include <string>  // std::string
#include <vector>  // std::vector


namespace zf {
    void AcceptanceSetter::SetCuts(ZFinderEvent* zf_event) {
        // Loop over all electrons and call SetCut_
        std::vector<ZFinderElectron*>* zf_electrons = zf_event->FilteredElectrons();
        for (auto& i_elec : *zf_electrons) {
            SetCut_(i_elec);
        }
        // Also MC
        if (zf_event->e0_truth != nullptr) {
            SetCut_(zf_event->e0_truth);
        }
        if (zf_event->e1_truth != nullptr) {
            SetCut_(zf_event->e1_truth);
        }
    }

    void AcceptanceSetter::SetCut_(ZFinderElectron* zf_elec) {
        const double WEIGHT = 1.;
        const double ETA = zf_elec->eta();
        const double FETA = fabs(zf_elec->eta());
        // All the bools
        bool detector = false;
        bool eb = false;
        bool ebm = false;
        bool ebp = false;
        bool ee = false;
        bool eem = false;
        bool eep = false;
        bool et = false;
        bool etp = false;
        bool etm = false;
        bool hf = false;
        bool hfm = false;
        bool hfp = false;
        bool nt = false;
        bool ntm = false;
        bool ntp = false;
        bool muon_tight = false;
        bool muon_tight_plus = false;
        bool muon_tight_minus = false;
        bool muon_loose = false;
        bool muon_loose_plus = false;
        bool muon_loose_minus = false;
        // EB
        if (EB_MIN_ < ETA && ETA < EB_MAX_) {  // In EB
            detector = true;
            et = true;
            eb = true;
            if (EB_CENT_ < ETA && ETA < EB_MAX_) {  // In EB+
                ebp = true;
                etp = true;
            } else if (EB_MIN_ < ETA && ETA < EB_CENT_) {  // In EB-
                ebm = true;
                etm = true;
            }
        }
        // EE and NT
        else if (EEP_MIN_ < FETA && FETA < EEP_MAX_) {  // In EE
            detector = true;
            et = true;
            ee = true;
            if (EEP_MIN_ < ETA && ETA < EEP_MAX_) {  // In EE+
                eep = true;
                etp = true;
                if (NTP_MIN_ < ETA && ETA < NTP_MAX_) {  // In NT+
                    nt = true;
                    ntp = true;
                }
            } else if (EEM_MIN_ < ETA && ETA < EEM_MAX_) {  // In EE-
                eem = true;
                etm = true;
                if (NTM_MIN_ < ETA && ETA < NTM_MAX_) {  // In NT-
                    nt = true;
                    ntp = true;
                }
            }
        }
        // HF
        else if (HFP_MIN_ < FETA && FETA < HFP_MAX_) {  // In HF
            detector = true;
            hf = true;
            if (HFP_MIN_ < ETA && ETA < HFP_MAX_) {  // In HF+
                hfp = true;
            } else if (HFM_MIN_ < ETA && ETA < HFM_MAX_) {  // In HF-
                hfm = true;
            }
        }
        // Selection for combination with the muon group, we want to reject in
        // the gap between EB and EE, so we also require either eb or ee
        if (FETA < MUON_LOOSE_MAX_ && (eb || ee)) {  // In ML
            detector = true;
            muon_loose = true;
            if (EB_CENT_ < ETA && ETA < MUON_LOOSE_MAX_) {  // In ML+
                muon_loose_plus = true;
            } else if (MUON_LOOSE_MIN_ < ETA && ETA < EB_CENT_) {  // In ML-
                muon_loose_minus = true;
            }
            if (FETA < MUON_TIGHT_MAX_) {  // In MT
                muon_tight = true;
                if (EB_CENT_ < ETA && ETA < MUON_TIGHT_MAX_) {  // In MT+
                    muon_tight_plus = true;
                } else if (MUON_TIGHT_MIN_ < ETA && ETA < EB_CENT_) {  // In MT-
                    muon_tight_minus = true;
                }
            }
        }

        // A special cut to start: ALL is now used to track all events in the
        // sample, which is useful for MC normalization based on the number of
        // events and cross section. The former definition of ALL (that is, an
        // electron that is anywhere in the detector region) has now taken over
        // by DETECTOR.
        zf_elec->AddCutResult("acc(ALL)", true, WEIGHT);
        // Set the cuts based on detector acceptance
        zf_elec->AddCutResult("acc(DETECTOR)", detector, WEIGHT);
        zf_elec->AddCutResult("acc(EB)", eb, WEIGHT);
        zf_elec->AddCutResult("acc(EB+)", ebp, WEIGHT);
        zf_elec->AddCutResult("acc(EB-)", ebm, WEIGHT);
        zf_elec->AddCutResult("acc(EE)", ee, WEIGHT);
        zf_elec->AddCutResult("acc(EE+)", eep, WEIGHT);
        zf_elec->AddCutResult("acc(EE-)", eem, WEIGHT);
        zf_elec->AddCutResult("acc(ET)", et, WEIGHT);
        zf_elec->AddCutResult("acc(ET+)", etp, WEIGHT);
        zf_elec->AddCutResult("acc(ET-)", etm, WEIGHT);
        zf_elec->AddCutResult("acc(HF)", hf, WEIGHT);
        zf_elec->AddCutResult("acc(HF+)", hfp, WEIGHT);
        zf_elec->AddCutResult("acc(HF-)", hfm, WEIGHT);
        zf_elec->AddCutResult("acc(NT)", nt, WEIGHT);
        zf_elec->AddCutResult("acc(NT+)", ntp, WEIGHT);
        zf_elec->AddCutResult("acc(NT-)", ntm, WEIGHT);
        zf_elec->AddCutResult("acc(MUON_TIGHT)", muon_tight, WEIGHT);
        zf_elec->AddCutResult("acc(MUON_TIGHT+)", muon_tight_plus, WEIGHT);
        zf_elec->AddCutResult("acc(MUON_TIGHT-)", muon_tight_minus, WEIGHT);
        zf_elec->AddCutResult("acc(MUON_LOOSE)", muon_loose, WEIGHT);
        zf_elec->AddCutResult("acc(MUON_LOOSE+)", muon_loose_plus, WEIGHT);
        zf_elec->AddCutResult("acc(MUON_LOOSE-)", muon_loose_minus, WEIGHT);

    }
}  // namespace zf
