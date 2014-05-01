#include "ZFinder/Event/interface/ZDefinitionWorkspace.h"

// Standard Library
#include <utility>      // std::make_pair
#include <sstream>  // std::ostringstream

// ZFinder Code
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
        z_mass = new RooRealVar("z_mass", "m_{ee}", -1, 1000, "GeV");
        z_eta = new RooRealVar("z_eta", "Z_{#eta}", -6, 6);
        z_y = new RooRealVar("z_y", "Z_{Y}", -6, 6);
        z_pt = new RooRealVar("z_pt", "Z_{p_{T}}", 0, 1000, "GeV");
        phistar = new RooRealVar("phistar", "#phi*", -0.1, 2);
        // Electrons
        e0_pt = new RooRealVar("e0_pt", "p_{T}^{e_{0}}", 0, 1000, "GeV");
        e0_eta = new RooRealVar("e0_eta", "#eta_{e_{0}}", -6, 6);
        e0_charge = new RooCategory("e0_charge", "q_{e_{0}}");
        e0_charge->defineType("Unmeasured", 0);
        e0_charge->defineType("Positive", 1);
        e0_charge->defineType("Negative", -1);
        e1_pt = new RooRealVar("e1_pt", "p_{T}^{e_{1}}", 0, 1000, "GeV");
        e1_eta = new RooRealVar("e1_eta", "#eta_{e_{1}}", -6, 6);
        e1_charge = new RooCategory("e1_charge", "q_{e_{1}}");
        e1_charge->defineType("Unmeasured", 0);
        e1_charge->defineType("Positive", 1);
        e1_charge->defineType("Negative", -1);
        // Event
        n_vert = new RooRealVar("n_vert", "Number of Vertices", 0, 100);
        weight = new RooRealVar("weight", "Event weight", 0, 1);
        data_type = new RooCategory("data_type", "Is the event from Data, or MC?");
        data_type->defineType("Truth MC");
        data_type->defineType("Reco MC");
        data_type->defineType("Data");
        numerator = new RooCategory("numerator", "Is the event in the efficiency numerator?");
        numerator->defineType("True", 1);
        numerator->defineType("False", 0);
        // Note that because we set t0p1_pass with an && of the previous value,
        // once one level of cuts fails, all others will fail. This means that
        // while it is possible to have a degenerate numerator events without
        // having a degenerate denominator
        degenerate = new RooCategory("degenerate", "Could either event be the tag, and the other the probe?");
        degenerate->defineType("False", 0);
        degenerate->defineType("Degenerate Denominator", 1);
        degenerate->defineType("Degenerate Denominator and Numerator", 2);

        // Argsets
        argset = new RooArgSet(*z_mass, *phistar, *z_pt, *z_eta, *z_y);
        argset->add(*e0_pt);
        argset->add(*e0_eta);
        argset->add(*e0_charge);
        argset->add(*e1_pt);
        argset->add(*e1_eta);
        argset->add(*e1_charge);
        argset->add(*weight);
        argset->add(*n_vert);
        argset->add(*data_type);
        argset->add(*numerator);
        argset->add(*degenerate);

        // Dataset
        roodataset = new RooDataSet(
                "roo_dataset",
                "All events selected by the ZDefinition",
                *argset,
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
        // We skip the very last cut because it is the mass cut, and we'll
        // apply that later. Therefore, "last" starts at 1 from the end.
        const CutLevel last_cutlevel = clv->rbegin()[1].second;  // Last element
        const CutLevel penult_cutlevel = clv->rbegin()[2].second;  // Penultimate element

        // Reject events that do not belong in the sample
        if (!penult_cutlevel.pass) {
            return;
        }

        // MC or Reco
        if (zf_event.is_real_data) {
            argset->setCatLabel("data_type", "Data");
        } else if (USE_TRUTH_) {
            argset->setCatLabel("data_type", "Truth MC");
        } else {
            argset->setCatLabel("data_type", "Reco MC");
        }

        // Check if we have a degenerate tag
        // Check denominator level first
        if (penult_cutlevel.t0p1_pass && penult_cutlevel.t1p0_pass) {
            // Then check numerator level
            if (last_cutlevel.t0p1_pass && last_cutlevel.t1p0_pass) {
                argset->setCatLabel("degenerate", "Degenerate Denominator and Numerator");
            } else {
                argset->setCatLabel("degenerate", "Degenerate Denominator");
            }
        } else {
            argset->setCatLabel("degenerate", "False");
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
            argset->setCatLabel("numerator", "False");
        } else {
            argset->setCatLabel("numerator", "True");
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
        // End if we have bad data
        if (     e_tag == NULL
                || e_probe == NULL
                || z_data == NULL
                || verts >= -1
           ) {
            return;
        }

        // Assign the variables
        argset->setRealValue("z_mass", z_data->m);
        argset->setRealValue("z_eta", z_data->eta);
        argset->setRealValue("z_y", z_data->y);
        argset->setRealValue("z_pt", z_data->pt);
        argset->setRealValue("phistar", z_data->phistar);
        argset->setRealValue("e0_pt", e_tag->pt);
        argset->setRealValue("e0_eta", e_tag->eta);
        argset->setCatIndex("e0_charge", e_tag->charge);
        argset->setRealValue("e1_pt", e_probe->pt);
        argset->setRealValue("e1_eta", e_probe->eta);
        argset->setCatIndex("e1_charge", e_probe->charge);
        argset->setRealValue("n_vert", verts);

        // We set the weight to be equal to the weight of the last cut level.
        double weight = 1.;
        if (tag == 0) {
            weight = last_cutlevel.t0p1_eff;
        } else  {
            weight = last_cutlevel.t1p0_eff;
        }
        argset->setRealValue("weight", weight);

        // Save the argset
        roodataset->add(*argset);
    }

    void ZDefinitionWorkspace::Write() {

        write_dir_->cd();
        // Make the workspace and save it
        RooWorkspace *w = new RooWorkspace("workspace", "RooWorkspace");
        w->import(*roodataset);
        w->Write();
        // Clean up, we're done!
        delete w;
    }

    ZDefinitionWorkspace::~ZDefinitionWorkspace() {
        // Clean up the heap
        delete z_mass;
        delete z_eta;
        delete z_y;
        delete z_pt;
        delete phistar;
        delete e0_pt;
        delete e0_eta;
        delete e1_pt;
        delete e1_eta;
        delete e0_charge;
        delete e1_charge;
        delete n_vert;
        delete weight;
        delete data_type;
        delete numerator;
        delete argset;
        delete roodataset;
    }
}  // namespace zf
