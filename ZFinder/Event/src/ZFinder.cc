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
#include "ZFinder/Event/interface/ZDefinitionWorkspace.h"  // ZDefinitionWorkspace
#include "ZFinder/Event/interface/ZEfficiencies.h" // ZEfficiencies
#include "ZFinder/Event/interface/ZFinderEvent.h"  // ZFinderEvent
#include "ZFinder/Event/interface/ZFinderPlotter.h"  // ZFinderPlotter

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
        std::vector<zf::ZDefinition*> zdefs_reco_;
        std::vector<zf::ZDefinition*> zdefs_truth_;
        std::vector<zf::ZDefinitionPlotter*> zdef_plotters_reco_;
        std::vector<zf::ZDefinitionPlotter*> zdef_plotters_truth_;
        std::vector<zf::ZDefinitionWorkspace*> zdef_workspaces_reco_;
        std::vector<zf::ZDefinitionWorkspace*> zdef_workspaces_truth_;
        zf::ZEfficiencies zeffs_;

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

    // Setup Cut Setters
    zf::AcceptanceSetter* accset = new zf::AcceptanceSetter();
    setters_.push_back(accset);
    zf::TruthMatchSetter* tmset = new zf::TruthMatchSetter();
    setters_.push_back(tmset);

    // Setup plotters
    edm::Service<TFileService> fs;

    // Setup ZDefinitions and plotters
    zdef_psets_ = iConfig.getUntrackedParameter<std::vector<edm::ParameterSet> >("ZDefinitions");
    for (auto& i_pset : zdef_psets_) {
        // Unpack each of the zdef_psets and set up the variables to make both
        // a reco and a truth set
        std::string name_reco = i_pset.getUntrackedParameter<std::string>("name") + " Reco";
        std::string name_truth = i_pset.getUntrackedParameter<std::string>("name") + " MC";
        std::vector<std::string> cuts0 = i_pset.getUntrackedParameter<std::vector<std::string> >("cuts0");
        std::vector<std::string> cuts1 = i_pset.getUntrackedParameter<std::vector<std::string> >("cuts1");
        double min_mz = i_pset.getUntrackedParameter<double>("min_mz");
        double max_mz = i_pset.getUntrackedParameter<double>("max_mz");
        bool use_truth_mass = i_pset.getUntrackedParameter<bool>("use_truth_mass");

        // Now we make the ZDefs for Reco/Truth, and use those to set up the
        // plotters.
        // Reco
        zf::ZDefinition* zd_reco = new zf::ZDefinition(name_reco, cuts0, cuts1, min_mz, max_mz, use_truth_mass);
        zdefs_reco_.push_back(zd_reco);
        TFileDirectory tdir_zd(fs->mkdir(name_reco));
        bool use_truth = false;
        zf::ZDefinitionPlotter* zdp_reco = new zf::ZDefinitionPlotter(*zd_reco, tdir_zd, use_truth);
        zdef_plotters_reco_.push_back(zdp_reco);
        zf::ZDefinitionWorkspace* zdw_reco = new zf::ZDefinitionWorkspace(*zd_reco, tdir_zd, use_truth, true);
        zdef_workspaces_reco_.push_back(zdw_reco);

        // Truth
        zf::ZDefinition* zd_truth = new zf::ZDefinition(name_truth, cuts0, cuts1, min_mz, max_mz, use_truth_mass);
        zdefs_truth_.push_back(zd_truth);
        TFileDirectory tdir_zd_truth(fs->mkdir(name_truth));
        use_truth = true;
        zf::ZDefinitionPlotter* zdp_truth = new zf::ZDefinitionPlotter(*zd_truth, tdir_zd_truth, use_truth);
        zdef_plotters_truth_.push_back(zdp_truth);
        zf::ZDefinitionWorkspace* zdw_truth = new zf::ZDefinitionWorkspace(*zd_truth, tdir_zd_truth, use_truth, true);
        zdef_workspaces_truth_.push_back(zdw_truth);
    }
}

ZFinder::~ZFinder() {
    // do anything here that needs to be done at destruction time
    // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void ZFinder::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    using namespace edm;

    zf::ZFinderEvent zfe(iEvent, iSetup, iConfig_);
    if (zfe.reco_z.m > -1 && zfe.e0 != NULL && zfe.e1 != NULL) {  // We have a good Z
        // Set all cuts
        for (auto& i_set : setters_) {
            i_set->SetCuts(&zfe);
        }
        // Set the weights; must be before setting the ZDefs, but after setting
        // the cuts
        if (!zfe.is_real_data) {
            // We set weights for MC only, as we don't want to change the data
            zeffs_.SetWeights(&zfe);
        }
        // Set all ZDefs
        for (auto& i_zdef : zdefs_reco_) {
            i_zdef->ApplySelection(&zfe);
        }
        // Set Truth ZDefs only for MC
        if (!zfe.is_real_data) {
            for (auto& i_zdef : zdefs_truth_) {
                i_zdef->ApplySelection(&zfe);
            }
        }
        // Make all ZDef plots
        for (auto& i_zdefp : zdef_plotters_reco_) {
            i_zdefp->Fill(zfe);
        }
        // Only make truth plots for MC
        if (!zfe.is_real_data) {
            for (auto& i_zdefp : zdef_plotters_truth_) {
                i_zdefp->Fill(zfe);
            }
        }
        // Make all ZDef workspaces for reco (data and reco MC)
        for (auto& i_zdefw : zdef_workspaces_reco_) {
            i_zdefw->Fill(zfe);
        }
        // Only fill with truth quantities for MC
        if (!zfe.is_real_data) {
            for (auto& i_zdefw : zdef_workspaces_truth_) {
                i_zdefw->Fill(zfe);
            }
        }
    }
}

// ------------ method called once each job just before starting event loop  ------------
void ZFinder::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void ZFinder::endJob() {
    // Write all ZDef workspaces
    for (auto& i_zdefw : zdef_workspaces_reco_) {
        i_zdefw->Write();
    }
    for (auto& i_zdefw : zdef_workspaces_truth_) {
        i_zdefw->Write();
    }
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
