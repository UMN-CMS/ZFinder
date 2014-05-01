#include "ZFinder/Event/interface/ZDefinition.h"

// Standard Libraries
#include <sstream>  // std::ostringstream


namespace zf {

    ZDefinition::ZDefinition(
            const std::string NAME,
            const std::vector<std::string>& CUTS0,
            const std::vector<std::string>& CUTS1,
            const double MZ_MIN,
            const double MZ_MAX,
            const bool USE_MC_MASS
            ) :
        NAME(NAME),
        MZ_MIN_(MZ_MIN),
        MZ_MAX_(MZ_MAX),
        USE_MC_MASS_(USE_MC_MASS)
    {
        /*
         * Save the cut values, and check that they are sane.
         */
        if (CUTS0.size() != CUTS1.size()) {
            throw "In ZDefinition, CUTS0 and CUTS1 have different length!";
        }

        if (MZ_MIN_ > MZ_MAX_) {
            throw "In ZDefinition, MZ_MIN > MZ_MAX!";
        }
        // Copy the input vectors and store them
        std::vector<std::string> cuts[2];
        cuts[0] = std::vector<std::string>(CUTS0);
        cuts[1] = std::vector<std::string>(CUTS1);

        // Fill in our cutinfo_ vector
        InitVariables(cuts[0].size());

        // Loop over all cut stages and file an object with relevant info.
        for (int i_cutset = 0; i_cutset < 2; ++i_cutset) {
            const std::vector<std::string>* CUT_VEC = &cuts[i_cutset];
            for (unsigned int i_cut = 0; i_cut < CUT_VEC->size(); ++i_cut) {
                CutInfo* cutinfo = &cutinfo_[i_cutset].at(i_cut);
                cutinfo->cut = CUT_VEC->at(i_cut);

                // We use a ! flag in the [0] slot to invert the cut
                if (cutinfo->cut[0] == '!') {
                    cutinfo->invert = true;
                    cutinfo->cut.erase(0, 1);
                } else {
                    cutinfo->invert = false;
                }

                // If there is an =, >, < in the string we use it to cut on a value
                // from the ZElectron, otherwise we assume it is a cut and pass it
                // through. We also pull out the variable being compared.
                cutinfo->comp_type = GetComparisonType(&cutinfo->cut);
                if (cutinfo->comp_type != CT_NONE) {  // Is a comparison
                    cutinfo->comp_var = GetComparisonVariable(&cutinfo->cut);
                    cutinfo->comp_val = GetComparisonValue(&cutinfo->cut);
                } else {
                    cutinfo->comp_var = CV_NONE;
                    cutinfo->comp_val = -1;
                }
            }
        }

        // Initialize the cutlevel_vector
        InitCutlevelVector(cuts[0].size());
    }

    void ZDefinition::InitVariables(const size_t SIZE) {
        /*
         * We fill in cutinfo_ with default values, as well as pass_
         */
        for (int j = 0; j < 2; ++j) {
            for (unsigned int i = 0; i < SIZE; ++i) {
                // Initialize cutinfo_
                CutInfo ci;
                ci.cut = "";
                ci.invert = false;
                ci.comp_type = CT_NONE;
                ci.comp_var = CV_NONE;
                ci.comp_val = -1;
                cutinfo_[j].push_back(ci);
                // Initialize pass_
                pass_[j][0].push_back(false);
                pass_[j][1].push_back(false);
                // Initialize eff_
                eff_[j][0].push_back(1.);
                eff_[j][1].push_back(1.);
            }
        }
    }

    void ZDefinition::InitCutlevelVector(const size_t SIZE) {
        // cutlevel_vector
        for (size_t i = 0; i < SIZE; ++i) {
            const std::string CUTLEVEL_NAME = cutinfo_[0].at(i).cut + " AND " + cutinfo_[1].at(i).cut;
            CutLevel cl;
            cutlevel_pair cut_pair(CUTLEVEL_NAME, cl);
            clv.push_back(cut_pair);
        }
        // Finally we add the level for MZ cuts
        std::ostringstream ss0;
        ss0 << MZ_MIN_;
        std::ostringstream ss1;
        ss1 << MZ_MAX_;
        std::string cutlevel_name;
        if (USE_MC_MASS_) {
            cutlevel_name = ss0.str() + " < GEN M_{ee} < " + ss1.str();
        } else {
            cutlevel_name = ss0.str() + " < M_{ee} < " + ss1.str();
        }
        CutLevel cl;
        cutlevel_pair cut_pair(cutlevel_name, cl);
        clv.push_back(cut_pair);
    }

