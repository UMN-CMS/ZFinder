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
        if (zf_event->e0_truth != NULL) {
            SetCut_(zf_event->e0_truth);
        }
        if (zf_event->e1_truth != NULL) {
            SetCut_(zf_event->e1_truth);
        }
    }

    void AcceptanceSetter::SetCut_(ZFinderElectron* zf_elec) {
        const double WEIGHT = 1.;
        const double ETA = zf_elec->eta;
        const double FETA = fabs(zf_elec->eta);
        // All the bools
        bool all = false;
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
        // EB
        if (EB_MIN_ < ETA && ETA < EB_MAX_) {  // In EB
            all = true;
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
            all = true;
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
            all = true;
            hf = true;
            if (HFP_MIN_ < ETA && ETA < HFP_MAX_) {  // In HF+
                hfp = true;
            } else if (HFM_MIN_ < ETA && ETA < HFM_MAX_) {  // In HF-
                hfm = true;
            }
        }

        // Now set all cuts
        zf_elec->AddCutResult("acc(ALL)", all, WEIGHT);
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
    }
}  // namespace zf
