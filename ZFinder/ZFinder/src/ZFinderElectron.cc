#include "ZFinder/ZFinder/interface/ZFinderElectron.h"
#include <iostream>

ZFinderElectron::ZFinderElectron(reco::CadidateBaseRef* particle){
/* Extract the useful quantities from a reco electron
 */
    pt = particle->p4().pt();
    phi = particle->p4().phi();
    eta = particle->p4().eta();
    charge = particle->charge();

    // Assign internal pointer to the object used to create the ZFinderElectron
    electron = particle;

    // TODO: PF Iso
}

ZFinderElectron::ZFinderElectron(GenParticle* particle){
/* Extract the useful quantities from a gen electron
 */
    pt = particle->momentum().perp();
    phi = particle->momentum().phi();
    eta = particle->momentum().eta();
    if (particle->pdg_id() == 11) {
        charge = -1;
    } else if (particle->pdg_id() == -11) {
        charge = 1;
    }

    // Assign internal pointer to the object used to create the ZFinderElectron
    electron = particle;

    // TODO: PF Iso
}

ZFinderElectron::getCutResult(const std::string& cutName){
    /* Return a CutResult based on the name */
    // Find the cut
    std::map<std::string, CutResult>::const_iterator i = m_CutResults.find(cutName);
    // Return a CutResult if it exists, otherwise return NULL
    if (i != m_CutResults.end()){
        return &(i->second);
    } else {
        return NULL;
    }
}

bool ZFinderElectron::cutPassed(const std::string& cutName){
    /* Return the passed status of a cut based on the name */
    CutResult* cr = getCutResult(cutName);
    // If the CutResult exists, return the passed value, otherwise return false
    if (CutResult != NULL){
        return CutResult->passed;
    } else {
        return false;
    }
}

double ZFinderElectron::cutWeight(const std::string& cutName){
    /* Return the weight of a cut based on the name */
    CutResult* cr = getCutResult(cutName);
    // If the CutResult exists, return the weight, otherwise return 0.
    if (CutResult != NULL){
        return CutResult->weight;
    } else {
        return 0.;
    }
}

void addCutResult(const std::string& cutName, const bool passed, const double weight=1.){
    /* Given a name and passed status, will add it to the cutmap. It may also
     * be given a weight, but if not it is assumed to be 1.
     *
     * This function will overwrite previous entries with the same name!
     */
    CutResult cr;
    cr.passed = passed;
    cr.name = cutName;
    cr.weight = weight;
    m_CutResults[cutName] = cr;  // This will overwrite
}
