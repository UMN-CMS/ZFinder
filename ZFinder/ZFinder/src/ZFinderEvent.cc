#include "ZFinder/ZFinder/interface/ZFinderEvent.h"

// Standard Library
#include <algorithm>  // std::sort

// CMSSW
#include "DataFormats/Common/interface/Handle.h"  // edm::Handle
#include "EgammaAnalysis/ElectronTools/interface/EGammaCutBasedEleId.h"  // EgammaCutBasedEleId::PassWP, EgammaCutBasedEleId::*
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"  // PileupSummaryInfo

// ZFinder
#include "ZFinder/ZFinder/interface/PDGID.h"  // PDGID enum (ELECTRON, POSITRON, etc.)


template <class Particle_T>
ZFinderEvent<Particle_T>::ZFinderEvent(const edm::Event& iEvent, const edm::EventSetup& iSetup, const edm::ParameterSet& iConfig, const bool use_truth=false) {
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

    /* Get InputTags */
    // Reco
    inputtags_.electron = iConfig.getParameter<edm::InputTag>("electronsInputTag");
    inputtags_.conversion = iConfig.getParameter<edm::InputTag>("conversionsInputTag");
    inputtags_.beamspot = iConfig.getParameter<edm::InputTag>("beamSpotInputTag");
    inputtags_.rho_iso = iConfig.getParameter<edm::InputTag>("rhoIsoInputTag");
    inputtags_.vertex = iConfig.getParameter<edm::InputTag>("primaryVertexInputTag");
    inputtags_.iso_vals = iConfig.getParameter<std::vector<edm::InputTag> >("isoValInputTags");
    // Truth
    inputtags_.pileup = iConfig.getParameter<edm::InputTag>("pileupInputTag");
    inputtags_.generator = iConfig.getParameter<edm::InputTag>("generatorInputTag");

    /* Finish initialization based on MC or not */
    if (is_real_data && !use_truth) {
        // Data
        InitReco(iEvent, iSetup, cuts);
    } else if (!is_real_data && use_truth) {
        // MC
        InitTruth(iEvent, iSetup, cuts);
    }   // Else do nothing
}

template <class Particle_T>
void ZFinderEvent<Particle_T>::InitReco(const edm::Event& iEvent, const edm::EventSetup& iSetup, const BasicRequirements& cuts) {

    /* Count Pile Up and store first vertex location*/
    edm::Handle<reco::VertexCollection> reco_vertices;
    iEvent.getByLabel(inputtags_.vertex, reco_vertices);
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
    iEvent.getByLabel(inputtags_.beamspot, beam_spot);
    bs.x = beam_spot->position().X();
    bs.y = beam_spot->position().Y();
    bs.z = beam_spot->position().Z();

    /* Find electrons */
    InitRecoElectrons(iEvent, iSetup, cuts);
}

