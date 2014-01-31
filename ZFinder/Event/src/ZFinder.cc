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
#include "ZFinder/Event/interface/AcceptanceSetter.h"  // AcceptanceSetter
#include "ZFinder/Event/interface/SetterBase.h"  // SetterBase
#include "ZFinder/Event/interface/TruthMatchSetter.h"  // TruthMatchSetter
#include "ZFinder/Event/interface/ZDefinition.h"  // ZDefinition
#include "ZFinder/Event/interface/ZDefinitionPlotter.h"  // ZDefinitionPlotter
#include "ZFinder/Event/interface/ZFinderEvent.h"  // ZFinderEvent
#include "ZFinder/Event/interface/ZFinderPlotter.h"  // ZFinderPlotter
#include "ZFinder/Event/interface/ZFinderFitter.h"  // ZFinderPlotter

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
        std::map<std::string, zf::ZFinderPlotter*> z_plotter_map_;
        std::vector<zf::SetterBase*> setters_;
        std::vector<edm::ParameterSet> zdef_psets_;
        std::vector<zf::ZDefinition*> zdefs_;
        std::vector<zf::ZDefinitionPlotter*> zdef_plotters_;
        zf::ZFinderFitter* z_fitter;

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

    // Set up Cut Setters
    zf::AcceptanceSetter* accset = new zf::AcceptanceSetter();
    setters_.push_back(accset);
    zf::TruthMatchSetter* tmset = new zf::TruthMatchSetter();
    setters_.push_back(tmset);

    // Set up plotters
    edm::Service<TFileService> fs;

    TFileDirectory tdir_1(fs->mkdir("reco"));
    zf::ZFinderPlotter* z_plotter_reco = new zf::ZFinderPlotter(tdir_1);

    TFileDirectory tdir_2(fs->mkdir("truth"));
    const bool USE_MC = true;
    zf::ZFinderPlotter* z_plotter_truth = new zf::ZFinderPlotter(tdir_2, USE_MC);

    // Make single cut plots
    z_plotter_map_.insert(std::pair<std::string, zf::ZFinderPlotter*>("reco", z_plotter_reco));
    z_plotter_map_.insert(std::pair<std::string, zf::ZFinderPlotter*>("truth", z_plotter_truth));

     z_fitter= new zf::ZFinderFitter();

    // Set up ZDefinitions and plotters
    zdef_psets_ = iConfig.getUntrackedParameter<std::vector<edm::ParameterSet> >("ZDefinitions");
    std::vector<edm::ParameterSet>::const_iterator i_pset;
    for (i_pset = zdef_psets_.begin(); i_pset != zdef_psets_.end(); ++i_pset) {
        std::string name = i_pset->getUntrackedParameter<std::string>("name");
        std::vector<std::string> cuts0 = i_pset->getUntrackedParameter<std::vector<std::string> >("cuts0");
        std::vector<std::string> cuts1 = i_pset->getUntrackedParameter<std::vector<std::string> >("cuts1");
        double min_mz = i_pset->getUntrackedParameter<double>("min_mz");
        double max_mz = i_pset->getUntrackedParameter<double>("max_mz");
        // Make the ZDef
        zf::ZDefinition* zd = new zf::ZDefinition(name, cuts0, cuts1, min_mz, max_mz);
        zdefs_.push_back(zd);
        // Make the Plotter for the ZDef
        // Reco
        TFileDirectory tdir_zd(fs->mkdir(name + " Reco"));
        zf::ZDefinitionPlotter* zdp_reco = new zf::ZDefinitionPlotter(*zd, tdir_zd, false);  // False = do not plot Truth
        zdef_plotters_.push_back(zdp_reco);
        // MC
        TFileDirectory tdir_zd_truth(fs->mkdir(name + " MC"));
        zf::ZDefinitionPlotter* zdp_truth = new zf::ZDefinitionPlotter(*zd, tdir_zd_truth, true);
        zdef_plotters_.push_back(zdp_truth);
    }
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
    z_fitter->FillAll(zfe);
    if (zfe.reco_z.m > -1 && zfe.e0 != NULL && zfe.e1 != NULL) {  // We have a good Z
        // Set all cuts
        std::vector<zf::SetterBase*>::const_iterator i_set;
        for (i_set = setters_.begin(); i_set != setters_.end(); ++i_set) {
            (*i_set)->SetCuts(&zfe);
        }
        // Set all ZDefs
        std::vector<zf::ZDefinition*>::const_iterator i_zdef;
        for (i_zdef = zdefs_.begin(); i_zdef != zdefs_.end(); ++i_zdef) {
            (*i_zdef)->ApplySelection(&zfe);
        }
        // Make all ZDef plots
        std::vector<zf::ZDefinitionPlotter*>::const_iterator i_zdefp;
        for (i_zdefp = zdef_plotters_.begin(); i_zdefp != zdef_plotters_.end(); ++i_zdefp) {
            (*i_zdefp)->Fill(zfe);
        }

        // Print all information about each electron
        //const bool PRINT_CUTS = true;
        //zfe.PrintRecoElectrons(PRINT_CUTS);
        //zfe.PrintTruthElectrons(PRINT_CUTS);

        // Print all information about the ZDefinitions
        //const bool VERBOSE = true;
        //zfe.PrintZDefs(VERBOSE);
        // Make plots
        //zdef_plot_->Fill(zfe);
        // Fill specific plots
        if (zfe.ZDefPassed("ET-HF")) {
            z_plotter_map_["reco"]->Fill(zfe);
            z_plotter_map_["truth"]->Fill(zfe);
        }
	if (zfe.ZDefPassed("ET-ET")) {
	  z_fitter->FillSelected(zfe);
	}

    }
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
