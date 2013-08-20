#include "ZShape/ZFinder/interface/ZFidnerEvent.h"


ZFinderEvent::ZFinderEvent(const edm::Event& iEvent, const edm::EventSetup iSetup, const bool useTruth=false){
    /* Given an event, parses them for the information needed to make the classe. 
     *
     * It selects electrons based on a minimum level of hard-coded cuts.
     */ 
    /* Cuts */
    BasicRequirements cuts; 
    cuts.ept_MIN = 20.;
    cuts.ept_MAX = 1e12;

    /* Get event info */
    id.runNum = iEvent.run();
    id.lumiNum = iEvent.luminosityBlock();
    id.eventNum = iEvent.id().event();

    /* Set local isRealData */
    isRealData = iEvent.isRealData;

    /* Finish initialization based on MC or not */
    if (iEvent.isRealData && !useTruth){
        // Data
       init_reco(iEvent, iSetup, cuts); 
    } else if (!iEvent.isRealData && useTruth){
        // MC
       init_truth(iEvent, iSetup, cuts); 
    }   // Else do nothing
}

ZFinderElectron* ZFinderEvent::allocateElectron() {
    return new ZFinderElectron(); 
}

void ZFinderEvent::init_reco(const edm::Event& iEvent, const edm::EventSetup iSetup, const BasicRequirements cuts){

    /* Count Pile Up and store first vertex location*/
    edm::Handle<reco::VertexCollection> recVtxs;
    iEvent.getByLabel("offlinePrimaryVertices", recVtxs);
    vert.num = 0;
    for(unsigned int ind=0; ind < recVtxs->size(); ind++) {
        if (    // Criteria copied from twiki
                !((*recVtxs)[ind].isFake()) 
                && ((*recVtxs)[ind].ndof()>4)
                && (fabs((*recVtxs)[ind].z())<=24.0) 
                && ((*recVtxs)[ind].position().Rho()<=2.0) 
           ) {
            vert.num++;
            if (vert.num++ == 1){
                // Store first good vertex as "primary"
                vert.x = (*recVtxs)[ind].x();
                vert.y = (*recVtxs)[ind].y();
                vert.z = (*recVtxs)[ind].z();
            }
        }
    }

    /* Beamspot */
    edm::Handle<reco::BeamSpot> beamSpot;
    iEvent.getByLabel("offlineBeamSpot", beamSpot);
    bs.x = beamSpot->position().X();
    bs.y = beamSpot->position().Y();
    bs.z = beamSpot->position().Z();

    /* Find electrons */

}

void ZFinderEvent::init_truth(const edm::Event& iEvent, const edm::EventSetup iSetup, const BasicRequirements cuts){

    /* Count Pile Up */
    edm::Handle<std::vector<PileupSummaryInfo> > pPU;
    iEvent.getByLabel("addPileupInfo", pPU);
    if (pPU.isValid()){
        vert.num = pPU->size();
    } else {
        vert.num = -1; 
    }

    /* Beamspot */
    // No beamspot in MC

    /* Find electrons
     *
     * In this case we know which is the Z, so we just grab it 
     */

    Handle<edm::HepMCProduct> HepMC;
    iEvent.getByLabel("generator", HepMC);
    const HepMC::GenEvent* genE=HepMC->GetEvent();

    // Read through the MC, looking for Z -> ee
    HepMC::GenEvent::vertex_const_iterator vtex;
    HepMC::GenVertex::particles_out_const_iterator Pout;
    HepMC::GenParticle* ge0=0;
    HepMC::GenParticle* ge1=0;
    HepMC::GenParticle* Z=0;
    for (vtex = genE->vertices_begin(); vtex != genE->vertices_end(); vtex++){
        if (((*vtex)->particles_in_size()) == 1) {
            if ((*((*vtex)->particles_in_const_begin()))->pdg_id() == 23){ // Test for Z
                Z=(*((*vtex)->particles_in_const_begin()));
                for (Pout=(*vtex)->particles_out_const_begin(); Pout != (*vtex)->particles_out_const_end(); Pout++){
                    if (abs((*Pout)->pdg_id()) == 11){ // To Electrons
                        if(ge0==0){
                            ge0 = *Pout;
                        } else {
                            ge1 = *Pout;
                        }
                    }
                }
            }
        } 
    }
    
    // Check cuts
    if (cuts.ept_MIN < ge0->momentum().perp() && ge0->momentum().perp() < cuts.ept_MAX){
        ZFinderElectron* zfe = addElectron(ge0); 
        setE0(zfe);
    }
    if (cuts.ept_MIN < ge1->momentum().perp() && ge1->momentum().perp() < cuts.ept_MAX){
        ZFinderElectron* zfe = addElectron(ge1); 
        setE1(zfe);
    }
}

/* Add an electron to m_electrons */
ZFinderElectron* ZFinderEvent::addElectron(Cadidate* particle){
    ZFinderElectron* zfe = new ZFinderElectron(particle);
    m_electrons.push_back(zfe);
    return zfe;
}

/* Set Electron 0 and 1 */

void ZFinderEvent::setE0(const ZFinderElectron* electron){
    /* Set the class's pointer for e0 */
    e0 = electron;
}

void ZFinderEvent::setE1(const ZFinderElectron* electron){
    /* Set the class's pointer for e1 */
    e1 = electron;
}

void ZFinderEvent::setEs(const ZFinderElectron* electron0, const ZFinderElectron* electron1){
    /* Set the class's pointer for e0 and e1 at the same time */
    e0 = electron0;
    e1 = electron1;
}
