#ifndef ZFINDER_WEIGHTID_H_
#define ZFINDER_WEIGHTID_H_

// Standard Library
#include <map>
#include <string>

namespace zf {

    /* IDs used in the tuples to identify the weights.
    */
    enum WeightID {
        TOTAL = 0,
        GEN_MC = 1,
        PILEUP = 2,
        PILEUP_PLUS = 3,
        PILEUP_MINUS = 4,
        VETO = 10,
        LOOSE = 11,
        MEDIUM = 12,
        TIGHT = 13,
        SINGLE_TRIG = 20,
        DOUBLE_TRIG = 21,
        GSF_RECO = 30
    };

    /* Map of cut strings to WeightID */
    static const std::map<std::string, int> STR_TO_WEIGHTID = {
        {"eg_veto", WeightID::VETO},
        {"eg_loose", WeightID::LOOSE},
        {"eg_medium", WeightID::MEDIUM},
        {"eg_tight", WeightID::TIGHT},
        {"trig(single_ele)", WeightID::SINGLE_TRIG},
        {"type_gsf", WeightID::GSF_RECO}
    };

}  // namespace zf
#endif  // ZFINDER_WEIGHTID_H_
