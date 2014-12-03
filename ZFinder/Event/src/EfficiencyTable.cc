#include "ZFinder/Event/interface/EfficiencyTable.h"

// Standard Library
#include <cmath>  // fabs
#include <iostream>  // std::cout, std::endl


namespace zf {

    EfficiencyTable::EfficiencyTable() {
        // Do nothing
    }

    void EfficiencyTable::AddEfficiency(
            const double PT_MIN,
            const double PT_MAX,
            const double ETA_MIN,
            const double ETA_MAX,
            const double EFFICIENCY,
            const double POSITIVE_ERROR_BAR,
            const double NEGATIVE_ERROR_BAR
            ) {
        /*
         * Insert a row from the efficiency tables into our object. If no error
         * bars are specified, they default to -1.
         */
        if (ETA_MIN >= ETA_MAX) {
            std::cout << "Adding EfficiencyTable entry with inverted eta!";
            std::cout << std::endl;
        }
        if (PT_MIN >= PT_MAX) {
            std::cout << "Adding EfficiencyTable entry with inverted pt!";
            std::cout << std::endl;
        }
        coordinate_pair pt_pair(PT_MIN, PT_MAX);
        coordinate_pair eta_pair(ETA_MIN, ETA_MAX);
        Efficiency eff(EFFICIENCY, POSITIVE_ERROR_BAR, NEGATIVE_ERROR_BAR);

        // Insert the efficiency into the map of maps
        eff_table_[eta_pair][pt_pair] = eff;
    }

    double EfficiencyTable::GetEfficiency(
            const double PT,
            const double ETA
            ) const {
        /*
         * Finds the efficiency for a given pt and eta bin. If no match exists,
         * instead returns -1. If, for some reason, there are overlapping bins
         * in eta and pt, then it selects the first bin.
         */
        // Check all eta bins
        for (auto& outter_pair : eff_table_) {
            const coordinate_pair ETA_PAIR = outter_pair.first;
            if (ETA_PAIR.first <= ETA && ETA < ETA_PAIR.second) {
                // Check all pt bins in the map from the matching eta bin
                for (auto& inner_pair : outter_pair.second) {
                    const coordinate_pair PT_PAIR = inner_pair.first;
                    if (PT_PAIR.first <= PT && PT < PT_PAIR.second) {
                        return inner_pair.second.efficiency;
                    }
                }
            }
        }

        // No match found
        return -1.;
    }

}  // namespace zf
