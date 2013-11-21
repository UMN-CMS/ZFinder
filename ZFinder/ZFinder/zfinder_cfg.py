import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:myfile.root'
    )
)

process.demo = cms.EDAnalyzer('ZFinder'
        # General tags
        electronsInputTag       = cms.InputTag("gsfElectrons"),
        conversionsInputTag     = cms.InputTag("allConversions"),
        beamSpotInputTag        = cms.InputTag("offlineBeamSpot"),
        rhoIsoInputTag          = cms.InputTag("kt6PFJetsForIsolation", "rho"),
        primaryVertexInputTag   = cms.InputTag("offlinePrimaryVertices"),
        isoValInputTags         = cms.VInputTag(
            cms.InputTag('elPFIsoValueCharged03PFIdPFIso'),
            cms.InputTag('elPFIsoValueGamma03PFIdPFIso'),
            cms.InputTag('elPFIsoValueNeutral03PFIdPFIso')
            ),
        # MC, but still required to be something for data
        pileupInputTag   = cms.InputTag("addPileupInfo"),
        generatorInputTag   = cms.InputTag("generator"),
        )

process.p = cms.Path(process.demo)
