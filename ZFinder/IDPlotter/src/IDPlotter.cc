// -*- C++ -*-
//
// Package:    IDPlotter
// Class:      IDPlotter
//
/**\class IDPlotter IDPlotter.cc ZFinder/IDPlotter/src/IDPlotter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Alexander Gude
//         Created:  Tue Mar 10 23:53:26 CDT 2015
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// CMSSW
#include "FWCore/ServiceRegistry/interface/Service.h" // edm::Service
#include "CommonTools/UtilAlgos/interface/TFileService.h" // TFileService
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/ValueMap.h"  // edm:ValueMap
#include "EgammaAnalysis/ElectronTools/interface/ElectronEffectiveArea.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"  // reco::GenParticle

// Electrons
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"  // GsfElectron
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"  // GsfTrack

// ROOT
#include <TH1D.h>

//
// class declaration
//

class IDPlotter : public edm::EDAnalyzer {
    public:
        explicit IDPlotter(const edm::ParameterSet&);
        ~IDPlotter();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


    private:
        virtual void beginJob() ;
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob() ;

        virtual void beginRun(edm::Run const&, edm::EventSetup const&);
        virtual void endRun(edm::Run const&, edm::EventSetup const&);
        virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
        virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

        void fill_histograms(const reco::GsfElectron& electron, const double ISO, const double PT, const bool CON_MATCH);

double get_iso(
        const double ISO_CH,
        const double ISO_EM,
        const double ISO_NH,
        const double RHO,
        const double PT,
        const double ETA
        );

        // ----------member data ---------------------------
        edm::InputTag ecal_electron_;
        double min_pt_;
        double max_eta_;
        TH1D* r9_;
        TH1D* sigma_ieta_ieta_;
        TH1D* h_over_e_;
        TH1D* deta_in_;
        TH1D* dphi_in_;
        TH1D* track_iso_;
        TH1D* ecal_iso_;
        TH1D* hcal_iso_;
        TH1D* one_over_e_mins_one_over_p_;
        TH1D* d0_;
        TH1D* dz_;
        TH1D* mhits_;
        TH1D* conversion_match_;
        TH1D* iso_;
        edm::Handle<reco::VertexCollection> reco_vertices_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
IDPlotter::IDPlotter(const edm::ParameterSet& iConfig) {
    //now do what ever initialization is needed
    edm::Service<TFileService> fs;

    // r9
    const std::string r9_name = "r9";
    const std::string r9_file = "r9";
    r9_ = fs->make<TH1D>(r9_file.c_str(), r9_name.c_str(), 11000, 0., 1.1);
    r9_->GetXaxis()->SetTitle("R9");
    r9_->GetYaxis()->SetTitle("Counts");

    // sigma_ieta_ieta
    const std::string sigma_ieta_ieta_name = "sigma_{i #eta i #eta}";
    const std::string sigma_ieta_ieta_file = "siesie";
    sigma_ieta_ieta_ = fs->make<TH1D>(sigma_ieta_ieta_file.c_str(), sigma_ieta_ieta_name.c_str(), 12000, 0., 0.12);
    sigma_ieta_ieta_->GetXaxis()->SetTitle("#sigma_{i #eta i #eta}");
    sigma_ieta_ieta_->GetYaxis()->SetTitle("Counts");

    // h_over_e
    const std::string h_over_e_name = "(H/E)";
    const std::string h_over_e_file = "he";
    h_over_e_ = fs->make<TH1D>(h_over_e_file.c_str(), h_over_e_name.c_str(), 10000, 0., 1.);
    h_over_e_->GetXaxis()->SetTitle("(H/E)");
    h_over_e_->GetYaxis()->SetTitle("Counts");

    // deta_in
    const std::string deta_in_name = "d#eta";
    const std::string deta_in_file = "deta";
    deta_in_ = fs->make<TH1D>(deta_in_file.c_str(), deta_in_name.c_str(), 40000, -0.2, 0.2);
    deta_in_->GetXaxis()->SetTitle("d#eta_{in}");
    deta_in_->GetYaxis()->SetTitle("Counts");

    // dphi_in
    const std::string dphi_in_name = "d#phi";
    const std::string dphi_in_file = "dphi";
    dphi_in_ = fs->make<TH1D>(dphi_in_file.c_str(), dphi_in_name.c_str(), 20000, -1., 1.);
    dphi_in_->GetXaxis()->SetTitle("d#phi_{in}");
    dphi_in_->GetYaxis()->SetTitle("Counts");

    // track_iso_in
    const std::string track_isoname = "Track ISO";
    const std::string track_isofile = "track_iso";
    track_iso_ = fs->make<TH1D>(track_isofile.c_str(), track_isoname.c_str(), 10000, 0., 100.);
    track_iso_->GetXaxis()->SetTitle("Track ISO");
    track_iso_->GetYaxis()->SetTitle("Counts");

    // ecal_iso_in
    const std::string ecal_isoname = "ECAL ISO";
    const std::string ecal_isofile = "ecal_iso";
    ecal_iso_ = fs->make<TH1D>(ecal_isofile.c_str(), ecal_isoname.c_str(), 100000, 0., 10.);
    ecal_iso_->GetXaxis()->SetTitle("ECAL ISO");
    ecal_iso_->GetYaxis()->SetTitle("Counts");

    // hcal_iso_in
    const std::string hcal_isoname = "HCAL ISO";
    const std::string hcal_isofile = "hcal_iso";
    hcal_iso_ = fs->make<TH1D>(hcal_isofile.c_str(), hcal_isoname.c_str(), 10000, 0., 1.);
    hcal_iso_->GetXaxis()->SetTitle("HCAL ISO");
    hcal_iso_->GetYaxis()->SetTitle("Counts");

    // one_over_e_mins_one_over_p
    const std::string one_over_e_mins_one_over_pname = "1/E - 1/P";
    const std::string one_over_e_mins_one_over_pfile = "1oe_1op";
    one_over_e_mins_one_over_p_ = fs->make<TH1D>(one_over_e_mins_one_over_pfile.c_str(), one_over_e_mins_one_over_pname.c_str(), 60000, -0.3, 0.3);
    one_over_e_mins_one_over_p_->GetXaxis()->SetTitle("1/E - 1/P");
    one_over_e_mins_one_over_p_->GetYaxis()->SetTitle("Counts");

    // d0
    const std::string d0name = "d_{0}";
    const std::string d0file = "d0";
    d0_ = fs->make<TH1D>(d0file.c_str(), d0name.c_str(), 240000, -0.12, 0.12);
    d0_->GetXaxis()->SetTitle("d_{0}");
    d0_->GetYaxis()->SetTitle("Counts");

    // dz
    const std::string dzname = "d_{z}";
    const std::string dzfile = "dz";
    dz_ = fs->make<TH1D>(dzfile.c_str(), dzname.c_str(), 20000, -1., 1.);
    dz_->GetXaxis()->SetTitle("d_{z}");
    dz_->GetYaxis()->SetTitle("Counts");

    // mhits
    const std::string mhitsname = "Missing Hits";
    const std::string mhitsfile = "mhits";
    mhits_ = fs->make<TH1D>(mhitsfile.c_str(), mhitsname.c_str(), 50, 0., 50.);
    mhits_->GetXaxis()->SetTitle("Missing Hits");
    mhits_->GetYaxis()->SetTitle("Counts");

    // mhits
    const std::string conversion_matchname = "Conversion Match";
    const std::string conversion_matchfile = "con_match";
    conversion_match_ = fs->make<TH1D>(conversion_matchfile.c_str(), conversion_matchname.c_str(), 2, 0., 2.);
    conversion_match_->GetXaxis()->SetTitle("Conversion Match");
    conversion_match_->GetYaxis()->SetTitle("Counts");

    // iso
    const std::string isoname = "PF Isolation";
    const std::string isofile = "iso";
    iso_ = fs->make<TH1D>(isofile.c_str(), isoname.c_str(), 15000, 0., 1.5);
    iso_->GetXaxis()->SetTitle("PF Isolation");
    iso_->GetYaxis()->SetTitle("Counts");

    // Get the tag of the electrons we want
    ecal_electron_ = iConfig.getParameter<edm::InputTag>("ecalElectronsInputTag");

    // Set the acceptance of the electrons
    min_pt_ = 20;
    max_eta_ = 2.4;
}

IDPlotter::~IDPlotter() {
    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)
}

//
// member functions
//

// ------------ method called for each event  ------------
void
IDPlotter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    using namespace edm;

    // Only check for a truth Z in MC
    if (!iEvent.isRealData()){
        // Must be Z->ee
        const int ZBOSON = 23;
        const int ELECTRON = 11;
        bool has_z = false;
        edm::Handle<reco::GenParticleCollection> mc_particles;
        iEvent.getByLabel("genParticles", mc_particles);
        for (unsigned int i = 0; i < mc_particles->size(); ++i) {
            const reco::GenParticle* gen_particle = &mc_particles->at(i);
            // Is a Z
            if (gen_particle->pdgId() == ZBOSON) {
                for (size_t j = 0; j < gen_particle->numberOfDaughters(); ++j) {
                    if (fabs(gen_particle->daughter(j)->pdgId()) == ELECTRON) {
                        has_z = true;
                        break;
                    }
                }
            }
            if (has_z) {
                break;
            }
        }

        if (!has_z) {
            return;
        }
    }

    // Get the electrons
    edm::Handle<reco::GsfElectronCollection> els_h;
    iEvent.getByLabel(ecal_electron_, els_h);

    iEvent.getByLabel("offlinePrimaryVertices", reco_vertices_);

    // ISO
    edm::Handle< edm::ValueMap<double> > IsoDepositVals_CH;
    edm::Handle< edm::ValueMap<double> > IsoDepositVals_EM;
    edm::Handle< edm::ValueMap<double> > IsoDepositVals_NH;
    iEvent.getByLabel("elPFIsoValueCharged03PFIdPFIso", IsoDepositVals_CH);
    iEvent.getByLabel("elPFIsoValueGamma03PFIdPFIso", IsoDepositVals_EM);
    iEvent.getByLabel("elPFIsoValueNeutral03PFIdPFIso", IsoDepositVals_NH);

    // rho for isolation
    // The python uses:
    // cms.InputTag("kt6PFJetsForIsolation", "rho")
    edm::Handle<double> rho_iso_h;
    iEvent.getByLabel("kt6PFJetsForIsolation", "rho", rho_iso_h);
    const double RHO_ISO = *(rho_iso_h.product());

    // beam spot
    edm::Handle<reco::BeamSpot> beamspot_h;
    iEvent.getByLabel("offlineBeamSpot", beamspot_h);
    const reco::BeamSpot &beamspot = *(beamspot_h.product());

    // conversions
    edm::Handle<reco::ConversionCollection> conversions_h;
    iEvent.getByLabel("allConversions", conversions_h);

    for (unsigned int i = 0; i < els_h->size(); ++i) {
        const reco::GsfElectron electron = els_h->at(i);

        if (electron.pt() > min_pt_ && fabs(electron.eta()) < max_eta_) {
            // get reference to electron and the electron
            reco::GsfElectronRef ele_ref(els_h, i);

            // get particle flow isolation
            const double ISO_CH = (*IsoDepositVals_CH)[ele_ref];
            const double ISO_EM = (*IsoDepositVals_EM)[ele_ref];
            const double ISO_NH = (*IsoDepositVals_NH)[ele_ref];

            // Fill the histograms
            const double ISO = get_iso(
                    ISO_CH,
                    ISO_EM,
                    ISO_NH,
                    RHO_ISO,
                    electron.pt(),
                    electron.eta()
                    );
            bool vtxFitConversion = ConversionTools::hasMatchedConversion(electron, conversions_h, beamspot.position());

            fill_histograms(electron, ISO, electron.pt(), vtxFitConversion);
        }
    }
}

double IDPlotter::get_iso(
        const double ISO_CH,
        const double ISO_EM,
        const double ISO_NH,
        const double RHO,
        const double PT,
        const double ETA
        ) {
    // effective area for isolation
    float AEFF = ElectronEffectiveArea::GetElectronEffectiveArea(ElectronEffectiveArea::kEleGammaAndNeutralHadronIso03, ETA, ElectronEffectiveArea::kEleEAData2011);

    // apply to neutrals
    const double RHO_PRIME = std::max(RHO, 0.0);
    const double ISO_N = std::max(ISO_NH + ISO_EM - RHO_PRIME * AEFF, 0.0);

    // compute final isolation
    return (ISO_N + ISO_CH) / PT;
}

void IDPlotter::fill_histograms(const reco::GsfElectron& electron, const double ISO, const double PT, const bool CON_MATCH) {
    r9_->Fill(electron.r9());
    sigma_ieta_ieta_->Fill(electron.sigmaIetaIeta());
    h_over_e_->Fill(electron.hadronicOverEm());
    deta_in_->Fill(electron.deltaEtaSuperClusterTrackAtVtx());
    dphi_in_->Fill(electron.deltaPhiSuperClusterTrackAtVtx());
    track_iso_->Fill(electron.dr03TkSumPt()/PT);
    ecal_iso_->Fill(electron.dr03EcalRecHitSumEt()/PT);
    hcal_iso_->Fill(electron.dr03HcalTowerSumEt()/PT);
    one_over_e_mins_one_over_p_->Fill((1.0/electron.ecalEnergy() - electron.eSuperClusterOverP()/electron.ecalEnergy()));
    // Get the vertex parameter. Code from: http://cmslxr.fnal.gov/source/EgammaAnalysis/ElectronTools/src/EGammaCutBasedEleId.cc?v=CMSSW_5_3_20#0111
    double d0vtx = 0.0;
    double dzvtx = 0.0;
    if (reco_vertices_->size() > 0) {
        reco::VertexRef vtx(reco_vertices_, 0);
        d0vtx = electron.gsfTrack()->dxy(vtx->position());
        dzvtx = electron.gsfTrack()->dz(vtx->position());
    }
    else {
        d0vtx = electron.gsfTrack()->dxy();
        dzvtx = electron.gsfTrack()->dz();
    }
    d0_->Fill(d0vtx);
    dz_->Fill(dzvtx);
    // Missing hits
    mhits_->Fill(electron.gsfTrack()->hitPattern().numberOfLostTrackerHits());
    // Conversion prob
    conversion_match_->Fill(CON_MATCH);
    // ISO
    iso_->Fill(ISO);
}

// ------------ method called once each job just before starting event loop  ------------
void IDPlotter::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void IDPlotter::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void IDPlotter::beginRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void IDPlotter::endRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void IDPlotter::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void IDPlotter::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void IDPlotter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(IDPlotter);
