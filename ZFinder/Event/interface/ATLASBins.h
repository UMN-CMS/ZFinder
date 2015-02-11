#ifndef ZFINDER_ATLASBINS_H_
#define ZFINDER_ATLASBINS_H_

/*
 * A vector of the Phi* binning used by ATLAS.
 */

namespace zf {
    static const std::vector<double> ATLAS_PHISTAR_BINNING = {
        0.000, 0.004, 0.008, 0.012, 0.016, 0.020, 0.024, 0.029, 0.034, 0.039,
        0.045, 0.052, 0.057, 0.064, 0.072, 0.081, 0.091, 0.102, 0.114, 0.128,
        0.145, 0.165, 0.189, 0.219, 0.258, 0.312, 0.391, 0.524, 0.695, 0.918,
        1.153, 1.496, 1.947, 2.522, 3.277,
    };
}  // namespace zf

#endif  //ZFINDER_ATLASBINS_H_
