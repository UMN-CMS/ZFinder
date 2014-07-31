#ifndef ZFINDER_ZDEFINITIONWRITER_H_
#define ZFINDER_ZDEFINITIONWRITER_H_

// Standard Library
#include <map>  // std::map
#include <string>  // string
#include <vector>  // std::vector

// CMSSW
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// ZFinder Code
#include "ZDefinition.h"  // ZDefinition
#include "ZFinderEvent.h"  // ZFinderEvent
#include "ZFinderPlotter.h"  // ZFinderPlotter
#include "ZFinderTree.h"  // ZFinderTree


namespace zf {
    class ZDefinitionWriter{
        public:
            // Constructor
            ZDefinitionWriter(
                    const ZDefinition& zdef,
                    TFileDirectory& tdir,
                    const bool USE_MC = false
                    );

            ~ZDefinitionWriter();
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

            // A Vector of our ZFinderPlotters, and ZFinderTrees
            std::map<std::string, ZFinderPlotter> zf_plotters;
            std::map<std::string, ZFinderTree*> zf_trees;

            // Space for our 0th plot of all events
            ZFinderPlotter* all_events_plot_;
            ZFinderTree* all_events_tree_;
    };
}  // namespace zf
#endif  // ZFINDER_ZDEFINITIONWRITER_H_
