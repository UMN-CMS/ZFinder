#ifndef ZFINDER_ZTRIGGEREFFICIENCIES_H_
#define ZFINDER_ZTRIGGEREFFICIENCIES_H_

// Standard Library
#include <string>  // std::string
#include <vector>  // std::vector

// ZFinder
#include "ZFinder/Event/interface/EfficiencyTable.h"  // EfficiencyTable
#include "ZFinder/Event/interface/ZFinderElectron.h"  // ZFinderElectron
#include "ZFinder/Event/interface/ZFinderEvent.h"  // ZFinderEvent

namespace zf {

    class ZTriggerEfficiencies {
        public:
            // Constructor
            ZTriggerEfficiencies();

            // Apply cuts to a ZEvent
            void SetWeights(ZFinderEvent* zf_event);

        protected:
            // Set the weights for events with 1 or 2 passing electrons
            void SetDoubleElectronWeight(ZFinderEvent* zf_event);
            void SetSingleElectronWeight(ZFinderEvent* zf_event);

            // Store the hard coded efficiencies
            void Setup();
            EfficiencyTable single_electron_efficiency_reco_;
            EfficiencyTable single_electron_efficiency_mc_;

    };
}  // namespace zf
#endif  // ZFINDER_ZTRIGGEREFFICIENCIES_H_
