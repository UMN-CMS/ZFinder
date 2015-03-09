#ifndef FITFUNCTION_H_
#define FITFUNCTION_H_

// ROOT
#include <TH1D.h>

class FitFunction {
    public:
        // Constructor
        FitFunction(TH1D histogram);

        // Operator for fitting
        double operator()(const double* x, const double* par);

        // Information about the class needed by fitter functions
        int nparms() {
            return 2;
        }

    private:
        TH1D histogram_;
};

#endif  // FITFUNCTION_H_
