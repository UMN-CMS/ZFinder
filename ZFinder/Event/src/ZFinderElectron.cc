#include "ZFinder/Event/interface/ZFinderElectron.h"

// Standard Library
#include <iostream>  // std::cout, std::endl;

// ZFinder
#include "ZFinder/Event/interface/PDGID.h"  // PDGID enum (ELECTRON, POSITRON, etc.)


namespace zf {
    ZFinderElectron::ZFinderElectron(reco::GsfElectron input_electron) {
        /* Set type of candidate and assign */
        candidate_type_ = RECO_GSFELECTRON;
        AddCutResult("type_gsf", true, 1.);
        gsf_elec_ = input_electron;
        candidate_ = &input_electron;
        /* Extract the useful quantities from a GsfElectron */
        pt = input_electron.pt();
        eta = input_electron.eta();
        phi = input_electron.phi();
        charge = input_electron.charge();
    }

    ZFinderElectron::ZFinderElectron(reco::GenParticle input_electron) {
        /* Set type of candidate and assign */
        candidate_type_ = RECO_GENPARTICLE;
        AddCutResult("type_gen", true, 1.);
        gen_elec_ = input_electron;
        // reco::GenParticle is a child of reco::candidate
        candidate_ = dynamic_cast<reco::Candidate*>(&gen_elec_);
        /* Extract the useful quantities from a gen electron */
        pt = input_electron.pt();
        phi = input_electron.phi();
        eta = input_electron.eta();
        // Using the input_electron Data Group ID Number, determine if the input_electron is an
        // electron or positron
        if (input_electron.pdgId() == ELECTRON) {
            charge = -1;
        } else if (input_electron.pdgId() == POSITRON) {
            charge = 1;
        }
    }

    ZFinderElectron::ZFinderElectron(reco::RecoEcalCandidate input_electron) {
        /* Set type of candidate and assign */
        candidate_type_ = RECO_RECOECALCANDIDATE;
        AddCutResult("type_ecalcandidate", true, 1.);
        recan_elec_ = input_electron;
        candidate_ = &input_electron;
        /* Extract the useful quantities from a GsfElectron */
        pt = input_electron.pt();
        eta = input_electron.eta();
        phi = input_electron.phi();
        charge = input_electron.charge();
        // Add cut result to store parent information
    }

    ZFinderElectron::ZFinderElectron(reco::Photon input_electron) {
        /* Set type of candidate and assign */
        candidate_type_ = RECO_PHOTON;
        AddCutResult("type_photon", true, 1.);
        photon_elec_ = input_electron;
        candidate_ = &input_electron;
        /* Extract the useful quantities from a GsfElectron */
        pt = input_electron.pt();
        eta = input_electron.eta();
        phi = input_electron.phi();
        charge = 0;  // No charge because no tracker
    }

    ZFinderElectron::ZFinderElectron(trigger::TriggerObject input_electron) {
        /* Set type of candidate and assign */
        candidate_type_ = RECO_TRIGGER;
        AddCutResult("type_hlt", true, 1.);
        trig_elec_ = input_electron;
        candidate_ = NULL;
        /* Extract the useful quantities from a GsfElectron */
        pt = input_electron.pt();
        eta = input_electron.eta();
        phi = input_electron.phi();
        charge = 0;  // No charge
    }

    const CutResult* ZFinderElectron::GetCutResult(const std::string& cut_name) const {
        /* Return a CutResult based on the name */
        // Find the cut
        std::map<std::string, CutResult>::const_iterator i = cutresults_.find(cut_name);
        // Return a CutResult if it exists, otherwise return NULL
        if (i != cutresults_.end()) {
            return &(i->second);
        } else {
            return NULL;
        }
    }

    int ZFinderElectron::CutPassed(const std::string& cut_name) const {
        /* Return the passed status of a cut based on the name */
        const CutResult* cr = GetCutResult(cut_name);
        // If the CutResult exists, return the passed value, otherwise return -1
        if (cr != NULL) {
            return cr->passed;
        } else {
            return -1;
        }
    }

    double ZFinderElectron::CutWeight(const std::string& cut_name) const {
        /* Return the weight of a cut based on the name */
        const CutResult* cr = GetCutResult(cut_name);
        // If the CutResult exists, return the weight, otherwise return 0.
        if (cr != NULL) {
            return cr->weight;
        } else {
            return 0.;
        }
    }

    void ZFinderElectron::AddCutResult(const std::string& cut_name, const bool passed, const double weight) {
        /* Given a name and passed status, will add it to the cutmap. It may also
         * be given a weight, but if not it is assumed to be 1.
         *
         * This function will overwrite previous entries with the same name!
         */
        CutResult cr;
        cr.passed = passed;
        cr.name = cut_name;
        cr.weight = weight;
        cutresults_[cut_name] = cr;  // This will overwrite
    }

    int ZFinderElectron::SetCutWeight(const std::string& cut_name, const double WEIGHT) {
        /* Given the name of a cut and a weight, sets the weight of the cut to
         * the new value. If the cut doesn't exist, returns 1, otherwise 0.
         */
        /* Return the weight of a cut based on the name */
        const CutResult* cr = GetCutResult(cut_name);
        // If the CutResult exists, return the weight, otherwise return 0.
        if (cr != NULL) {
            AddCutResult(cr->name, cr->passed, WEIGHT);
            return 0;
        } else {
            // Failure
            return 1.;
        }
    }

    std::vector<const CutResult*>* ZFinderElectron::GetAllCuts() {
        /* Return all cuts */
        std::vector<const CutResult*>* tmp_vec = new std::vector<const CutResult*>();
        for (auto& i_cut : cutresults_) {
            tmp_vec->push_back(&(i_cut.second));
        }
        return tmp_vec;
    }

    std::vector<const CutResult*>* ZFinderElectron::GetCutsBool(const bool PASSED) {
        /* Finds all cuts that have passed matching PASSED */
        std::vector<const CutResult*>* tmp_vec = new std::vector<const CutResult*>();
        for (auto& i_cut : cutresults_) {
            if (i_cut.second.passed == PASSED) {
                tmp_vec->push_back(&(i_cut.second));
            }
        }
        return tmp_vec;
    }
}
