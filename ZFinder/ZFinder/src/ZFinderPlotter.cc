#include "ZFinder/ZFinder/interface/ZFinderPlotter.h"

// Standard Library
#include <string>  // string

// Root
#include <TCanvas.h>  // TCanvas

// ZFinder Code
#include "ZFinder/ZFinder/interface/ZFinderElectron.h"  // ZFinderElectron


namespace zf {
    // Constructor
    ZFinderPlotter::ZFinderPlotter(TFileDirectory* tdir) {
        /*
         * Initialize a set of histograms and associate them with a given TDirectory.
         */

        // Set up histograms
        // z0_mass_coarse_
        const std::string z0_mass_coarse_name = "Z0 Mass: Coarse";
        z0_mass_coarse_ = tdir->make<TH1I>(z0_mass_coarse_name.c_str(), z0_mass_coarse_name.c_str(), 100, 50., 150.);
        z0_mass_coarse_->GetXaxis()->SetTitle("m_{ee} [GeV]");
        z0_mass_coarse_->GetYaxis()->SetTitle("Counts / GeV");

        // z0_mass_fine_
        const std::string z0_mass_fine_name = "Z0 Mass: Fine";
        z0_mass_fine_ = tdir->make<TH1I>(z0_mass_fine_name.c_str(), z0_mass_fine_name.c_str(), 80, 80., 100.);
        z0_mass_fine_->GetXaxis()->SetTitle("m_{ee} [GeV]");
        z0_mass_fine_->GetYaxis()->SetTitle("Counts / 0.25 GeV");

        // z0_rapidity_
        const std::string z0_rapidity_name = "Z0 Rapidity";
        z0_rapidity_ = tdir->make<TH1I>(z0_rapidity_name.c_str(), z0_rapidity_name.c_str(), 100, -5., 5.);
        z0_rapidity_->GetXaxis()->SetTitle("Y_{ee}");
        z0_rapidity_->GetYaxis()->SetTitle("Counts");

        // z0_pt
        const std::string z0_pt_name = "Z0 p_{T}";
        z0_pt_ = tdir->make<TH1I>(z0_pt_name.c_str(), z0_pt_name.c_str(), 200, 0., 200.);
        z0_pt_->GetXaxis()->SetTitle("p_{T,Z}");
        z0_pt_->GetYaxis()->SetTitle("Counts / GeV");

        // e0_pt
        const std::string e0_pt_name = "p_{T,e_{0}}";
        e0_pt_ = tdir->make<TH1I>(e0_pt_name.c_str(), e0_pt_name.c_str(), 200, 0., 200.);
        e0_pt_->GetXaxis()->SetTitle("p_{T,e_{0}}");
        e0_pt_->GetYaxis()->SetTitle("Counts / GeV");

        // e1_pt
        const std::string e1_pt_name = "p_{T,e_{1}}";
        e1_pt_ = tdir->make<TH1I>(e1_pt_name.c_str(), e1_pt_name.c_str(), 200, 0., 200.);
        e1_pt_->GetXaxis()->SetTitle("p_{T,e_{0}}");
        e1_pt_->GetYaxis()->SetTitle("Counts / GeV");

        // e0_eta_
        const std::string e0_eta_name = "#eta_{e_{0}}";
        e0_eta_ = tdir->make<TH1I>(e0_eta_name.c_str(), e0_eta_name.c_str(), 50, -5., 5.);
        e0_eta_->GetXaxis()->SetTitle("#eta_{e_{0}}");
        e0_eta_->GetYaxis()->SetTitle("Counts");

        // e1_eta_
        const std::string e1_eta_name = "#eta_{e_{1}}";
        e1_eta_ = tdir->make<TH1I>(e1_eta_name.c_str(), e1_eta_name.c_str(), 50, -5., 5.);
        e1_eta_->GetXaxis()->SetTitle("#eta_{e_{1}}");
        e1_eta_->GetYaxis()->SetTitle("Counts");

        // e0_phi_
        const std::string e0_phi_name = "#phi_{e_{0}}";
        e0_phi_ = tdir->make<TH1I>(e0_phi_name.c_str(), e0_phi_name.c_str(), 60, -3.15, 3.15);
        e0_phi_->GetXaxis()->SetTitle("#phi_{e_{0}}");
        e0_phi_->GetYaxis()->SetTitle("Counts");

        // e1_phi_
        const std::string e1_phi_name = "#phi_{e_{1}}";
        e1_phi_ = tdir->make<TH1I>(e1_phi_name.c_str(), e1_phi_name.c_str(), 50, -3.15, 3.15);
        e1_phi_->GetXaxis()->SetTitle("#phi_{e_{1}}");
        e1_phi_->GetYaxis()->SetTitle("counts");

        // phistar
        const std::string phistar_name = "#phi^{*}";
        phistar_ = tdir->make<TH1I>(phistar_name.c_str(), phistar_name.c_str(), 100, 0., 1.);
        phistar_->GetXaxis()->SetTitle("#phi^{*}");
        phistar_->GetYaxis()->SetTitle("Counts");

        // pileup
        const std::string pileup_name = "N_{Vertices}";
        pileup_ = tdir->make<TH1I>(pileup_name.c_str(), pileup_name.c_str(), 100, 0., 100.);
        pileup_->GetXaxis()->SetTitle("Number of Vertices");
        pileup_->GetYaxis()->SetTitle("Counts");

        // nelectrons
        const std::string nelectrons_name = "N_{e}";
        nelectrons_ = tdir->make<TH1I>(nelectrons_name.c_str(), nelectrons_name.c_str(), 10, 0., 10.);
        nelectrons_->GetXaxis()->SetTitle("N_{e}");
        nelectrons_->GetYaxis()->SetTitle("Events");
    }

