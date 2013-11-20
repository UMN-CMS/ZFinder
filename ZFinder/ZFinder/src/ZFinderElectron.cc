#include "ZFinder/ZFinder/interface/ZFinderElectron.h"

// Standard Library
#include <iostream>

// ZFinder
#include "ZFinder/ZFinder/interface/PDGID.h"  // PDGID enum (ELECTRON, POSITRON, etc.)

template <> 
ZFinderElectron<reco::GsfElectron>::ZFinderElectron(reco::GsfElectron particle) : electron(particle) {
    /* Extract the useful quantities from a GsfElectron */
    pt = particle.pt();
    eta = particle.superCluster()->eta(); 
    phi = particle.superCluster()->phi();
    charge = particle.charge();
}

template <> 
ZFinderElectron<HepMC::GenParticle>::ZFinderElectron(HepMC::GenParticle particle) : electron(particle) {
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
