#include "ZFinderPlotter.h"

// Constructor
ZFinderPlotter::ZFinderPlotter(TDirectory* tdir){
    /* 
     * Initialize a set of histograms and associate them with a given TDirectory.
     */ 

    // Set tdir to the internal directory and then set it as the current
    // working directory
    m_tdir = tdir;
    m_tdir->cd();

    // Set up histograms
    // Z0MassCoarse
    const std::string Z0MassCoarseName = "Z0 Mass: Coarse";
    Z0MassCoarse = new TH1I(Z0MassCoarseName.c_str(), Z0MassCoarseName.c_str(), 100, 50., 150.);
    Z0MassCoarse->SetDirectory(m_tdir);
    Z0MassCoarse->GetXaxis()->SetTitle("m_{ee} [GeV]");
    Z0MassCoarse->GetYaxis()->SetTitle("Counts / GeV");

    // Z0MassFine
    const std::string Z0MassFineName = "Z0 Mass: Fine";
    Z0MassFine = new TH1I(Z0MassFineName.c_str(), Z0MassFineName.c_str(), 80, 80., 100.);
    Z0MassFine->SetDirectory(m_tdir);
    Z0MassFine->GetXaxis()->SetTitle("m_{ee} [GeV]");
    Z0MassFine->GetYaxis()->SetTitle("Counts / 0.25 GeV");

    // Z0Rapidity
    const std::string Z0RapidityName = "Z0 Rapidity";
    Z0Rapidity = new TH1I(Z0RapidityName.c_str(), Z0RapidityName.c_str(), 100, -5., 5.);
    Z0Rapidity->SetDirectory(m_tdir);
    Z0Rapidity->GetXaxis()->SetTitle("Y_{ee}");
    Z0Rapidity->GetYaxis()->SetTitle("Counts");

    // Z0pt
    const std::string Z0ptName = "Z0 p_{T}";
    Z0pt = new TH1I(Z0ptName.c_str(), Z0ptName.c_str(), 200, 0., 200.);
    Z0pt->SetDirectory(m_tdir);
    Z0pt->GetXaxis()->SetTitle("p_{T,Z}");
    Z0pt->GetYaxis()->SetTitle("Counts / GeV");

    // e0pt
    const std::string e0ptName = "p_{T,e_{0}}";
    e0pt = new TH1I(e0ptName.c_str(), e0ptName.c_str(), 200, 0., 200.);
    e0pt->SetDirectory(m_tdir);
    e0pt->GetXaxis()->SetTitle("p_{T,e_{0}}");
    e0pt->GetYaxis()->SetTitle("Counts / GeV");

    // e1pt
    const std::string e1ptName = "p_{T,e_{1}}";
    e1pt = new TH1I(e1ptName.c_str(), e1ptName.c_str(), 200, 0., 200.);
    e1pt->SetDirectory(m_tdir);
    e1pt->GetXaxis()->SetTitle("p_{T,e_{0}}");
    e1pt->GetYaxis()->SetTitle("Counts / GeV");

    // e0eta
    const std::string e0etaName = "#eta_{e_{0}}";
    e0eta = new TH1I(e0etaName.c_str(), e0etaName.c_str(), 50, -5., 5.);
    e0eta->SetDirectory(m_tdir);
    e0eta->GetXaxis()->SetTitle("#eta_{e_{0}}");
    e0eta->GetYaxis()->SetTitle("Counts");

    // e1eta
    const std::string e1etaName = "#eta_{e_{1}}";
    e1eta = new TH1I(e1etaName.c_str(), e1etaName.c_str(), 50, -5., 5.);
    e1eta->SetDirectory(m_tdir);
    e1eta->GetXaxis()->SetTitle("#eta_{e_{1}}");
    e1eta->GetYaxis()->SetTitle("Counts");

    // e0phi
    const std::string e0phiName = "#phi_{e_{0}}";
    e0phi = new TH1I(e0phiName.c_str(), e0phiName.c_str(), 60, -3.15, 3.15);
    e0phi->SetDirectory(m_tdir);
    e0phi->GetXaxis()->SetTitle("#phi_{e_{0}}");
    e0phi->GetYaxis()->SetTitle("Counts");

    // e1phi
    const std::string e1phiName = "#phi_{e_{1}}";
    e1phi = new TH1I(e1phiName.c_str(), e1phiName.c_str(), 50, -3.15, 3.15);
    e1phi->SetDirectory(m_tdir);
    e1phi->GetXaxis()->SetTitle("#phi_{e_{1}}");
    e1phi->GetYaxis()->SetTitle("counts");

    // phistar
    const std::string phistarName = "#phi^{*}";
    phistar = new TH1I(phistarName.c_str(), phistarName.c_str(), 100, 0., 1.);
    phistar->SetDirectory(m_tdir);
    phistar->GetXaxis()->SetTitle("#phi^{*}");
    phistar->GetYaxis()->SetTitle("Counts");

    // pileup
    const std::string pileupName = "Pileup";
    pileup = new TH1I(pileupName.c_str(), pileupName.c_str(), 100, 0., 100.);
    pileup->SetDirectory(m_tdir);
    pileup->GetXaxis()->SetTitle("Pileup");
    pileup->GetYaxis()->SetTitle("Counts");

}

