#include "ZFinder/ZFinder/interface/ZFinderElectron.h"

// Standard Library
#include <iostream>  // std::cout, std::endl;


namespace zf {
    ZFinderElectron::ZFinderElectron(reco::GsfElectron input_electron) {
        /* Set type of candidate and assign */
        candidate_type_ = GSFELECTRON;
        gsf_elec_ = input_electron;
        candidate_ = &input_electron;
        /* Extract the useful quantities from a GsfElectron */
        pt = input_electron.pt();
        eta = input_electron.superCluster()->eta(); 
        phi = input_electron.superCluster()->phi();
        charge = input_electron.charge();
    }

    ZFinderElectron::ZFinderElectron(HepMC::GenParticle input_electron) {
        /* Set type of candidate and assign */
        candidate_type_ = GENPARTICLE;
        gen_elec_ = input_electron;
        // HepMC::GenParticle is a child of reco::candidate
        candidate_ = dynamic_cast<reco::Candidate*>(&gen_elec_);
        /* Extract the useful quantities from a gen electron */
        pt = input_electron.momentum().perp();
        phi = input_electron.momentum().phi();
        eta = input_electron.momentum().eta();
        // Using the input_electron Data Group ID Number, determine if the input_electron is an
        // electron or positron
        if (input_electron.pdg_id() == ELECTRON) {
            charge = -1;
        } else if (input_electron.pdg_id() == POSITRON) {
            charge = 1;
        }
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

    bool ZFinderElectron::CutPassed(const std::string& cut_name) const {
        /* Return the passed status of a cut based on the name */
        const CutResult* cr = GetCutResult(cut_name);
        // If the CutResult exists, return the passed value, otherwise return false
        if (cr != NULL) {
            return cr->passed;
        } else {
            return false;
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
}
