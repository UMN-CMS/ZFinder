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
            tag_cut = "";
            probe_cut = "";
            t0p1_tag_weight = 1.;
            t0p1_probe_weight = 1.;
            t1p0_tag_weight = 1.;
            t1p0_probe_weight = 1.;
        }
        bool pass;
        bool t0p1_pass;
        bool t1p0_pass;
        double t0p1_eff;  // Product of all previous t0p1 weights
        double t1p0_eff;  // Product of all previous t1p0 weights
        double event_weight;
        std::string tag_cut;
        std::string probe_cut;
        double t0p1_tag_weight;
        double t0p1_probe_weight;
        double t1p0_tag_weight;  // The weight for this level
        double t1p0_probe_weight;  // The Weight for this level
    };


    // Used to match cut levels to names
    typedef std::pair<std::string, CutLevel> cutlevel_pair;
    // Used to pass around cut levels
    typedef std::vector<cutlevel_pair> cutlevel_vector;

}  // namespace zf
#endif  // ZFINDER_CUTLEVEL_H_
