// -*- C++ -*-
//
// Package:    ZFinder
// Class:      ZFinder
//
/**\class ZFinder ZFinder.cc ZShape/ZFinder/src/ZFinder.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Alexander Gude
//         Created:  Thu Aug  8 15:19:00 CDT 2013
// $Id$
//
//


// system include files
#include <memory>

// standard library files
#include <map>  // std::map
#include <string>  // std::string
#include <utility>  // std::pair

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// CMSSW
#include "FWCore/ServiceRegistry/interface/Service.h" // edm::Service
#include "CommonTools/UtilAlgos/interface/TFileService.h" // TFileService

// Particle Flow
#include "RecoParticleFlow/Configuration/test/PFCandidateAnalyzer.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"

// Electrons
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"  // GsfElectron
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"  // GenParticle

// ZFinder
#include "ZFinder/ZFinder/interface/ZFinderEvent.h"  // ZFinderEvent
#include "ZFinder/ZFinder/interface/ZFinderPlotter.h"  // ZFinderPlotter

//
// class declaration
//

class ZFinder : public edm::EDAnalyzer {
    public:
        explicit ZFinder(const edm::ParameterSet&);
        ~ZFinder();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


    private:
        virtual void beginJob() ;
        virtual void analyze(const edm::Event&, const edm::EventSetup&);
        virtual void endJob() ;

        virtual void beginRun(edm::Run const&, edm::EventSetup const&);
        virtual void endRun(edm::Run const&, edm::EventSetup const&);
        virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
        virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

        // ----------member data ---------------------------
        const edm::ParameterSet& iConfig_;
        std::map<std::string, zf::ZFinderPlotter*> z_plotter_map;

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
ZFinder::ZFinder(const edm::ParameterSet& iConfig) : iConfig_(iConfig) {
    //now do what ever initialization is needed
    edm::Service<TFileService> fs;

    TFileDirectory* tdir_1 = new TFileDirectory(fs->mkdir("reco"));
    zf::ZFinderPlotter* z_plotter_reco = new zf::ZFinderPlotter(tdir_1);

    TFileDirectory* tdir_2 = new TFileDirectory(fs->mkdir("truth"));
    const bool USE_MC = true;
    zf::ZFinderPlotter* z_plotter_truth = new zf::ZFinderPlotter(tdir_2, USE_MC);

    z_plotter_map.insert( std::pair<std::string, zf::ZFinderPlotter*>("reco", z_plotter_reco));
    z_plotter_map.insert( std::pair<std::string, zf::ZFinderPlotter*>("truth", z_plotter_truth));
}

ZFinder::~ZFinder() {
    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void ZFinder::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    using namespace edm;

    zf::ZFinderEvent zfe(iEvent, iSetup, iConfig_);
    if (zfe.reco_z.m > -1) {
        zfe.PrintElectrons();
        const bool PRINT_MC = true;
        zfe.PrintElectrons(PRINT_MC);
        z_plotter_map["reco"]->Fill(zfe);
        z_plotter_map["truth"]->Fill(zfe);
    }

    // Add plots
}

// ------------ method called once each job just before starting event loop  ------------
void ZFinder::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void ZFinder::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void ZFinder::beginRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ZFinder::endRun(edm::Run const&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ZFinder::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ZFinder::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void ZFinder::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ZFinder);