    void ZFinderPlotter::Fill(const ZFinderEvent& zf_event, const int electron_0, const int electron_1) {
        /*
         * Given a zf_event, fills all the histograms.
         *
         * electron_0 and electron_1 can be used to assign zf_event.eN to the given
         * number in the histogram. For example, assigning electron_0 = 1 will fill
         * the e0 histograms with data from zf_event.e1.
         */
        // Z Info
        z0_mass_coarse_->Fill(zf_event.reco_z.m);
        z0_mass_fine_->Fill(zf_event.reco_z.m);
        z0_rapidity_->Fill(zf_event.reco_z.y);
        z0_pt_->Fill(zf_event.reco_z.pt);
        phistar_->Fill(zf_event.reco_z.phistar);

        // Fill the histograms with the information from the approriate electron
        if (zf_event.e0 != NULL && zf_event.e1 != NULL){
            if (electron_0 == 0 && electron_1 == 1) {
                e0_pt_->Fill(zf_event.e0->pt);
                e0_eta_->Fill(zf_event.e0->eta);
                e0_phi_->Fill(zf_event.e0->phi);
                e1_pt_->Fill(zf_event.e1->pt);
                e1_eta_->Fill(zf_event.e1->eta);
                e1_phi_->Fill(zf_event.e1->phi);
            } else if (electron_0 == 1 && electron_1 == 0) {
                e0_pt_->Fill(zf_event.e1->pt);
                e0_eta_->Fill(zf_event.e1->eta);
                e0_phi_->Fill(zf_event.e1->phi);
                e1_pt_->Fill(zf_event.e0->pt);
                e1_eta_->Fill(zf_event.e0->eta);
                e1_phi_->Fill(zf_event.e0->phi);
            }
        }

        // Event Info
        pileup_->Fill(zf_event.reco_vert.num);
        nelectrons_->Fill(zf_event.n_electrons);
    }

