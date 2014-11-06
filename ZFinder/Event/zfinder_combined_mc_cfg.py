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
        fileNames = cms.untracked.vstring('file:/hdfs/cms/phedex/store/mc/Summer12_DR53X/DYToEE_M-20_CT10_TuneZ2star_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/0000/AC6646E7-36F0-E111-B2F8-00259073E3FC.root')
        )

# Output file
process.TFileService = cms.Service("TFileService",
        fileName = cms.string("test_combined_mc.root")
        )

# Produce PDF weights (maximum is 3)

# Import rho value for isolation correction
from ZFinder.Event.kt6_pfjets_cfi import kt6PFJetsForIsolation
process.kt6PFJetsForIsolation = kt6PFJetsForIsolation.clone()

# Particle flow isolation
from CommonTools.ParticleFlow.Tools.pfIsolation import setupPFElectronIso
process.eleIsoSequence = setupPFElectronIso(process, 'gsfElectrons')
process.pfiso = cms.Sequence(process.pfParticleSelectionSequence + process.eleIsoSequence)

# Compute PDF weights for uncertainty calculation
process.pdfWeights = cms.EDProducer("PdfWeightProducer",
        # Fix POWHEG if buggy (this PDF set will also appear on output,
        # so only two more PDF sets can be added in PdfSetNames if not "")
        #FixPOWHEG = cms.untracked.string("cteq66.LHgrid"),
        GenTag = cms.untracked.InputTag("genParticles"),
        PdfInfoTag = cms.untracked.InputTag("generator"),
        PdfSetNames = cms.untracked.vstring(
            "NNPDF21_100.LHgrid",
            "MSTW2008nlo68cl.LHgrid",
            "CT10.LHgrid",
            #"Cteq66.LHgrid",
            #"MRST2006nnlo.LHgrid",
            #"NNPDF10_100.LHgrid",
            ),
        )

# Compute FSR weights for uncertainty calculation
process.fsrWeight = cms.EDProducer("FSRWeightProducer",
        GenTag = cms.untracked.InputTag("genParticles"),
        )

# Get the python string from the file name, and use it set up the name of the
# tuple output file. Note that pythonValue() returns the string with literal '
# on either end, so we need to strip those.
output_name = process.TFileService.fileName.pythonValue().strip("'")
tuple_names = cms.untracked.string(output_name.split('.root')[0] + "_tuples.root")

# ZFinder
from ZFinder.Event.zdefinitions_cfi import zdefs_combined_mc
from ZFinder.Event.zfinder_cfi import ZFinder
process.ZFinder = ZFinder.clone(
        ZDefinitions = zdefs_combined_mc,
        is_mc = cms.bool(True),
        use_muon_acceptance = cms.bool(True),
        require_gen_z = cms.bool(False),
        tuple_output_file = tuple_names,
        )

# RUN
process.p = cms.Path(
        process.pdfWeights
        * process.fsrWeight
        * process.kt6PFJetsForIsolation
        * process.pfiso
        * process.ZFinder
        )
