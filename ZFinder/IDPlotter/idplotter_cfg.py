import FWCore.ParameterSet.Config as cms

process = cms.Process("IDPlotter")

# Energy and calibrations for electrons
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
# Tag from: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions?redirectedfrom=CMS.SWGuideFrontierConditions
# Based on the FT_R_53_V18 tag
process.GlobalTag.globaltag = 'FT_53_V21_AN6::All'

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
        fileNames = cms.untracked.vstring('file:/hdfs/cms/phedex/store/data/Run2012A/DoubleElectron/AOD/22Jan2013-v1/20000/003EC246-5E67-E211-B103-00259059642E.root')
        )

# Output file
process.TFileService = cms.Service("TFileService",
        fileName = cms.string("id_plots.root")
        )

# Run only on lumis specified in the lumi file
# Recipe from:
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuidePythonTips#Use_a_JSON_file_of_good_lumi_sec
from FWCore.ParameterSet.Types import untracked, VLuminosityBlockRange
from FWCore.PythonUtilities.LumiList import LumiList
#json_file = "../../Metadata/lumi_json/Run2012ABCD.json"  # File location
json_file = '/home/user1/gude/CMS/src/CMSSW_5_3_13_UNSTABLE/src/Metadata/lumi_json/Run2012ABCD.json'
run_2012abcd_lumis = LumiList(filename = json_file).getCMSSWString().split(',')
process.source.lumisToProcess = untracked(VLuminosityBlockRange(run_2012abcd_lumis))

#Energy correction
from ZFinder.Event.electron_regression_cfi import CalibratedElectrons, RandomNumberGeneratorService, ElectronEnergyRegressions
process.RandomNumberGeneratorService = RandomNumberGeneratorService
process.CalibratedElectrons = CalibratedElectrons
process.eleRegressionEnergy = ElectronEnergyRegressions

process.IDPlotter = cms.EDAnalyzer('IDPlotter',
        ecalElectronsInputTag = cms.InputTag("CalibratedElectrons", "calibratedGsfElectrons"),
)

# rho value for isolation
from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets  # the 4 references the rParam = 0.4
process.kt6PFJetsForIsolation = kt4PFJets.clone(
        rParam = 0.6,
        doRhoFastjet = True,
        Rho_EtaMax = cms.double(2.5)
        )

# Particle flow isolation
from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso
process.eleIsoSequence = setupPFElectronIso(process, 'CalibratedElectrons:calibratedGsfElectrons')
process.pfiso = cms.Sequence(process.pfParticleSelectionSequence + process.eleIsoSequence)

# RUN
process.p = cms.Path(
        process.eleRegressionEnergy
        * process.CalibratedElectrons
        * process.kt6PFJetsForIsolation
        * process.pfiso
        * process.IDPlotter
        )
