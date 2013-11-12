#ifndef ZFINDER_ZFINDEREVENT_H_
#define ZFINDER_ZFINDEREVENT_H_

// Standard Library
#include <string>  // string
#include <vector>  // vector

// CMSSW
#include "FWCore/Framework/interface/Event.h"  // edm::Event, edm::EventSetup
#include "DataFormats/Candidate/interface/Candidate.h"  // Candidate
#include "DataFormats/Candidate/interface/CandidateFwd.h"  // CandidateBaseRef
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"  // GenParticle

// ZFinder
#include "ZFinder/ZFinder/interface/ZFinderElectron.h"

struct BasicRequirements{
    // TODO: Fill in defaults
    double ept_min;
    double ept_max;
};

class ZFinderEvent{
    public:
        // Constructor (iEvent and iSetup violate our naming convention, but
        // are almost ubiquitous in CMSSW code)
        ZFinderEvent(const edm::Event& iEvent, const edm::EventSetup& iSetup, const bool use_truth);

        // Data or MC
        bool is_real_data;

        // Beam Spot
        struct Beamspot{
            double x;
            double y;
            double z;
        } bs;

        // Primary vertexes
        struct Vertexes{
            unsigned int num;
            double x;
            double y;
            double z;
        } vert;

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
        } z;

        // These are the special, selected electrons
        ZFinderElectron* e0;
        ZFinderElectron* e1;
        void setE0(const ZFinderElectron* electron);
        void setE1(const ZFinderElectron* electron);
        void setEs(const ZFinderElectron* e0, const ZFinderElectron* e1);

        // Add an electron to the internal list
        ZFinderElectron* AddElectron(Cadidate* particle);  // Both CandidateBaseRef and GenParticle inherit from Candidate

    private:
        // Called by the constructor to handle MC and Data separately
        void init_reco(const edm::Event& iEvent, const edm::EventSetup iSetup, const BasicRequirements cuts);
        void init_truth(const edm::Event& iEvent, const edm::EventSetup iSetup, const BasicRequirements cuts);

        // A list of all electrons
        std::vector<ZFinderElectron*> electrons_;

};
#endif  // ZFINDER_ZFINDEREVENT_H_
