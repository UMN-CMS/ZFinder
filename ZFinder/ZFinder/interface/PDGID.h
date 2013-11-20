#ifndef ZFINDER_PDFID_H_
#define ZFINDER_PDFID_H_

/* IDs as defined by the Particle Data Group here:
 * http://pdg.lbl.gov/mc_particle_id_contents.html
 *
 * Names are selected based on frequency of use in the high
 * energy physics community. So, for example, both MUON and MU
 * are provided because both are commonly used, but only TAU is
 * provided (because no one says TAUON).
 */
enum PDGID {
    // Quarks and Antiquarks
    DQUARK = 1,
    DBAR = -1,
    DBARQUARK = -1,
    UQUARK = 2,
    UBAR = -2,
    UBARQUARK = -2,
    SQUARK = 3,
    SBAR = -3,
    SBARQUARK = -3,
    CQUARK = 4,
    CBAR = -4,
    CBARQUARK = -4,
    BQUARK = 5,
    BBAR = -5,
    BBARQUARK = -5,
    TQUARK = 6,
    TBAR = -6,
    TBARQUARK = -6,
    // Electron and Electron Neutrino
    ELECTRON = 11,
    POSITRON = -11,
    ANTIELECTRON = -11,
    NU_E = 12,
    ANTINU_E = -12,
    NU_EBAR = -12,
    // Muon and Muon Neutrino
    MUON = 13,
    MU = 13,
    ANTIMUON = -13,
    ANTIMU = -13,
    NU_MU = 14,
    ANTINU_MU = -14,
    NU_MUBAR = -14,
    // Tau and Tau Neutrino
    TAU = 15,
    ANTITAU = -15,
    NU_TAU = 16,
    ANTINU_TAU = -16,
    NU_TAUBAR = -16,
    // Gauge and Higgs Bosons
    GLUON = 21,
    PHOTON = 22,
    GAMMA = 22,
    ZBOSON = 23,
    WPLUS = 24,
    WPLUSBOSON = 24,
    WMINUS = -24,
    WMINUSBOSON = -24,
    HIGGS = 25,
    HIGGSBOSON = 25
};

#endif  // ZFINDER_PDFID_H_
