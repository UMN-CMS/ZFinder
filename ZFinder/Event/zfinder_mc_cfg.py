import FWCore.ParameterSet.Config as cms

process = cms.Process("ZFinderMC")

# Set up message output and logging
from FWCore.MessageService.MessageLogger_cfi import MessageLogger
process.MessageLogger = MessageLogger
process.MessageLogger.cerr.FwkReport.reportEvery = 100  # Report status ever 100 events

# Number of events from each file to process. It should be -1 (all) when
# running for an analysis
N_EVENTS_TO_PROCESS = -1
if N_EVENTS_TO_PROCESS != -1:
    print "NOT RUNNING ON ALL EVENTS IN THE FILE!"
process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(N_EVENTS_TO_PROCESS)
        )

# Input file
process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring('file:/local/cms/phedex/store/mc/Summer12_DR53X/DYToEE_M-20_CT10_TuneZ2star_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/0000/AC6646E7-36F0-E111-B2F8-00259073E3FC.root')
        )

# Output file
process.TFileService = cms.Service("TFileService",
        fileName = cms.string("test_mc.root")
        )

# Import rho value for isolation correction
from ZFinder.Event.kt6_pfjets_cfi import kt6PFJetsForIsolation
process.kt6PFJetsForIsolation = kt6PFJetsForIsolation.clone()

# Particle flow isolation
from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso
process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons')
process.pfiso = cms.Sequence(process.pfParticleSelectionSequence + process.eleIsoSequence)

# ZFinder
from ZFinder.Event.zdefinitions_cfi import zdefs_mc
from ZFinder.Event.zfinder_cfi import ZFinder
process.ZFinder = ZFinder.clone(
        ZDefinitions = zdefs_mc,
        is_mc = cms.bool(True),
        )

# RUN
process.p = cms.Path(
        process.kt6PFJetsForIsolation
        * process.pfiso
        * process.ZFinder
        )
