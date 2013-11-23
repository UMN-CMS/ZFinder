#include "ZFinder/ZFinder/interface/ZFinderElectron.h"

// Standard Library
#include <iostream>  // std::cout, std::endl;


namespace zf {
    ZFinderElectron::ZFinderElectron(reco::GsfElectron particle) {
        /* Extract the useful quantities from a GsfElectron */
        pt = particle.pt();
        eta = particle.superCluster()->eta(); 
        phi = particle.superCluster()->phi();
        charge = particle.charge();
    }

    ZFinderElectron::ZFinderElectron(HepMC::GenParticle particle) {
        /* Extract the useful quantities from a gen electron */
        pt = particle.momentum().perp();
        phi = particle.momentum().phi();
        eta = particle.momentum().eta();
        // Using the Particle Data Group ID Number, determine if the particle is an
        // electron or positron
        if (particle.pdg_id() == ELECTRON) {
            charge = -1;
        } else if (particle.pdg_id() == POSITRON) {
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