    void ZFinderPlotter::Print(const std::string& basename) {
        // Write all PNGs
        std::string z0_mass_coarse_Str = basename + "_";
        TCanvas* z0_mass_coarse_C = new TCanvas(z0_mass_coarse_Str.c_str(), z0_mass_coarse_Str.c_str(), X_SIZE, Y_SIZE);
        z0_mass_coarse_->Draw();
        z0_mass_coarse_C->Print(z0_mass_coarse_Str.c_str());

        std::string z0_mass_fine_Str = basename + "_";
        TCanvas* z0_mass_fine_C = new TCanvas(z0_mass_fine_Str.c_str(), z0_mass_fine_Str.c_str(), X_SIZE, Y_SIZE);
        z0_mass_fine_->Draw();
        z0_mass_fine_C->Print(z0_mass_fine_Str.c_str());

        std::string z0_rapidity_Str = basename + "_";
        TCanvas* z0_rapidity_C = new TCanvas(z0_rapidity_Str.c_str(), z0_rapidity_Str.c_str(), X_SIZE, Y_SIZE);
        z0_rapidity_->Draw();
        z0_rapidity_C->Print(z0_rapidity_Str.c_str());

        std::string z0_ptStr = basename + "_";
        TCanvas* z0_ptC = new TCanvas(z0_ptStr.c_str(), z0_ptStr.c_str(), X_SIZE, Y_SIZE);
        z0_pt_->Draw();
        z0_ptC->Print(z0_ptStr.c_str());

        std::string e0_ptStr = basename + "_";
        TCanvas* e0_ptC = new TCanvas(e0_ptStr.c_str(), e0_ptStr.c_str(), X_SIZE, Y_SIZE);
        e0_pt_->Draw();
        e0_ptC->Print(e0_ptStr.c_str());

        std::string e1_ptStr = basename + "_";
        TCanvas* e1_ptC = new TCanvas(e1_ptStr.c_str(), e1_ptStr.c_str(), X_SIZE, Y_SIZE);
        e1_pt_->Draw();
        e1_ptC->Print(e1_ptStr.c_str());

        std::string e0_eta_Str = basename + "_";
        TCanvas* e0_eta_C = new TCanvas(e0_eta_Str.c_str(), e0_eta_Str.c_str(), X_SIZE, Y_SIZE);
        e0_eta_->Draw();
        e0_eta_C->Print(e0_eta_Str.c_str());

        std::string e1_eta_Str = basename + "_";
        TCanvas* e1_eta_C = new TCanvas(e1_eta_Str.c_str(), e1_eta_Str.c_str(), X_SIZE, Y_SIZE);
        e1_eta_->Draw();
        e1_eta_C->Print(e1_eta_Str.c_str());

        std::string e0_phi_Str = basename + "_";
        TCanvas* e0_phi_C = new TCanvas(e0_phi_Str.c_str(), e0_phi_Str.c_str(), X_SIZE, Y_SIZE);
        e0_phi_->Draw();
        e0_phi_C->Print(e0_phi_Str.c_str());

        std::string e1_phi_Str = basename + "_";
        TCanvas* e1_phi_C = new TCanvas(e1_phi_Str.c_str(), e1_phi_Str.c_str(), X_SIZE, Y_SIZE);
        e1_phi_->Draw();
        e1_phi_C->Print(e1_phi_Str.c_str());

        std::string phistarStr = basename + "_";
        TCanvas* phistarC = new TCanvas(phistarStr.c_str(), phistarStr.c_str(), X_SIZE, Y_SIZE);
        phistar_->Draw();
        phistarC->Print(phistarStr.c_str());

        std::string pileupStr = basename + "_";
        TCanvas* pileupC = new TCanvas(pileupStr.c_str(), pileupStr.c_str(), X_SIZE, Y_SIZE);
        pileup_->Draw();
        pileupC->Print(pileupStr.c_str());

        std::string nelectronsStr = basename + "_";
        TCanvas* nelectronsC = new TCanvas(nelectronsStr.c_str(), nelectronsStr.c_str(), X_SIZE, Y_SIZE);
        nelectrons_->Draw();
        nelectronsC->Print(nelectronsStr.c_str());
    }
}  // namespace zf
