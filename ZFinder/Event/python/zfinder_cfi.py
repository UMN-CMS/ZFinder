import FWCore.ParameterSet.Config as cms

# Import ZDefinitions
from ZFinder.Event.zdefinitions_cfi import zdefs_extended_data

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
        ZDefinitions = zdefs_extended_data,
        # Run on MC or not. If true, every ZDefinition is included twice: once
        # plotting reco quantities, and once plotting generator quantities. If
        # false, only the reco quantities are plotted.
        is_mc = cms.bool(False),
        # Use the muon acceptance (pt>30 & |eta| < 2.1 + pt>20 & |eta|<2.4) to
        # select electrons to make into ZFinderElectrons. This means that only
        # these electrons will be considered to make Zs.
        use_muon_acceptance = cms.bool(False)
        )
