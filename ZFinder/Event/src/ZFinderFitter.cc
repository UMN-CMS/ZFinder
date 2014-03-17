#include "ZFinder/Event/interface/ZFinderFitter.h"

// Standard Library
#include <string>

// Root
#include <TCanvas.h>  // TCanvas
#include <TDirectory.h>  // TDirectory

// ZFinder Code
#include "ZFinder/Event/interface/ZFinderElectron.h"  // ZFinderElectron
#include "ZFinder/Event/interface/ArraysDefinition.h"  // EfficiencyEtaBins,  EfficiencyETBins,  Efficiency,  phistarBins,  etaBins

#include "FWCore/ServiceRegistry/interface/Service.h" // edm::Service
#include "CommonTools/UtilAlgos/interface/TFileService.h" // TFileService


namespace zf {

    const std::vector<std::string> ALL_CUTS = {
        "trig(hf_loose)", "trig(hf_tight)", "trig(et_et_tight)",
        "trig(et_et_loose)", "trig(et_et_dz)", "trig(et_nt_etleg)",
        "trig(et_hf_tight)", "trig(et_hf_loose)", "eg_loose", "eg_medium",
        "eg_tight", "hf_e9e25", "hf_2dloose", "hf_2dmedium", "hf_2dtight",
        "nt_loose"
    };

    // Constructor
    ZFinderFitter::ZFinderFitter() {
        // Variables
        // Z
        z_mass = new RooRealVar("z_mass", "m_{ee}", -10, 10000, "GeV");
        z_eta = new RooRealVar("z_eta", "Z_{#eta}", -20, 20);
        z_y = new RooRealVar("z_y", "Z_{Y}", -1000, 1000);
        z_pt = new RooRealVar("z_pt", "Z_{p_{T}}", 0, 10000, "GeV");
        phistar = new RooRealVar("phistar", "#phi*", 0, 100);
        // Electrons
        e0_pt = new RooRealVar("e0_pt", "p_{T}^{e_{0}}", 0, 10000, "GeV");
        e0_phi = new RooRealVar("e0_phi", "#phi_{e_{0}}", 0, 10000);
        e0_eta = new RooRealVar("e0_eta", "#eta_{e_{0}}", 0, 10000);
        //e0_charge = new RooRealVar("e0_charge", "#q_{e_{0}}", 0, 10000);
        e1_pt = new RooRealVar("e1_pt", "p_{T}^{e_{1}}", 0, 10000, "GeV");
        e1_phi = new RooRealVar("e1_phi", "#phi_{e_{1}}", 0, 10000);
        e1_eta = new RooRealVar("e1_eta", "#eta_{e_{1}}", 0, 10000);
        //e1_charge = new RooRealVar("e1_charge", "#q_{e_{1}}", 0, 10000);
        // Event
        n_vert = new RooRealVar("n_vert", "Number of Vertices", 0, 1000);
        weight = new RooRealVar("weight", "Event weight", 0, 100);
        pass = new RooRealVar("pass", "Pass", -2, 2);
        // Argsets
        zf_arg_set = new RooArgSet(*z_mass, *phistar, *z_pt, *z_eta, *z_y, *weight, *pass);
        zf_arg_set->add(*e0_pt);
        zf_arg_set->add(*e0_phi);
        zf_arg_set->add(*e0_eta);
        //zf_arg_set->add(*e0_charge);
        zf_arg_set->add(*e1_pt);
        zf_arg_set->add(*e1_phi);
        zf_arg_set->add(*e1_eta);
        //zf_arg_set->add(*e1_charge);
        zf_arg_set->add(*n_vert);

        // Add all cuts
        for (auto& i_cut : ALL_CUTS) {
            std::string e0_cut = "e0_" + i_cut;
            std::string e1_cut = "e1_" + i_cut;
            RooRealVar* e0_rrv = new RooRealVar(e0_cut.c_str(), e0_cut.c_str(), -1, 2);
            RooRealVar* e1_rrv = new RooRealVar(e1_cut.c_str(), e1_cut.c_str(), -1, 2);
            rrv_cuts.push_back(e0_rrv);
            rrv_cuts.push_back(e1_rrv);
            zf_arg_set->add(*e0_rrv);
            zf_arg_set->add(*e1_rrv);
        }

        // Datasets
        mc_truth_dataset = new RooDataSet(
                "mc_truth_dataset", "All events from the Truth Level MC",
                *zf_arg_set, RooFit::WeightVar("weight")
                );
        mc_reco_dataset = new RooDataSet(
                "mc_reco_dataset", "Events that pass our selection in Reco MC",
                *zf_arg_set, RooFit::WeightVar("weight")
                );
        data_reco_dataset = new RooDataSet(
                "data_reco_dataset", "Events that pass our selection in Reco Data",
                *zf_arg_set, RooFit::WeightVar("weight")
                );
    }

