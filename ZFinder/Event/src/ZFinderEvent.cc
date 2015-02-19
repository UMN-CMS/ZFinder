#include "ZFinder/Event/interface/ZFinderEvent.h"

// Standard Library
#include <algorithm>  // std::sort, std::swap
#include <iostream>  // std::cout, std::endl

// CMSSW
#include "DataFormats/Common/interface/Handle.h"  // edm::Handle
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"  // reco::PhotonCollection
#include "DataFormats/EgammaReco/interface/HFEMClusterShape.h"  // reco::HFEMClusterShape
#include "DataFormats/EgammaReco/interface/HFEMClusterShapeAssociation.h"  // reco::HFEMClusterShapeAssociationCollection
#include "DataFormats/EgammaReco/interface/HFEMClusterShapeFwd.h"  // reco::HFEMClusterShapeRef,
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"  // reco::SuperClusterCollection, reco::SuperClusterRef
#include "DataFormats/HLTReco/interface/TriggerEvent.h" // trigger::TriggerEvent
#include "DataFormats/RecoCandidate/interface/RecoEcalCandidateFwd.h"  // reco::RecoEcalCandidateCollection
#include "EgammaAnalysis/ElectronTools/interface/EGammaCutBasedEleId.h"  // EgammaCutBasedEleId::PassWP, EgammaCutBasedEleId::*
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"  // GenEventInfoProduct
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"  // PileupSummaryInfo

// ZFinder
#include "ZFinder/Event/interface/PDGID.h"  // PDGID enum (ELECTRON, POSITRON, etc.)
#include "ZFinder/Event/interface/TriggerList.h"  // ET_ET_TIGHT, ET_ET_DZ, ET_ET_LOOSE, ET_NT_ET_TIGHT, ET_HF_ET_TIGHT, ET_HF_ET_LOOSE, ET_HF_HF_TIGHT, ET_HF_HF_LOOSE, SINGLE_ELECTRON_TRIGGER, ALL_TRIGGERS
#include "ZFinder/Event/interface/PileupReweighting.h"  // RUN_2012_*_TRUE_PILEUP, SUMMER12_53X_MC_TRUE_PILEUP


namespace zf {
    /*
     * These variables are hard coded here for easy access, instead of randomly
     * scattering them throughout the code
     */
    // Electrons are considered matched to a trigger object if close than this
    // value
    const double ZFinderEvent::TRIG_DR_ = 0.3;
    const double ZFinderEvent::NT_DR_ = 0.1;

    /*
     * The edm::LumiReWeighting constructor spams std::cout like mad, and it
     * can't be turned off. We get around this be constructing one static
     * instance and sharing it with all instances of the class.
     */
    edm::LumiReWeighting* ZFinderEvent::lumi_weights_ = nullptr;
    edm::LumiReWeighting* ZFinderEvent::lumi_weights_plus_ = nullptr;
    edm::LumiReWeighting* ZFinderEvent::lumi_weights_minus_ = nullptr;

    ZFinderEvent::ZFinderEvent(const edm::Event& iEvent, const edm::EventSetup& iSetup, const edm::ParameterSet& iConfig) {
        /* Given an event, parses them for the information needed to make the
         * classe.
         *
         * It selects electrons based on a minimum level of hard-coded cuts.
         */
        // Clear Events
        InitVariables();

        // Get event info
        id.run_num = iEvent.run();
        id.lumi_num = iEvent.luminosityBlock();
        id.event_num = iEvent.id().event();

        // Set local is_real_data
        is_real_data = iEvent.isRealData();

        // Get InputTags
        // Reco
        inputtags_.ecal_electron = iConfig.getParameter<edm::InputTag>("ecalElectronsInputTag");
        inputtags_.nt_electron = iConfig.getParameter<edm::InputTag>("ntElectronsInputTag");
        inputtags_.hf_electron = iConfig.getParameter<edm::InputTag>("hfElectronsInputTag");
        inputtags_.hf_clusters = iConfig.getParameter<edm::InputTag>("hfClustersInputTag");
        inputtags_.conversion = iConfig.getParameter<edm::InputTag>("conversionsInputTag");
        inputtags_.beamspot = iConfig.getParameter<edm::InputTag>("beamSpotInputTag");
        inputtags_.rho_iso = iConfig.getParameter<edm::InputTag>("rhoIsoInputTag");
        inputtags_.vertex = iConfig.getParameter<edm::InputTag>("primaryVertexInputTag");
        inputtags_.iso_vals = iConfig.getParameter<std::vector<edm::InputTag> >("isoValInputTags");

        // Truth
        inputtags_.pileup = iConfig.getParameter<edm::InputTag>("pileupInputTag");
        inputtags_.generator = iConfig.getParameter<edm::InputTag>("generatorInputTag");

        // Use the muon acceptance requirements to select electrons before
        // making Zs
        use_muon_acceptance_ = iConfig.getParameter<bool>("use_muon_acceptance");

        // Reject events that do not have a generator Z->ee event
        require_gen_z_ = iConfig.getParameter<bool>("require_gen_z");

        // Set up the lumi reweighting, but only if it is MC.
        if (!is_real_data
            && lumi_weights_ == nullptr
            && lumi_weights_plus_ == nullptr
            && lumi_weights_minus_ == nullptr
        ) {
            const std::string PILEUP_ERA = iConfig.getParameter<std::string>("pileup_era");
            // We use a flag in the python file to set the pileup reweighting
            // to use. If a blank, or an unrecognized, string is passed, then
            // we use the full ABCD reweighting.
            std::vector<float> pileup_distribution_in_data = RUN_2012_ABCD_TRUE_PILEUP;
            std::vector<float> pileup_distribution_in_data_plus = RUN_2012_ABCD_TRUE_PILEUP_PLUS;
            std::vector<float> pileup_distribution_in_data_minus = RUN_2012_ABCD_TRUE_PILEUP_MINUS;

            std::cout << "Pileup reweighting using era: " << PILEUP_ERA << std::endl;
            if (PILEUP_ERA == "A") {
                pileup_distribution_in_data = RUN_2012_A_TRUE_PILEUP;
                pileup_distribution_in_data_plus = RUN_2012_A_TRUE_PILEUP_PLUS;
                pileup_distribution_in_data_minus = RUN_2012_A_TRUE_PILEUP_MINUS;
            }
            else if (PILEUP_ERA == "B") {
                pileup_distribution_in_data = RUN_2012_B_TRUE_PILEUP;
                pileup_distribution_in_data_plus = RUN_2012_B_TRUE_PILEUP_PLUS;
                pileup_distribution_in_data_minus = RUN_2012_B_TRUE_PILEUP_MINUS;
            }
            else if (PILEUP_ERA == "C") {
                pileup_distribution_in_data = RUN_2012_C_TRUE_PILEUP;
                pileup_distribution_in_data_plus = RUN_2012_C_TRUE_PILEUP_PLUS;
                pileup_distribution_in_data_minus = RUN_2012_C_TRUE_PILEUP_MINUS;
            }
            else if (PILEUP_ERA == "D") {
                pileup_distribution_in_data = RUN_2012_D_TRUE_PILEUP;
                pileup_distribution_in_data_plus = RUN_2012_D_TRUE_PILEUP_PLUS;
                pileup_distribution_in_data_minus = RUN_2012_D_TRUE_PILEUP_MINUS;
            }
            else {
                std::cout << "Using RUN_2012_ABCD_TRUE_PILEUP" << std::endl;
            }

            lumi_weights_ = new edm::LumiReWeighting(
                    SUMMER12_53X_MC_TRUE_PILEUP,  // MC distribution
                    pileup_distribution_in_data   // Data distribution
                    );
            lumi_weights_plus_ = new edm::LumiReWeighting(
                    SUMMER12_53X_MC_TRUE_PILEUP,
                    pileup_distribution_in_data_plus
                    );
            lumi_weights_minus_ = new edm::LumiReWeighting(
                    SUMMER12_53X_MC_TRUE_PILEUP,
                    pileup_distribution_in_data_minus
                    );
        }
        // Use the lumi reweighting to set the event weight. It is 1. for data,
        // and dependent on the pileup reweighting for MC.
        event_weight = 1.;
        if (!is_real_data) {
            SetMCEventWeight(iEvent);
            if (lumi_weights_ != nullptr
                && lumi_weights_plus_ != nullptr
                && lumi_weights_minus_ != nullptr
            ) {
                SetLumiEventWeight(iEvent);
            }
        }

        // Finish initialization of electrons
        InitReco(iEvent, iSetup);  // Data

        // Set up the MC, including saving some of the truth quantites to the
        // reco quantities in MC
        if (!is_real_data) {
            InitTruth(iEvent, iSetup);  // MC
            // In MC we want to store the value of the Truth phistar and Y with
            // the reco events, and vice versa, so that they may be used for
            // unfolding. We also store the MC naked phistar and born phistar
            // in the reco.
            if (truth_z.m != -1) {  // Good truth Z
                reco_z.other_phistar = truth_z.phistar;
                reco_z.other_y = truth_z.y;
                reco_z.bornPhistar = truth_z.bornPhistar;
                reco_z.nakedPhistar = truth_z.nakedPhistar;
            }
            if (reco_z.m != -1) {  // Good reco Z
                truth_z.other_phistar = reco_z.phistar;
                truth_z.other_y = reco_z.y;
            }
            // Gen Z check
            if (require_gen_z_ and truth_z.m == -1) {
                // We set the electrons to nullptr and the z mass to -1, which
                // mark the event as bad
                set_both_e_truth(nullptr, nullptr);
                set_both_e(nullptr, nullptr);
                truth_z.m = -1;
                reco_z.m = -1;
                return;
            }
        }
        InitTrigger(iEvent, iSetup);  // Trigger Matching
    }

