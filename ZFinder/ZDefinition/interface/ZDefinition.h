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
                    const std::vector<std::string>& E0_CUTS,
                    const std::vector<std::string>& E1_CUTS,
                    const double MZ_MIN,
                    const double MZ_MAX
                    );

            void ApplySelection(ZFinderEvent* zf_event);

        protected:
            void InitVariables();

            bool IsComparison(const std::string* TEST_STRING);

            // MZ cuts
            const double MZ_MIN_;
            const double MZ_MAX_;
            std::vector<std::string> E0_CUTS_;
            std::vector<std::string> E1_CUTS_;

            // Internal Logic
            std::vector<bool> e0_invert;
            std::vector<bool> e1_invert;
            std::vector<bool> e0_iscomp;
            std::vector<bool> e1_iscomp;
            
            // Cut Results
            std::vector<bool> e0_pass;
            std::vector<bool> e1_pass;

            // Handle Cut Checking
            bool ComparisonCut(const bool INVERT, ZFinderEvent* zf_event);
            bool NormalCut(const bool INVERT, ZFinderEvent* zf_event);

    };
}  // namespace zf
#endif  // ZDEFINITION_ZDEFINITION_H_
