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
        fileNames = cms.untracked.vstring('file:/hdfs/cms/phedex/store/data/Run2012B/SingleElectron/AOD/22Jan2013-v1/20000/001C4345-867D-E211-93E8-001E6739762F.root')
        )

# Output file
process.out = cms.OutputModule( "PoolOutputModule",
        fileName = cms.untracked.string("test.root"),
        SelectEvents = cms.untracked.PSet(
            SelectEvents = cms.vstring('p1'),
            ),
        outputCommands = cms.untracked.vstring(
            'keep *',
            )
        )

# Run only on lumis specified in the lumi file
# Recipe from:
# https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuidePythonTips#Use_a_JSON_file_of_good_lumi_sec
from FWCore.ParameterSet.Types import untracked, VLuminosityBlockRange
from FWCore.PythonUtilities.LumiList import LumiList
json_file = "../../Metadata/lumi_json/Run2012ABCD.json"  # File location
run_2012abcd_lumis = LumiList(filename = json_file).getCMSSWString().split(',')
process.source.lumisToProcess = untracked(VLuminosityBlockRange(run_2012abcd_lumis))

## Skimmer
process.zskimmer = cms.EDFilter('ZSkimmer')

process.p1 = cms.Path(
        process.zskimmer
        )

process.final = cms.EndPath(
        process.out
        )
