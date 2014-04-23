#include "ZFinder/Event/interface/ZEfficiencies.h"

namespace zf {

    ZEfficiencies::ZEfficiencies() {
        // Set up the hardcoded variables
        Setup();
    }

    void ZEfficiencies::SetWeights(ZFinderEvent* zf_event) {
        /*
         * Loop over all the electrons, then loop over all the cuts, and set
         * the weights. Cuts that aren't found in table_map_ are left alone. If
         * the pt and eta value of the electron don't fall into a bin, we also
         * leave the efficiency alone.
         */
        std::vector<ZFinderElectron*> const * const ZF_ELECTRONS = zf_event->FilteredElectrons();
        for (auto& i_elec : *ZF_ELECTRONS) {
            const double PT = i_elec->pt;
            const double ETA = i_elec->eta;
            std::vector<const CutResult*> const * const ALL_CUTS = i_elec->GetAllCuts();
            for (auto& i_cut : *ALL_CUTS) {
                const std::string CUT_NAME = i_cut->name;
                const double WEIGHT = GetEfficiency(CUT_NAME, PT, ETA);
                // We only reset the weight if we got a valid value from
                // the table, otherwise it is left as whatever it was set
                // to when created.
                if (WEIGHT >= 0.) {
                    i_elec->SetCutWeight(CUT_NAME, WEIGHT);
                }
            }
        }
    }

    double ZEfficiencies::GetEfficiency(
                    const std::string CUT_NAME,
                    const double PT,
                    const double ETA
            ) const {
        /*
         * Looks through the cut table to find the efficiency. If the cut
         * doesn't match one that is in table_map_, we return -1. If the pt or
         * eta ranges are not found in the EfficiencyTable that the cut name
         * points to, then again we return -1.
         */
        // Find the cut
        std::map<std::string, EfficiencyTable*>::const_iterator table_iterator;
        table_iterator = table_map_.find(CUT_NAME);
        if (table_iterator != table_map_.end()) {
            EfficiencyTable const * const eff_table = table_iterator->second;
            return eff_table->GetEfficiency(PT, ETA);  // Returns 1 on failure
        }

        // Failure to find a matching cut
        return -1.;
    }

