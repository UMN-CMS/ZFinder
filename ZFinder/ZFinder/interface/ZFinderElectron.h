#ifndef ZFINDER_ZFINDERELECTRON_H_
#define ZFINDER_ZFINDERELECTRON_H_

// Standard Library
#include <string>
#include <map>

// CMSSW
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"  // GsfElectron
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"  // GenParticle

// ZFinder
#include "ZFinder/ZFinder/interface/PDGID.h"  // PDGID enum (ELECTRON, POSITRON, etc.)


struct CutResult{
    bool passed;
    std::string name;  // Redundant, but nice to have as a check
    double weight;
};

class ZFinderElectronBase {
    public:
        ZFinderElectronBase() {};

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

    protected:
        std::map<std::string, CutResult> cutresults_;
};

template <class Particle_T> 
class ZFinderElectron : public ZFinderElectronBase {
    public:
        // Constructor
        ZFinderElectron(Particle_T particle) : parent_electron(particle) {
            // We need to construct specialized versions, so this should never
            // be called directly 
        }

        // A copy of the object ZFinderElectron was created from
        const Particle_T parent_electron;
};

#endif  // ZFINDER_ZFINDERELECTRON_H_