ZFinderPlotter::fill(const ZFinderEvent* ZEvent, const short e0 = 0, const short e1 = 1){
    /* 
     * Given a ZEvent, fills all the histograms. e0 and e1 can be used to
     * assign ZEvent.eN to the given number in the histogram. That is,
     * assigning e0 = 1 will fill the e1 histograms with data from ZEvent->e0.
     */
    // Z Info
    Z0MassCoarse->Fill(ZEvent->z.m);
    Z0MassFine->Fill(ZEvent->z.m);
    Z0Rapidity->Fill(ZEvent->z.y);
    Z0pt->Fill(ZEvent->z.pt);
    phistar->Fill(ZEvent->z.phistar);

    // e0 and e1 Info
    if ( e0 == 0 && e1 == 1 ){
        e0pt->Fill(ZEvent->e0->pt);
        e0eta->Fill(ZEvent->e0->eta);
        e0phi->Fill(ZEvent->e0->phi);
        e1pt->Fill(ZEvent->e1->pt);
        e1eta->Fill(ZEvent->e1->eta);
        e1phi->Fill(ZEvent->e1->phi);
    } else if ( e0 == 1 && e1 == 0 ){
        e0pt->Fill(ZEvent->e1->pt);
        e0eta->Fill(ZEvent->e1->eta);
        e0phi->Fill(ZEvent->e1->phi);
        e1pt->Fill(ZEvent->e0->pt);
        e1eta->Fill(ZEvent->e0->eta);
        e1phi->Fill(ZEvent->e0->phi);
    }

    // Event Info
    pileup->Fill(ZEvent->vert.num);
}

void ZFinderPlotter::print(){
    // Get the name of the TDir
    std::string basename;
    basename.assign(tdir->GetName());

    // Set Image Sizes
    const int XSIZE = 1280;
    const int YSIZE = 640;

    // Write all PNGs
    std::string Z0MassCoarseStr = basename + "_";
    TCanvas* Z0MassCoarseC = new TCanvas(Z0MassCoarse.c_str(), Z0MassCoarse.c_str(), XSIZE, YSIZE);
    Z0MassCoarse->Draw();
    Z0MassCoarseC->Print(Z0MassCoarseStr.c_str());

    std::string Z0MassFineStr = basename + "_";
    TCanvas* Z0MassFineC = new TCanvas(Z0MassFine.c_str(), Z0MassFine.c_str(), XSIZE, YSIZE);
    Z0MassFine->Draw();
    Z0MassFineC->Print(Z0MassFineStr.c_str());

    std::string Z0RapidityStr = basename + "_";
    TCanvas* Z0RapidityC = new TCanvas(Z0Rapidity.c_str(), Z0Rapidity.c_str(), XSIZE, YSIZE);
    Z0Rapidity->Draw();
    Z0RapidityC->Print(Z0RapidityStr.c_str());

    std::string Z0ptStr = basename + "_";
    TCanvas* Z0ptC = new TCanvas(Z0pt.c_str(), Z0pt.c_str(), XSIZE, YSIZE);
    Z0pt->Draw();
    Z0ptC->Print(Z0ptStr.c_str());

    std::string e0ptStr = basename + "_";
    TCanvas* e0ptC = new TCanvas(e0pt.c_str(), e0pt.c_str(), XSIZE, YSIZE);
    e0pt->Draw();
    e0ptC->Print(e0ptStr.c_str());

    std::string e1ptStr = basename + "_";
    TCanvas* e1ptC = new TCanvas(e1pt.c_str(), e1pt.c_str(), XSIZE, YSIZE);
    e1pt->Draw();
    e1ptC->Print(e1ptStr.c_str());

    std::string e0etaStr = basename + "_";
    TCanvas* e0etaC = new TCanvas(e0eta.c_str(), e0eta.c_str(), XSIZE, YSIZE);
    e0eta->Draw();
    e0etaC->Print(e0etaStr.c_str());

    std::string e1etaStr = basename + "_";
    TCanvas* e1etaC = new TCanvas(e1eta.c_str(), e1eta.c_str(), XSIZE, YSIZE);
    e1eta->Draw();
    e1etaC->Print(e1etaStr.c_str());

    std::string e0phiStr = basename + "_";
    TCanvas* e0phiC = new TCanvas(e0phi.c_str(), e0phi.c_str(), XSIZE, YSIZE);
    e0phi->Draw();
    e0phiC->Print(e0phiStr.c_str());

    std::string e1phiStr = basename + "_";
    TCanvas* e1phiC = new TCanvas(e1phi.c_str(), e1phi.c_str(), XSIZE, YSIZE);
    e1phi->Draw();
    e1phiC->Print(e1phiStr.c_str());

    std::string phistarStr = basename + "_";
    TCanvas* phistarC = new TCanvas(phistar.c_str(), phistar.c_str(), XSIZE, YSIZE);
    phistar->Draw();
    phistarC->Print(phistarStr.c_str());

    std::string pileupStr = basename + "_";
    TCanvas* pileupC = new TCanvas(pileup.c_str(), pileup.c_str(), XSIZE, YSIZE);
    pileup->Draw();
    pileupC->Print(pileupStr.c_str());
}