    void ZEfficiencies::Setup() {
        /*
         * He we set up the hard coded variables
         */
        // Tight Scale Factors from:
        // http://lovedeep.web.cern.ch/lovedeep/WORK13/TnPRun2012ReReco_2013Oct28/Tables/Tight/sFGsfIdTight.txt
        table_map_["eg_tight"] = &ecal_tight_scale_factors_;
        ecal_tight_scale_factors_.AddEfficiency(10.00, 15.00, 0.00, 0.80, 0.827, 0.021, 0.021);
        ecal_tight_scale_factors_.AddEfficiency(15.00, 20.00, 0.00, 0.80, 0.924, 0.010, 0.010);
        ecal_tight_scale_factors_.AddEfficiency(20.00, 30.00, 0.00, 0.80, 0.960, 0.003, 0.003);
        ecal_tight_scale_factors_.AddEfficiency(30.00, 40.00, 0.00, 0.80, 0.978, 0.001, 0.001);
        ecal_tight_scale_factors_.AddEfficiency(40.00, 50.00, 0.00, 0.80, 0.981, 0.001, 0.001);
        ecal_tight_scale_factors_.AddEfficiency(50.00, 200.00, 0.00, 0.80, 0.982, 0.002, 0.002);
        ecal_tight_scale_factors_.AddEfficiency(10.00, 15.00, 0.80, 1.44, 0.948, 0.023, 0.024);
        ecal_tight_scale_factors_.AddEfficiency(15.00, 20.00, 0.80, 1.44, 0.932, 0.012, 0.012);
        ecal_tight_scale_factors_.AddEfficiency(20.00, 30.00, 0.80, 1.44, 0.936, 0.004, 0.004);
        ecal_tight_scale_factors_.AddEfficiency(30.00, 40.00, 0.80, 1.44, 0.958, 0.002, 0.002);
        ecal_tight_scale_factors_.AddEfficiency(40.00, 50.00, 0.80, 1.44, 0.969, 0.001, 0.001);
        ecal_tight_scale_factors_.AddEfficiency(50.00, 200.00, 0.80, 1.44, 0.969, 0.002, 0.002);
        ecal_tight_scale_factors_.AddEfficiency(10.00, 15.00, 1.44, 1.57, 1.073, 0.107, 0.117);
        ecal_tight_scale_factors_.AddEfficiency(15.00, 20.00, 1.44, 1.57, 0.808, 0.042, 0.045);
        ecal_tight_scale_factors_.AddEfficiency(20.00, 30.00, 1.44, 1.57, 0.933, 0.017, 0.015);
        ecal_tight_scale_factors_.AddEfficiency(30.00, 40.00, 1.44, 1.57, 0.907, 0.008, 0.008);
        ecal_tight_scale_factors_.AddEfficiency(40.00, 50.00, 1.44, 1.57, 0.904, 0.004, 0.004);
        ecal_tight_scale_factors_.AddEfficiency(50.00, 200.00, 1.44, 1.57, 0.926, 0.011, 0.011);
        ecal_tight_scale_factors_.AddEfficiency(10.00, 15.00, 1.57, 2.00, 0.854, 0.047, 0.048);
        ecal_tight_scale_factors_.AddEfficiency(15.00, 20.00, 1.57, 2.00, 0.853, 0.022, 0.022);
        ecal_tight_scale_factors_.AddEfficiency(20.00, 30.00, 1.57, 2.00, 0.879, 0.007, 0.007);
        ecal_tight_scale_factors_.AddEfficiency(30.00, 40.00, 1.57, 2.00, 0.909, 0.003, 0.003);
        ecal_tight_scale_factors_.AddEfficiency(40.00, 50.00, 1.57, 2.00, 0.942, 0.002, 0.002);
        ecal_tight_scale_factors_.AddEfficiency(50.00, 200.00, 1.57, 2.00, 0.957, 0.004, 0.004);
        ecal_tight_scale_factors_.AddEfficiency(10.00, 15.00, 2.00, 2.50, 1.007, 0.046, 0.047);
        ecal_tight_scale_factors_.AddEfficiency(15.00, 20.00, 2.00, 2.50, 0.903, 0.029, 0.029);
        ecal_tight_scale_factors_.AddEfficiency(20.00, 30.00, 2.00, 2.50, 0.974, 0.004, 0.004);
        ecal_tight_scale_factors_.AddEfficiency(30.00, 40.00, 2.00, 2.50, 0.987, 0.004, 0.004);
        ecal_tight_scale_factors_.AddEfficiency(40.00, 50.00, 2.00, 2.50, 0.991, 0.003, 0.003);
        ecal_tight_scale_factors_.AddEfficiency(50.00, 200.00, 2.00, 2.50, 0.999, 0.005, 0.005);

        // Medium Scale Factors from:
        // http://lovedeep.web.cern.ch/lovedeep/WORK13/TnPRun2012ReReco_2013Oct28/effiTable.html
        table_map_["eg_medium"] = &ecal_medium_scale_factors_;
        ecal_medium_scale_factors_.AddEfficiency(10.00, 15.00, 0.00, 0.80, 0.863, 0.020, 0.008);
        ecal_medium_scale_factors_.AddEfficiency(5.00, 20.00, 0.00, 0.80, 0.956, 0.009, 0.009);
        ecal_medium_scale_factors_.AddEfficiency(20.00, 30.00, 0.00, 0.80, 0.986, 0.001, 0.002);
        ecal_medium_scale_factors_.AddEfficiency(30.00, 40.00, 0.00, 0.80, 1.002, 0.001, 0.001);
        ecal_medium_scale_factors_.AddEfficiency(40.00, 50.00, 0.00, 0.80, 1.005, 0.001, 0.001);
        ecal_medium_scale_factors_.AddEfficiency(50.00, 200.00, 0.00, 0.80, 1.004, 0.001, 0.001);
        ecal_medium_scale_factors_.AddEfficiency(10.00, 15.00, 0.80, 1.44, 0.973, 0.022, 0.022);
        ecal_medium_scale_factors_.AddEfficiency(15.00, 20.00, 0.80, 1.44, 0.962, 0.011, 0.011);
        ecal_medium_scale_factors_.AddEfficiency(20.00, 30.00, 0.80, 1.44, 0.959, 0.003, 0.003);
        ecal_medium_scale_factors_.AddEfficiency(30.00, 40.00, 0.80, 1.44, 0.980, 0.001, 0.001);
        ecal_medium_scale_factors_.AddEfficiency(40.00, 50.00, 0.80, 1.44, 0.988, 0.001, 0.001);
        ecal_medium_scale_factors_.AddEfficiency(50.00, 200.00, 0.80, 1.44, 0.988, 0.002, 0.002);
        ecal_medium_scale_factors_.AddEfficiency(10.00, 15.00, 1.44, 1.57, 1.090, 0.097, 0.107);
        ecal_medium_scale_factors_.AddEfficiency(15.00, 20.00, 1.44, 1.57, 0.867, 0.037, 0.040);
        ecal_medium_scale_factors_.AddEfficiency(20.00, 30.00, 1.44, 1.57, 0.967, 0.013, 0.007);
        ecal_medium_scale_factors_.AddEfficiency(30.00, 40.00, 1.44, 1.57, 0.950, 0.007, 0.006);
        ecal_medium_scale_factors_.AddEfficiency(40.00, 50.00, 1.44, 1.57, 0.958, 0.005, 0.005);
        ecal_medium_scale_factors_.AddEfficiency(50.00, 200.00, 1.44, 1.57, 0.966, 0.009, 0.009);
        ecal_medium_scale_factors_.AddEfficiency(10.00, 15.00, 1.57, 2.00, 0.944, 0.045, 0.042);
        ecal_medium_scale_factors_.AddEfficiency(15.00, 20.00, 1.57, 2.00, 0.911, 0.012, 0.012);
        ecal_medium_scale_factors_.AddEfficiency(20.00, 30.00, 1.57, 2.00, 0.941, 0.005, 0.005);
        ecal_medium_scale_factors_.AddEfficiency(30.00, 40.00, 1.57, 2.00, 0.967, 0.003, 0.003);
        ecal_medium_scale_factors_.AddEfficiency(40.00, 50.00, 1.57, 2.00, 0.992, 0.002, 0.002);
        ecal_medium_scale_factors_.AddEfficiency(50.00, 200.00, 1.57, 2.00, 1.000, 0.003, 0.003);
        ecal_medium_scale_factors_.AddEfficiency(10.00, 15.00, 2.00, 2.50, 1.024, 0.039, 0.040);
        ecal_medium_scale_factors_.AddEfficiency(15.00, 20.00, 2.00, 2.50, 0.934, 0.018, 0.018);
        ecal_medium_scale_factors_.AddEfficiency(20.00, 30.00, 2.00, 2.50, 1.020, 0.003, 0.003);
        ecal_medium_scale_factors_.AddEfficiency(30.00, 40.00, 2.00, 2.50, 1.021, 0.003, 0.003);
        ecal_medium_scale_factors_.AddEfficiency(40.00, 50.00, 2.00, 2.50, 1.019, 0.002, 0.002);
        ecal_medium_scale_factors_.AddEfficiency(50.00, 200.00, 2.00, 2.50, 1.022, 0.004, 0.004);

        // Loose Scale Factors from:
        // http://lovedeep.web.cern.ch/lovedeep/WORK13/TnPRun2012ReReco_2013Oct28/Tables/Loose/sFGsfIdLoose.txt
        table_map_["eg_loose"] = &ecal_loose_scale_factors_;
        ecal_loose_scale_factors_.AddEfficiency(10.00, 15.00, 0.00, 0.80, 0.865, 0.019, 0.023);
        ecal_loose_scale_factors_.AddEfficiency(15.00, 20.00, 0.00, 0.80, 0.958, 0.009, 0.009);
        ecal_loose_scale_factors_.AddEfficiency(20.00, 30.00, 0.00, 0.80, 0.988, 0.002, 0.002);
        ecal_loose_scale_factors_.AddEfficiency(30.00, 40.00, 0.00, 0.80, 1.002, 0.001, 0.001);
        ecal_loose_scale_factors_.AddEfficiency(40.00, 50.00, 0.00, 0.80, 1.005, 0.001, 0.001);
        ecal_loose_scale_factors_.AddEfficiency(50.00, 200.00, 0.00, 0.80, 1.005, 0.001, 0.001);
        ecal_loose_scale_factors_.AddEfficiency(10.00, 15.00, 0.80, 1.44, 0.967, 0.020, 0.020);
        ecal_loose_scale_factors_.AddEfficiency(15.00, 20.00, 0.80, 1.44, 0.971, 0.010, 0.010);
        ecal_loose_scale_factors_.AddEfficiency(20.00, 30.00, 0.80, 1.44, 0.965, 0.003, 0.003);
        ecal_loose_scale_factors_.AddEfficiency(30.00, 40.00, 0.80, 1.44, 0.985, 0.001, 0.001);
        ecal_loose_scale_factors_.AddEfficiency(40.00, 50.00, 0.80, 1.44, 0.989, 0.001, 0.001);
        ecal_loose_scale_factors_.AddEfficiency(50.00, 200.00, 0.80, 1.44, 0.989, 0.002, 0.002);
        ecal_loose_scale_factors_.AddEfficiency(10.00, 15.00, 1.44, 1.57, 1.064, 0.092, 0.103);
        ecal_loose_scale_factors_.AddEfficiency(15.00, 20.00, 1.44, 1.57, 0.902, 0.032, 0.036);
        ecal_loose_scale_factors_.AddEfficiency(20.00, 30.00, 1.44, 1.57, 0.990, 0.011, 0.011);
        ecal_loose_scale_factors_.AddEfficiency(30.00, 40.00, 1.44, 1.57, 0.966, 0.005, 0.005);
        ecal_loose_scale_factors_.AddEfficiency(40.00, 50.00, 1.44, 1.57, 0.971, 0.004, 0.004);
        ecal_loose_scale_factors_.AddEfficiency(50.00, 200.00, 1.44, 1.57, 0.980, 0.008, 0.008);
        ecal_loose_scale_factors_.AddEfficiency(10.00, 15.00, 1.57, 2.00, 0.939, 0.038, 0.039);
        ecal_loose_scale_factors_.AddEfficiency(15.00, 20.00, 1.57, 2.00, 0.897, 0.016, 0.016);
        ecal_loose_scale_factors_.AddEfficiency(20.00, 30.00, 1.57, 2.00, 0.953, 0.005, 0.005);
        ecal_loose_scale_factors_.AddEfficiency(30.00, 40.00, 1.57, 2.00, 0.980, 0.003, 0.003);
        ecal_loose_scale_factors_.AddEfficiency(40.00, 50.00, 1.57, 2.00, 0.999, 0.002, 0.002);
        ecal_loose_scale_factors_.AddEfficiency(50.00, 200.00, 1.57, 2.00, 1.004, 0.002, 0.003);
        ecal_loose_scale_factors_.AddEfficiency(10.00, 15.00, 2.00, 2.50, 1.050, 0.019, 0.019);
        ecal_loose_scale_factors_.AddEfficiency(15.00, 20.00, 2.00, 2.50, 0.941, 0.017, 0.017);
        ecal_loose_scale_factors_.AddEfficiency(20.00, 30.00, 2.00, 2.50, 1.017, 0.005, 0.005);
        ecal_loose_scale_factors_.AddEfficiency(30.00, 40.00, 2.00, 2.50, 1.019, 0.003, 0.003);
        ecal_loose_scale_factors_.AddEfficiency(40.00, 50.00, 2.00, 2.50, 1.019, 0.002, 0.002);
        ecal_loose_scale_factors_.AddEfficiency(50.00, 200.00, 2.00, 2.50, 1.023, 0.004, 0.004);

        // Veto Scale Factors from:
        // http://lovedeep.web.cern.ch/lovedeep/WORK13/TnPRun2012ReReco_2013Oct28/Tables/Veto/sFGsfIdVeto.txt
        table_map_["eg_veto"] = &ecal_veto_scale_factors_;
        ecal_veto_scale_factors_.AddEfficiency(10.00, 15.00, 0.00, 0.80, 0.862, 0.019, 0.019);
        ecal_veto_scale_factors_.AddEfficiency(15.00, 20.00, 0.00, 0.80, 0.953, 0.008, 0.008);
        ecal_veto_scale_factors_.AddEfficiency(20.00, 30.00, 0.00, 0.80, 0.990, 0.002, 0.002);
        ecal_veto_scale_factors_.AddEfficiency(30.00, 40.00, 0.00, 0.80, 1.005, 0.001, 0.001);
        ecal_veto_scale_factors_.AddEfficiency(40.00, 50.00, 0.00, 0.80, 1.006, 0.001, 0.001);
        ecal_veto_scale_factors_.AddEfficiency(50.00, 200.00, 0.00, 0.80, 1.007, 0.001, 0.001);
        ecal_veto_scale_factors_.AddEfficiency(10.00, 15.00, 0.80, 1.44, 0.957, 0.019, 0.020);
        ecal_veto_scale_factors_.AddEfficiency(15.00, 20.00, 0.80, 1.44, 0.975, 0.004, 0.010);
        ecal_veto_scale_factors_.AddEfficiency(20.00, 30.00, 0.80, 1.44, 0.972, 0.003, 0.003);
        ecal_veto_scale_factors_.AddEfficiency(30.00, 40.00, 0.80, 1.44, 0.990, 0.001, 0.001);
        ecal_veto_scale_factors_.AddEfficiency(40.00, 50.00, 0.80, 1.44, 0.994, 0.001, 0.001);
        ecal_veto_scale_factors_.AddEfficiency(50.00, 200.00, 0.80, 1.44, 0.993, 0.001, 0.001);
        ecal_veto_scale_factors_.AddEfficiency(10.00, 15.00, 1.44, 1.57, 1.017, 0.089, 0.121);
        ecal_veto_scale_factors_.AddEfficiency(15.00, 20.00, 1.44, 1.57, 0.933, 0.032, 0.036);
        ecal_veto_scale_factors_.AddEfficiency(20.00, 30.00, 1.44, 1.57, 0.998, 0.010, 0.010);
        ecal_veto_scale_factors_.AddEfficiency(30.00, 40.00, 1.44, 1.57, 0.978, 0.004, 0.004);
        ecal_veto_scale_factors_.AddEfficiency(40.00, 50.00, 1.44, 1.57, 0.979, 0.003, 0.003);
        ecal_veto_scale_factors_.AddEfficiency(50.00, 200.00, 1.44, 1.57, 0.997, 0.006, 0.005);
        ecal_veto_scale_factors_.AddEfficiency(10.00, 15.00, 1.57, 2.00, 0.944, 0.035, 0.035);
        ecal_veto_scale_factors_.AddEfficiency(15.00, 20.00, 1.57, 2.00, 0.934, 0.007, 0.007);
        ecal_veto_scale_factors_.AddEfficiency(20.00, 30.00, 1.57, 2.00, 0.947, 0.002, 0.004);
        ecal_veto_scale_factors_.AddEfficiency(30.00, 40.00, 1.57, 2.00, 0.974, 0.002, 0.002);
        ecal_veto_scale_factors_.AddEfficiency(40.00, 50.00, 1.57, 2.00, 0.988, 0.001, 0.001);
        ecal_veto_scale_factors_.AddEfficiency(50.00, 200.00, 1.57, 2.00, 0.989, 0.002, 0.002);
        ecal_veto_scale_factors_.AddEfficiency(10.00, 15.00, 2.00, 2.50, 1.044, 0.032, 0.015);
        ecal_veto_scale_factors_.AddEfficiency(15.00, 20.00, 2.00, 2.50, 0.958, 0.014, 0.014);
        ecal_veto_scale_factors_.AddEfficiency(20.00, 30.00, 2.00, 2.50, 0.984, 0.004, 0.004);
        ecal_veto_scale_factors_.AddEfficiency(30.00, 40.00, 2.00, 2.50, 0.990, 0.002, 0.002);
        ecal_veto_scale_factors_.AddEfficiency(40.00, 50.00, 2.00, 2.50, 0.993, 0.001, 0.001);
        ecal_veto_scale_factors_.AddEfficiency(50.00, 200.00, 2.00, 2.50, 0.990, 0.002, 0.002);
    }

}  // namespace zf;
