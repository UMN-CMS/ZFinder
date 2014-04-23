#ifndef ZFINDER_EFFICIENCYTABLE_H_
#define ZFINDER_EFFICIENCYTABLE_H_

// Standard Library
#include <utility>  // std::pair
#include <map>  // std::map


namespace zf {

    typedef std::pair<double, double> coordinate_pair;

    struct Efficiency{
        Efficiency () {}  // Empty constructor
        Efficiency(
                const double efficiency,
                const double err_up,
                const double err_down
                ) :
            efficiency(efficiency),
            err_up(err_up),
            err_down(err_down)
        {}
        // Values
        double efficiency;
        double err_up;    // Positive error bar size
        double err_down;  // Negative error bar size
    };

    class EfficiencyTable {
        public:
            // Constructor
            EfficiencyTable();

            // Add efficiencies
            void AddEfficiency(
                    const double PT_MIN,
                    const double PT_MAX,
                    const double ETA_MIN,
                    const double ETA_MAX,
                    const double EFFICIENCY,
                    const double POSITIVE_ERROR_BAR = -1.,
                    const double NEGATIVE_ERROR_BAR = -1.
                    );

            // Get efficiency
            double GetEfficiency(const double PT, const double ETA) const;

        protected:
            std::map<coordinate_pair, std::map<coordinate_pair, Efficiency>> eff_table_;

    };

}  // namespace zf
#endif  // ZFINDER_EFFICIENCYTABLE_H_
