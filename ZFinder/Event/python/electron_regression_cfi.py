import FWCore.ParameterSet.Config as cms

# Electron calibrations
from EgammaAnalysis.ElectronTools.calibratedElectrons_cfi import calibratedElectrons

# Set up random numbers (needed for the electron regression)
RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
        CalibratedElectrons = cms.PSet(
            initialSeed = cms.untracked.uint32(1),
            engineName = cms.untracked.string('TRandom3')
            ),
        )

CalibratedElectrons = calibratedElectrons.clone(
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

ElectronEnergyRegressions = eleRegressionEnergy.clone(
        inputElectronsTag = cms.InputTag('gsfElectrons'),
        inputCollectionType = cms.uint32(0),
        useRecHitCollections = cms.bool(True),
        produceValueMaps = cms.bool(True),
        regressionInputFile = cms.string("EgammaAnalysis/ElectronTools/data/eleEnergyRegWeights_WithSubClusters_VApr15.root"),
        energyRegressionType = cms.uint32(2),
        )