    void ZDefinition::ResetCutlevelVector() {
        for (auto& i_cutlevel : clv) {
            i_cutlevel.second.pass = false;
            i_cutlevel.second.t0p1_pass = false;
            i_cutlevel.second.t0p1_pass = false;
            i_cutlevel.second.t0p1_eff = 1.;
            i_cutlevel.second.t1p0_eff = 1.;
        }
    }

    void ZDefinition::ApplySelection(ZFinderEvent* zf_event) {
        /*
         * Given a ZFinderEvent, apply all steps of the selection to it. We
         * loop over the each cut stage, then each set of cuts, then each
         * electron and determine which combinations pass.
         */

        // Clear our vector
        ResetCutlevelVector();

        if (USE_MC_MASS_) {
            if (zf_event->truth_z.m > MZ_MAX_ || zf_event->truth_z.m < MZ_MIN_) {
                pass_mz_cut_ = false;
            } else {
                pass_mz_cut_ = true;
            }
        } else {
            if (zf_event->reco_z.m > MZ_MAX_ || zf_event->reco_z.m < MZ_MIN_) {
                pass_mz_cut_ = false;
            } else {
                pass_mz_cut_ = true;
            }
        }

        for (int i_cutset = 0; i_cutset < 2; ++i_cutset) {
            std::vector<CutInfo>* cutinfo_vec = &cutinfo_[i_cutset];
            for (int i_elec = 0; i_elec < 2; ++i_elec) {
                for (unsigned int i_cutinfo = 0; i_cutinfo < cutinfo_vec->size(); ++i_cutinfo) {
                    CutInfo* cutinfo = &(cutinfo_vec->at(i_cutinfo));
                    // Set the efficiency
                    eff_[i_cutset][i_elec][i_cutinfo] = Efficiency(*cutinfo, i_elec, zf_event);
                    // Check the cuts
                    if (cutinfo->comp_type != CT_NONE) {
                        pass_[i_cutset][i_elec][i_cutinfo] = ComparisonCut(*cutinfo, i_elec, zf_event);
                    } else {
                        pass_[i_cutset][i_elec][i_cutinfo] = NormalCut(*cutinfo, i_elec, zf_event);
                    }
                }
            }
        }

        /*
         * We now produce a cutlevel_vector and store it in the zf_event
         */
        FillCutLevelVector();
        zf_event->AddZDef(NAME, clv);
    }

    double ZDefinition::Efficiency(const CutInfo& CUTINFO, const int I_ELEC, ZFinderEvent const * const zf_event) {
        /*
         * Returns the efficiency for a given cut type.
         */
        // Read internal variables
        const bool INVERT = CUTINFO.invert;
        const std::string CUT = CUTINFO.cut;

        // Get the value for the efficiency
        double efficiency = 1.;
        if (I_ELEC == 0) {
            efficiency = zf_event->e0->CutWeight(CUT);
        } else {
            efficiency = zf_event->e1->CutWeight(CUT);
        }
        // Check if the cut failed to get a result, if so we assume it is 1 and
        // return, because it makes no sense to invert at this point since we
        // don't know the efficiency in either case.
        if (efficiency < 0.) {
            return 1.;
        }

        // Invert means we should take 1 - eff
        if (INVERT) {
            efficiency = 1 - efficiency;
        }

        return efficiency;
    }

    bool ZDefinition::NormalCut(const CutInfo& CUTINFO, const int I_ELEC, ZFinderEvent* zf_event) {
        // Read internal variables
        const bool INVERT = CUTINFO.invert;
        const std::string CUT = CUTINFO.cut;

        // Check the cut on the event
        bool passed = false;
        if (I_ELEC == 0) {
            passed = zf_event->e0->CutPassed(CUT);
        } else {
            passed = zf_event->e1->CutPassed(CUT);
        }

        // -1 is used to indicate a missing cut, we never invert this, they are
        // just always false
        if (passed == -1) {
            return false;
        } else {
            if (INVERT) {
                return !passed;
            } else {
                return passed;
            }
        }
    }