template <class Particle_T>
void ZFinderEvent<Particle_T>::InitRecoElectrons(const edm::Event& iEvent, const edm::EventSetup& iSetup, const BasicRequirements& cuts) {
    // We split this part into a new function because it is very long
    // Most of this code is stolen from the example here:
    // http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/EGamma/EGammaAnalysisTools/src/EGammaCutBasedEleIdAnalyzer.cc?view=markup

    // electrons
    edm::Handle<reco::GsfElectronCollection> els_h;
    iEvent.getByLabel(inputtags_.electron, els_h);

    // conversions
    edm::Handle<reco::ConversionCollection> conversions_h;
    iEvent.getByLabel(inputtags_.conversion, conversions_h);

    // iso deposits
    typedef std::vector< edm::Handle< edm::ValueMap<double> > > IsoDepositVals;
    IsoDepositVals isoVals(inputtags_.iso_vals.size());
    for (size_t j = 0; j < inputtags_.iso_vals.size(); ++j) {
        iEvent.getByLabel(inputtags_.iso_vals[j], isoVals[j]);
    }

    // beam spot
    edm::Handle<reco::BeamSpot> beamspot_h;
    iEvent.getByLabel(inputtags_.beamspot, beamspot_h);
    const reco::BeamSpot &beamSpot = *(beamspot_h.product());

    // vertices
    edm::Handle<reco::VertexCollection> vtx_h;
    iEvent.getByLabel(inputtags_.vertex, vtx_h);

    // rho for isolation
    // The python uses:
    // cms.InputTag("kt6PFJetsForIsolation", "rho")
    edm::Handle<double> rhoIso_h;
    iEvent.getByLabel(inputtags_.rho_iso, rhoIso_h);
    double rhoIso = *(rhoIso_h.product());

    // loop on electrons
    for(unsigned int i = 0; i < els_h->size(); ++i) {
        // Get the electron and set put it into the electrons vector
        reco::GsfElectron electron = els_h->at(i);
        ZFinderElectron<Particle_T>* zf_electron = AddElectron(electron);

        // get reference to electron and the electron
        reco::GsfElectronRef ele_ref(els_h, i);

        // get particle flow isolation
        const double ISO_CH =  (*(isoVals)[0])[ele_ref];
        const double ISO_EM = (*(isoVals)[1])[ele_ref];
        const double ISO_NH = (*(isoVals)[2])[ele_ref];

        // test ID
        // working points
        bool veto = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::VETO, ele_ref, conversions_h, beamSpot, vtx_h, ISO_CH, ISO_EM, ISO_NH, rhoIso);
        bool loose = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::LOOSE, ele_ref, conversions_h, beamSpot, vtx_h, ISO_CH, ISO_EM, ISO_NH, rhoIso);
        bool medium = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::MEDIUM, ele_ref, conversions_h, beamSpot, vtx_h, ISO_CH, ISO_EM, ISO_NH, rhoIso);
        bool tight = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::TIGHT, ele_ref, conversions_h, beamSpot, vtx_h, ISO_CH, ISO_EM, ISO_NH, rhoIso);

        // eop/fbrem cuts for extra tight ID
        bool fbremeopin = EgammaCutBasedEleId::PassEoverPCuts(ele_ref);

        // cuts to match tight trigger requirements
        bool trigtight = EgammaCutBasedEleId::PassTriggerCuts(EgammaCutBasedEleId::TRIGGERTIGHT, ele_ref);

        // for 2011 WP70 trigger
        bool trigwp70 = EgammaCutBasedEleId::PassTriggerCuts(EgammaCutBasedEleId::TRIGGERWP70, ele_ref);
        
        // Add the cuts to our electron
        const double WEIGHT = 1.;
        zf_electron->AddCutResult("wp:veto", veto, WEIGHT);
        zf_electron->AddCutResult("wp:loose", loose, WEIGHT);
        zf_electron->AddCutResult("wp:medium", medium, WEIGHT);
        zf_electron->AddCutResult("wp:tight", tight, WEIGHT);
        zf_electron->AddCutResult("wp:eop_cut", fbremeopin, WEIGHT);
        zf_electron->AddCutResult("wp:trigtight", trigtight, WEIGHT);
        zf_electron->AddCutResult("wp:trigwp70", trigwp70, WEIGHT);
    }

    // Set up the Z
    InitZ();
}

template <class Particle_T>
void ZFinderEvent<Particle_T>::InitZ() {
    // Sort our electrons and set e0, e1 as the two with the highest pt
    std::sort(electrons_.begin(), electrons_.end(), SortByPTHighLow);
    set_both_e(&electrons_[0], &electrons_[1]);

    // Set Z properties
    const double ELECTRON_MASS = 5.109989e-4;
    math::PtEtaPhiMLorentzVector e0lv(e0->pt(), e0->eta(), e0->phi(), ELECTRON_MASS);
    math::PtEtaPhiMLorentzVector e1lv(e1->pt(), e1->eta(), e1->phi(), ELECTRON_MASS);
    math::PtEtaPhiMLorentzVector zlv;
    zlv = e0lv + e1lv;
        
    z.m = zlv.mass();
    z.y = zlv.Rapidity();
    z.pt = zlv.pt();
    z.phistar = ReturnPhistar(e0->eta, e0->phi, e1->eta, e1->phi);
}

