#include "ZFinder/Event/interface/ZDefinitionWorkspace.h"

// Standard Library
#include <utility>      // std::make_pair
#include <sstream>  // std::ostringstream

// ZFinder Code
#include "ZFinder/Event/interface/CutLevel.h"  // CutLevel, cutlevel_vector
#include "ZFinder/Event/interface/ZFinderElectron.h"  // ZFinderElectron


namespace zf {
    // Constructor
    ZDefinitionWorkspace::ZDefinitionWorkspace(
            const ZDefinition& zdef,
            TFileDirectory& tdir,
            const bool USE_TRUTH,
            const bool PROBE_FIRST
            )
        : PROBE_FIRST_(PROBE_FIRST),
          USE_TRUTH_(USE_TRUTH)
    {
        // Get the name of the cut we want
        zdef_name_ = zdef.NAME;

        // Set up the Roo variables
        InitVariables();

        // Set up the directory
        write_dir_ = tdir.getBareDirectory();
    }

    void ZDefinitionWorkspace::InitVariables() {
        /*
         * Set up all the Variables
         */
        // Set up the variables
        z_mass_ = new RooRealVar("z_mass", "m_{ee}", -1, 1000, "GeV");
        z_eta_ = new RooRealVar("z_eta", "Z_{#eta}", -6, 6);
        z_y_ = new RooRealVar("z_y", "Z_{Y}", -6, 6);
        other_y_ = new RooRealVar("other_y", "Other Z_{Y}", -6, 6);
        z_pt_ = new RooRealVar("z_pt", "Z_{p_{T}}", 0, 1000, "GeV");
        phistar_ = new RooRealVar("phistar", "#phi*", -0.1, 10);
        phistar_born_ = new RooRealVar("born_phistar", "#phi*", -0.1, 10);
        phistar_naked_ = new RooRealVar("naked_phistar", "#phi*", -0.1, 10);
        other_phistar_ = new RooRealVar("other_phistar", "Other #phi*", -0.1, 10);
        // Electrons
        e0_pt_ = new RooRealVar("e0_pt", "p_{T}^{e_{0}}", -1, 1000, "GeV");
        e0_eta_ = new RooRealVar("e0_eta", "#eta_{e_{0}}", -6, 6);
        e0_phi_ = new RooRealVar("e0_phi", "#phi_{e_{0}}", -3.142, 3.142);
        e0_charge_ = new RooCategory("e0_charge", "q_{e_{0}}");
        e0_charge_->defineType("Unmeasured", 0);
        e0_charge_->defineType("Positive", 1);
        e0_charge_->defineType("Negative", -1);
        e1_pt_ = new RooRealVar("e1_pt", "p_{T}^{e_{1}}", -1, 1000, "GeV");
        e1_eta_ = new RooRealVar("e1_eta", "#eta_{e_{1}}", -6, 6);
        e1_phi_ = new RooRealVar("e1_phi", "#phi_{e_{1}}", -3.142, 3.142);
        e1_charge_ = new RooCategory("e1_charge", "q_{e_{1}}");
        e1_charge_->defineType("Unmeasured", 0);
        e1_charge_->defineType("Positive", 1);
        e1_charge_->defineType("Negative", -1);
        // Event
        n_vert_ = new RooRealVar("n_vert", "Number of Vertices", -1, 100);
        // We use an unsigned int in ZFinderEvent, so event number must be
        // within these bounds
        event_num_ = new RooRealVar("event_num", "Event Number", 0, 4294967295);
        data_type_ = new RooCategory("data_type", "Is the event from Data, or MC?");
        data_type_->defineType("Truth MC");
        data_type_->defineType("Reco MC");
        data_type_->defineType("Data");
        numerator_ = new RooCategory("numerator", "Is the event in the efficiency numerator?");
        numerator_->defineType("True", 1);
        numerator_->defineType("False", 0);
        // Note that because we set t0p1_pass with an && of the previous value,
        // once one level of cuts fails, all others will fail. This means that
        // while it is possible to have a degenerate numerator events without
        // having a degenerate denominator
        degenerate_ = new RooCategory("degenerate", "Could either event be the tag, and the other the probe?");
        degenerate_->defineType("False", 0);
        degenerate_->defineType("Degenerate Denominator", 1);
        degenerate_->defineType("Degenerate Denominator and numerator", 2);

        // Did the event pass ALL cut criteria?
        pass_all_ = new RooCategory("pass_all", "Did the event pass all cuts applied by the ZDefinition?");
        pass_all_->defineType("True", 1);
        pass_all_->defineType("False", 0);

        // Add the weight variable
        weight_ = new RooRealVar("weight", "Event weight", 0, 100);

        // Argsets
        argset_ = new RooArgSet(*z_mass_, *phistar_, *z_pt_, *z_eta_, *z_y_);
        argset_->add(*other_y_);
        argset_->add(*phistar_born_);
        argset_->add(*phistar_naked_);
        argset_->add(*other_phistar_);
        argset_->add(*e0_pt_);
        argset_->add(*e0_eta_);
        argset_->add(*e0_phi_);
        argset_->add(*e0_charge_);
        argset_->add(*e1_pt_);
        argset_->add(*e1_eta_);
        argset_->add(*e1_phi_);
        argset_->add(*e1_charge_);
        argset_->add(*n_vert_);
        argset_->add(*data_type_);
        argset_->add(*numerator_);
        argset_->add(*degenerate_);
        argset_->add(*weight_);
        argset_->add(*event_num_);
        argset_->add(*pass_all_);

        // Dataset
        roodataset_ = new RooDataSet(
                "roo_dataset",
                "All events selected by the ZDefinition",
                *argset_,
                RooFit::WeightVar("weight")
                );
    }

