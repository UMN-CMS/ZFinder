#ifndef ZFINDER_ZFINDERELECTRON_H_
#define ZFINDER_ZFINDERELECTRON_H_

// Standard Library
#include <string>  // std::string
#include <map>  // std::map
#include <vector>  // std::vector

// CMSSW
#include "DataFormats/Candidate/interface/Candidate.h"  // reco::Candidate
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"  // GsfElectron
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"  // reco::GenParticle
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidate.h"  // reco::RecoEcalCandidate

namespace zf {

    enum ElectronType {
        GSFELECTRON,
        GENPARTICLE,
        RECOECALCANDIDATE
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
            ZFinderElectron(reco::RecoEcalCandidate input_electron);

            // Kinematics variables
            double pt;
            double eta;
            double phi;

            // Other physical properties
            int charge;

            //Efficiency
            double eff;
            double eff_uncertainty;

            // Handling cuts
            const CutResult* GetCutResult(const std::string& cut_name) const;
            void AddCutResult(const std::string& cut_name, const bool passed, const double weight=1.);
            bool CutPassed(const std::string& cut_name) const;
            double CutWeight(const std::string& cut_name) const;

            std::vector<const CutResult*>* GetPassedCuts() { return GetCutsBool_(true); };
            std::vector<const CutResult*>* GetFailedCuts() { return GetCutsBool_(false); };
            std::vector<const CutResult*>* GetAllCuts();

            // Return type
            ElectronType get_type() const { return candidate_type_; }

        protected:
            std::map<std::string, CutResult> cutresults_;

            std::vector<const CutResult*>* GetCutsBool_(const bool PASSED);

            // TODO: fill out the candidate field
            reco::Candidate* candidate_;
            ElectronType candidate_type_;

            // Used to store a copy of the object used to create the ZFElectron
            reco::GsfElectron gsf_elec_;
            reco::GenParticle gen_elec_;
            reco::RecoEcalCandidate recan_elec_;

    };
}  // namespace zfe
#endif  // ZFINDER_ZFINDERELECTRON_H_
