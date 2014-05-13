#ifndef ZFINDER_CUTLEVEL_H_
#define ZFINDER_CUTLEVEL_H_

// Standard Library
#include <string>  // std::string
#include <utility>  // std::pair
#include <vector>  // std::vector

namespace zf {

    // Cut level struct
    struct CutLevel{
        // Constructor sets all values to false
        CutLevel() {
            pass = false;
            t0p1_pass = false;
            t0p1_eff = 1.;
            t1p0_pass = false;
            t1p0_eff = 1.;
            event_weight = 1.;
        }
        bool pass;
        bool t0p1_pass;
        bool t1p0_pass;
        double t0p1_eff;
        double t1p0_eff;
        double event_weight;
    };


    // Used to match cut levels to names
    typedef std::pair<std::string, CutLevel> cutlevel_pair;
    // Used to pass around cut levels
    typedef std::vector<cutlevel_pair> cutlevel_vector;

}  // namespace zf
#endif  // ZFINDER_CUTLEVEL_H_
