#include "ZFinder/Event/interface/ZFinderFitter.h"

// Standard Library
#include <string>  // string

// Root
#include <TCanvas.h>  // TCanvas
#include <TDirectory.h>  // TDirectory

// ZFinder Code
#include "ZFinder/Event/interface/ZFinderElectron.h"  // ZFinderElectron
#include "ZFinder/Event/interface/ArraysDefinition.h"  // EfficiencyEtaBins,  EfficiencyETBins,  Efficiency,  phistarBins,  etaBins


namespace zf {
    // Constructor
    ZFinderFitter::ZFinderFitter() {
        Zmass = new RooRealVar("Zmass", "Zmass", -10, 10000);
        Zeta = new RooRealVar("Zeta", "Zeta", -20, 20);
        Zy = new RooRealVar("Zy", "Zy", -1000, 1000);
        Zpt = new RooRealVar("Zpt", "Zpt", 0, 10000);
        Zphistar = new RooRealVar("Zphistar", "Zphistar", 0, 100);
        Weight = new RooRealVar("Weight", "Weight", 0, 100);
        Pass = new RooRealVar("Pass", "Pass", -2, 2);
        ZEventArgSet = new RooArgSet(*Zmass,  *Zphistar,  *Zpt,  *Zeta,  *Zy,  *Weight,  *Pass);
        MC_true_all = new RooDataSet("MC_true_all", "MC_true_all", *ZEventArgSet);
        MC_reco = new RooDataSet("MC_reco", "MC_reco", *ZEventArgSet);
        Data_reco = new RooDataSet("Data_reco", "Data_reco", *ZEventArgSet);
    }

    void ZFinderFitter::FillAll(const ZFinderEvent& zf_event) {
        if (!zf_event.is_real_data) {
            ZEventArgSet->setRealValue("Zmass", zf_event.truth_z.m);
            ZEventArgSet->setRealValue("Zeta", zf_event.truth_z.eta);
            ZEventArgSet->setRealValue("Zy", zf_event.truth_z.y);
            ZEventArgSet->setRealValue("Zphistar", zf_event.truth_z.phistar);
            ZEventArgSet->setRealValue("Zpt", zf_event.truth_z.pt);
            ZEventArgSet->setRealValue("Weight", 1);
            ZEventArgSet->setRealValue("Pass", -1);
            MC_true_all->add(*ZEventArgSet);
        }
    }

    void ZFinderFitter::FillSelected(const ZFinderEvent& zf_event) {
        double weight0 = 1;
        double weight1 = 1;
        if (!zf_event.is_real_data){
            int netasf = (sizeof(EfficiencyEtaBins)/sizeof(EfficiencyEtaBins[0]))-1;
            int nptsf = (sizeof(EfficiencyETBins)/sizeof(EfficiencyETBins[0]))-1;
            int bin0eta = BinInArray(fabs(zf_event.e0->eta), EfficiencyEtaBins, netasf);
            int bin0pt  = BinInArray(zf_event.e0->pt       , EfficiencyETBins,  nptsf);
            int bin1eta = BinInArray(fabs(zf_event.e1->eta), EfficiencyEtaBins, netasf);
            int bin1pt  = BinInArray(zf_event.e1->pt       , EfficiencyETBins,  nptsf);
            if (bin0eta >= 0 && bin0pt >= 0) {
                weight0 = EfficiencySF[bin0eta][bin0pt][0];
            }
            if (bin1eta >= 0 && bin1pt >= 0) {
                weight1 = EfficiencySF[bin1eta][bin1pt][0];
            }
        }
        ZEventArgSet->setRealValue("Zmass", zf_event.reco_z.m);
        ZEventArgSet->setRealValue("Zeta", zf_event.reco_z.eta);
        ZEventArgSet->setRealValue("Zy", zf_event.reco_z.y);
        ZEventArgSet->setRealValue("Zphistar", zf_event.reco_z.phistar);
        ZEventArgSet->setRealValue("Zpt", zf_event.reco_z.pt);
        ZEventArgSet->setRealValue("Weight", weight0*weight1);
        ZEventArgSet->setRealValue("Pass", 1);
        if (zf_event.is_real_data) {
            Data_reco->add(*ZEventArgSet);
        } else {
            MC_reco->add(*ZEventArgSet);
        }
    }

    void ZFinderFitter::Write(TFileDirectory& tfiledir) {
        // Get a TDirectory from the TFileDirectory and cd to it for writing
        TDirectory* tdir = tfiledir.getBareDirectory();
        tdir->cd();

        // Make the workspace and save it
        RooWorkspace *w = new RooWorkspace("w", "workspace");
        w->import(*MC_true_all);
        w->import(*MC_reco);
        w->import(*Data_reco);
        w->Write();
    }
}
