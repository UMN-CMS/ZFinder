#include "ZFinder/Event/interface/ZFinderBinning.h"

// Standard Library
#include <math.h>  // fabs


namespace zf {
    /* The binning lists here _MUST_ be sorted, as the loop terminates
     *
     * as soon as it finds i such that:
     * BINS[i] < input_value < BINS[i + 1]
     *
     * The last entry should be the upper bounds, if it is reached the
     * code will return -1
     */
    // ATLAS binning
    const std::vector<double> ZFinderBinning::PHISTAR_BINS_ = {
        0.000,
        0.004,
        0.008,
        0.012,
        0.016,
        0.020,
        0.024,
        0.029,
        0.034,
        0.039,
        0.045,
        0.052,
        0.057,
        0.064,
        0.072,
        0.081,
        0.091,
        0.102,
        0.114,
        0.128,
        0.145,
        0.165,
        0.189,
        0.219,
        0.258,
        0.312,
        0.391,
        0.524,
        0.695,
        0.918,
        1.153,
        1.496,
        1.947,
        2.522,
        3.277,
        10.0,
    };

    // ATLAS binning
    const std::vector<double> ZFinderBinning::RAPIDITY_BINS_ = {
        0.0,
        0.8,
        1.6,
        10.0,
    };

    int ZFinderBinning::GetBin(const double VALUE, const std::vector<double>& VECTOR) const {
        const double FVALUE = fabs(VALUE);
        int bin_number = 0;
        for (unsigned int i = 0; i < VECTOR.size(); ++i) {
            if (VECTOR[i] <= FVALUE && FVALUE < VECTOR[i + 1]) {
                return bin_number;
            }
            ++bin_number;
        }
        return -1;  // Out of range
    }

    int ZFinderBinning::GetPhistarBin(const double PHISTAR) const {
        return GetBin(PHISTAR, PHISTAR_BINS_);
    }

    int ZFinderBinning::GetRapidityBin(const double Y) const {
        return GetBin(Y, RAPIDITY_BINS_);
    }

}  // namespace zf
