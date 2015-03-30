#ifndef SAME_SIGN_H_
#define SAME_SIGN_H_

// Standard Library
#include <string>
#include <map>
#include <utility>  // std::pair

// ROOT
#include <TH1D.h>
#include <TH2D.h>
#include <TTree.h>

typedef std::map<std::string, TH2D*> histogram_map;

struct branch_struct {
    double z_m;
    double z_y;
    double z_phistar_born;
    double z_phistar_dressed;
    double z_phistar_naked;
    double z_phistar_sc;
    double z_pt;
    double z_eta;
    double e_pt[2];
    double e_eta[2];
    double e_phi[2];
    int e_charge[2];
    unsigned int n_verts;
};

struct event_branch {
    unsigned int event_number;
    bool is_mc;
};

TTree* GetTTree(const std::string TFILE, const std::string TTREE);

double GetWeight(
    const int& WEIGHT_SIZE,
    const double WEIGHTS[],
    const int WEIGHTIDS[]
);

double GetOverallNormalization(const std::string NAME);

histogram_map Get2DHistoMap();

TH2D* GetTemplate(histogram_map histo_map);

TH1D* Get1DFromBin(TH2D* histo, const int BIN);

std::pair<double, double> FitForQCD(TH1D* data_histo, TH1D* template_histo, const std::string BIN, const std::string PHISTAR_RANGE);

void MakePhistarPlot(TH1D* histo);

#endif  // SAME_SIGN_H_
