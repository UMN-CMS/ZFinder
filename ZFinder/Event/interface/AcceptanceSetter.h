#ifndef ZFINDER_ACCEPTANCESETTER_H_
#define ZFINDER_ACCEPTANCESETTER_H_

// ZFinder Code
#include "SetterBase.h"  // SetterBase


namespace zf {
    class AcceptanceSetter: public SetterBase {
        public:
            // Constructor
            AcceptanceSetter() { }

            // Call this method on your ZFinderEvent and it will set cut for
            // all electrons using the SetCut_ method
            void SetCuts(ZFinderEvent* zf_event);

        protected:
            void SetCut_(ZFinderElectron* zf_elec);

            /* Detector Locations
             *
             * We define it so that you can always check
             * X_MIN_ < ETA && ETA < X_MAX_, hence the minus side minimum is
             * equal to the negative plus side max.
             */
            static constexpr double EB_MAX_ = 1.4442;
            static constexpr double EB_MIN_ = -EB_MAX_;
            static constexpr double EB_CENT_ = 0;
            static constexpr double EEP_MIN_ = 1.56;
            static constexpr double EEP_MAX_ = 2.850;
            static constexpr double EEM_MIN_ = -EEP_MAX_;
            static constexpr double EEM_MAX_ = -EEP_MIN_;
            static constexpr double NTP_MIN_ = 2.5;
            static constexpr double NTP_MAX_ = EEP_MAX_;
            static constexpr double NTM_MIN_ = -NTP_MAX_;
            static constexpr double NTM_MAX_ = -NTP_MIN_;
            static constexpr double HFP_MIN_ = 3.1;
            static constexpr double HFP_MAX_ = 4.6;
            static constexpr double HFM_MIN_ = -HFP_MAX_;
            static constexpr double HFM_MAX_ = -HFP_MIN_;
            static constexpr double MUON_TIGHT_MAX_ = 2.1;
            static constexpr double MUON_TIGHT_MIN_ = -MUON_TIGHT_MAX_;
            static constexpr double MUON_LOOSE_MAX_ = 2.4;
            static constexpr double MUON_LOOSE_MIN_ = -MUON_LOOSE_MAX_;
    };
}  // namespace zf
#endif  // ZFINDER_ACCEPTANCESETTER_H_