    bool ZDefinition::ComparisonCut(const CutInfo& CUTINFO, const int I_ELEC, ZFinderEvent* zf_event) {
        // An enum to track what cut we're making
        enum CUTTYPE {
            RECO,
            TRUTH,
            TRIG
        };

        // Read internal variables
        const bool INVERT = CUTINFO.invert;
        const std::string CUT = CUTINFO.cut;
        const ComparisonType COMP_TYPE = CUTINFO.comp_type;
        const ComparisonVariable COMP_VAR = CUTINFO.comp_var;
        const double COMP_VAL = CUTINFO.comp_val;

        // Check if the cut is on a generator quantity
        int cut_type;
        if (    COMP_VAR == CV_GPT
                || COMP_VAR == CV_GETA
                || COMP_VAR == CV_GPHI
                || COMP_VAR == CV_GCHARGE
           ) {
            cut_type = TRUTH;
        } else if (
                COMP_VAR == CV_TPT
                || COMP_VAR == CV_TETA
                || COMP_VAR == CV_TPHI
                || COMP_VAR == CV_TCHARGE
                ){
            cut_type = TRIG;
        } else {
            cut_type = RECO;
        }

        // If we're not MC, always fail Gen cuts
        if (cut_type == TRUTH && zf_event->is_real_data) {
            //std::cout << "MC CUT ON DATA" << std::endl;
            return false;
        }

        // Get the electron we want
        ZFinderElectron* zf_elec = NULL;
        if (I_ELEC == 0) {
            if (cut_type == TRUTH && zf_event->e0 != NULL && zf_event->e1 != NULL) {
                zf_elec = zf_event->e0_truth;
            } else if (cut_type == TRIG && zf_event->e0 != NULL && zf_event->e1 != NULL) {
                zf_elec = zf_event->e0_trig;
            } else {
                zf_elec = zf_event->e0;
            }
        } else {
            if (cut_type == TRUTH && zf_event->e0 != NULL && zf_event->e1 != NULL) {
                zf_elec = zf_event->e1_truth;
            } else if (cut_type == TRIG && zf_event->e0 != NULL && zf_event->e1 != NULL) {
                zf_elec = zf_event->e1_trig;
            } else {
                zf_elec = zf_event->e1;
            }
        }

        // Not all the required electrons existed, so fail
        if (zf_elec == NULL) {
            return false;
        }

        // Pull the value from the electron
        double e_val = -1;
        switch(COMP_VAR) {
            case CV_PT:
            case CV_GPT:
            case CV_TPT:
                e_val = zf_elec->pt;
                break;
            case CV_ETA:
            case CV_GETA:
            case CV_TETA:
                e_val = zf_elec->eta;
                break;
            case CV_PHI:
            case CV_GPHI:
            case CV_TPHI:
                e_val = zf_elec->phi;
                break;
            case CV_CHARGE:
            case CV_GCHARGE:
            case CV_TCHARGE:
                e_val = zf_elec->charge;
                break;
                // Cases where it makes no sense to continue
            case CV_NONE:
            default:
                return false;
        }

        // Check the cut
        bool passed = false;
        switch(COMP_TYPE) {
            case CT_EQUAL:
                passed = (e_val == COMP_VAL);
                break;
            case CT_GT:
                passed = (e_val > COMP_VAL);
                break;
            case CT_LT:
                passed = (e_val < COMP_VAL);
                break;
            case CT_GTE:
                passed = (e_val >= COMP_VAL);
                break;
            case CT_LTE:
                passed = (e_val <= COMP_VAL);
                break;
                // Cases where it makes no sense to continue
            case CT_NONE:
            default:
                return false;
        }

        // Return result with invert if needed
        if (INVERT) {
            return !passed;
        } else {
            return passed;
        }
    }

