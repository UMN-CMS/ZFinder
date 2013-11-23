#ifndef ZFINDER_ZFINDERELECTRON_H_
#define ZFINDER_ZFINDERELECTRON_H_

// Standard Library
#include <string>  // std::string
#include <map>  // std::map
#include <iostream>  // std::cout, std::endl

// CMSSW
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"  // GsfElectron
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"  // GenParticle
#include "DataFormats/Candidate/interface/Candidate.h"  // reco::Candidate

// ZFinder
#include "ZFinder/ZFinder/interface/PDGID.h"  // PDGID enum (ELECTRON, POSITRON, etc.)


namespace zf {

    enum ElectronType {
        GSFELECTRON,
        GENPARTICLE
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
            ZFinderElectron(HepMC::GenParticle input_electron);

            // Kinematics variables
            double pt;
            double eta;
            double phi;

            // Other physical properties
            int charge;

            // Handling cuts
            const CutResult* GetCutResult(const std::string& cut_name) const;
            bool CutPassed(const std::string& cut_name) const;
            double CutWeight(const std::string& cut_name) const;
            void AddCutResult(const std::string& cut_name, const bool passed, const double weight=1.);

            // Return type
            ElectronType get_type() const { return candidate_type_; }

        protected:
            std::map<std::string, CutResult> cutresults_;
            // TODO: fill out the candidate field
            const reco::Candidate* candidate_;
            ElectronType candidate_type_;
    };
}  // namespace zfe
#endif  // ZFINDER_ZFINDERELECTRON_H_
