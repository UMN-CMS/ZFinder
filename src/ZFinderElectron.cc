#include "ZShape/ZFinder/interface/ZFinderElectron.h"
#include <iostream>

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