    void ZFinderEvent::SetLumiEventWeight(const edm::Event& iEvent) {
        /* Reweight the event to correct for pileup (but only MC). This recipe
         * is give on the Twiki:
         * https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupMCReweightingUtilities
         */
        edm::Handle<std::vector<PileupSummaryInfo> > pileup_info;
        iEvent.getByLabel(inputtags_.pileup, pileup_info);

        // Must be a float because weight() below takes float or int
        float true_number_of_pileup = -1.;
        std::vector<PileupSummaryInfo>::const_iterator PILEUP_ELEMENT;
        for(PILEUP_ELEMENT = pileup_info->begin(); PILEUP_ELEMENT != pileup_info->end(); ++PILEUP_ELEMENT) {
            const int BUNCH_CROSSING = PILEUP_ELEMENT->getBunchCrossing();
            if (BUNCH_CROSSING == 0) {
                true_number_of_pileup = PILEUP_ELEMENT->getTrueNumInteractions();
                break;
            }
        }
        truth_vert.true_num = true_number_of_pileup;
        weight_vertex = lumi_weights_->weight(true_number_of_pileup);
        event_weight *= weight_vertex;

        // Set the weights to use for systematic uncertainty
        weight_vertex_plus = lumi_weights_plus_->weight(true_number_of_pileup);
        weight_vertex_minus = lumi_weights_minus_->weight(true_number_of_pileup);
    }

    void ZFinderEvent::SetMCEventWeight(const edm::Event& iEvent) {
        // Some MC is also weighted; multiply by this weight also
        edm::Handle<GenEventInfoProduct> gen_event_info;
        iEvent.getByLabel("generator", gen_event_info);
        weight_natural_mc = gen_event_info->weight();
        event_weight *= weight_natural_mc;

        edm::InputTag pdfWeightTag_cteq("pdfWeights:CT10"); // or any other PDF set
        edm::Handle<std::vector<double> > weightHandle_cteq;
        iEvent.getByLabel(pdfWeightTag_cteq, weightHandle_cteq);
        weights_cteq = (*weightHandle_cteq);

        edm::InputTag pdfWeightTag_mstw("pdfWeights:MSTW2008nlo68cl"); // or any other PDF set
        edm::Handle<std::vector<double> > weightHandle_mstw;
        iEvent.getByLabel(pdfWeightTag_mstw, weightHandle_mstw);
        weights_mstw = (*weightHandle_mstw);

        edm::InputTag pdfWeightTag_nnpdf("pdfWeights:NNPDF23"); // or any other PDF set
        edm::Handle<std::vector<double> > weightHandle_nnpdf;
        iEvent.getByLabel(pdfWeightTag_nnpdf, weightHandle_nnpdf);
        weights_nnpdf = (*weightHandle_nnpdf);

        edm::Handle<double > weightHandle_fsr;
        iEvent.getByLabel("fsrWeight", weightHandle_fsr);
        weight_fsr = (*weightHandle_fsr);

    }

