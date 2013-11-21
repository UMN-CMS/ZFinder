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

template <class Particle_T> class ZFinderElectron{
    public:
        // Constructor
        ZFinderElectron(Particle_T particle) : electron(particle) {
            // This class should never ever ever take anything else.
        }

        // A copy of the object ZFinderElectron was created from
        const Particle_T electron;

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
        void AddCutResult(const std::string& cut_name, const bool passed, const double weight=1.);

    private:
        std::map<std::string, CutResult> cutresults_;
};

#endif  // ZFINDER_ZFINDERELECTRON_H_
