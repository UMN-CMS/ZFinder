// -*- C++ -*-
//
// Package:    SelectEvents
// Class:      SelectEvents
//
/**\class SelectEvents SelectEvents.cc ZFinder/SelectEvents/src/SelectEvents.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/


// system include files
#include <memory>

// standard library files
#include <algorithm>  // std::sort, std::binary_search

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
// class declaration
//

class SelectEvents : public edm::EDFilter {
    public:
        explicit SelectEvents(const edm::ParameterSet&);
        ~SelectEvents();

        static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

    private:
        virtual void beginJob() ;
        virtual bool filter(edm::Event&, const edm::EventSetup&);
        virtual void endJob() ;

        virtual bool beginRun(edm::Run&, edm::EventSetup const&);
        virtual bool endRun(edm::Run&, edm::EventSetup const&);
        virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
        virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

        // ----------member data ---------------------------
        std::vector<unsigned int> good_runs_;
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
SelectEvents::SelectEvents(const edm::ParameterSet& iConfig) {
    //now do what ever initialization is needed
    good_runs_ = iConfig.getUntrackedParameter<std::vector<unsigned int> >("runs_to_save");

    // Sort for faster lookup
    std::sort(good_runs_.begin(), good_runs_.end());
}


SelectEvents::~SelectEvents() {
    // do anything here that needs to be done at destruction time
    // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool SelectEvents::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {
    using namespace edm;
    const unsigned int EVENT_NUMBER = iEvent.id().event();

    // Binary search the good runs, reject if we don't find it
    return std::binary_search(good_runs_.begin(), good_runs_.end(), EVENT_NUMBER);
}

// ------------ method called once each job just before starting event loop  ------------
void SelectEvents::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void SelectEvents::endJob() {
}

// ------------ method called when starting to processes a run  ------------
bool SelectEvents::beginRun(edm::Run&, edm::EventSetup const&) {
    return true;
}

// ------------ method called when ending the processing of a run  ------------
bool SelectEvents::endRun(edm::Run&, edm::EventSetup const&) {
    return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
bool SelectEvents::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {
    return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
bool SelectEvents::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {
    return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void SelectEvents::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
    //The following says we do not know what parameters are allowed so do no validation
    // Please change this to state exactly what you do use, even if it is no parameters
    edm::ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(SelectEvents);
