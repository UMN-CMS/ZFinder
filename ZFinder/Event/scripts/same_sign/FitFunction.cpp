#include "FitFunction.h"

FitFunction::FitFunction(TH1D histogram) {
    histogram_ = histogram;
}

double FitFunction::operator()(const double* x, const double* par) {
    return par[0] * histogram_.GetBinContent(histogram_.FindBin(x[0]));
}
