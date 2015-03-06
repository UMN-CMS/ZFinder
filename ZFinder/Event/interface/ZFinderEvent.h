#ifndef ZFINDER_ZFINDEREVENT_H_
#define ZFINDER_ZFINDEREVENT_H_

// Standard Library
#include <map>  // std::map
#include <string>  // std::string
#include <utility>  // std::pair
#include <vector>  // std::vector

// CMSSW
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"  // reco::GsfElectron
#include "DataFormats/EgammaCandidates/interface/Photon.h"  // reco::Photon
#include "DataFormats/HLTReco/interface/TriggerObject.h"  // trigger::TriggerObject
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"  // reco::GenParticle
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"  // reco::RecoEcalCandidate
#include "FWCore/Framework/interface/Event.h"  // edm::Event, edm::EventSetup
#include "FWCore/ParameterSet/interface/ParameterSet.h"  // edm::ParameterSet
#include "FWCore/Utilities/interface/InputTag.h"  // edm::InputTag
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"  // edm::LumiReWeighting

// ZFinder
#include "ZFinder/Event/interface/ZFinderElectron.h"  // ZFinderElectron, ZFinderElectron
#include "ZFinder/Event/interface/CutLevel.h"  // CutLevel, cutlevel_pair, cutlevel_vector

namespace zf {

    // Used to pass around trigger objects for matching
    typedef std::pair<const trigger::TriggerObject*, double> trig_dr_pair;
    typedef std::vector<trig_dr_pair> trig_dr_vec;

    class ZFinderEvent{
        public:
            // Constructor. Although iEvent, iSetup, and iConfig violate our naming
            // convention, they are almost ubiquitous in CMSSW code
            ZFinderEvent() {}
            ZFinderEvent(
                    const edm::Event& iEvent,
                    const edm::EventSetup& iSetup,
                    const edm::ParameterSet& iConfig
                    );
            // Destructor
            ~ZFinderEvent();

            // Data or MC
            bool is_real_data;

            // Beam Spot
            struct Beamspot{
                double x;
                double y;
                double z;
            } reco_bs;

            // Primary vertexes
            struct Vertexes{
                int num;
                double true_num;
                double x;
                double y;
                double z;
            } truth_vert, reco_vert;

            // Event ID
            struct EventID{
                unsigned int run_num;
                unsigned int lumi_num;
                unsigned int event_num;
            } id;

            // Z Data
            struct ZData{
                double m;
                double pt;
                double y;
                double phistar;
                double bornPhistar;
                double nakedPhistar;
                double scPhistar;
                double eta;
                double deltaR;
                //for events where gen XOR reco failed given selection:
                double other_y;
                double other_phistar;
            } reco_z, truth_z;

            // Event weight, used for things like pileup reweighting and the
            // natural weight of the MC event. Most other weights are cut
            // dependent (efficiencies for example) and so are store with the
            // cuts in each electron, with a combined efficiency calculated in
            // ZDefinition.
            double event_weight;
            double weight_fsr;
            double weight_vertex;
            double weight_vertex_plus;
            double weight_vertex_minus;
            double weight_natural_mc;
            std::vector<double> weights_cteq;
            std::vector<double> weights_mstw;
            std::vector<double> weights_nnpdf;

            // These are the special, selected electrons used to make the Z
            ZFinderElectron* e0;
            ZFinderElectron* e1;
            void set_e0(ZFinderElectron* electron) { e0 = electron; }
            void set_e1(ZFinderElectron* electron) { e1 = electron; }
            void set_both_e(ZFinderElectron* electron0, ZFinderElectron* electron1) { e0 = electron0; e1 = electron1; }
            ZFinderElectron* e0_truth;
            ZFinderElectron* e1_truth;
            void set_e0_truth(ZFinderElectron* electron) { e0_truth = electron; }
            void set_e1_truth(ZFinderElectron* electron) { e1_truth = electron; }
            void set_both_e_truth(ZFinderElectron* electron0, ZFinderElectron* electron1) { e0_truth = electron0; e1_truth = electron1; }
            ZFinderElectron* e0_trig;
            ZFinderElectron* e1_trig;
            void set_e0_trig(ZFinderElectron* electron) { e0_trig = electron; }
            void set_e1_trig(ZFinderElectron* electron) { e1_trig = electron; }
            void set_both_e_trig(ZFinderElectron* electron0, ZFinderElectron* electron1) { e0_trig = electron0; e1_trig = electron1; }

            // Access pruned lists of the internal electrons
            std::vector<ZFinderElectron*>* FilteredElectrons();
            std::vector<ZFinderElectron*>* AllElectrons() { return FilteredElectrons(); }
            std::vector<ZFinderElectron*>* FilteredElectrons(const std::string& cut_name);

            // Number of Electrons
            int n_reco_electrons;

            // Output
            void PrintElectrons(const int TYPE = 0, const bool PRINT_CUTS = false);  // 0 is reco, 1 is truth, 2 is trig
            void PrintTruthElectrons(const bool PRINT_CUTS = false) { PrintElectrons(1, PRINT_CUTS); }
            void PrintRecoElectrons(const bool PRINT_CUTS = false) { PrintElectrons(0, PRINT_CUTS); }
            void PrintTrigElectrons(const bool PRINT_CUTS = false) { PrintElectrons(2, PRINT_CUTS); }

