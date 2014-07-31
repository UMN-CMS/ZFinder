#include "ZFinder/Event/interface/ZDefinitionPlotter.h"

// Standard Library
#include <utility>      // std::make_pair
#include <sstream>  // std::ostringstream

// ZFinder Code
#include "ZFinder/Event/interface/CutLevel.h"  // cutlevel_vector


namespace zf {
    // Constructor
    ZDefinitionPlotter::ZDefinitionPlotter(const ZDefinition& zdef, TFileDirectory& tdir, const bool USE_MC) : USE_MC_(USE_MC) {
        // Get the name of the cut we want
        zdef_name = zdef.NAME;

        // Add the "0 All Events" set of plots
        // Make our TFileDirectory for the plotter
        TFileDirectory t_subdir_0 = tdir.mkdir("0 All Events", "0 All Events");
        all_events_plot_ = new ZFinderPlotter(t_subdir_0, USE_MC_);
        all_events_tree_ = new ZFinderTree(t_subdir_0, USE_MC_);

        // Fill zf_ploters
        // Cut names in the output start with this number and count up
        int counter = 1;
        for (auto& i_cutlevel : zdef.clv) {
            const std::string CUT_NAME = i_cutlevel.first;

            // Make a name for our cuts
            std::ostringstream oss;
            oss << counter;
            std::string level_name = oss.str() + " " + CUT_NAME;
            ++counter;

            // Make our TFileDirectory for the plotter and tree
            TFileDirectory t_subdir = tdir.mkdir(level_name.c_str(), level_name.c_str());

            // Set up the plotter
            ZFinderPlotter zf_plotter(t_subdir, USE_MC_);
            std::pair<std::string, ZFinderPlotter> plot_pair(CUT_NAME, zf_plotter);
            zf_plotters.insert(plot_pair);
            // Set up the tree
            ZFinderTree* zf_tree = new ZFinderTree(t_subdir, USE_MC_);
            std::pair<std::string, ZFinderTree*> tree_pair(CUT_NAME, zf_tree);
            zf_trees.insert(tree_pair);
        }
    }

    ZDefinitionPlotter::~ZDefinitionPlotter(){
        // Clean up our pointer
        delete all_events_plot_;
        delete all_events_tree_;
        // Clean up the trees
        for (auto& i_map : zf_trees) {
            delete i_map.second;
        }
    }

    void ZDefinitionPlotter::Fill(const ZFinderEvent& zf_event, const int electron_0, const int electron_1) {
        /*
         * We loop over the cutlevel_vector specified by the name given to use
         * by the zdef in the constructor. We then plot the event until it
         * fails a cut, then we stop.
         */
        // All events plot, which is always filled
        const double GEN_WEIGHT = zf_event.event_weight;
        all_events_plot_->Fill(zf_event, electron_0, electron_1, GEN_WEIGHT);
        all_events_tree_->Fill(zf_event, electron_0, electron_1, GEN_WEIGHT, GEN_WEIGHT);

        // Cutlevel_vector loop
        const cutlevel_vector* clv = zf_event.GetZDef(zdef_name);
        if (clv != NULL) {
            bool cont = true;
            // Loop over cuts until one fails
            for (auto& i_cutlevel : *clv) {
                const std::string CUT_NAME = i_cutlevel.first;
                cont = i_cutlevel.second.pass && cont;
                // Check if cut fails, if it does break, otherwise fill the
                // histogram associated with the cut
                if (cont) {
                    double weight = 1.;
                    if (i_cutlevel.second.t0p1_pass) {
                        weight = i_cutlevel.second.t0p1_eff;
                    } else if (i_cutlevel.second.t1p0_pass) {
                        weight = i_cutlevel.second.t1p0_eff;
                    }
                    // Fill the plot
                    auto i_map_plotter = zf_plotters.find(CUT_NAME);
                    if (i_map_plotter != zf_plotters.end()) {
                        i_map_plotter->second.Fill(zf_event, electron_0, electron_1, weight);
                    }
                    // Fill the tree
                    auto i_map_tree = zf_trees.find(CUT_NAME);
                    if (i_map_tree != zf_trees.end()) {
                        i_map_tree->second->Fill(zf_event, electron_0, electron_1, weight, GEN_WEIGHT);
                    }
                } else {  // We stop at the first failed cut
                    break;
                }
            }
        }
    }

}  // namespace zf
