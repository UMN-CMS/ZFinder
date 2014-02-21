#include "ZFinder/Event/interface/ZDefinition.h"

// Standard Libraries
#include <sstream>  // std::ostringstream


namespace zf {
    ZDefinition::ZDefinition(
            const std::string NAME,
            const std::vector<std::string>& CUTS0,
            const std::vector<std::string>& CUTS1,
            const double MZ_MIN, const double MZ_MAX
            )
        : NAME(NAME), MZ_MIN_(MZ_MIN), MZ_MAX_(MZ_MAX) {
            /*
             * Save the cut values, and check that they are sane.
             */
            if (CUTS0.size() != CUTS1.size()) {
                throw "In ZDefinition, CUTS0 and CUTS1 have different length!";
            }

            if (MZ_MIN_ > MZ_MAX_) {
                throw "In ZDefinition, MZ_MIN > MZ_MAX!";
            }
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
                // Now we got level by level and save the cut status
            }
        }
    }

    void ZDefinition::InitCutlevelVector(const size_t SIZE) {
        // cutlevel_vector
        for (size_t i = 0; i < SIZE; ++i) {
            const std::string CUTLEVEL_NAME = cutinfo_[0].at(i).cut + " AND " + cutinfo_[1].at(i).cut;
            std::pair<std::string, bool> cut_pair(CUTLEVEL_NAME, false);
            clv.push_back(cut_pair);
        }
        // Finally we add the level for MZ cuts
        std::ostringstream ss0;
        ss0 << MZ_MIN_;
        std::ostringstream ss1;
        ss1 << MZ_MAX_;
        const std::string CUTLEVEL_NAME = ss0.str() + " < M_{ee} < " + ss1.str();
        std::pair<std::string, bool> cut_pair(CUTLEVEL_NAME, false);
        clv.push_back(cut_pair);
    }

    void ZDefinition::ResetCutlevelVector() {
        cutlevel_vector::iterator i_cutlevel;
        for (i_cutlevel = clv.begin(); i_cutlevel != clv.end(); ++i_cutlevel) {
            i_cutlevel->second = false;
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

        if (zf_event->reco_z.m > MZ_MAX_ || zf_event->reco_z.m < MZ_MIN_) {
            pass_mz_cut_ = false;
        } else {
            pass_mz_cut_ = true;
        }

        for (int i_cutset = 0; i_cutset < 2; ++i_cutset) {
            std::vector<CutInfo>* cutinfo_vec = &cutinfo_[i_cutset];
            for (int i_elec = 0; i_elec < 2; ++i_elec) {
                for (unsigned int i_cutinfo = 0; i_cutinfo < cutinfo_vec->size(); ++i_cutinfo) {
                    CutInfo* cutinfo = &(cutinfo_vec->at(i_cutinfo));
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
        // Read internal variables
        const bool INVERT = CUTINFO.invert;
        const std::string CUT = CUTINFO.cut;
        const ComparisonType COMP_TYPE = CUTINFO.comp_type;
        const ComparisonVariable COMP_VAR = CUTINFO.comp_var;
        const double COMP_VAL = CUTINFO.comp_val;

        // Check if the cut is on a generator quantity
        bool use_truth;
        if (   COMP_VAR == CV_GPT
                || COMP_VAR == CV_GETA
                || COMP_VAR == CV_GPHI
                || COMP_VAR == CV_GCHARGE
           ) {
            use_truth = true;
        } else {
            use_truth = false;
        }

        // If we're not MC, always fail Gen cuts
        if (use_truth && zf_event->is_real_data) {
            std::cout << "MC CUT ON DATA" << std::endl;
            return false;
        }

        // Get the electron we want
        ZFinderElectron* zf_elec = NULL;
        if (I_ELEC == 0) {
            if (use_truth && zf_event->e0 != NULL && zf_event->e1 != NULL) {
                zf_elec = zf_event->e0_truth;
            } else {
                zf_elec = zf_event->e0;
            }
        } else {
            if (use_truth && zf_event->e0 != NULL && zf_event->e1 != NULL) {
                zf_elec = zf_event->e1_truth;
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
                e_val = zf_elec->pt;
                break;
            case CV_ETA:
            case CV_GETA:
                e_val = zf_elec->eta;
                break;
            case CV_PHI:
            case CV_GPHI:
                e_val = zf_elec->phi;
                break;
            case CV_CHARGE:
            case CV_GCHARGE:
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
        else if (cut->compare(0, 3, "eta") == 0 ) {
            return CV_ETA;
        }
        else if (cut->compare(0, 4, "geta") == 0 ) {
            return CV_GETA;
        }
        else if (cut->compare(0, 3, "phi") == 0 ) {
            return CV_PHI;
        }
        else if (cut->compare(0, 4, "gphi") == 0 ) {
            return CV_GPHI;
        }
        else if (cut->compare(0, 6, "charge") == 0 ) {
            return CV_CHARGE;
        }
        else if (cut->compare(0, 7, "gcharge") == 0 ) {
            return CV_GCHARGE;
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
        std::vector<bool>* set0 = NULL;
        std::vector<bool>* set1 = NULL;

        // First we find the combination of cut path and electron that gets us
        // the furthest
        const size_t SIZE = pass_[0][0].size();
        for (size_t i = 0; i < SIZE; ++i) {
            bool normal_pass = pass_[0][0].at(i) && pass_[1][1].at(i);
            bool other_pass = pass_[1][0].at(i) && pass_[0][1].at(i);
            if (normal_pass && !other_pass) {
                set0 = &pass_[0][0];
                set1 = &pass_[1][1];
                break;
            } else if (!normal_pass && other_pass) {
                set0 = &pass_[0][1];
                set1 = &pass_[1][0];
                break;
            } else {  // Both pass, so use normal and keep trying
                set0 = &pass_[0][0];
                set1 = &pass_[1][1];
            }
        }
        // Now set the cut levels that pass
        for (size_t i = 0; i < SIZE; ++i) {
            std::pair<std::string, bool>* cut_pair = &clv.at(i);
            cut_pair->second = set0->at(i) && set1->at(i);
        }
        // Finally, we add the Mass window cut, which is the very last one (and
        // not included in the above loop)
        clv.back().second = pass_mz_cut_;
    }
}  // namespace zf
