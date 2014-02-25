#ifndef ZFINDER_ZDEFINITIONPLOTTER_H_
#define ZFINDER_ZDEFINITIONPLOTTER_H_

// Standard Library
#include <map>  // std::map
#include <string>  // string
#include <vector>  // std::vector

// CMSSW
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// ZFinder Code
#include "ZFinderEvent.h"  // ZFinderEvent, cutlevel_vector
#include "ZFinderPlotter.h"  // ZFinderPlotter
#include "ZDefinition.h"  // ZDefinition


namespace zf {
    class ZDefinitionPlotter{
        public:
            // Constructor
            ZDefinitionPlotter(
                    const ZDefinition& zdef,
                    TFileDirectory& tdir,
                    const bool USE_MC = false
                    );

            // Add events
            void Fill(
                    const ZFinderEvent& zf_event,
                    const int first_electron = 0,
                    const int second_electron = 1
                    );

        protected:
            // Name
            std::string zdef_name;

            // Use the MC or reco data
            const bool USE_MC_;

            // A Vector of our ZFinderPlotter
            std::map<std::string, ZFinderPlotter> zf_plotters;
    };
}  // namespace zf
#endif  // ZFINDER_ZDEFINITIONPLOTTER_H_
