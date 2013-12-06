#include "ZFinder/ZDefinition/interface/ZDefinition.h"


namespace zf {
    ZDefinition::ZDefinition(
            const std::vector<std::string>* const E0_CUTS,
            const std::vector<std::string>* const E1_CUTS,
            const double MZ_MIN, const double MZ_MAX
            )
        : MZ_MIN_(MZ_MIN), MZ_MAX_(MZ_MAX), E0_CUTS_(E0_CUTS), E1_CUTS_(E1_CUTS) {
        /*
         * Save the cut values, and check that they are sane.
         */
        if ((E0_CUTS_->size()) != (E1_CUTS_->size())) {
            throw "In ZDefinition, E0_CUTS and E1_CUTS have different length!";
        }
        if (MZ_MIN_ > MZ_MAX_) {
            throw "In ZDefinition, MZ_MIN > MZ_MAX!";
        }
    }

    void ZDefinition::ApplySelection(const ZFinderEvent&) {
        /*
         * Given a ZFinderEvent, apply all steps of the selection to it.
         */

        // Loop over all cuts
        for (unsigned int i = 0; i < E0_CUTS_->size(); ++i) {
            const std::string E0_CUT = E0_CUTS_->at(i);
            const std::string E1_CUT = E1_CUTS_->at(i);
        }

    }
}  // namespace zf
