#ifndef ZFINDEREVENT_H
#define ZFINDEREVENT_H 1

//#include "ZShape/Base/interface/ZShapeEvent.h"  // Parent class

#include "ZShape/ZFinder/interface/ZFinderElectron.h"
#include "FWCore/Framework/interface/Event.h" 

#include"CMSSW/DataFormats/Candidate/interface/Candidate.h"  // Candidate
#include "DataFormats/Candidate/interface/Candidate.h"  // CandidateBaseRef
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"  // GenParticle

struct BasicRequirements{
    // TODO: Fill in defaults
    double ept_MIN;
    double ept_MAX;
};

class ZFinderEvent{
    public:
        // Constructor
        ZFinderEvent(const edm::Event& iEvent, const edm::EventSetup iSetup, const bool useTruth);

        // Data or MC
        bool isRealData;

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
            unsigned int runNum;
            unsigned int lumiNum;
            unsigned int eventNum;
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

    private:
        // Called by the constructor to handle MC and Data separately
        void init_reco(const edm::Event& iEvent, const edm::EventSetup iSetup, const BasicRequirements);
        void init_truth(const edm::Event& iEvent, const edm::EventSetup iSetup, const BasicRequirements);

        // A list of all electrons
        ZFinderElectron* addElectron(Cadidate* particle);  // Both CandidateBaseRef and GenParticle inherit from Candidate
        std::vector<ZFinderElectron*> m_electrons;

};

#endif