    void ZFinderEvent::InitReco(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
        /* Count Pile Up and store first vertex location*/
        edm::Handle<reco::VertexCollection> reco_vertices;
        iEvent.getByLabel(inputtags_.vertex, reco_vertices);
        reco_vert.num = 0;
        bool first_vertex = true;
        for(unsigned int vertex=0; vertex < reco_vertices->size(); ++vertex) {
            if (    // Criteria copied from twiki
                    !((*reco_vertices)[vertex].isFake())
                    && ((*reco_vertices)[vertex].ndof() > 4)
                    && (fabs((*reco_vertices)[vertex].z()) <= 24.0)
                    && ((*reco_vertices)[vertex].position().Rho() <= 2.0)
               ) {
                reco_vert.num++;
                // Store first good vertex as "primary"
                if (first_vertex) {
                    first_vertex = false;
                    reco_vert.x = (*reco_vertices)[vertex].x();
                    reco_vert.y = (*reco_vertices)[vertex].y();
                    reco_vert.z = (*reco_vertices)[vertex].z();
                }
            }
        }


        /* Beamspot */
        edm::Handle<reco::BeamSpot> beam_spot;
        iEvent.getByLabel(inputtags_.beamspot, beam_spot);
        reco_bs.x = beam_spot->position().X();
        reco_bs.y = beam_spot->position().Y();
        reco_bs.z = beam_spot->position().Z();

        /* Find electrons */
        InitGSFElectrons(iEvent, iSetup);
        if (!use_muon_acceptance_) {
            // HF and NT electrons are NEVER in the muon acceptance
            InitHFElectrons(iEvent, iSetup);
            InitNTElectrons(iEvent, iSetup);
        }

        // Sort our electrons and set e0, e1 as the two with the highest pt
        std::sort(reco_electrons_.begin(), reco_electrons_.end(), SortByPTHighLow);

        // Set up the internal electron pointers and, if we have enough
        // electrons, use them to make a Z
        n_reco_electrons = reco_electrons_.size();
        if (n_reco_electrons == 1) {
            set_e0(reco_electrons_[0]);
        }
        else if (n_reco_electrons >= 2) {
            set_both_e(reco_electrons_[0], reco_electrons_[1]);
            // We can only apply the NT Bending correction if we already have
            // both electrons, since we need the sign of the tracked one to
            // infer the sign of the untracked electron.
            ApplyNTBendingCorrection();
            // Set up the Z
            InitZ();
        }
    }

