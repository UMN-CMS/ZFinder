#ifndef ZFINDER_ZFINDERELECTRON_H_
#define ZFINDER_ZFINDERELECTRON_H_

// Standard Library
#include <string>
#include <map>

// CMSSW
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"  // GsfElectron
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"  // GenParticle

struct CutResult{
    bool passed;
    std::string name;  // Redundant, but nice to have as a check
    double weight;
};

class ZFinderElectron{
    public:
        // Constructor
        ZFinderElectron(HepMC::GenParticle particle);
        ZFinderElectron(GsfElectron particle);

        // A copy of the object ZFinderElectron was created from
        const reco::CandidateBaseRef electron;

        // Kinematics variables
        double pt;
        double eta;
        double phi;

        // Other physical properties
        int charge;

        // Handling cuts
        CutResult* GetCutResult(const std::string& cut_name) const;
        bool CutPassed(const std::string& cut_name) const;
        double CutWeight(const std::string& cut_name) const;
        void AddCutResult(const std::string& cut_name, const bool passed, const double weight);

    private:
        std::map<std::string, CutResult> cutresults_;
};

#endif  // ZFINDER_ZFINDERELECTRON_H_
