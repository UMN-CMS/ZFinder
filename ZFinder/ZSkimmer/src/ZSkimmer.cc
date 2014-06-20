// -*- C++ -*-
//
// Package:    ZSkimmer
// Class:      ZSkimmer
//
/**\class ZSkimmer ZSkimmer.cc ZFinder/ZSkimmer/src/ZSkimmer.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Alexander Gude
//         Created:  Fri Jun 20 13:16:14 CDT 2014
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Electrons
#include "DataFormats/Candidate/interface/Candidate.h"  // reco::Candidate
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"  // GsfElectron
#include "DataFormats/EgammaCandidates/interface/Photon.h"  // reco::Photon
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"  // reco::RecoEcalCandidate
// Collections
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"  // reco::PhotonCollection
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"  // reco::RecoEcalCandidateCollection

#include <iostream>

//
// class declaration
//

class ZSkimmer : public edm::EDFilter {
    public:
        explicit ZSkimmer(const edm::ParameterSet&);
        ~ZSkimmer();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
        virtual void beginJob() ;
        virtual bool filter(edm::Event&, const edm::EventSetup&);
        virtual void endJob() ;

        virtual bool beginRun(edm::Run&, edm::EventSetup const&);
        virtual bool endRun(edm::Run&, edm::EventSetup const&);
        virtual bool beginLuminosityBlock(edm::LuminosityBlock&,
                                          edm::EventSetup const&);
        virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

        // ----------member data ---------------------------
        double min_leading_gsf_pt_;
        double min_subleading_gsf_pt_;
        double min_hf_pt_;
        double min_nt_pt_;
        edm::InputTag gsf_electron_tag_;
        edm::InputTag hf_electron_tag_;
        edm::InputTag nt_electron_tag_;
};

//
// constructors and destructor
//
ZSkimmer::ZSkimmer(const edm::ParameterSet& iConfig) {
    // Get the minimum PT values. We set the to sane defaults as well.
    min_leading_gsf_pt_ = iConfig.getUntrackedParameter<double>("min_leading_gsf_pt", 15);
    min_subleading_gsf_pt_ = iConfig.getUntrackedParameter<double>("min_subleading_gsf_pt", 10);
    min_hf_pt_ = iConfig.getUntrackedParameter<double>("min_hf_pt", 10);
    min_nt_pt_ = iConfig.getUntrackedParameter<double>("min_nt_pt", 10);
    // Get the tags for the various electrons
    gsf_electron_tag_ = iConfig.getUntrackedParameter<edm::InputTag>("gsf_input_tag", edm::InputTag("gsfElectrons"));
    hf_electron_tag_ = iConfig.getUntrackedParameter<edm::InputTag>("hf_input_tag", edm::InputTag("hfRecoEcalCandidate"));
    nt_electron_tag_ = iConfig.getUntrackedParameter<edm::InputTag>("nt_input_tag", edm::InputTag("photons"));
}


ZSkimmer::~ZSkimmer() {
    // do anything here that needs to be done at destruction time (e.g. close
    // files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool ZSkimmer::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    using namespace edm;

    // We look for two electrons in the event. One must be a GSF electron, the
    // other can be NT or HF. If both electrons are GSF, then one must have PT
    // >= 15, otherwise they only require PT >= 10.

    // We demand at least one GSF electron with pt >= 15; this should be
    // insured by the trigger, but we'll double check it. If we have more, then
    // we accept the event.
    int number_leading_gsf = 0;
    int number_subleading_gsf = 0;
    edm::Handle<reco::GsfElectronCollection> gsf_electrons;
    iEvent.getByLabel(gsf_electron_tag_, gsf_electrons);
    for(unsigned int i = 0; i < gsf_electrons->size(); ++i) {
        reco::GsfElectron gsf_electron = gsf_electrons->at(i);
        if (gsf_electron.pt() >= min_leading_gsf_pt_) {
            ++number_leading_gsf;
        }
        // We use else if to avoid double counting
        else if (gsf_electron.pt() >= min_subleading_gsf_pt_) {
            ++number_subleading_gsf;
        }
    }
    // If we have no good GSF, reject
    if (number_leading_gsf + number_subleading_gsf <= 0) {
        return false;
    }
    // If we have at least two good electrons, we accept the event. Otherwise
    // we check No Track and HF Electrons also.
    else if (number_leading_gsf + number_subleading_gsf >= 2) {
        return true;
    }

    // Check for HF electrons
    edm::Handle<reco::RecoEcalCandidateCollection> hf_electrons;
    iEvent.getByLabel(hf_electron_tag_, hf_electrons);
    for(unsigned int i = 0; i < hf_electrons->size(); ++i) {
        reco::RecoEcalCandidate hf_electron = hf_electrons->at(i);
        if (hf_electron.pt() >= min_hf_pt_) {
            // We have already insured that we have at least one GSF electron
            // (even if it is subleading) and so only need one HF to have two
            // electrons in the event.
            return true;
        }
    }

    // Check for NT Electrons
    edm::Handle<reco::PhotonCollection> nt_electrons;
    iEvent.getByLabel(nt_electron_tag_, nt_electrons);

    // Loop over all electrons
    for(unsigned int i = 0; i < nt_electrons->size(); ++i) {
        reco::Photon nt_electron = nt_electrons->at(i);
        const double FETA = fabs(nt_electron.eta());
        if (2.4 <= FETA && FETA <= 3.) {
            if (nt_electron.pt() >= min_nt_pt_) {
                // Since we already have a GSF electron, we only need one NT
                // electron to insure two electrons in the event.
                return true;
            }
        }
    }

    // We've tried everything; the event isn't good
    return false;
}

// ------------ method called once each job just before starting event loop  ------------
void ZSkimmer::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void ZSkimmer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
bool ZSkimmer::beginRun(edm::Run&, edm::EventSetup const&) {
    return true;
}

// ------------ method called when ending the processing of a run  ------------
bool ZSkimmer::endRun(edm::Run&, edm::EventSetup const&) {
    return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
bool ZSkimmer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {
    return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
bool ZSkimmer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {
    return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void ZSkimmer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(ZSkimmer);
