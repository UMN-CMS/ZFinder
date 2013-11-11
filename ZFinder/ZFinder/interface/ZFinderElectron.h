#ifndef ZFinderElectron_h_included
#define ZFinderElectron_h_included 1

#include <string>
#include <map>

#include "DataFormats/Candidate/interface/Candidate.h"  // CandidateBaseRef
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"  // GenParticle


struct CutResult{
    bool passed;
    std::string name;  // Redundant, but nice to have as a check
    double weight;
};

class ZFinderElectron{
    public:
        // Constructor
        ZFinderElectron(reco::CadidateBaseRef* particle);
        ZFinderElectron(GenParticle* particle);

        // A pointer to the object ZFinderElectron was created from
        const reco::CandidateBaseRef* electron;

        // Kinematics variables
        double pt;
        double eta;
        double phi;

        // Other physical properties
        int charge;

        // Calculated Quantities
        double pfIso;

        // Handling cuts
        CutResult* getCutResult(const std::string& cutName);
        bool cutPassed(const std::string& cutName);
        double cutWeight(const std::string& cutName);
        void addCutResult(const std::string& cutName, const bool passed, const double weight);

    private:
        std::map<std::string, CutResult> m_CutResults;
};

#endif // ZFinderElectron_h_included