    void ZFinderFitter::FillAll(const ZFinderEvent& zf_event) {
        if (!zf_event.is_real_data) {
            zf_arg_set->setRealValue("z_mass", zf_event.truth_z.m);
            zf_arg_set->setRealValue("z_eta", zf_event.truth_z.eta);
            zf_arg_set->setRealValue("z_y", zf_event.truth_z.y);
            zf_arg_set->setRealValue("z_pt", zf_event.truth_z.pt);
            zf_arg_set->setRealValue("phistar", zf_event.truth_z.phistar);
            zf_arg_set->setRealValue("weight", 1);
            zf_arg_set->setRealValue("pass", -1);
            zf_arg_set->setRealValue("e0_pt", zf_event.e0_truth->pt);
            zf_arg_set->setRealValue("e0_phi", zf_event.e0_truth->phi);
            zf_arg_set->setRealValue("e0_eta", zf_event.e0_truth->eta);
            //zf_arg_set->setRealValue("e0_charge", zf_event.e0_truth->charge);
            zf_arg_set->setRealValue("e1_pt", zf_event.e1_truth->pt);
            zf_arg_set->setRealValue("e1_phi", zf_event.e1_truth->phi);
            zf_arg_set->setRealValue("e1_eta", zf_event.e1_truth->eta);
            //zf_arg_set->setRealValue("e1_charge", zf_event.e1_truth->charge);
            zf_arg_set->setRealValue("n_vert", zf_event.truth_vert.num);

            // Set all cuts
            for (auto& i_cut : ALL_CUTS) {
                std::string e0_cut = "e0_" + i_cut;
                std::string e1_cut = "e1_" + i_cut;
                int e0_res = zf_event.e0_truth->CutPassed(i_cut);
                int e1_res = zf_event.e1_truth->CutPassed(i_cut);
                if (e0_res < 0) { e0_res = 0; }  // We return -1 if the cut wasn't set
                if (e1_res < 0) { e1_res = 0; }
                zf_arg_set->setRealValue(e0_cut.c_str(), e0_res);
                zf_arg_set->setRealValue(e1_cut.c_str(), e1_res);
            }
	    //           mc_truth_dataset->add(*zf_arg_set);
        }
    }

