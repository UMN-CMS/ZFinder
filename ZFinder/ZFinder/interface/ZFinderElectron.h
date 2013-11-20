#ifndef ZFINDER_ZFINDERELECTRON_H_
#define ZFINDER_ZFINDERELECTRON_H_

// Standard Library
#include <string>
#include <map>

// CMSSW
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"  // GsfElectron
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"  // GenParticle

struct CutResult{
    bool passed;
    std::string name;  // Redundant, but nice to have as a check
    double weight;
};

template <class Particle_T> class ZFinderElectron{
    public:
        // Constructor
        ZFinderElectron(Particle_T particle) : electron(particle) {
            // This class should never ever ever take anything else.
        }

        // A copy of the object ZFinderElectron was created from
        const Particle_T electron;

        // Kinematics variables
        double pt;
        double eta;
        double phi;

        // Other physical properties
        int charge;

        // Handling cuts
        //CutResult* GetCutResult(const std::string& cut_name) const;
        CutResult* GetCutResult(const std::string& cut_name) const {
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
        bool CutPassed(const std::string& cut_name) const {
            /* Return the passed status of a cut based on the name */
            CutResult* cr = GetCutResult(cut_name);
            // If the CutResult exists, return the passed value, otherwise return false
            if (cr != NULL) {
                return cr->passed;
            } else {
                return false;
            }
        }

        double CutWeight(const std::string& cut_name) const {
            /* Return the weight of a cut based on the name */
            CutResult* cr = GetCutResult(cut_name);
            // If the CutResult exists, return the weight, otherwise return 0.
            if (cr != NULL) {
                return cr->weight;
            } else {
                return 0.;
            }
        }

        void AddCutResult(const std::string& cut_name, const bool passed, const double weight=1.) {
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

    private:
        std::map<std::string, CutResult> cutresults_;
};

#endif  // ZFINDER_ZFINDERELECTRON_H_
