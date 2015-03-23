#ifndef SAME_SIGN_H_
#define SAME_SIGN_H_

// Standard Library
#include <map>
#include <string>
#include <utility>  // std::pair

// ROOT
#include <TH1D.h>
#include <TTree.h>

struct histogram_container{
    TH1D* mass;
    TH1D* low_side_phistar;
    TH1D* high_side_phistar;
};

typedef std::map<std::string, histogram_container> histogram_map;

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

histogram_map GetHistoMap();

histogram_container GetTemplates(std::map<std::string, std::pair<TH1D*, TH1D*>> histo_map);
TH1D* GetRatioHistogram(std::map<std::string, TH1D*> histo_map);

#endif  // SAME_SIGN_H_