    ZDefinition::ComparisonType ZDefinition::GetComparisonType(const std::string* cut) {
        /*
         * Determine what type of comparison the cut string is requesting. Can be one of:
         * <, >, =, <=, >=
         */
        using std::string;
        // Less Than
        if (cut->find("<") != string::npos) {
            if (cut->find("=") != string::npos) {
                return CT_LTE;
            } else {
                return CT_LT;
            }
        }
        // Greater Than
        else if (cut->find(">") != string::npos) {
            if (cut->find("=") != string::npos) {
                return CT_GTE;
            } else {
                return CT_GT;
            }
        }
        // Just Equal
        else if (cut->find("=") != string::npos) {
            return CT_EQUAL;
        }
        // None of the above
        return CT_NONE;
    }

    ZDefinition::ComparisonVariable ZDefinition::GetComparisonVariable(const std::string* cut) {
        /*
         * Determine what variable for comparison the cut string is requesting.
         * We just check all possibilities.
         */
        using std::string;
        if (cut->compare(0, 2, "pt") == 0) {
            return CV_PT;
        }
        else if (cut->compare(0, 3, "gpt") == 0 ) {
            return CV_GPT;
        }
        else if (cut->compare(0, 3, "tpt") == 0 ) {
            return CV_TPT;
        }
        else if (cut->compare(0, 3, "eta") == 0 ) {
            return CV_ETA;
        }
        else if (cut->compare(0, 4, "geta") == 0 ) {
            return CV_GETA;
        }
        else if (cut->compare(0, 4, "teta") == 0 ) {
            return CV_TETA;
        }
        else if (cut->compare(0, 3, "phi") == 0 ) {
            return CV_PHI;
        }
        else if (cut->compare(0, 4, "gphi") == 0 ) {
            return CV_GPHI;
        }
        else if (cut->compare(0, 4, "tphi") == 0 ) {
            return CV_TPHI;
        }
        else if (cut->compare(0, 6, "charge") == 0 ) {
            return CV_CHARGE;
        }
        else if (cut->compare(0, 7, "gcharge") == 0 ) {
            return CV_GCHARGE;
        }
        else if (cut->compare(0, 7, "tcharge") == 0 ) {
            return CV_TCHARGE;
        }
        return CV_NONE;
    }

    double ZDefinition::GetComparisonValue(const std::string* cut) {
        /*
         * Convert everything after the operator in the cut string to a double
         */
        int gt_pos = cut->find(">");
        int lt_pos = cut->find("<");
        int eq_pos = cut->find("=");
        int pos;

        // Find the largest
        if (gt_pos > lt_pos && gt_pos > eq_pos) {
            pos = gt_pos;
        } else if (lt_pos > gt_pos && lt_pos > eq_pos) {
            pos = lt_pos;
        } else {
            pos = eq_pos;
        }

        // Get the end of the string starting after the operator and convert it
        // to a double
        std::string ds = cut->substr(pos+1);
        std::istringstream iss(ds);
        double x;
        iss >> x;
        return x;
    }

    void ZDefinition::FillCutLevelVector() {
        // First we find the combination of cut path and electron that gets us
        // the furthest. Remember that pass_ is indexed as
        // pass_[probe/tag][electron].
        const size_t SIZE = pass_[0][0].size();
        bool t0p1_pass = true;
        bool t1p0_pass = true;
        double t0p1_eff = 1.;
        double t1p0_eff = 1.;
        for (size_t i = 0; i < SIZE; ++i) {
            t0p1_pass = t0p1_pass && pass_[0][0].at(i) && pass_[1][1].at(i);
            t1p0_pass = t1p0_pass && pass_[0][1].at(i) && pass_[1][0].at(i);
            t0p1_eff = t0p1_eff * eff_[0][0].at(i) * eff_[1][1].at(i);
            t1p0_eff = t1p0_eff * eff_[0][1].at(i) * eff_[1][0].at(i);
            // Now set the values in the vector
            CutLevel* cl = &clv.at(i).second;
            cl->t0p1_pass = t0p1_pass;
            cl->t1p0_pass = t1p0_pass;
            cl->pass = cl->t0p1_pass || cl->t1p0_pass;
            cl->t0p1_eff = t0p1_eff;
            cl->t1p0_eff = t1p0_eff;
        }
        // Finally, we add the Mass window cut, which is the very last one (and
        // not included in the above loop)
        clv.back().second.pass = pass_mz_cut_;
    }
}  // namespace zf
