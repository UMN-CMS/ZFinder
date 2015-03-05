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
        # MadGraph
        fileNames = cms.untracked.vstring('/store/mc/Summer12_DR53X/DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball/AODSIM/PU_S10_START53_V7A-v1/0000/00037C53-AAD1-E111-B1BE-003048D45F38.root')
        )

# Output file
process.TFileService = cms.Service("TFileService",
        fileName = cms.string("test_combined_mc.root")
        )

# Energy and calibrations for electrons
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
# Tag from: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions?redirectedfrom=CMS.SWGuideFrontierConditions
# Based on the FT_R_53_V18 tag
process.GlobalTag.globaltag = 'START53_V27::All'

from ZFinder.Event.electron_regression_cfi import CalibratedElectrons_MC, RandomNumberGeneratorService, ElectronEnergyRegressions_MC
process.RandomNumberGeneratorService = RandomNumberGeneratorService
process.CalibratedElectrons = CalibratedElectrons_MC
process.eleRegressionEnergy = ElectronEnergyRegressions_MC

# Import rho value for isolation correction
from ZFinder.Event.kt6_pfjets_cfi import kt6PFJetsForIsolation
process.kt6PFJetsForIsolation = kt6PFJetsForIsolation.clone()

# Particle flow isolation
from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso
process.eleIsoSequence = setupPFElectronIso(process, 'CalibratedElectrons:calibratedGsfElectrons')
process.pfiso = cms.Sequence(process.pfParticleSelectionSequence + process.eleIsoSequence)

# Compute PDF weights for uncertainty calculation
process.pdfWeights = cms.EDProducer("PdfWeightProducer",
        # Fix POWHEG if buggy (this PDF set will also appear on output,
        # so only two more PDF sets can be added in PdfSetNames if not "")
        #FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
        GenTag = cms.untracked.InputTag("genParticles"),
        PdfInfoTag = cms.untracked.InputTag("generator"),
        # Produce PDF weights (maximum is 3)
        PdfSetNames = cms.untracked.vstring(
            "NNPDF23_nlo_as_0118.LHgrid",
            "MSTW2008nlo68cl.LHgrid",
            "CT10.LHgrid",
            #"cteq6ll.LHpdf",
            ),
        )

# Compute FSR weights for uncertainty calculation
process.fsrWeight = cms.EDProducer("FSRWeightProducer",
        GenTag = cms.untracked.InputTag("genParticles"),
        )

# ZFinder
from ZFinder.Event.zdefinitions_cfi import zdefs_combined_mc
from ZFinder.Event.zfinder_cfi import ZFinder
process.ZFinder = ZFinder.clone(
        # Use the calibrated electrons we make with process.CalibratedElectrons
        ecalElectronsInputTag = cms.InputTag("CalibratedElectrons", "calibratedGsfElectrons"),
        ZDefinitions = zdefs_combined_mc,
        is_mc = cms.bool(True),
        use_muon_acceptance = cms.bool(True),
        require_gen_z = cms.bool(False),
        run_pdf_weights = cms.bool(False),
        run_fsr_weight = cms.bool(False),
        gen_electrons = cms.string("Dressed"),
        )

# RUN
process.p = cms.Path(
        process.pdfWeights
        * process.fsrWeight
        * process.kt6PFJetsForIsolation
        * process.eleRegressionEnergy
        * process.CalibratedElectrons
        * process.pfiso
        * process.ZFinder
        )
