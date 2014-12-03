import FWCore.ParameterSet.Config as cms

process = cms.Process("TrigEff")

process.load("FWCore.MessageService.MessageLogger_cfi")
# Set up message output and logging
from FWCore.MessageService.MessageLogger_cfi import MessageLogger
process.MessageLogger = MessageLogger
process.MessageLogger.cerr.FwkReport.reportEvery = 100  # Report status ever 100 events

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# Output file
process.TFileService = cms.Service("TFileService",
        fileName = cms.string("test.root")
        )

# rho value for isolation
from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets  # the 4 references the rParam = 0.4
process.kt6PFJetsForIsolation = kt4PFJets.clone(
        rParam = 0.6,
        doRhoFastjet = True,
        Rho_EtaMax = cms.double(2.5)
        )

# Energy and calibrations for electrons
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_P_V42_AN3::All'

from ZFinder.Event.electron_regression_cfi import RandomNumberGeneratorService
# Set up random numbers (needed for the electron regression)
process.RandomNumberGeneratorService = RandomNumberGeneratorService


# Run on MC
IS_MC = False

if not IS_MC:
    process.source = cms.Source("PoolSource",
            fileNames = cms.untracked.vstring('file:/hdfs/cms/phedex/store/data/Run2012A/DoubleElectron/AOD/22Jan2013-v1/20000/003EC246-5E67-E211-B103-00259059642E.root')
            )

    from ZFinder.Event.electron_regression_cfi import CalibratedElectrons, ElectronEnergyRegressions
    # Energy Regression
    process.eleRegressionEnergy = ElectronEnergyRegressions
    # Energy Scale calibration
    process.CalibratedElectrons = CalibratedElectrons


else:
    process.source = cms.Source("PoolSource",
            fileNames = cms.untracked.vstring('file:/hdfs/cms/phedex/store/mc/Summer12_DR53X/DYToEE_M-20_CT10_TuneZ2star_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/0000/02894AF7-27F0-E111-8566-00259073E38A.root')
            )

    from ZFinder.Event.electron_regression_cfi import CalibratedElectrons_MC, ElectronEnergyRegressions_MC
    # Energy Regression
    process.eleRegressionEnergy = ElectronEnergyRegressions_MC
    # Energy Scale calibration
    process.CalibratedElectrons = CalibratedElectrons_MC

# Particle flow isolation
from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso
process.eleIsoSequence = setupPFElectronIso(process, 'CalibratedElectrons:calibratedGsfElectrons')
process.pfiso = cms.Sequence(process.pfParticleSelectionSequence + process.eleIsoSequence)

# My analyzer
process.trigeff = cms.EDAnalyzer('TrigEff',
        ecalElectronsInputTag = cms.InputTag("CalibratedElectrons", "calibratedGsfElectrons"),
        )

process.p = cms.Path(
        process.kt6PFJetsForIsolation
        * process.eleRegressionEnergy
        * process.CalibratedElectrons
        * process.pfiso
        * process.trigeff
        )
