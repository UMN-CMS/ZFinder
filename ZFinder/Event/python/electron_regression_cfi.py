import FWCore.ParameterSet.Config as cms

# Set up random numbers (needed for the electron regression)
RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
        CalibratedElectrons = cms.PSet(
            initialSeed = cms.untracked.uint32(1),
            engineName = cms.untracked.string('TRandom3')
            ),
        )

# Electron calibrations
# From: https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaElectronEnergyScale#Electron_energy_scale_and_resolu
from EgammaAnalysis.ElectronTools.calibratedElectrons_cfi import calibratedElectrons
CalibratedElectrons = calibratedElectrons.clone(
        isMC = cms.bool(False),
        inputDataset = cms.string("22Jan2013ReReco"),
        updateEnergyError = cms.bool(True),
        correctionsType = cms.int32(2),
        combinationType = cms.int32(3),
        verbose = cms.bool(False),
        )

CalibratedElectrons_MC = calibratedElectrons.clone(
        isMC = cms.bool(True),
        inputDataset = cms.string("Summer12_LegacyPaper"),
        updateEnergyError = cms.bool(True),
        correctionsType = cms.int32(2),
        combinationType = cms.int32(3),
        lumiRatio = cms.double(0.607),  # For 2012A + B + C + D
        synchronization = cms.bool(False),
        verbose = cms.bool(False),
        )

# Energy Regression
# From: https://twiki.cern.ch/twiki/bin/viewauth/CMS/ElectronMomentumRegression#Recipe_for_53X
from EgammaAnalysis.ElectronTools.electronRegressionEnergyProducer_cfi import eleRegressionEnergy

ElectronEnergyRegressions = eleRegressionEnergy.clone(
        inputElectronsTag = cms.InputTag('gsfElectrons'),
        inputCollectionType = cms.uint32(0),
        useRecHitCollections = cms.bool(True),
        produceValueMaps = cms.bool(True),
        regressionInputFile = cms.string("EgammaAnalysis/ElectronTools/data/eleEnergyRegWeights_WithSubClusters_VApr15.root"),
        energyRegressionType = cms.uint32(2),
        )

ElectronEnergyRegressions_MC = eleRegressionEnergy.clone(
        inputElectronsTag = cms.InputTag('gsfElectrons'),
        inputCollectionType = cms.uint32(0),
        useRecHitCollections = cms.bool(True),
        produceValueMaps = cms.bool(True),
        regressionInputFile = cms.string("EgammaAnalysis/ElectronTools/data/eleEnergyRegWeights_WithSubClusters_VApr15.root"),
        energyRegressionType = cms.uint32(2),
        )
