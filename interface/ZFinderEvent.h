#ifndef ZFINDEREVENT_H
#define ZFINDEREVENT_H 1

//#include "ZShape/Base/interface/ZShapeEvent.h"  // Parent class

#include "ZShape/ZFinder/interface/ZFinderElectron.h"

class ZFinderEvent{
    public:
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
            unsigned int runNum;
        } id;

        // These are the special, selected electrons
        ZFinderElectron* e0;
        ZFinderElectron* e1;
        void setE0(const ZFinderElectron* electron);
        void setE1(const ZFinderElectron* electron);

    private:
        // A list of all electrons
        std::vector<ZFinderElectron*> electrons_;
};

#endif
