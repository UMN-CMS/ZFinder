import FWCore.ParameterSet.Config as cms

process = cms.Process("ExREG")
process.load("Configuration.StandardSequences.Services_cff")
process.load('Configuration.Geometry.GeometryDB_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.GlobalTag.globaltag = 'GR_P_V42_AN3::All'

# Set up message output and logging
from FWCore.MessageService.MessageLogger_cfi import MessageLogger
process.MessageLogger = MessageLogger
process.MessageLogger.cerr.FwkReport.reportEvery = 10  # Report status ever 100 events

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
        calibratedElectrons = cms.PSet(
            initialSeed = cms.untracked.uint32(1),
            engineName = cms.untracked.string('TRandom3')
            ),
        )

process.load("EgammaAnalysis.ElectronTools.calibratedElectrons_cfi")

# dataset to correct
process.calibratedElectrons.isMC = cms.bool(False)
process.calibratedElectrons.inputDataset = cms.string("22Jan2013ReReco")
process.calibratedElectrons.updateEnergyError = cms.bool(True)
process.calibratedElectrons.correctionsType = cms.int32(2)
process.calibratedElectrons.combinationType = cms.int32(3)
process.calibratedElectrons.lumiRatio = cms.double(1.0)
process.calibratedElectrons.verbose = cms.bool(False)
process.calibratedElectrons.synchronization = cms.bool(True)

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(-1)
        )

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring( *(
            #'/store/relval/CMSSW_5_3_4_cand1/RelValZEE/GEN-SIM-RECO/PU_START53_V10-v1/0003/0CBBC6C2-42F7-E111-B1C8-0030486780B4.root'
            #'/store/relval/CMSSW_5_3_6/RelValZEE/GEN-SIM-RECO/PU_START53_V14-v1/0003/2C92DB85-E82C-E211-B8DE-003048D37560.root'
            #'/store/data/Run2012D/DoubleElectron/AOD/PromptReco-v1/000/203/777/D82E96C2-240B-E211-AFF3-001D09F2905B.root'
            #'/store/data/Run2012A/DoubleElectron/AOD/13Jul2012-v1/00000/FEEE5F6A-26DA-E111-B08C-00266CFAE8D0.root'
            'file:/hdfs/cms/phedex/store/data/Run2012A/DoubleElectron/AOD/22Jan2013-v1/20000/003EC246-5E67-E211-B103-00259059642E.root',
            )
            ))

process.load('EgammaAnalysis.ElectronTools.electronRegressionEnergyProducer_cfi')
process.eleRegressionEnergy.inputElectronsTag = cms.InputTag('gsfElectrons')
process.eleRegressionEnergy.inputCollectionType = cms.uint32(0)
process.eleRegressionEnergy.useRecHitCollections = cms.bool(True)
process.eleRegressionEnergy.produceValueMaps = cms.bool(True)
process.eleRegressionEnergy.regressionInputFile = cms.string("EgammaAnalysis/ElectronTools/data/eleEnergyRegWeights_WithSubClusters_VApr15.root")
process.eleRegressionEnergy.energyRegressionType = cms.uint32(2)

# Import rho value for isolation correction
from ZFinder.Event.kt6_pfjets_cfi import kt6PFJetsForIsolation
process.kt6PFJetsForIsolation = kt6PFJetsForIsolation.clone()

# Particle flow isolation
from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso
#process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons')
process.eleIsoSequence = setupPFElectronIso(process, 'calibratedElectrons:calibratedGsfElectrons')
process.pfiso = cms.Sequence(process.pfParticleSelectionSequence + process.eleIsoSequence)

# Output file
process.TFileService = cms.Service("TFileService",
        fileName = cms.string("test_combined_data.root")
        )
# ZFinder
from ZFinder.Event.zdefinitions_cfi import zdefs_combined_data
from ZFinder.Event.zfinder_cfi import ZFinder
process.ZFinder = ZFinder.clone(
        # Use the calibrated electrons we make with
        # process.CalibratedElectrons
        ecalElectronsInputTag = cms.InputTag("calibratedElectrons", "calibratedGsfElectrons"),
        ZDefinitions = zdefs_combined_data,
        use_muon_acceptance = cms.bool(True),
        )

process.out = cms.OutputModule("PoolOutputModule",
        outputCommands = cms.untracked.vstring('drop *',
            'keep *_*_*_ExREG'),
        fileName = cms.untracked.string('electrons.root')
        )
process.load("FWCore.MessageLogger.MessageLogger_cfi")


process.p = cms.Path(
        process.kt6PFJetsForIsolation
        * process.eleRegressionEnergy
        * process.calibratedElectrons
        * process.pfiso
        * process.ZFinder
        )
process.outpath = cms.EndPath(process.out)