    void ZFinderFitter::FillSelected(const ZFinderEvent& zf_event) {
        double weight0 = 1;
        double weight1 = 1;
        if (!zf_event.is_real_data){
            const int netasf = (sizeof(EfficiencyEtaBins)/sizeof(EfficiencyEtaBins[0]))-1;
            const int nptsf = (sizeof(EfficiencyETBins)/sizeof(EfficiencyETBins[0]))-1;
            const int bin0eta = BinInArray(fabs(zf_event.e0->eta), EfficiencyEtaBins, netasf);
            const int bin0pt  = BinInArray(zf_event.e0->pt       , EfficiencyETBins,  nptsf);
            const int bin1eta = BinInArray(fabs(zf_event.e1->eta), EfficiencyEtaBins, netasf);
            const int bin1pt  = BinInArray(zf_event.e1->pt       , EfficiencyETBins,  nptsf);
            if (bin0eta >= 0 && bin0pt >= 0) {
                weight0 = EfficiencySF[bin0eta][bin0pt][0];
            }
            if (bin1eta >= 0 && bin1pt >= 0) {
                weight1 = EfficiencySF[bin1eta][bin1pt][0];
            }
        }
        zf_arg_set->setRealValue("z_mass", zf_event.reco_z.m);
        zf_arg_set->setRealValue("z_eta", zf_event.reco_z.eta);
        zf_arg_set->setRealValue("z_y", zf_event.reco_z.y);
        zf_arg_set->setRealValue("phistar", zf_event.reco_z.phistar);
        zf_arg_set->setRealValue("z_pt", zf_event.reco_z.pt);
        zf_arg_set->setRealValue("weight", weight0*weight1);
        zf_arg_set->setRealValue("pass", 1);
        zf_arg_set->setRealValue("e0_pt", zf_event.e0->pt);
        zf_arg_set->setRealValue("e0_phi", zf_event.e0->phi);
        zf_arg_set->setRealValue("e0_eta", zf_event.e0->eta);
        //zf_arg_set->setRealValue("e0_charge", zf_event.e0->charge);
        zf_arg_set->setRealValue("e1_pt", zf_event.e1->pt);
        zf_arg_set->setRealValue("e1_phi", zf_event.e1->phi);
        zf_arg_set->setRealValue("e1_eta", zf_event.e1->eta);
        //zf_arg_set->setRealValue("e1_charge", zf_event.e1->charge);
        zf_arg_set->setRealValue("n_vert", zf_event.reco_vert.num);

        // Set all cuts
        for (auto& i_cut : ALL_CUTS) {
            std::string e0_cut = "e0_" + i_cut;
            std::string e1_cut = "e1_" + i_cut;
            int e0_res = zf_event.e0->CutPassed(i_cut);
            int e1_res = zf_event.e1->CutPassed(i_cut);
            if (e0_res < 0) { e0_res = 0; }  // We return -1 if the cut wasn't set
            if (e1_res < 0) { e1_res = 0; }
            zf_arg_set->setRealValue(e0_cut.c_str(), e0_res);
            zf_arg_set->setRealValue(e1_cut.c_str(), e1_res);
        }

        if (zf_event.is_real_data) {
            data_reco_dataset->add(*zf_arg_set);
        } else {
            mc_reco_dataset->add(*zf_arg_set);
        }
    }

    void ZFinderFitter::Write() {
        // Get a TDirectory from the TFileDirectory and cd to it for writing

        edm::Service<TFileService> fs;
        fs->cd();
        // Make the workspace and save it
        RooWorkspace *w = new RooWorkspace("w", "workspace");
        w->import(*mc_truth_dataset);
        w->import(*mc_reco_dataset);
        w->import(*data_reco_dataset);
        w->Write();
        //data_reco_dataset->Print("v");
    }

    ZFinderFitter::~ZFinderFitter() {
        // First we clean up the RooRealVariables
        delete z_mass;
        delete z_eta ;
        delete z_y;
        delete z_pt;
        delete phistar;
        delete weight;
        delete pass;
        delete e0_pt;
        delete e0_phi;
        delete e0_eta;
        //delete e0_charge;
        delete e1_pt;
        delete e1_phi;
        delete e1_eta;
        //delete e1_charge;
        delete n_vert;

        // Then we clean up all the RRVs made from the cut vector
        for (auto& i_rrv : rrv_cuts) {
            delete i_rrv;
        }

        // Finally we remove the argset and datasets, which are full of the
        // above objects
        delete zf_arg_set;
        delete mc_truth_dataset;
        delete mc_reco_dataset;
        delete data_reco_dataset;
    }
}
