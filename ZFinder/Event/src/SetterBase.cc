#include "ZFinder/Event/interface/SetterBase.h"

// Standard Library
#include <string>  // std::string
#include <vector>  // std::vector


namespace zf {
    void SetterBase::SetCuts(ZFinderEvent* zf_event) {
        // Loop over all electrons and call SetCut_

        std::vector<ZFinderElectron*>* zf_electrons = zf_event->FilteredElectrons();
        for (auto& i_elec : *zf_electrons) {
            SetCut_(i_elec);
        }
    }
}  // namespace zf
