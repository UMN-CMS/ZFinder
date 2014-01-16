#include "ZFinder/Event/interface/SetterBase.h"

// Standard Library
#include <string>  // std::string
#include <vector>  // std::vector


namespace zf {
    void SetterBase::SetCuts(ZFinderEvent* zf_event) {
        // Loop over all electrons and call SetCut_

        std::vector<ZFinderElectron*>* zf_electrons = zf_event->FilteredElectrons();
        for (std::vector<ZFinderElectron*>::iterator i_elec = zf_electrons->begin(); i_elec != zf_electrons->end(); ++i_elec) {
            ZFinderElectron* zf_elec = *i_elec;
            SetCut_(zf_elec);
        }
    }
}  // namespace zf
