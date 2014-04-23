#ifndef ZFINDER_ZEFFICIENCIES_H_
#define ZFINDER_ZEFFICIENCIES_H_

// Standard Library
#include <map>  // std::map
#include <string>  // std::string
#include <utility>  // std::pair
#include <vector>  // std::vector

// ZFinder
#include "ZFinder/Event/interface/EfficiencyTable.h"  // EfficiencyTable
#include "ZFinder/Event/interface/ZFinderElectron.h"  // ZFinderElectron
#include "ZFinder/Event/interface/ZFinderEvent.h"  // ZFinderEvent

namespace zf {

    class ZEfficiencies {
        public:
            // Constructor
            ZEfficiencies();

            // Apply cuts to a ZEvent
            void SetWeights(ZFinderEvent* zf_event);

        protected:
            // Get efficiencies based on a cut name, pt, and eta
            double GetEfficiency(
                    const std::string CUT_NAME,
                    const double PT,
                    const double ETA
                    ) const;

            // Store the hard coded efficiencies
            void Setup();
            EfficiencyTable ecal_tight_scale_factors_;
            EfficiencyTable ecal_medium_scale_factors_;
            EfficiencyTable ecal_loose_scale_factors_;
            EfficiencyTable ecal_veto_scale_factors_;

            // A map of cut names to an EfficiencyTable
            std::map<std::string, EfficiencyTable*> table_map_;
    };
}  // namespace zf
#endif  // ZFINDER_ZEFFICIENCIES_H_
