#ifndef ZFinderElectron_h_included
#define ZFinderElectron_h_included 1

#include <string>
#include <map>

#include "DataFormats/Candidate/interface/Candidate.h"  // CandidateBaseRef

struct CutResult{
    bool passed;
    std::string name;  // Redundant, but nice to have as a check
    double weight;
};

class ZFinderElectron{
    public:
        double pfIso;
        const reco::CandidateBaseRef* electron;

        CutResult* getCutResult(const std::string& cutName);
        bool cutPassed(const std::string& cutName);
        double cutWeight(const std::string& cutName);
        void addCutResult(const std::string& cutName, const bool passed, const double weight);

    private:
        std::map<std::string, CutResult> m_CutResults;
};


#endif // ZFinderElectron_h_included