    void ZFinderEvent::InitGSFElectrons(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
        // We split this part into a new function because it is very long
        // Most of this code is stolen from the example here:
        // http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/EGamma/EGammaAnalysisTools/src/EGammaCutBasedEleIdAnalyzer.cc?view=markup

        // electrons
        edm::Handle<reco::GsfElectronCollection> els_h;
        iEvent.getByLabel(inputtags_.ecal_electron, els_h);

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
        edm::Handle<double> rho_iso_h;
        iEvent.getByLabel(inputtags_.rho_iso, rho_iso_h);
        const double RHO_ISO = *(rho_iso_h.product());

        // loop on electrons
        for(unsigned int i = 0; i < els_h->size(); ++i) {
            // Get the electron and set put it into the electrons vector
            reco::GsfElectron electron = els_h->at(i);
            // We enforce a minimum quality cut
            if (electron.pt() < 20) {
                continue;
            }
            if (use_muon_acceptance_ && fabs(electron.eta()) > 2.4) {
                continue;
            }
            ZFinderElectron* zf_electron = AddRecoElectron(electron);

            // get reference to electron and the electron
            reco::GsfElectronRef ele_ref(els_h, i);

            // get particle flow isolation
            const double ISO_CH = (*(isoVals[0]))[ele_ref];
            const double ISO_EM = (*(isoVals[1]))[ele_ref];
            const double ISO_NH = (*(isoVals[2]))[ele_ref];

            // test ID
            // working points
            const bool VETO = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::VETO, ele_ref, conversions_h, beamSpot, vtx_h, ISO_CH, ISO_EM, ISO_NH, RHO_ISO);
            const bool LOOSE = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::LOOSE, ele_ref, conversions_h, beamSpot, vtx_h, ISO_CH, ISO_EM, ISO_NH, RHO_ISO);
            const bool MEDIUM = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::MEDIUM, ele_ref, conversions_h, beamSpot, vtx_h, ISO_CH, ISO_EM, ISO_NH, RHO_ISO);
            const bool TIGHT = EgammaCutBasedEleId::PassWP(EgammaCutBasedEleId::TIGHT, ele_ref, conversions_h, beamSpot, vtx_h, ISO_CH, ISO_EM, ISO_NH, RHO_ISO);

            // eop/fbrem cuts for extra tight ID
            const bool FBREMEOPIN = EgammaCutBasedEleId::PassEoverPCuts(ele_ref);

            // cuts to match tight trigger requirements
            const bool TRIGTIGHT = EgammaCutBasedEleId::PassTriggerCuts(EgammaCutBasedEleId::TRIGGERTIGHT, ele_ref);

            // for 2011 WP70 trigger
            const bool TRIGWP70 = EgammaCutBasedEleId::PassTriggerCuts(EgammaCutBasedEleId::TRIGGERWP70, ele_ref);

            // Add the cuts to our electron
            const double WEIGHT = 1.;
            zf_electron->AddCutResult("eg_veto", VETO, WEIGHT);
            zf_electron->AddCutResult("eg_loose", LOOSE, WEIGHT);
            zf_electron->AddCutResult("eg_medium", MEDIUM, WEIGHT);
            zf_electron->AddCutResult("eg_tight", TIGHT, WEIGHT);
            zf_electron->AddCutResult("eg_eop_cut", FBREMEOPIN, WEIGHT);
            zf_electron->AddCutResult("eg_trigtight", TRIGTIGHT, WEIGHT);
            zf_electron->AddCutResult("eg_trigwp70", TRIGWP70, WEIGHT);

            // Check for trigger matching
            const bool EE_TIGHT = TriggerMatch(iEvent, ET_ET_TIGHT, zf_electron->eta(), zf_electron->phi(), TRIG_DR_);
            const bool EE_LOOSE = TriggerMatch(iEvent, ET_ET_LOOSE, zf_electron->eta(), zf_electron->phi(), TRIG_DR_);
            const bool EE_DZ = TriggerMatch(iEvent, ET_ET_DZ, zf_electron->eta(), zf_electron->phi(), TRIG_DR_);
            const bool EENT_TIGHT = TriggerMatch(iEvent, ET_NT_ET_TIGHT, zf_electron->eta(), zf_electron->phi(), TRIG_DR_);
            const bool EEHF_TIGHT = EENT_TIGHT;
            const bool EEHF_LOOSE = TriggerMatch(iEvent, ET_HF_ET_LOOSE, zf_electron->eta(), zf_electron->phi(), TRIG_DR_);
            const bool SINGLE_E = TriggerMatch(iEvent, SINGLE_ELECTRON_TRIGGER, zf_electron->eta(), zf_electron->phi(), TRIG_DR_);

            zf_electron->AddCutResult("trig(et_et_tight)", EE_TIGHT, WEIGHT);
            zf_electron->AddCutResult("trig(et_et_loose)", EE_LOOSE, WEIGHT);
            zf_electron->AddCutResult("trig(et_et_dz)", EE_DZ, WEIGHT);
            zf_electron->AddCutResult("trig(et_nt_etleg)", EENT_TIGHT, WEIGHT);
            zf_electron->AddCutResult("trig(et_hf_tight)", EEHF_TIGHT, WEIGHT);
            zf_electron->AddCutResult("trig(et_hf_loose)", EEHF_LOOSE, WEIGHT);
            zf_electron->AddCutResult("trig(single_ele)", SINGLE_E, WEIGHT);
        }
    }

    void ZFinderEvent::InitHFElectrons(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
        // HF Electrons
        edm::Handle<reco::RecoEcalCandidateCollection> els_h;
        iEvent.getByLabel(inputtags_.hf_electron, els_h);
        // HF Superclusters
        edm::Handle<reco::SuperClusterCollection> scs_h;
        iEvent.getByLabel(inputtags_.hf_clusters, scs_h);
        edm::Handle<reco::HFEMClusterShapeAssociationCollection> scas_h;
        iEvent.getByLabel(inputtags_.hf_clusters, scas_h);

        // Loop over electrons
        for(unsigned int i = 0; i < els_h->size(); ++i) {
            // Get the electron and set put it into the electrons vector
            reco::RecoEcalCandidate electron = els_h->at(i);
            // We enforce a minimum quality cut
            if (electron.pt() < 20) {
                continue;
            }
            ZFinderElectron* zf_electron = AddRecoElectron(electron);

            reco::SuperClusterRef cluster_ref = electron.superCluster();
            const reco::HFEMClusterShapeRef CLUSTER_SHAPE_REF = scas_h->find(cluster_ref)->val;
            const reco::HFEMClusterShape& CLUSTER_SHAPE = *CLUSTER_SHAPE_REF;

            const double ECE9 = CLUSTER_SHAPE.eCOREe9();
            const double ESEL = CLUSTER_SHAPE.eSeL();
            const double E9E25 = (CLUSTER_SHAPE.eLong3x3() * 1.0 / CLUSTER_SHAPE.eLong5x5());

            // e9e25 cut
            const bool PASS_E9E25 = (E9E25 > 0.94);

            // HF Tight (as defined in hfRecoEcalCandidate_cfi.py in ZShape)
            const double TIGHT2D = (ECE9 - (ESEL * 0.20));
            const bool HFTIGHT = (TIGHT2D > 0.92);

            // HF Medium
            const double MEDIUM2D = (ECE9 - (ESEL * 0.275));
            const bool HFMEDIUM = (MEDIUM2D > 0.875);

            // HF Loose
            const double LOOSE2D = (ECE9 - (ESEL * 0.475));
            const bool HFLOOSE = (LOOSE2D > 0.815);

            // Add the cuts to our electron
            const double WEIGHT = 1.;
            zf_electron->AddCutResult("hf_e9e25", PASS_E9E25, WEIGHT);
            zf_electron->AddCutResult("hf_2dtight", HFTIGHT, WEIGHT);
            zf_electron->AddCutResult("hf_2dmedium", HFMEDIUM, WEIGHT);
            zf_electron->AddCutResult("hf_2dloose", HFLOOSE, WEIGHT);

            // Check for trigger matching
            const bool HIGHLOW_03 = TriggerMatch(iEvent, ET_HF_HF_LOOSE, zf_electron->eta(), zf_electron->phi(), TRIG_DR_);
            zf_electron->AddCutResult("trig(hf_loose)", HIGHLOW_03, WEIGHT);

            const bool LOWHIGH_03 = TriggerMatch(iEvent, ET_HF_HF_TIGHT, zf_electron->eta(), zf_electron->phi(), TRIG_DR_);
            zf_electron->AddCutResult("trig(hf_tight)", LOWHIGH_03, WEIGHT);
        }
    }

    void ZFinderEvent::InitNTElectrons(const edm::Event& iEvent, const edm::EventSetup& iSetup) {

        // NT Electrons
        edm::Handle<reco::PhotonCollection> els_h;
        iEvent.getByLabel(inputtags_.nt_electron, els_h);

        // Loop over all electrons
        for(unsigned int i = 0; i < els_h->size(); ++i) {
            reco::Photon electron = els_h->at(i);
            // We enforce a minimum quality cut
            if (electron.pt() < 20) {
                continue;
            }
            // Because the photon collect is NOT filtered for electrons, we
            // reject photons that are too close to GSF electrons, and only
            // accept photons within 2.5 < |eta| < 2.850.
            bool is_unmatched = true;
            if (2.5 < fabs(electron.eta()) && fabs(electron.eta()) < 2.850) {
                ZFinderElectron* zf_electron;
                // Now check for nearby GSF electrons
                for(auto& i_elec : reco_electrons_) {
                    if(i_elec->CutPassed("type_gsf") == 1) {
                        const double DR = deltaR(i_elec->eta(), i_elec->phi(), electron.eta(), electron.phi());
                        if(DR <= NT_DR_) {
                            is_unmatched = false;
                            break;
                        }
                    }
                }
                if(is_unmatched) {
                    zf_electron = AddRecoElectron(electron);
                }
                else {
                    continue;  // Check the next photon
                }

                // Apply Alexey's Cuts
                //const double PHOTON_ET = electron.superCluster()->rawEnergy() * sin(electron.superCluster()->theta());
                if (       0.89 < electron.r9() && electron.r9() < 1.02
                        && electron.hadronicOverEm() < 0.05
                        && fabs(electron.superCluster()->eta()) > 2.5
                        //&& PHOTON_ET > 20.
                        && electron.sigmaIetaIeta() < 0.029
                        && (electron.ecalRecHitSumEtConeDR03() / electron.pt()) < 0.035
                        && (electron.hcalTowerSumEtConeDR03() / electron.pt()) < 0.11
                   ) {
                    const bool PASSED = true;
                    const double WEIGHT = 1.;
                    zf_electron->AddCutResult("nt_loose", PASSED, WEIGHT);
                }
                else {
                    zf_electron->AddCutResult("nt_loose", false, 1.);
                }
                //diagnostic printout:
                //else std::cout<<"NT loose Failed!"<<std::endl;

                // Check for trigger matching
                // HLT_Ele27_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele15_CaloIdT_CaloIsoVL_trackless_v8
            }
        }
    }

    void ZFinderEvent::InitZ() {
        if (e0 != nullptr && e1 != nullptr) {
            // Sometimes we want to preselect our electrons using the muon acceptance
            if (use_muon_acceptance_) {
                const double FETA0 = fabs(e0->eta());
                const double FETA1 = fabs(e1->eta());
                // Both electrons have already passed the looser pt>20 and
                // |eta|<2.4 selection, so we just need one that passes the
                // tighter pt>30 and |eta|<2.1 selection
                if (
                    !(
                        (FETA0 < 2.1 && e0->pt() > 30)
                        || (FETA1 < 2.1 && e1->pt() > 30)
                    )
                ) {
                    return;
                }
            }

            // Set Z properties
            const double ELECTRON_MASS = 5.109989e-4;
            math::PtEtaPhiMLorentzVector e0lv(e0->pt(), e0->eta(), e0->phi(), ELECTRON_MASS);
            math::PtEtaPhiMLorentzVector e1lv(e1->pt(), e1->eta(), e1->phi(), ELECTRON_MASS);
            math::PtEtaPhiMLorentzVector zlv;
            zlv = e0lv + e1lv;

            reco_z.m = zlv.mass();
            reco_z.y = zlv.Rapidity();
            reco_z.pt = zlv.pt();
            reco_z.phistar = ReturnPhistar(e0->eta(), e0->phi(), e1->eta(), e1->phi());
            reco_z.eta = zlv.eta();
            reco_z.deltaR = deltaR(e0->eta(), e0->phi(), e1->eta(), e1->phi());

            // Set the SC phi*
            // sc_eta and sec_phi are set to -10 to indicate they don't exist
            // for an object
            const bool E0_GOOD = e0->sc_eta() >= -9 && e0->sc_phi() >= -9;
            const bool E1_GOOD = e1->sc_eta() >= -9 && e1->sc_phi() >= -9;
            if (E0_GOOD && E1_GOOD) {
                reco_z.scPhistar = ReturnPhistar(e0->sc_eta(), e0->sc_phi(), e1->sc_eta(), e1->sc_phi());
            }
        }
    }

    void ZFinderEvent::InitVariables() {
        // Beamspot
        reco_bs.x = -1000;
        reco_bs.y = -1000;
        reco_bs.z = -1000;

        // Vertexes
        reco_vert.num = -1;
        reco_vert.true_num = -1;
        reco_vert.x = -1000;
        reco_vert.y = -1000;
        reco_vert.z = -1000;
        truth_vert.num = -1;
        truth_vert.true_num = -1;
        truth_vert.x = -1000;
        truth_vert.y = -1000;
        truth_vert.z = -1000;

        // Event ID
        id.run_num = 0;
        id.lumi_num = 0;
        id.event_num = 0;

        // Z Data
        reco_z.m = -1;
        reco_z.y = -1000;
        reco_z.pt = -1;
        reco_z.phistar = -1;
        reco_z.bornPhistar = -1;
        reco_z.nakedPhistar = -1;
        reco_z.scPhistar = -1;
        reco_z.eta = -1000;
        reco_z.deltaR = -1;
        reco_z.other_y = -1000;
        reco_z.other_phistar = -1;

        truth_z.m = -1;
        truth_z.y = -1000;
        truth_z.pt = -1;
        truth_z.phistar = -1;
        truth_z.bornPhistar = -1;
        truth_z.nakedPhistar = -1;
        truth_z.scPhistar = -1;
        truth_z.eta = -1000;
        truth_z.deltaR = -1;
        truth_z.other_y = -1000;
        truth_z.other_phistar = -1;

        // Electrons
        e0 = nullptr;
        e1 = nullptr;
        n_reco_electrons = -1;
        e0_truth = nullptr;
        e1_truth = nullptr;
        e0_trig = nullptr;
        e1_trig = nullptr;

        // Is Data
        is_real_data = false;

        // Weights
        event_weight = 1;
        weight_fsr = 1;
        weight_vertex = 1;
        weight_vertex_plus = 1;
        weight_vertex_minus = 1;
        weight_natural_mc = 1;
    }

    void ZFinderEvent::InitTruth(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
        /* Count Pile Up */
        edm::Handle<std::vector<PileupSummaryInfo> > pileup_info;
        iEvent.getByLabel(inputtags_.pileup, pileup_info);
        // Loop over the pileup info and take the number of pileup events from
        // the 0th bunch crossing
        std::vector<PileupSummaryInfo>::const_iterator PILEUP_ELEMENT;
        if (pileup_info.isValid()) {
            for (PILEUP_ELEMENT = pileup_info->begin(); PILEUP_ELEMENT != pileup_info->end(); ++PILEUP_ELEMENT) {
                const int BUNCH_CROSSING = PILEUP_ELEMENT->getBunchCrossing();
                if (BUNCH_CROSSING == 0) {
                    // The main vertex is counted as well, so add +1
                    truth_vert.num = PILEUP_ELEMENT->getPU_NumInteractions() + 1;
                }
            }
        }
        else {
            truth_vert.num = -1;
        }

        /*
         * We don't need to select electrons with cuts, because in Monte Carlo we
         * can just ask for the Z.
         */
        edm::Handle<reco::GenParticleCollection> mc_particles;
        iEvent.getByLabel(inputtags_.generator, mc_particles);

        /* Finding the Z and daughter electrons
         *
         * We loop over all gen particles. If it is a Z, we check its daughters
         * until we find an electron, then we know that it is a Z->ee decay. If
         * this is the first Z we save it. If the particle is an electron, we
         * make sure it came from a Z. This might have problems in ZZ->eeee
         * decays, but we expect those to be impossibly rare.
         */
        //default electrons--which are now DRESSED!
        const reco::GenParticle* electron_0 = nullptr;
        const reco::GenParticle* electron_1 = nullptr;
        //born electrons:
        const reco::GenParticle* bornElectron_0 = nullptr;
        const reco::GenParticle* bornElectron_1 = nullptr;
        //naked (final) electrons:
        const reco::GenParticle* nakedElectron_0 = nullptr;
        const reco::GenParticle* nakedElectron_1 = nullptr;
        const reco::GenParticle* z_boson = nullptr;

        for (unsigned int i = 0; i < mc_particles->size(); ++i) {
            const reco::GenParticle* gen_particle = &mc_particles->at(i);
            // Is a Z
            if (gen_particle->pdgId() == PDGID::ZBOSON && z_boson == nullptr) {
                for (size_t j = 0; j < gen_particle->numberOfDaughters(); ++j) {
                    if (gen_particle->daughter(j)->pdgId() == PDGID::ELECTRON) {
                        z_boson = gen_particle;
                        break;
                    }
                }
            }
            // Is an electron
            else if (fabs(gen_particle->pdgId()) == PDGID::ELECTRON  // In pdgId, fabs(POSITRON) == ELECTRON
                    && (electron_0 == nullptr || electron_1 == nullptr)
                    ) {
                for (size_t j = 0; j < gen_particle->numberOfMothers(); ++j) {
                    if (gen_particle->mother(j)->pdgId() == PDGID::ZBOSON && gen_particle->status() == 3) {
                        // If we haven't filled the first electron
                        if (bornElectron_0 == nullptr) {
                            //WARNING: this funciton uses a POINTER to a
                            //POINTER to make gen_particle point at the NAKED
                            //electron at the end of the decay chain
                            bornElectron_0 = gen_particle;
                            nakedElectron_0 = GetNakedElectron(bornElectron_0);

                            //now we DRESS it:
                            if (bornElectron_0 && nakedElectron_0) {
                                electron_0 = GetDressedElectron(bornElectron_0, nakedElectron_0);
                            }
                        }
                        // We have filled the first electron, so fill the second
                        else {
                            bornElectron_1 = gen_particle;
                            nakedElectron_1 = GetNakedElectron(bornElectron_1);
                            //now we DRESS it:
                            if (bornElectron_1 && nakedElectron_1) {
                                electron_1 = GetDressedElectron(bornElectron_1, nakedElectron_1);
                            }
                        }
                    }
                }
            }
            // If we've found all our particles, exit the loop
            if (z_boson != nullptr && electron_0 != nullptr && electron_1 != nullptr) {
                break;
            }
        }

        // Continue only if all particles have been found
        if (z_boson != nullptr && electron_0 != nullptr && electron_1 != nullptr) {
            // We set electron_0 to the higher pt electron
            if (electron_0->pt() < electron_1->pt()) {
                std::swap(electron_0, electron_1);
                std::swap(nakedElectron_0, nakedElectron_1);
                std::swap(bornElectron_0, bornElectron_1);
            }

            // Add electrons to the ZFEvent
            ZFinderElectron* zf_electron_0 = AddTruthElectron(*bornElectron_0, *electron_0, *nakedElectron_0);
            set_e0_truth(zf_electron_0);
            ZFinderElectron* zf_electron_1 = AddTruthElectron(*bornElectron_1, *electron_1, *nakedElectron_1);
            set_e1_truth(zf_electron_1);

            // Z Properties
            truth_z.m = z_boson->mass();
            truth_z.pt = z_boson->pt();
            const double ZEPP = z_boson->energy() + z_boson->pz();
            const double ZEMP = z_boson->energy() - z_boson->pz();
            truth_z.y = 0.5 * log(ZEPP / ZEMP);
            truth_z.phistar = ReturnPhistar(e0_truth->eta(), e0_truth->phi(), e1_truth->eta(), e1_truth->phi());
            truth_z.bornPhistar = ReturnPhistar(e0_truth->bornEta(), e0_truth->bornPhi(), e1_truth->bornEta(), e1_truth->bornPhi());
            truth_z.nakedPhistar = ReturnPhistar(e0_truth->nakedEta(), e0_truth->nakedPhi(), e1_truth->nakedEta(), e1_truth->nakedPhi());
            truth_z.eta = z_boson->eta();
            truth_z.deltaR = deltaR(e0_truth->eta(), e0_truth->phi(), e1_truth->eta(), e1_truth->phi());
        }
    }

    void ZFinderEvent::InitTrigger(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
        // Get the trigger objects that are closest in dR to our reco electrons
        if (e0 != nullptr && e1 != nullptr) {
            const trigger::TriggerObject* trig_obj_0 = GetBestMatchedTriggerObject(iEvent, ALL_TRIGGERS, e0->eta(), e0->phi());
            const trigger::TriggerObject* trig_obj_1 = GetBestMatchedTriggerObject(iEvent, ALL_TRIGGERS, e1->eta(), e1->phi());

            // If the electrons are good, set them as our trigger electrons
            if (trig_obj_0 != nullptr) {
                ZFinderElectron* tmp_e0 = AddHLTElectron(*trig_obj_0);
                set_e0_trig(tmp_e0);
            }
            if (trig_obj_1 != nullptr) {
                ZFinderElectron* tmp_e1 = AddHLTElectron(*trig_obj_1);
                set_e1_trig(tmp_e1);
            }
        }
    }

    ZFinderElectron* ZFinderEvent::AddRecoElectron(reco::GsfElectron electron) {
        ZFinderElectron* zf_electron = new ZFinderElectron(electron);
        reco_electrons_.push_back(zf_electron);
        return zf_electron;
    }

    ZFinderElectron* ZFinderEvent::AddRecoElectron(reco::RecoEcalCandidate electron) {
        ZFinderElectron* zf_electron = new ZFinderElectron(electron);
        reco_electrons_.push_back(zf_electron);
        return zf_electron;
    }

    ZFinderElectron* ZFinderEvent::AddRecoElectron(reco::Photon electron) {
        ZFinderElectron* zf_electron = new ZFinderElectron(electron);
        reco_electrons_.push_back(zf_electron);
        return zf_electron;
    }

    ZFinderElectron* ZFinderEvent::AddTruthElectron(reco::GenParticle electron) {
        ZFinderElectron* zf_electron = new ZFinderElectron(electron);
        truth_electrons_.push_back(zf_electron);
        return zf_electron;
    }
    //the trifecto version:
    ZFinderElectron* ZFinderEvent::AddTruthElectron(reco::GenParticle bornElectron,
                                                    reco::GenParticle dressedElectron,
                                                    reco::GenParticle nakedElectron) {
        ZFinderElectron* zf_electron = new ZFinderElectron(bornElectron,dressedElectron,nakedElectron);
        truth_electrons_.push_back(zf_electron);
        return zf_electron;
    }

    ZFinderElectron* ZFinderEvent::AddHLTElectron(trigger::TriggerObject electron) {
        ZFinderElectron* zf_electron = new ZFinderElectron(electron);
        hlt_electrons_.push_back(zf_electron);
        return zf_electron;
    }

    const reco::GenParticle* ZFinderEvent::GetNakedElectron(const reco::GenParticle * const BORN_ELECTRON) {
        const reco::GenParticle* naked_electron = BORN_ELECTRON;
        // We walk down the tree of decays, grabbing the electron in the decay
        // each time until we come to a"status() == 1" electron, meaning it is
        // stable and will no longer FSR.
        while (naked_electron->status() != 1) {
            // For some reason there are no daughters, but the particle is
            // "unstable". Abort and return nullptr.
            if (naked_electron->numberOfDaughters() == 0) {
                return nullptr;
            }
            // Otherwise look through the daughters and find an electron
            for (size_t i = 0; i < naked_electron->numberOfDaughters(); ++i) {
                const reco::Candidate* test_particle = naked_electron->daughter(i);
                if (fabs(test_particle->pdgId()) == ELECTRON) {
                    naked_electron = dynamic_cast<const reco::GenParticle*>(test_particle);
                    break;
                }
            }
        }

        return naked_electron;
    }

    const reco::GenParticle* ZFinderEvent::GetDressedElectron(
            const reco::GenParticle * const BORN_ELECTRON,
            const reco::GenParticle * const NAKED_ELECTRON,
            const double MAX_DELTA_R
            ) {
        const double ELECTRON_MASS = 5.109989e-4;
        // Make a 4 vector for the dressed electron
        math::PtEtaPhiMLorentzVector dressed_p4(NAKED_ELECTRON->pt(), NAKED_ELECTRON->eta(), NAKED_ELECTRON->phi(), ELECTRON_MASS);

        // Dive down the decay tree from the born electron until we hit the
        // naked electron, saving all the photons and summing them if they are
        // within DeltaR of 0.1 of the naked electon.
        const reco::GenParticle* tmp_electron = BORN_ELECTRON;
        while (tmp_electron != NAKED_ELECTRON) {
            // For some reason there are no daughters, but the particle is
            // "unstable". Abort and return nullptr.
            if (tmp_electron->numberOfDaughters() == 0) {
                return nullptr;
            }
            // Otherwise look through the daughters and find an electron
            const reco::GenParticle* swap_electron = nullptr;
            for (size_t i = 0; i < tmp_electron->numberOfDaughters(); ++i) {
                const reco::Candidate* test_particle = tmp_electron->daughter(i);
                // If we find electron, we save it as the next item to recurse over
                if (fabs(test_particle->pdgId()) == ELECTRON) {
                    swap_electron = dynamic_cast<const reco::GenParticle*>(test_particle);
                }
                // If we find a photon, add its 4 vector if it is within some
                // distance of the naked electron
                else if (fabs(test_particle->pdgId()) == PHOTON) {
                    const double DELTA_R = deltaR(test_particle->eta(), test_particle->phi(), NAKED_ELECTRON->eta(), NAKED_ELECTRON->phi());
                    if (DELTA_R < MAX_DELTA_R) {
                        dressed_p4 += math::PtEtaPhiMLorentzVector(
                                test_particle->pt(),
                                test_particle->eta(),
                                test_particle->phi(),
                                ELECTRON_MASS
                                );
                    }
                }
            }
            // Now that we done searching this level of the decay tree, move to
            // the next
            if (swap_electron) {
                tmp_electron = swap_electron;
            }
        }

        // Make a GenParticle from the vector and return it
        reco::GenParticle* dressed_e = new reco::GenParticle(
                NAKED_ELECTRON->charge(),
                dressed_p4,
                NAKED_ELECTRON->vertex(),
                NAKED_ELECTRON->pdgId(),
                NAKED_ELECTRON->status(),
                1
            );

        return dressed_e;
    }

    double ZFinderEvent::ReturnPhistar(const double& eta0, const double& phi0, const double& eta1, const double& phi1) {
        /* Calculate phi star */
        static const double PI = 3.14159265358979323846;
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

    void ZFinderEvent::PrintCuts(ZFinderElectron* zf_elec) {
        using std::cout;
        using std::endl;
        // Print all the cuts of the given zf_elec
        for (auto& i_cut : *zf_elec->GetAllCuts()) {
            cout << "\t\t" << i_cut->name << ": pass " << i_cut->passed << " weight " << i_cut->weight << endl;
        }
    }

    void ZFinderEvent::PrintElectrons(const int TYPE, const bool PRINT_CUTS) {
        using std::cout;
        using std::endl;

        enum ETYPE {
            RECO = 0,
            TRUTH = 1,
            TRIG = 2
        };
        /*
         * Loops over the electrons, and prints out the information about them.
         */
        cout << "Run " << id.run_num;
        cout << " event " << id.event_num;
        if (TYPE == RECO) {
            cout << " Reco Z Mass " << reco_z.m << std::endl;
            for (auto& i_elec : reco_electrons_) {
                cout << "\tpt: " << i_elec->pt();
                cout << " eta: " << i_elec->eta();
                cout << " phi: " << i_elec->phi() << endl;
                if (PRINT_CUTS) { PrintCuts(i_elec); }
            }
        } else if (TYPE == TRUTH && !is_real_data) {
            if (e0_truth != nullptr && e1_truth != nullptr) {
                cout << " Truth Z Mass " << truth_z.m << endl;
                cout << "\tpt: " << e0_truth->pt();
                cout << " eta: " << e0_truth->eta();
                cout << " phi: " << e0_truth->phi() << endl;
                if (PRINT_CUTS) { PrintCuts(e0_truth); }
                cout << "\tpt: " << e1_truth->pt();
                cout << " eta: " << e1_truth->eta();
                cout << " phi: " << e1_truth->phi() << endl;
                if (PRINT_CUTS) { PrintCuts(e1_truth); }
            }
        } else if (TYPE == TRIG) {
            if (e0_trig != nullptr || e1_trig != nullptr) {
                cout << " Trigger Electrons:" << std::endl;
            }
            if (e0_trig != nullptr) {
                cout << "\tpt: " << e0_trig->pt();
                cout << " eta: " << e0_trig->eta();
                cout << " phi: " << e0_trig->phi() << endl;
                if (PRINT_CUTS) { PrintCuts(e0_trig); }
            }
            if (e1_trig != nullptr) {
                cout << "\tpt: " << e1_trig->pt();
                cout << " eta: " << e1_trig->eta();
                cout << " phi: " << e1_trig->phi() << endl;
                if (PRINT_CUTS) { PrintCuts(e1_trig); }
            }
        }
    }

    std::vector<ZFinderElectron*>* ZFinderEvent::FilteredElectrons() {
        /*
         * Return all electrons
         */
        std::vector<ZFinderElectron*>* tmp_vec = new std::vector<ZFinderElectron*>();
        for (auto& i_elec : reco_electrons_) {
            tmp_vec->push_back(i_elec);
        }

        return tmp_vec;
    }

    std::vector<ZFinderElectron*>* ZFinderEvent::FilteredElectrons(const std::string& cut_name) {
        /*
         * Return all electrons that pass a specified cut
         */
        std::vector< ZFinderElectron*>* tmp_vec = new std::vector< ZFinderElectron*>();
        for (auto& i_elec : reco_electrons_) {
            if (i_elec->CutPassed(cut_name)) {
                tmp_vec->push_back(i_elec);
            }
        }

        return tmp_vec;
    }

    bool ZFinderEvent::ZDefPassed(const std::string& NAME) const {
        /*
         * Try to find the ZDef name in the map, if it exists return the pass
         * value, else return false.
         */
        std::map<std::string, cutlevel_vector>::const_iterator it = zdef_map_.find(NAME);
        if (it != zdef_map_.end()) {
            const cutlevel_vector* cuts_vec = &it->second;
            bool has_passed = true;
            for (auto& v_it : *cuts_vec) {
                has_passed = v_it.second.pass && has_passed;
            }
            return has_passed;
        } else {
            return false;
        }
    }

    void ZFinderEvent::PrintZDefs(const bool VERBOSE) const {
        /*
         * Loop over all ZDefs and print the results.
         */
        using std::cout;
        using std::endl;
        cout << "ZDefinitions:" << endl;
        for (auto& i_map : zdef_map_) {
            cout << "\t" << i_map.first << ": ";
            cout << ZDefPassed(i_map.first) << endl;
            // If VERBOSE, print out each cutlevel as well
            if (VERBOSE) {
                const cutlevel_vector* clv = &i_map.second;

                for (auto& i_cutlevel : *clv) {
                    cout << "\t\t" << i_cutlevel.first << ": " << i_cutlevel.second.pass;
                    cout << "t0p1 " << i_cutlevel.second.t0p1_pass << ' ' << i_cutlevel.second.t0p1_eff;
                    cout << "t1p0 " << i_cutlevel.second.t1p0_pass << ' ' << i_cutlevel.second.t1p0_eff << endl;
                }
            }
        }
    }

    const cutlevel_vector* ZFinderEvent::GetZDef(const std::string& NAME) const {
        std::map<std::string, cutlevel_vector>::const_iterator it = zdef_map_.find(NAME);
        if (it != zdef_map_.end()) {
            return &(it->second);
        } else {
            return nullptr;
        }
    }

    const trig_dr_vec* ZFinderEvent::GetMatchedTriggerObjects(
            const edm::Event& iEvent,
            const std::vector<std::string>& trig_names,
            const double ETA, const double PHI, const double DR_CUT
            ) {
        /*
         * Find all trigger objects that match a vector of trigger names and
         * are within some minimum dR of a specified eta and phi. Return them
         * as a vector of pairs of the object, and the dr.
         */
        // If our vector is empty or the first item is blank
        if (trig_names.size() == 0 || trig_names[0].size() == 0) {
            return nullptr;
        }

        // Load Trigger Objects
        edm::InputTag hltTrigInfoTag("hltTriggerSummaryAOD","","HLT");
        edm::Handle<trigger::TriggerEvent> trig_event;

        iEvent.getByLabel(hltTrigInfoTag, trig_event);
        if (!trig_event.isValid() ){
            std::cout << "No valid hltTriggerSummaryAOD." << std::endl;
            return nullptr;
        }

        trig_dr_vec* out_v = new trig_dr_vec();
        // Loop over triggers, filter the objects from these triggers, and then try to match
        for (auto& trig_name : trig_names) {
            // Loop over triggers, filter the objects from these triggers, and then try to match
            // Grab objects that pass our filter
            edm::InputTag filter_tag(trig_name, "", "HLT");
            trigger::size_type filter_index = trig_event->filterIndex(filter_tag);
            if(filter_index < trig_event->sizeFilters()) { // Check that the filter is in triggerEvent
                const trigger::Keys& trig_keys = trig_event->filterKeys(filter_index);
                const trigger::TriggerObjectCollection& trig_obj_collection(trig_event->getObjects());
                // Get the objects from the trigger keys
                for (auto& i_key : trig_keys) {
                    const trigger::TriggerObject* trig_obj = &trig_obj_collection[i_key];
                    const double DR = deltaR(ETA, PHI, trig_obj->eta(), trig_obj->phi());
                    // Do Delta R matching, and assign a new object if we have a
                    // better match
                    if (DR < DR_CUT) {
                        out_v->push_back(std::make_pair(trig_obj, DR));
                    }
                }
            }
        }
        return out_v;
    }

    const trigger::TriggerObject* ZFinderEvent::GetBestMatchedTriggerObject(
            const edm::Event& iEvent,
            const std::vector<std::string>& trig_names,
            const double ETA, const double PHI
            ) {
        /* Given the ETA and PHI of a particle, and a list of trigger paths,
         * returns the trigger object from those paths that is closest to the
         * given coordinates. */
        const double MIN_DR = 0.3;
        const trig_dr_vec* trig_vec = GetMatchedTriggerObjects(iEvent, trig_names, ETA, PHI, MIN_DR);

        double best_dr = 1.;
        const trigger::TriggerObject* trig_obj = nullptr;
        for (auto& i_obj : *trig_vec) {
            if (i_obj.second < best_dr) {
                best_dr = i_obj.second;
                trig_obj = i_obj.first;
            }
        }
        return trig_obj;
    }

    bool ZFinderEvent::TriggerMatch(
            const edm::Event& iEvent,
            const std::vector<std::string>& trig_names,
            const double ETA, const double PHI, const double DR_CUT
            ) {
        // Get the vector and see if there are objects
        const trig_dr_vec* zev = GetMatchedTriggerObjects(iEvent, trig_names, ETA, PHI, DR_CUT);
        if (zev != nullptr && zev->size() >= 1) {
            return true;
        } else {
            return false;
        }
    }

    void ZFinderEvent::ApplyNTBendingCorrection(
            ) {
        // The electrons
        ZFinderElectron* electron_to_correct = nullptr;
        const ZFinderElectron* spectator_electron = nullptr;
        if (e0->CutPassed("type_photon") == 1) {
            electron_to_correct = e0;
            if (e1->CutPassed("type_gsf") == 1) {
                spectator_electron = e1;
            }
        }
        else if (e1->CutPassed("type_photon") == 1) {
            electron_to_correct = e1;
            if (e0->CutPassed("type_gsf") == 1) {
                spectator_electron = e0;
            }
        }

        // If we have an NT electron and a GSF electron, we apply the
        // correction. If we only have an NT electron, we mark that the
        // correction was no applied. If we have neither, we do nothing.
        if (electron_to_correct != nullptr) {
            const double WEIGHT = 1.0;
            // We can do the correction
            if (spectator_electron != nullptr) {
                //3.18 m to EE, 3.8 T field, and a factor of 10/3 for GeV/c
                const double B_FIELD = 3.8;  // Tesla
                const double DIST_TO_EE = 3.18;  // Distance to EE in meters

                const int Q_NT = -1 * spectator_electron->charge();//inferred NT electron charge
                const double NUMERIC_FACTOR = DIST_TO_EE * B_FIELD * 3 / 20;
                // 1/(10/3) is from GeV/c, and 1/2 is geometric
                const double ETA_NT = fabs(electron_to_correct->eta());//sign of eta is unimportant
                const double PT_NT = electron_to_correct->pt();
                const double ADDITIVE_CORRECTION =  Q_NT * NUMERIC_FACTOR / sinh( ETA_NT ) / PT_NT;

                electron_to_correct->set_phi(electron_to_correct->phi() + ADDITIVE_CORRECTION);
                electron_to_correct->AddCutResult("nt_corrected", true, WEIGHT);
                return;
            }
            // No GSF to use to correct
            else {
                electron_to_correct->AddCutResult("nt_corrected", false, WEIGHT);
                return;
            }
        }
    }

    ZFinderEvent::~ZFinderEvent() {
        // Clean up all the heap variables we have declared
        for (auto& i_elec : reco_electrons_) {
            delete i_elec;
        }
        for (auto& i_elec : hlt_electrons_) {
            delete i_elec;
        }
        for (auto& i_elec : truth_electrons_) {
            delete i_elec;
        }
    }
}  // namespace zf
