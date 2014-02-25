#ifndef ZDEFINITION_ZDEFINITION_H_
#define ZDEFINITION_ZDEFINITION_H_

// Standard Library
#include <string>  // string
#include <vector>  // vector
#include <utility>  // std::pair

// ZFinder
#include "ZFinder/Event/interface/ZFinderEvent.h"  // ZFinderEvent, cutlevel_vector
#include "ZFinder/Event/interface/ZFinderElectron.h"  // ZFinderElectron


namespace zf {

    class ZDefinition{
        public:
            ZDefinition(
                    const std::string NAME,
                    const std::vector<std::string>& CUTS0,
                    const std::vector<std::string>& CUTS1,
                    const double MZ_MIN,
                    const double MZ_MAX
                    );

            void ApplySelection(ZFinderEvent* zf_event);

            // Making clv and NAME public so that other classes can find out about the
            // cuts it contains.
            cutlevel_vector clv;
            const std::string NAME;

        protected:
            void InitVariables(const size_t SIZE);
            void InitCutlevelVector(const size_t SIZE);

            // MZ cuts
            const double MZ_MIN_;
            const double MZ_MAX_;
            bool pass_mz_cut_;

            // Comparison Cut Types
            enum ComparisonType {
                CT_NONE,   // Not a comparison
                CT_EQUAL,  // ==
                CT_GT,     // >
                CT_LT,     // <
                CT_GTE,    // >=
                CT_LTE     // <=
            };

            // Comparison Cut Variable, G indicates a generator quantity
            enum ComparisonVariable {
                CV_NONE,   // Not a comparison
                CV_PT,
                CV_GPT,
                CV_TPT,
                CV_ETA,
                CV_GETA,
                CV_TETA,
                CV_PHI,
                CV_GPHI,
                CV_TPHI,
                CV_CHARGE,
                CV_GCHARGE,
                CV_TCHARGE
            };

            ComparisonType GetComparisonType(const std::string* cut);
            ComparisonVariable GetComparisonVariable(const std::string* cut);
            double GetComparisonValue(const std::string* cut);

            struct CutInfo{
                std::string cut;
                bool invert;
                ComparisonType comp_type;
                ComparisonVariable comp_var;
                double comp_val;
            };

            std::vector<CutInfo> cutinfo_[2];
            /* Cut Results
             *
             * We use two sets: pass_[0][0] is for cut set 0 with zf_event->e0,
             *pass_[0][1] is for cut set 0 with zf_event->e1, etc.
             */
            std::vector<bool> pass_[2][2];

            // Handle Cut Checking
            bool ComparisonCut(const CutInfo& CUTINFO, const int I_ELEC, ZFinderEvent* zf_event);
            bool NormalCut(const CutInfo& CUTINFO, const int I_ELEC, ZFinderEvent* zf_event);
            bool CompCutEqual(const ComparisonVariable COMP_VAR, const ZFinderElectron* ZF_ELEC);

            // Build a cut level vector
            void FillCutLevelVector();
            void ResetCutlevelVector();
    };
}  // namespace zf
#endif  // ZDEFINITION_ZDEFINITION_H_
