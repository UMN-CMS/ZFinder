#ifndef ZDEFINITION_ZDEFINITION_H_
#define ZDEFINITION_ZDEFINITION_H_

// Standard Library
#include <string>  // string
#include <vector>  // vector

// ZFinder
#include "ZFinder/ZFinder/interface/ZFinderEvent.h"  // ZFinderEvent

namespace zf {
    class ZDefinition{
        public:
            ZDefinition(
                    const std::vector<std::string>* const E0_CUTS,
                    const std::vector<std::string>* const E1_CUTS,
                    const double MZ_MIN,
                    const double MZ_MAX
                    );

            void ApplySelection(const ZFinderEvent&);

        protected:
            // MZ cuts
            const double MZ_MIN_;
            const double MZ_MAX_;
            const std::vector<std::string>* const E0_CUTS_;
            const std::vector<std::string>* const E1_CUTS_;

    };
}  // namespace zf
#endif  // ZDEFINITION_ZDEFINITION_H_
