#include "FitFunction.h"

#include "TMath.h"

FitFunction::FitFunction(TH1D histogram) {
    histogram_ = histogram;
}

double FitFunction::operator()(const double* x, const double* par) {
    const double hist_val = par[0] * histogram_.GetBinContent(histogram_.FindBin(x[0]));
    const double bg = Background(x[0], par[1], par[2], par[3]);
    const double func_val = par[4] * bg;

    return hist_val + func_val;
}

double FitFunction::Background(
        const double x,
        const double alpha,
        const double gamma,
        const double delta
        ) {
    return TMath::Erfc((alpha-x)/delta) * TMath::Exp(-gamma*x);
}