template <class Particle_T>
void ZFinderEvent<Particle_T>::InitTruth(const edm::Event& iEvent, const edm::EventSetup& iSetup, const BasicRequirements& cuts) {
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
    bs.x = bs.y = bs.z = -1.;

    /*
     * We don't need to select electrons with cuts, because in Monte Carlo we
     * can just ask for the Z.
     */
    edm::Handle<edm::HepMCProduct> mc_particles;
    iEvent.getByLabel("generator", mc_particles);
    const HepMC::GenEvent* gen_event = mc_particles->GetEvent();

    // Read through the MC, looking for Z -> ee
    HepMC::GenEvent::vertex_const_iterator vertex;
    HepMC::GenVertex::particles_out_const_iterator outgoing_particle;
    HepMC::GenParticle* outgoing_electron_0 = NULL;
    HepMC::GenParticle* outgoing_electron_1 = NULL;
    HepMC::GenParticle* Z = NULL;
    for (vertex = gen_event->vertices_begin(); vertex != gen_event->vertices_end(); ++vertex) {
        // Make sure there is 1 incoming particle, and it is a ZBOSON
        if (    (*vertex)->particles_in_size() == 1
                && (*((*vertex)->particles_in_const_begin()))->pdg_id() == ZBOSON
           ) {
            Z = (*((*vertex)->particles_in_const_begin()));
            for (outgoing_particle = (*vertex)->particles_out_const_begin(); outgoing_particle != (*vertex)->particles_out_const_end(); ++outgoing_particle) {
                if (abs((*outgoing_particle)->pdg_id()) == ELECTRON) {
                    if(outgoing_electron_0 == NULL) {
                        outgoing_electron_0 = *outgoing_particle;
                    } else {
                        outgoing_electron_1 = *outgoing_particle;
                    }
                }
            }
        }
    }

    // Check cuts and add electrons
    if (    cuts.ept_min < outgoing_electron_0->momentum().perp() 
            && outgoing_electron_0->momentum().perp() < cuts.ept_max
       ) {
        ZFinderElectron<Particle_T> zf_electron = AddElectron(*outgoing_electron_0);
        set_e0(&zf_electron);
    }
    if (    cuts.ept_min < outgoing_electron_1->momentum().perp() 
            && outgoing_electron_1->momentum().perp() < cuts.ept_max
       ) {
        ZFinderElectron<Particle_T> zf_electron = AddElectron(*outgoing_electron_1);
        set_e1(&zf_electron);
    }

    // Z Properties
    z.m = Z->momentum().m();
    z.pt = Z->momentum().perp();
    const double ZEPP = Z->momentum().e() + Z->momentum().pz();
    const double ZEMP = Z->momentum().e() - Z->momentum().pz();
    z.y = 0.5 * log(ZEPP / ZEMP);
    z.phistar = ReturnPhistar(e0->eta, e0->phi, e1->eta, e1->phi);
}

/* Add an electron to electrons_ */
template <class Particle_T>
ZFinderElectron<Particle_T>* ZFinderEvent<Particle_T>::AddElectron(reco::GsfElectron electron) {
    ZFinderElectron<Particle_T> zf_electron(electron);
    electrons_.push_back(zf_electron);
    return &electrons_.back();
}

template <class Particle_T>
ZFinderElectron<Particle_T>* ZFinderEvent<Particle_T>::AddElectron(HepMC::GenParticle electron) {
    ZFinderElectron<Particle_T> zf_electron(electron);
    electrons_.push_back(zf_electron);
    return &electrons_.back();
}

template <class Particle_T>
double ZFinderEvent<Particle_T>::ReturnPhistar(const double& eta0, const double& phi0, const double& eta1, const double& phi1) {
    /* Calculate phi star */
    const double PI = 3.14159265358979323846;
    double dphi = phi0 - phi1;

    // Properly account for the fact that 2pi == 0.
    if (dphi < 0){
        if (dphi > -PI){
            dphi = fabs(dphi);
        }
        if (dphi < -PI) {
            dphi += 2*PI;
        }
    }
    if (dphi > PI){
        dphi = 2*PI - dphi;
    }

    const double DETA = fabs(eta0 - eta1);

    /* PhiStar */
    return ( 1 / cosh( DETA / 2 ) ) * (1 / tan( dphi / 2 ) );
}
