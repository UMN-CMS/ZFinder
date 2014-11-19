// -*- C++ -*-
//
// Package:    TrigEff
// Class:      TrigEff
//
/**\class TrigEff TrigEff.cc TrigEff/TrigEff/src/TrigEff.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Alexander Gude
//         Created:  Wed Sep  3 15:40:32 CDT 2014
// $Id$
//
//


// system include files
#include <memory>

// standard library files
#include <vector>  // std::vector
#include <iostream>  // std::cout, std::endl

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// CMSSW
#include "CommonTools/UtilAlgos/interface/TFileService.h" // TFileService
#include "DataFormats/Common/interface/Handle.h"  // edm::Handle
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"  // reco::PhotonCollection
#include "DataFormats/EgammaReco/interface/HFEMClusterShape.h"  // reco::HFEMClusterShape
#include "DataFormats/EgammaReco/interface/HFEMClusterShapeAssociation.h"  // reco::HFEMClusterShapeAssociationCollection
#include "DataFormats/EgammaReco/interface/HFEMClusterShapeFwd.h"  // reco::HFEMClusterShapeRef,
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"  // reco::SuperClusterCollection, reco::SuperClusterRef
#include "DataFormats/HLTReco/interface/TriggerEvent.h" // trigger::TriggerEvent
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"  // reco::RecoEcalCandidateCollection
#include "EgammaAnalysis/ElectronTools/interface/EGammaCutBasedEleId.h"  // EgammaCutBasedEleId::PassWP, EgammaCutBasedEleId::*
#include "FWCore/ServiceRegistry/interface/Service.h" // edm::Service
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"  // GenEventInfoProduct
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"  // PileupSummaryInfo

// L1
#include "DataFormats/L1Trigger/interface/L1EmParticle.h"  // l1extra::L1EmParticle
#include "DataFormats/L1Trigger/interface/L1EmParticleFwd.h"  // l1extra::L1EmParticleCollection

// Electrons
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"  // GsfElectron

// ROOT
#include <TH2D.h>

//
// class declaration
//

class TrigEff : public edm::EDAnalyzer {
    public:
        explicit TrigEff(const edm::ParameterSet&);
        ~TrigEff();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


    private:
        virtual void beginJob();
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob();

        virtual void beginRun(edm::Run const&, edm::EventSetup const&);
        virtual void endRun(edm::Run const&, edm::EventSetup const&);
        virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
        virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

        // ----------member data ---------------------------
        TH2D* numerator_;
        TH2D* denominator_;
        edm::InputTag ecal_electron_;
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
TrigEff::TrigEff(const edm::ParameterSet& iConfig) {
    //now do what ever initialization is needed
    edm::Service<TFileService> fs;

    const std::vector<double> ETA_BINS = {0., 0.8, 1.478, 2.5};
    const std::vector<double> PT_BINS = {30., 40., 50., 200.};

    numerator_ = fs->make<TH2D>("numerator", "numerator", PT_BINS.size() - 1, &PT_BINS[0], ETA_BINS.size() - 1, &ETA_BINS[0]);
    numerator_->GetYaxis()->SetTitle("Probe #eta");
    numerator_->GetXaxis()->SetTitle("Probe p_{T}");
    denominator_ = fs->make<TH2D>("denominator", "denominator", PT_BINS.size() - 1, &PT_BINS[0], ETA_BINS.size() - 1, &ETA_BINS[0]);
    denominator_->GetYaxis()->SetTitle("Probe #eta");
    denominator_->GetXaxis()->SetTitle("Probe p_{T}");

    // Get config variables
    ecal_electron_ = iConfig.getParameter<edm::InputTag>("ecalElectronsInputTag");
    //conversion_ = iConfig.getParameter<edm::InputTag>("conversionsInputTag");
    //beamspot_ = iConfig.getParameter<edm::InputTag>("beamSpotInputTag");
    //rho_iso_ = iConfig.getParameter<edm::InputTag>("rhoIsoInputTag");
    //vertex_ = iConfig.getParameter<edm::InputTag>("primaryVertexInputTag");
    //iso_vals_ = iConfig.getParameter<std::vector<edm::InputTag> >("isoValInputTags");
}

TrigEff::~TrigEff() {

    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void TrigEff::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

    // Get 2 good GSF electrons
    edm::Handle<reco::GsfElectronCollection> els_h;
    //iEvent.getByLabel("CalibratedElectrons", "calibratedGsfElectrons", els_h);
    iEvent.getByLabel(ecal_electron_, els_h);

    // conversions
    edm::Handle<reco::ConversionCollection> conversions_h;
    iEvent.getByLabel("allConversions", conversions_h);

    // iso deposits
    edm::Handle< edm::ValueMap<double> > IsoDepositVals_CH;
    edm::Handle< edm::ValueMap<double> > IsoDepositVals_EM;
    edm::Handle< edm::ValueMap<double> > IsoDepositVals_NH;
    iEvent.getByLabel("elPFIsoValueCharged03PFIdPFIso", IsoDepositVals_CH);
    iEvent.getByLabel("elPFIsoValueGamma03PFIdPFIso", IsoDepositVals_EM);
    iEvent.getByLabel("elPFIsoValueNeutral03PFIdPFIso", IsoDepositVals_NH);

    // beam spot
    edm::Handle<reco::BeamSpot> beamspot_h;
    iEvent.getByLabel("offlineBeamSpot", beamspot_h);
    const reco::BeamSpot &beamSpot = *(beamspot_h.product());

    // vertices
    edm::Handle<reco::VertexCollection> vtx_h;
    iEvent.getByLabel("offlinePrimaryVertices", vtx_h);

    // rho for isolation
    // The python uses:
    // cms.InputTag("kt6PFJetsForIsolation", "rho")
    edm::Handle<double> rho_iso_h;
    iEvent.getByLabel("kt6PFJetsForIsolation", "rho", rho_iso_h);
    const double RHO_ISO = *(rho_iso_h.product());

    // loop on electrons
    std::vector<reco::GsfElectron> our_electrons;
    for (unsigned int i = 0; i < els_h->size(); ++i) {
        // Get the electron and set put it into the electrons vector
        reco::GsfElectron electron = els_h->at(i);
        // We enforce a minimum quality cut
        if (electron.pt() < 30 || fabs(electron.eta()) > 2.1) {
            continue;
        }

        // get reference to electron and the electron
        reco::GsfElectronRef ele_ref(els_h, i);

        // get particle flow isolation
        const double ISO_CH = (*IsoDepositVals_CH)[ele_ref];
        const double ISO_EM = (*IsoDepositVals_EM)[ele_ref];
        const double ISO_NH = (*IsoDepositVals_NH)[ele_ref];

        // test ID
        // working points
        //const bool VETO = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::VETO, ele_ref, conversions_h, beamSpot, vtx_h, ISO_CH, ISO_EM, ISO_NH, RHO_ISO);
        //const bool LOOSE = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::LOOSE, ele_ref, conversions_h, beamSpot, vtx_h, ISO_CH, ISO_EM, ISO_NH, RHO_ISO);
        //const bool MEDIUM = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::MEDIUM, ele_ref, conversions_h, beamSpot, vtx_h, ISO_CH, ISO_EM, ISO_NH, RHO_ISO);
        const bool TIGHT = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::TIGHT, ele_ref, conversions_h, beamSpot, vtx_h, ISO_CH, ISO_EM, ISO_NH, RHO_ISO);
        //const bool TRIGWP70 = EgammaCutBasedEleId::PassTriggerCuts(EgammaCutBasedEleId::TRIGGERWP70, ele_ref);

        bool save_electron = false;
        // Use only tight
        if (TIGHT) {
            save_electron = true;
        }

        // Save the electron if it passes our cuts
        if (save_electron) {
            our_electrons.push_back(electron);
        }
    }

    // Number of electrons
    if (our_electrons.size() == 2) {

        // Check how many trigger objects we have
        edm::InputTag hltTrigInfoTag("hltTriggerSummaryAOD","","HLT");
        edm::Handle<trigger::TriggerEvent> trig_event;

        iEvent.getByLabel(hltTrigInfoTag, trig_event);
        if (!trig_event.isValid() ){
            std::cout << "No valid hltTriggerSummaryAOD." << std::endl;
            return;
        }

        bool match_hlt_0 = false;
        bool match_hlt_1 = false;
        int trig_size = 0;
        edm::InputTag filter_tag("hltEle27WP80TrackIsoFilter", "", "HLT");
        trigger::size_type filter_index = trig_event->filterIndex(filter_tag);
        if(filter_index < trig_event->sizeFilters()) { // Check that the filter is in triggerEvent
            const trigger::Keys& trig_keys = trig_event->filterKeys(filter_index);
            //std::cout << "Size: " << trig_keys.size() << std::endl;
            trig_size = trig_keys.size();
            const trigger::TriggerObjectCollection& trig_obj_collection(trig_event->getObjects());
            for (auto& i_key : trig_keys) {
                const trigger::TriggerObject* trig_obj = &trig_obj_collection[i_key];
                const double DR_0 = deltaR(our_electrons[0].eta(), our_electrons[0].phi(), trig_obj->eta(), trig_obj->phi());
                if (DR_0 < 0.3) {
                    match_hlt_0 = true;
                }
                const double DR_1 = deltaR(our_electrons[1].eta(), our_electrons[1].phi(), trig_obj->eta(), trig_obj->phi());
                if (DR_1 < 0.3) {
                    match_hlt_1 = true;
                }
            }
        }

        //const int N_HLT = trig_size;

        // L1 objects match
        //bool match_0 = false;
        //bool match_1 = false;
        //edm::Handle<l1extra::L1EmParticleCollection> l1ems;
        //edm::InputTag l1em_tag("l1extraParticles", "Isolated", "RECO");
        //iEvent.getByLabel(l1em_tag, l1ems);
        ////std::cout << "Found " << l1ems->size() << " L1 EM Objects" << std::endl;
        //for(unsigned int i = 0; i < l1ems->size(); ++i) {
        //    l1extra::L1EmParticle l1_particle = l1ems->at(i);
        //    //std::cout << l1_particle.pt() << std::endl;
        //    const double DR_0 = deltaR(our_electrons[0].eta(), our_electrons[0].phi(), l1_particle.eta(), l1_particle.phi());
        //    const double DR_1 = deltaR(our_electrons[1].eta(), our_electrons[1].phi(), l1_particle.eta(), l1_particle.phi());
        //    if (DR_0 < 0.3) {
        //        match_0 = true;
        //    }
        //    if (DR_1 < 0.3) {
        //        match_1 = true;
        //    }
        //}

        // Fill historgrams
        if (match_hlt_0) {
            denominator_->Fill(our_electrons[0].pt(), fabs(our_electrons[0].eta()));
            if (match_hlt_1) {
                numerator_->Fill(our_electrons[0].pt(), fabs(our_electrons[0].eta()));
            }
        }
        if (match_hlt_1) {
            denominator_->Fill(our_electrons[1].pt(), fabs(our_electrons[1].eta()));
            if (match_hlt_0) {
                numerator_->Fill(our_electrons[1].pt(), fabs(our_electrons[1].eta()));
            }
        }
    }
}


// ------------ method called once each job just before starting event loop  ------------
void TrigEff::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void TrigEff::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void TrigEff::beginRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void TrigEff::endRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void TrigEff::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void TrigEff::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void TrigEff::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TrigEff);
