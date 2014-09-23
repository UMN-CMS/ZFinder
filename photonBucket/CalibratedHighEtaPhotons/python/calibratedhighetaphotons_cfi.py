import FWCore.ParameterSet.Config as cms

calibratedHighEtaPhotons = cms.EDProducer('CalibratedHighEtaPhotons',
    photons       = cms.InputTag( "photons" ),
	ecalRecHits   = cms.InputTag("reducedEcalRecHitsEE"),
)
