#ifndef ZFINDER_ZFINDERBINNING_H_
#define ZFINDER_ZFINDERBINNING_H_

// Standard Library
#include <vector>


namespace zf {

    typedef std::pair<double, double> coordinate_pair;

    class ZFinderBinning {
        public:
            // Get Bins
            int GetPhistarBin(const double PHISTAR) const;
            int GetRapidityBin(const double Y) const;

        protected:
            int GetBin(const double VALUE, const std::vector<double>& VECTOR) const;

            // Bin edges, defined in the cc file
            static const std::vector<double> PHISTAR_BINS_;
            static const std::vector<double> RAPIDITY_BINS_;
    };

}  // namespace zf
#endif  // ZFINDER_ZFINDERBINNING_H_