            // Access ZDefinition information
            void AddZDef(const std::string NAME, cutlevel_vector PASS_OBJ) { zdef_map_[NAME] = PASS_OBJ; }
            const cutlevel_vector* GetZDef(const std::string& NAME) const;
            bool ZDefPassed(const std::string& NAME) const;
            void PrintZDefs(const bool VERBOSE = false) const;

        protected:
            // These variables are defined at the top of ZFinderEvent.cc to
            // avoid compilation issues
            static const double TRIG_DR_;
            static const double NT_DR_;

            // Called by the constructor to handle MC and Data separately
            void InitReco(const edm::Event& iEvent, const edm::EventSetup& iSetup);
            void InitTruth(const edm::Event& iEvent, const edm::EventSetup& iSetup);
            void InitTrigger(const edm::Event& iEvent, const edm::EventSetup& iSetup);

            void InitGSFElectrons(const edm::Event& iEvent, const edm::EventSetup& iSetup);
            void InitHFElectrons(const edm::Event& iEvent, const edm::EventSetup& iSetup);
            void InitNTElectrons(const edm::Event& iEvent, const edm::EventSetup& iSetup);

            void SetLumiEventWeight(const edm::Event& iEvent);
            void SetMCEventWeight(const edm::Event& iEvent);

            // Update the Z Info from e0, e1
            void InitZ();

            // Initialize all variables to safe values
            void InitVariables();

            // Input tags
            struct InputTags{
                edm::InputTag ecal_electron;
                edm::InputTag nt_electron;
                edm::InputTag conversion;
                edm::InputTag beamspot;
                edm::InputTag rho_iso;
                edm::InputTag vertex;
                edm::InputTag pileup;
                edm::InputTag generator;
                std::vector<edm::InputTag> iso_vals;
                edm::InputTag hf_electron;
                edm::InputTag hf_clusters;
            } inputtags_;

            // Find matching trigger objects
            const trig_dr_vec* GetMatchedTriggerObjects(
                    const edm::Event& iEvent,
                    const std::vector<std::string>& trig_names,
                    const double ETA, const double PHI, const double DR_CUT
                    );
            const trigger::TriggerObject* GetBestMatchedTriggerObject(
                    const edm::Event& iEvent,
                    const std::vector<std::string>& trig_names,
                    const double ETA, const double PHI
                    );
            bool TriggerMatch(
                    const edm::Event& iEvent,
                    const std::vector<std::string>& trig_names,
                    const double ETA, const double PHI, const double DR_CUT
                    );

            // A list of all electrons, split into reco and gen
            std::vector<ZFinderElectron*> reco_electrons_;
            ZFinderElectron* AddRecoElectron(reco::GsfElectron electron);
            ZFinderElectron* AddRecoElectron(reco::RecoEcalCandidate electron);
            ZFinderElectron* AddRecoElectron(reco::Photon electron);

            std::vector<ZFinderElectron*> truth_electrons_;
            ZFinderElectron* AddTruthElectron(reco::GenParticle electron);
            ZFinderElectron* AddTruthElectron(
                    reco::GenParticle bornElectron,
                    reco::GenParticle dressedElectron,
                    reco::GenParticle nakedElectron
                    );

            std::vector<ZFinderElectron*> hlt_electrons_;
            ZFinderElectron* AddHLTElectron(trigger::TriggerObject electron);

            //follow an electron from vertex to post-fsr
            const reco::GenParticle* GetNakedElectron(
                    const reco::GenParticle * const BORN_ELECTRON
                    );
            //dress a post-fsr electron with photons around it
            const reco::GenParticle* GetDressedElectron(
                    const reco::GenParticle * const BORN_ELECTRON,
                    const reco::GenParticle * const NAKED_ELECTRON,
                    const double MAX_DELTA_R = 0.1
                    );

            // Calculate phistar
            static double ReturnPhistar(const double& eta0, const double& phi0, const double& eta1, const double& phi1);

            // Sorting functions
            static bool SortByPTHighLow(const ZFinderElectron* e0, const ZFinderElectron* e1) { return (e0->pt() > e1->pt()); }

            // Print cuts
            void PrintCuts(ZFinderElectron* zf_elec);

            // Store ZDefinition Information
            std::map<std::string, cutlevel_vector> zdef_map_;

            // Pileup reweighting
            static edm::LumiReWeighting* lumi_weights_;
            static edm::LumiReWeighting* lumi_weights_plus_;
            static edm::LumiReWeighting* lumi_weights_minus_;

            // Use the muon acceptance requirements to select electrons
            bool use_muon_acceptance_;

            // Reject events that do not have a generator Z->ee event
            bool require_gen_z_;

            // Allow turning off of PDF weight vectors
            bool run_pdf_weights_;

            // Allow turning off of fsr weight calculation
            bool run_fsr_weight_;

            // NT Bending Correction
            void ApplyNTBendingCorrection();
    };
}  // namespace zf
#endif  // ZFINDER_ZFINDEREVENT_H_
