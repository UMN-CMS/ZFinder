#ifndef ZFINDER_ZFINDERELECTRON_H_
#define ZFINDER_ZFINDERELECTRON_H_

// Standard Library
#include <string>  // std::string
#include <map>  // std::map
#include <vector>  // std::vector

// CMSSW
#include "DataFormats/Candidate/interface/Candidate.h"  // reco::Candidate
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"  // GsfElectron
#include "DataFormats/EgammaCandidates/interface/Photon.h"  // reco::Photon
#include "DataFormats/HLTReco/interface/TriggerObject.h"  // trigger::TriggerObject
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"  // reco::GenParticle
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"  // reco::RecoEcalCandidate

namespace zf {

    enum ElectronType {
        RECO_GSFELECTRON,
        RECO_GENPARTICLE,
        RECO_RECOECALCANDIDATE,
        RECO_PHOTON,
        RECO_TRIGGER
    };

    struct CutResult {
        bool passed;
        std::string name;  // Redundant, but nice to have as a check
        double weight;
    };

    class ZFinderElectron {
        public:
            ZFinderElectron() {};
            ZFinderElectron(reco::GsfElectron input_electron);
            ZFinderElectron(reco::GenParticle input_electron);
            ZFinderElectron(
                    reco::GenParticle born_electron,
                    reco::GenParticle dressed_electron,
                    reco::GenParticle naked_electron
                );
            ZFinderElectron(reco::RecoEcalCandidate input_electron);
            ZFinderElectron(reco::Photon input_electron);
            ZFinderElectron(trigger::TriggerObject input_electron);

            // Handling cuts
            const CutResult* GetCutResult(const std::string& cut_name) const;
            void AddCutResult(const std::string& cut_name, const bool passed, const double weight=1.);
            int CutPassed(const std::string& cut_name) const;
            double CutWeight(const std::string& cut_name) const;
            void SetCutWeight(const std::string& cut_name, const double WEIGHT);

            std::vector<const CutResult*>* GetPassedCuts() { return GetCutsBool(true); };
            std::vector<const CutResult*>* GetFailedCuts() { return GetCutsBool(false); };
            std::vector<const CutResult*>* GetAllCuts();

            // Return type
            ElectronType get_type() const { return candidate_type_; }

            // Getters
            double pt() const { return pt_; }
            double eta() const { return eta_; }
            double phi() const { return phi_; }
            double bornPt() const { return bornPt_; }
            double bornEta() const { return bornEta_; }
            double bornPhi() const { return bornPhi_; }
            double nakedPt() const { return nakedPt_; }
            double nakedEta() const { return nakedEta_; }
            double nakedPhi() const { return nakedPhi_; }
            int charge() const { return charge_; }
            double r9() const { return r9_; }
            double sc_eta() const { return sc_eta_; }
            double sc_phi() const { return sc_phi_; }

            //Setters
            void set_phi(double new_phi) { phi_ = new_phi; }

        protected:
            std::map<std::string, CutResult> cutresults_;

            std::vector<const CutResult*>* GetCutsBool(const bool PASSED);

            // TODO: fill out the candidate field
            reco::Candidate* candidate_;
            ElectronType candidate_type_;

            // Used to store a copy of the object used to create the ZFElectron
            reco::GsfElectron gsf_elec_;
            reco::GenParticle gen_elec_;
            reco::RecoEcalCandidate recan_elec_;
            reco::Photon photon_elec_;
            trigger::TriggerObject trig_elec_;

            // Kinematics variables
            //NOTE: the basic versions will, for the Truth case, correspond to *DRESSED* electrons
            double pt_;
            double eta_;
            double phi_;
            //"born" corresponds to PRE-FSR, "status==3" electrons
            double bornPt_;
            double bornEta_;
            double bornPhi_;
            //"naked" corresponds to POST-FSR, without FSR
            double nakedPt_;
            double nakedEta_;
            double nakedPhi_;

            // Other interesting quantities
            double r9_;

            // Other physical properties
            int charge_;

            // Supercluster quantities
            double sc_eta_;
            double sc_phi_;
    };
}  // namespace zfe
#endif  // ZFINDER_ZFINDERELECTRON_H_
