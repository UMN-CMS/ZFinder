#ifndef ZFINDER_TRUTHMATCHSETTER_H_
#define ZFINDER_TRUTHMATCHSETTER_H_

// ZFinder Code
#include "SetterBase.h"  // SetterBase


namespace zf {
    class TruthMatchSetter: public SetterBase {
        public:
            // Constructor
            TruthMatchSetter() { }

            // Call this method on your ZFinderEvent and it will set cut for
            // all electrons using the SetCut_ method
            void SetCuts(ZFinderEvent* zf_event);

        protected:
            void SetCut_(ZFinderElectron* zf_elec);
            bool is_real_data;

            ZFinderEvent* zf_event_;

            // Check for matching Truth Electron
            bool MatchTruth(const ZFinderElectron& ZF_ELEC, const double DELTA_R);

    };
}  // namespace zf
#endif  // ZFINDER_TRUTHMATCHSETTER_H_
