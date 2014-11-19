import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
# Set up message output and logging
from FWCore.MessageService.MessageLogger_cfi import MessageLogger
process.MessageLogger = MessageLogger
process.MessageLogger.cerr.FwkReport.reportEvery = 1000  # Report status ever 100 events

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


# Run on MC
IS_MC = False

# Input file
if not IS_MC:
    process.source = cms.Source("PoolSource",
            fileNames = cms.untracked.vstring('file:/hdfs/cms/phedex/store/data/Run2012A/DoubleElectron/AOD/22Jan2013-v1/20000/003EC246-5E67-E211-B103-00259059642E.root')
            )

    # Energy and calibrations for electrons
    process.load('Configuration.StandardSequences.GeometryDB_cff')
    process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
    process.GlobalTag.globaltag = 'GR_P_V42_AN3::All'

    # Electron calibrations
    from EgammaAnalysis.ElectronTools.calibratedElectrons_cfi import calibratedElectrons

    # Set up random numbers (needed for the electron regression)
    process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
            CalibratedElectrons = cms.PSet(
                initialSeed = cms.untracked.uint32(1),
                engineName = cms.untracked.string('TRandom3')
                ),
            )

    process.CalibratedElectrons = calibratedElectrons.clone(
            isMC = cms.bool(False),
            inputDataset = cms.string("22Jan2013ReReco"),
            updateEnergyError = cms.bool(True),
            correctionsType = cms.int32(2),
            combinationType = cms.int32(3),
            lumiRatio = cms.double(1.0),
            verbose = cms.bool(False),
            synchronization = cms.bool(True),
            )

    # Energy Regression
    from EgammaAnalysis.ElectronTools.electronRegressionEnergyProducer_cfi import eleRegressionEnergy

    process.eleRegressionEnergy = eleRegressionEnergy.clone(
            inputElectronsTag = cms.InputTag('gsfElectrons'),
            inputCollectionType = cms.uint32(0),
            useRecHitCollections = cms.bool(True),
            produceValueMaps = cms.bool(True),
            regressionInputFile = cms.string("EgammaAnalysis/ElectronTools/data/eleEnergyRegWeights_WithSubClusters_VApr15.root"),
            energyRegressionType = cms.uint32(2),
            )

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
else:
    process.source = cms.Source("PoolSource",
            fileNames = cms.untracked.vstring('file:/hdfs/cms/phedex/store/mc/Summer12_DR53X/DYToEE_M-20_CT10_TuneZ2star_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/0000/02894AF7-27F0-E111-8566-00259073E38A.root')
            )

    # Particle flow isolation
    from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso
    process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons')
    process.pfiso = cms.Sequence(process.pfParticleSelectionSequence + process.eleIsoSequence)

    # My analyzer
    process.trigeff = cms.EDAnalyzer('TrigEff',
            ecalElectronsInputTag = cms.InputTag("gsfElectrons"),
            )

    process.p = cms.Path(
            process.kt6PFJetsForIsolation
            * process.pfiso
            * process.trigeff
            )
