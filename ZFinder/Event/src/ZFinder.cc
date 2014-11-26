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
#include <algorithm>  // std::find
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

// Root
#include <TH1I.h>

// ZFinder
#include "ZFinder/Event/interface/AcceptanceSetter.h"  // AcceptanceSetter
#include "ZFinder/Event/interface/SetterBase.h"  // SetterBase
#include "ZFinder/Event/interface/TruthMatchSetter.h"  // TruthMatchSetter
#include "ZFinder/Event/interface/ZDefinition.h"  // ZDefinition
#include "ZFinder/Event/interface/ZDefinitionTree.h"  // ZDefinitionTree
#include "ZFinder/Event/interface/ZDefinitionWriter.h"  // ZDefinitionWriter
#include "ZFinder/Event/interface/ZEfficiencies.h" // ZEfficiencies
#include "ZFinder/Event/interface/ZFinderEvent.h"  // ZFinderEvent

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
        std::vector<zf::SetterBase*> setters_;
        std::vector<edm::ParameterSet> zdef_psets_;
        std::vector<zf::ZDefinition*> zdefs_;
        std::vector<zf::ZDefinitionWriter*> zdef_plotters_;
        std::vector<zf::ZDefinitionTree*> zdef_tuples_;
        zf::ZEfficiencies zeffs_;
        bool is_mc_;
        TH1I* unweighted_counter_;
        TH1D* weighted_counter_;

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

    // is_mc_ is used to determine if we should make truth objects
    is_mc_ = iConfig.getParameter<bool>("is_mc");

    // Setup Cut Setters
    zf::AcceptanceSetter* accset = new zf::AcceptanceSetter();
    setters_.push_back(accset);
    if (is_mc_) {
        zf::TruthMatchSetter* tmset = new zf::TruthMatchSetter();
        setters_.push_back(tmset);
    }

    // Setup plotters
    edm::Service<TFileService> fs;

    // We use a counter histogram to count events considered, this is used to
    // normalize MC
    unweighted_counter_ = fs->make<TH1I>("unweighted_counter", "Unweighted Event Count", 3, 0, 2);
    unweighted_counter_->GetXaxis()->SetTitle("");
    unweighted_counter_->GetYaxis()->SetTitle("Number of events considered");
    weighted_counter_ = fs->make<TH1D>("weighted_counter", "weighted Event Count", 3, 0, 2);
    weighted_counter_->GetXaxis()->SetTitle("");
    weighted_counter_->GetYaxis()->SetTitle("Number of events considered");

    // Setup ZDefinitions and plotters
    zdef_psets_ = iConfig.getUntrackedParameter<std::vector<edm::ParameterSet> >("ZDefinitions");
    for (auto& i_pset : zdef_psets_) {
        // Unpack each of the zdef_psets and set up the variables to make both
        // a reco and a truth set
        std::string name_reco = i_pset.getUntrackedParameter<std::string>("name") + " Reco";
        std::vector<std::string> cuts0 = i_pset.getUntrackedParameter<std::vector<std::string> >("cuts0");
        std::vector<std::string> cuts1 = i_pset.getUntrackedParameter<std::vector<std::string> >("cuts1");
        double min_mz = i_pset.getUntrackedParameter<double>("min_mz");
        double max_mz = i_pset.getUntrackedParameter<double>("max_mz");
        bool use_truth_mass = i_pset.getUntrackedParameter<bool>("use_truth_mass");

        // Now we make the ZDefs for Reco/Truth, and use those to set up the
        // plotters.
        // Reco
        zf::ZDefinition* zd_reco = new zf::ZDefinition(name_reco, cuts0, cuts1, min_mz, max_mz, use_truth_mass);
        zdefs_.push_back(zd_reco);
        TFileDirectory tdir_zd(fs->mkdir(name_reco));
        bool use_truth = false;
        zf::ZDefinitionWriter* zdwriter_reco = new zf::ZDefinitionWriter(*zd_reco, tdir_zd, use_truth);
        zdef_plotters_.push_back(zdwriter_reco);

        if (is_mc_) {
            std::string name_truth = i_pset.getUntrackedParameter<std::string>("name") + " MC";
            // Truth
            zf::ZDefinition* zd_truth = new zf::ZDefinition(name_truth, cuts0, cuts1, min_mz, max_mz, use_truth_mass);
            zdefs_.push_back(zd_truth);
            TFileDirectory tdir_zd_truth(fs->mkdir(name_truth));
            use_truth = true;
            zf::ZDefinitionWriter* zdwriter_truth = new zf::ZDefinitionWriter(*zd_truth, tdir_zd_truth, use_truth);
            zdef_plotters_.push_back(zdwriter_truth);
        }

        // We use zd_reco, but that's only because both the "reco" and
        // "truth" quantities are stored in the same Tree, so there is no
        // need to make a second tree like there is with the plots.
        zf::ZDefinitionTree* zdtree = new zf::ZDefinitionTree(*zd_reco, tdir_zd, is_mc_);
        zdef_tuples_.push_back(zdtree);
    }
}

ZFinder::~ZFinder() {
    // do anything here that needs to be done at destruction time
    // (e.g. close files, deallocate resources etc.)

    // Delete our heap variables
    for (auto& i_set : setters_) {
        delete i_set;
    }
    for (auto& i_zdef : zdefs_) {
        delete i_zdef;
    }
    for (auto& i_zdefp : zdef_plotters_) {
        delete i_zdefp;
    }
    for (auto& i_zdeft : zdef_tuples_) {
        delete i_zdeft;
    }
}


//
// member functions
//

// ------------ method called for each event  ------------
void ZFinder::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
    using namespace edm;

    // We count every event, even if they do not pass any cuts
    unweighted_counter_->Fill(1);

    // Construct a ZFinderEvent
    zf::ZFinderEvent zfe(iEvent, iSetup, iConfig_);

    // For MC, some events are weighted even without any additional
    // reweighting, so we count those, again, even if they don't pass any cuts.
    weighted_counter_->Fill(1, zfe.weight_natural_mc);

    // If the even has a good Z, process further
    if ( (zfe.reco_z.m > -1 && zfe.e0 != nullptr && zfe.e1 != nullptr)
            || (zfe.truth_z.m > -1 && zfe.e0_truth != nullptr && zfe.e1_truth != nullptr)
       ) {  // We have a good Z in Reco OR Truth
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
        for (auto& i_zdef : zdefs_) {
            i_zdef->ApplySelection(&zfe);
        }
        // Make all ZDef plots
        for (auto& i_zdefp : zdef_plotters_) {
            i_zdefp->Fill(zfe);
        }
        // Make all ZDef Trees
        for (auto& i_zdeft : zdef_tuples_) {
            i_zdeft->Fill(zfe);
        }
    }
}

// ------------ method called once each job just before starting event loop  ------------
void ZFinder::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void ZFinder::endJob() {
    // Since large trees will automatically make new files, we need to get
    // the current file from each tree and write it, but only if it is new
    // and hasn't be previously written
    TFile* file = nullptr;
    std::vector<TFile*> seen;

    for (auto& i_zdeft : zdef_tuples_) {
        file = i_zdeft->GetCurrentFile();
        // Check if we have seen this file before. If we have not then
        // write the file and add it to the vector
        if (std::find(seen.begin(), seen.end(), file) == seen.end()) {
            file->Write();
            seen.push_back(file);
        }
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
