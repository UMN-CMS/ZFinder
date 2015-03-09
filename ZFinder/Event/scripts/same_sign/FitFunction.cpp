#include "FitFunction.h"

FitFunction::FitFunction(TH1D histogram) {
    histogram_ = histogram;
}

double FitFunction::operator()(const double* x, const double* par) {
    const double hist_val = par[0] * histogram_.GetBinContent(histogram_.FindBin(x[0]));
    const double constant = par[1];
    return hist_val + constant;
}