    void ZDefinitionWorkspace::Fill(const ZFinderEvent& zf_event) {
        /*
        */
        const cutlevel_vector* clv = zf_event.GetZDef(zdef_name_);
        // Reject if the cut wasn't found
        if (clv == NULL) {
            return;
        }
        // If our vector doesn't have enough cuts, we skip it
        if (clv->size() <= 2) {
            return;
        }
        // We get the end of the vector, the mass cut, as well as the next two.
        // In general we ignore the mass cut (and let it be set by hand) EXCEPT
        // when filling pass_all_
        const CutLevel mass_cutlevel = clv->rbegin()[0].second;  // Mass Cut element
        const CutLevel last_cutlevel = clv->rbegin()[1].second;  // Last element
        const CutLevel penult_cutlevel = clv->rbegin()[2].second;  // Penultimate element

        // Reject events that do not belong in the sample
        if (!penult_cutlevel.pass) {
            return;
        }

        // MC or Reco
        if (zf_event.is_real_data) {
            argset_->setCatLabel("data_type", "Data");
        } else if (USE_TRUTH_) {
            argset_->setCatLabel("data_type", "Truth MC");
        } else {
            argset_->setCatLabel("data_type", "Reco MC");
        }

        // Check if we have a degenerate tag
        // Check denominator level first
        if (penult_cutlevel.t0p1_pass && penult_cutlevel.t1p0_pass) {
            // Then check numerator level
            if (last_cutlevel.t0p1_pass && last_cutlevel.t1p0_pass) {
                argset_->setCatLabel("degenerate", "Degenerate Denominator and numerator");
            } else {
                argset_->setCatLabel("degenerate", "Degenerate Denominator");
            }
        } else {
            argset_->setCatLabel("degenerate", "False");
        }

        // Now we set the tag If it is degenerate in the numerator, we pick
        // a random number, otherwise we pick the only possible tag. If the
        // event is a denominator only event, we select the in the same manner.
        int tag = -1;
        if (last_cutlevel.t0p1_pass && last_cutlevel.t1p0_pass) {
            tag = zf_event.id.event_num % 2;  // Assign a reproducible random tag
        } else if (last_cutlevel.t0p1_pass && !last_cutlevel.t1p0_pass) {
            tag = 0;
        } else if (!last_cutlevel.t0p1_pass && last_cutlevel.t1p0_pass) {
            tag = 1;
        } else if (penult_cutlevel.t0p1_pass && penult_cutlevel.t1p0_pass) {
            tag = zf_event.id.event_num % 2;  // Assign a reproducible random tag
        } else if (penult_cutlevel.t0p1_pass && !penult_cutlevel.t1p0_pass) {
            tag = 0;
        } else if (!penult_cutlevel.t0p1_pass && penult_cutlevel.t1p0_pass) {
            tag = 1;
        } else {
            // Events that get this far should not be in our dataset! They've
            // failed all requirements.
            std::cout << "FAILED TAG STATE" << std::endl;
            return;
        }

        // Check if our event is in the numerator of the efficiency, or just
        // the denominator
        if (!last_cutlevel.pass && penult_cutlevel.pass) {
            argset_->setCatLabel("numerator", "False");
        } else {
            argset_->setCatLabel("numerator", "True");
        }

        // Check if the entire cut requirements were met
        if (mass_cutlevel.pass){
            argset_->setCatLabel("pass_all", "True");
        }
        else {
            argset_->setCatLabel("pass_all", "False");
        }

        // Assign the correct electrons
        ZFinderElectron* e_tag = NULL;
        ZFinderElectron* e_probe = NULL;
        const ZFinderEvent::ZData* z_data = NULL;
        int verts = -1;
        if (zf_event.is_real_data || !USE_TRUTH_) {
            z_data = &zf_event.reco_z;
            verts = zf_event.reco_vert.num;
            if (tag == 0) {
                e_tag = zf_event.e0;
                e_probe = zf_event.e1;
            } else {
                e_tag = zf_event.e1;
                e_probe = zf_event.e0;
            }
        } else {
            z_data = &zf_event.truth_z;
            verts = zf_event.truth_vert.num;
            if (tag == 0) {
                e_tag = zf_event.e0_truth;
                e_probe = zf_event.e1_truth;
            } else {
                e_tag = zf_event.e1_truth;
                e_probe = zf_event.e0_truth;
            }
        }

        // Assign the variables
        if (z_data != NULL) {
            argset_->setRealValue("z_mass", z_data->m);
            argset_->setRealValue("z_eta", z_data->eta);
            argset_->setRealValue("z_y", z_data->y);
            argset_->setRealValue("z_pt", z_data->pt);
            argset_->setRealValue("phistar", z_data->phistar);
            argset_->setRealValue("born_phistar", z_data->bornPhistar);
            argset_->setRealValue("naked_phistar", z_data->nakedPhistar);
            if (!zf_event.is_real_data) { // Is MC
                argset_->setRealValue("other_y", z_data->other_y);
                argset_->setRealValue("other_phistar", z_data->other_phistar);
            }
            else {
                argset_->setRealValue("other_y", -6);
                argset_->setRealValue("other_phistar", -0.1);
            }
        }
        else {
            argset_->setRealValue("z_mass", -1);
            argset_->setRealValue("z_eta", -6);
            argset_->setRealValue("z_y", -6);
            argset_->setRealValue("z_pt", 0);
            argset_->setRealValue("phistar", -0.1);
            argset_->setRealValue("other_y", -6);
            argset_->setRealValue("other_phistar", -0.1);
        }
        if (e_tag != NULL) {
            argset_->setRealValue("e0_pt", e_tag->pt);
            argset_->setRealValue("e0_eta", e_tag->eta);
            argset_->setRealValue("e0_phi", e_tag->phi);
            argset_->setCatIndex("e0_charge", e_tag->charge);
        }
        else {
            argset_->setRealValue("e0_pt", -1);
            argset_->setRealValue("e0_eta", -6);
            argset_->setRealValue("e0_phi", -3.142);
            argset_->setCatIndex("e0_charge", 0);
        }
        if (e_probe != NULL) {
            argset_->setRealValue("e1_pt", e_probe->pt);
            argset_->setRealValue("e1_eta", e_probe->eta);
            argset_->setRealValue("e1_phi", e_probe->phi);
            argset_->setCatIndex("e1_charge", e_probe->charge);
        }
        else {
            argset_->setRealValue("e1_pt", -1);
            argset_->setRealValue("e1_eta", -6);
            argset_->setRealValue("e1_phi", -3.142);
            argset_->setCatIndex("e1_charge", 0);
        }
        argset_->setRealValue("n_vert", verts);
        argset_->setRealValue("event_num", zf_event.id.event_num);

        // We set the weight to be equal to the weight of the last cut level,
        // unless the event is from MC and we have are plotting truth values,
        // in which case we set it to the event_weight, which is just the
        // weight used for pileup reweighting.
        double weight = 1.;
        if (USE_TRUTH_ && zf_event.is_real_data) {
            weight = zf_event.event_weight;
        }
        else {
            if (tag == 0) {
                weight = last_cutlevel.t0p1_eff;
            } else  {
                weight = last_cutlevel.t1p0_eff;
            }
        }

        // Save the argset
        const double WEIGHT_ERROR = 0.;
        roodataset_->add(*argset_, weight, WEIGHT_ERROR);
    }

    void ZDefinitionWorkspace::Write() {

        write_dir_->cd();
        // Make the workspace and save it
        RooWorkspace *w = new RooWorkspace("workspace", "RooWorkspace");
        w->import(*roodataset_);
        w->Write();
        // Clean up, we're done!
        delete w;
    }

    ZDefinitionWorkspace::~ZDefinitionWorkspace() {
        // Clean up the heap
        delete z_mass_;
        delete z_eta_;
        delete z_y_;
        delete other_y_;
        delete z_pt_;
        delete phistar_;
        delete phistar_born_;
        delete phistar_naked_;
        delete other_phistar_;
        delete e0_pt_;
        delete e0_eta_;
        delete e0_phi_;
        delete e1_pt_;
        delete e1_eta_;
        delete e1_phi_;
        delete e0_charge_;
        delete e1_charge_;
        delete n_vert_;
        delete weight_;
        delete data_type_;
        delete numerator_;
        delete event_num_;
        delete argset_;
        delete degenerate_;
        delete roodataset_;
        delete pass_all_;
    }
}  // namespace zf
