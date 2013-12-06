#include "ZFinder/ZDefinition/interface/ZDefinition.h"

// Standard Libraries
#include <algorithm>  // std::fill


namespace zf {
    ZDefinition::ZDefinition(
            const std::vector<std::string>& E0_CUTS,
            const std::vector<std::string>& E1_CUTS,
            const double MZ_MIN, const double MZ_MAX
            )
        : MZ_MIN_(MZ_MIN), MZ_MAX_(MZ_MAX), E0_CUTS_(E0_CUTS), E1_CUTS_(E1_CUTS) {
            /*
             * Save the cut values, and check that they are sane.
             */
            if ((E0_CUTS_.size()) != (E1_CUTS_.size())) {
                throw "In ZDefinition, E0_CUTS and E1_CUTS have different length!";
            }
            if (MZ_MIN_ > MZ_MAX_) {
                throw "In ZDefinition, MZ_MIN > MZ_MAX!";
            }

            // Set initial variables values
            InitVariables();

            // Loop over all cuts
            for (unsigned int i = 0; i < E0_CUTS_.size(); ++i) {
                std::string* E0_CUT = &E0_CUTS_.at(i);
                std::string* E1_CUT = &E1_CUTS_.at(i);

                // We use a ! flag in the [0] slot to invert the cut
                if ((*E0_CUT)[0] == '!') {
                    e0_invert[i] = true;
                    E0_CUT->erase(0, 1);
                }
                if ((*E1_CUT)[0] == '!') {
                    e1_invert[i] = true;
                    E1_CUT->erase(0, 1);
                }

                // If there is an =, >, < in the string we use it to cut on a value
                // from the ZElectron, otherwise we assume it is a cut and pass it
                // through
                e0_iscomp[i] = IsComparison(E0_CUT);
                e1_iscomp[i] = IsComparison(E1_CUT);
            }
        }

    void ZDefinition::InitVariables() {
        std::fill(e0_invert.begin(), e0_invert.end(), false);
        std::fill(e1_invert.begin(), e1_invert.end(), false);
        std::fill(e0_iscomp.begin(), e0_iscomp.end(), false);
        std::fill(e1_iscomp.begin(), e1_iscomp.end(), false);
    }

    void ZDefinition::ApplySelection(ZFinderEvent* zf_event) {
        /*
         * Given a ZFinderEvent, apply all steps of the selection to it.
         */
        for (unsigned int i = 0; i < E0_CUTS_.size(); ++i) {
            // Check e0
            if (e0_iscomp[i]) {
               e0_pass[i] = ComparisonCut(e0_invert[i], zf_event);     
            } else {
               e0_pass[i] = NormalCut(e0_invert[i], zf_event);     
            }
            // Check e1
            if (e0_iscomp[i]) {
               e0_pass[i] = ComparisonCut(e0_invert[i], zf_event);     
            } else {
               e0_pass[i] = NormalCut(e0_invert[i], zf_event);     
            }
        }
    }

    bool ZDefinition::NormalCut(const bool INVERT, ZFinderEvent* zf_event) {
        // TODO: Add logic
        return false;
    }

    bool ZDefinition::ComparisonCut(const bool INVERT, ZFinderEvent* zf_event) {
        // TODO: Add logic
        return false;
    }

    bool ZDefinition::IsComparison(const std::string* TEST_STRING) {
        using std::string;
        /*
         * Return true if '<', '>', or '=' is in the string, otherwise false.
         */
        return (   TEST_STRING->find("<") != string::npos
                || TEST_STRING->find(">") != string::npos
                || TEST_STRING->find("=") != string::npos);
    }
}  // namespace zf
