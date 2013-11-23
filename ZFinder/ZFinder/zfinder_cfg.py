import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

# Set up message output and logging
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100  # Report status ever 100 events

# Number of events from each file to process. It should be -1 (all) when
# running for an analysis
N_EVENTS_TO_PROCESS = -1
if N_EVENTS_TO_PROCESS == -1:
    print "NOT RUNNING ON ALL EVENTS IN THE FILE!"
process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(N_EVENTS_TO_PROCESS)
        )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        'file:/local/cms/phedex/store/data/Run2012A/DoubleElectron/AOD/22Jan2013-v1/20000/F8C30E72-AE67-E211-A375-002618943922.root'
    )
)

process.TFileService = cms.Service("TFileService",
        fileName = cms.string("test.root")
        )

#
# rho value for isolation
#

from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets  # the 4 references the rParam = 0.4
process.kt6PFJetsForIsolation = kt4PFJets.clone( rParam = 0.6, doRhoFastjet = True )
process.kt6PFJetsForIsolation.Rho_EtaMax = cms.double(2.5)

#
# particle flow isolation
#

from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso, setupPFMuonIso
process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons')
process.pfiso = cms.Sequence(process.pfParticleSelectionSequence + process.eleIsoSequence)


process.ZFinder = cms.EDAnalyzer('ZFinder',
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

# RUN
process.p = cms.Path(process.kt6PFJetsForIsolation * process.pfiso * process.ZFinder)
process.schedule = cms.Schedule(process.p)
