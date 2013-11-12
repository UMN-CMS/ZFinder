#include "ZFinder/ZFinder/interface/ZFinderEvent.h"

// ZFinder
#include "PDGID.h"  // PDGID enum (ELECTRON, POSITRON, etc.)

ZFinderEvent::ZFinderEvent(const edm::Event& iEvent, const edm::EventSetup& iSetup, const bool use_truth=false) {
    /* Given an event, parses them for the information needed to make the classe.
     *
     * It selects electrons based on a minimum level of hard-coded cuts.
     */
    /* Cuts */
    BasicRequirements cuts;
    cuts.ept_min = 20.;
    cuts.ept_max = 1e12;

    /* Get event info */
    id.run_num = iEvent.run();
    id.lumi_num = iEvent.luminosityBlock();
    id.event_num = iEvent.id().event();

    /* Set local is_real_data */
    is_real_data = iEvent.isRealData;

    /* Finish initialization based on MC or not */
    if (iEvent.isRealData && !use_truth) {
        // Data
        init_reco(iEvent, iSetup, cuts);
    } else if (!iEvent.isRealData && use_truth) {
        // MC
        init_truth(iEvent, iSetup, cuts);
    }   // Else do nothing
}

ZFinderElectron* ZFinderEvent::allocateElectron() {
    return new ZFinderElectron();
}

void ZFinderEvent::init_reco(const edm::Event& iEvent, const edm::EventSetup iSetup, const BasicRequirements cuts) {

    /* Count Pile Up and store first vertex location*/
    edm::Handle<reco::VertexCollection> reco_vertices;
    iEvent.getByLabel("offlinePrimaryVertices", reco_vertices);
    vert.num = 0;
    bool first_vertex = true;
    for(unsigned int vertex=0; vertex < reco_vertices->size(); ++vertex) {
        if (    // Criteria copied from twiki
                !((*reco_vertices)[vertex].isFake())
                && ((*reco_vertices)[vertex].ndof() > 4)
                && (fabs((*reco_vertices)[vertex].z()) <= 24.0)
                && ((*reco_vertices)[vertex].position().Rho() <= 2.0)
           ) {
            vert.num++;
            // Store first good vertex as "primary"
            if (first_vertex) {
                first_vertex = false;
                vert.x = (*reco_vertices)[vertex].x();
                vert.y = (*reco_vertices)[vertex].y();
                vert.z = (*reco_vertices)[vertex].z();
            }
        }
    }

    /* Beamspot */
    edm::Handle<reco::BeamSpot> beam_spot;
    iEvent.getByLabel("offlineBeamSpot", beam_spot);
    bs.x = beam_spot->position().X();
    bs.y = beam_spot->position().Y();
    bs.z = beam_spot->position().Z();

    /* Find electrons */

}

void ZFinderEvent::init_truth(const edm::Event& iEvent, const edm::EventSetup iSetup, const BasicRequirements cuts) {

    /* Count Pile Up */
    edm::Handle<std::vector<PileupSummaryInfo> > pileup_info;
    iEvent.getByLabel("addPileupInfo", pileup_info);
    if (pileup_info.isValid()) {
        vert.num = pileup_info->size();
    } else {
        vert.num = -1;
    }

    /* Beamspot */
    // No beamspot in MC

    /*
     * We don't need to select electrons with cuts, because in Monte Carlo we
     * can just ask for the Z.
     */
    Handle<edm::HepMCProduct> mc_particles;
    iEvent.getByLabel("generator", mc_particles);
    const HepMC::GenEvent* gen_event = mc_particles->GetEvent();

    // Read through the MC, looking for Z -> ee
    HepMC::GenEvent::vertex_const_iterator vertex;
    HepMC::GenVertex::particles_out_const_iterator outgoing_particle;
    HepMC::GenParticle* outgoing_electron_0 = 0;
    HepMC::GenParticle* outgoing_electron_1 = 0;
    HepMC::GenParticle* Z = 0;
    for (vertex = gen_event->vertices_begin(); vertex != gen_event->vertices_end(); ++vertex) {
        // Make sure there is 1 incoming particle, and it is a ZBOSON
        if (    (*vertex)->particles_in_size() == 1
                && (*((*vertex)->particles_in_const_begin()))->pdg_id() == ZBOSON
           ) {
            Z = (*((*vertex)->particles_in_const_begin()));
            for (outgoing_particle = (*vertex)->particles_out_const_begin(); outgoing_particle != (*vertex)->particles_out_const_end(); ++outgoing_particle) {
                if (abs((*outgoing_particle)->pdg_id()) == ELECTRON) {
                    if(outgoing_electron_0 == 0) {
                        outgoing_electron_0 = *outgoing_particle;
                    } else {
                        outgoing_electron_1 = *outgoing_particle;
                    }
                }
            }
        }
    }

    // Check cuts
    if (cuts.ept_min < outgoing_electron_0->momentum().perp() && ge0->momentum().perp() < cuts.ept_max) {
        ZFinderElectron* zfe = addElectron(outgoing_electron_0);
        setE0(zfe);
    }
    if (cuts.ept_min < outgoing_electron_1->momentum().perp() && ge1->momentum().perp() < cuts.ept_max) {
        ZFinderElectron* zfe = addElectron(outgoing_electron_1);
        setE1(zfe);
    }
}

/* Add an electron to electrons_ */
ZFinderElectron* ZFinderEvent::addElectron(Cadidate* particle) {
    ZFinderElectron* zfe = new ZFinderElectron(particle);
    electrons_.push_back(zfe);
    return zfe;
}

/* Set Electron 0 and 1 */

void ZFinderEvent::setE0(const ZFinderElectron* electron) {
    /* Set the class's pointer for e0 */
    e0 = electron;
}

void ZFinderEvent::setE1(const ZFinderElectron* electron) {
    /* Set the class's pointer for e1 */
    e1 = electron;
}

void ZFinderEvent::setEs(const ZFinderElectron* electron0, const ZFinderElectron* electron1) {
    /* Set the class's pointer for e0 and e1 at the same time */
    e0 = electron0;
    e1 = electron1;
}
