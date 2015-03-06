import FWCore.ParameterSet.Config as cms

# Import ZDefinitions
from ZFinder.Event.zdefinitions_cfi import zdefs_combined_data

# ZFinder
ZFinder = cms.EDAnalyzer('ZFinder',
        # General tags
        ecalElectronsInputTag  = cms.InputTag("gsfElectrons"),
        hfElectronsInputTag    = cms.InputTag("hfRecoEcalCandidate"),
        hfClustersInputTag     = cms.InputTag("hfEMClusters"),
        conversionsInputTag    = cms.InputTag("allConversions"),
        beamSpotInputTag       = cms.InputTag("offlineBeamSpot"),
        rhoIsoInputTag         = cms.InputTag("kt6PFJetsForIsolation", "rho"),
        primaryVertexInputTag  = cms.InputTag("offlinePrimaryVertices"),
        ntElectronsInputTag    = cms.InputTag("photons"),
        isoValInputTags        = cms.VInputTag(
            cms.InputTag('elPFIsoValueCharged03PFIdPFIso'),
            cms.InputTag('elPFIsoValueGamma03PFIdPFIso'),
            cms.InputTag('elPFIsoValueNeutral03PFIdPFIso')
            ),
        # Used in MC, but still required to be something for data
        pileupInputTag = cms.InputTag("addPileupInfo"),
        generatorInputTag = cms.InputTag("genParticles"),
        # ZDefinitions from ZFinder.ZFinder.zdefinitions_cfi
        ZDefinitions = zdefs_combined_data,
        # Run on MC or not. If true, every ZDefinition is included twice: once
        # plotting reco quantities, and once plotting generator quantities. If
        # false, only the reco quantities are plotted.
        is_mc = cms.bool(False),
        # Use the muon acceptance (pt>30 & |eta| < 2.1 + pt>20 & |eta|<2.4) to
        # select electrons to make into ZFinderElectrons. This means that only
        # these electrons will be considered to make Zs.
        use_muon_acceptance = cms.bool(False),
        # Reject MC events that don't have a generator Z->ee event.
        require_gen_z = cms.bool(False),
        # Pileup Era to correct MC to. Valid values are A, B, C, D. Anything
        # else will default to the full 2012.
        pileup_era = cms.string("ABCD"),  # defaults to ABCD if not given
        # Turn on/off the PDF weight vectors in MC
        run_pdf_weights = cms.bool(False),
        # Turn on/off the FSR weight calculation in MC
        run_fsr_weight = cms.bool(False),
        )
