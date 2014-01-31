import FWCore.ParameterSet.Config as cms

MIN_MZ = 40.
MAX_MZ = 150.

zdefs = cms.untracked.VPSet(
        cms.untracked.PSet(
            name = cms.untracked.string("All-All"),
            cuts0 = cms.untracked.vstring("acc(ALL)"),
            cuts1 = cms.untracked.vstring("acc(ALL)"),
            min_mz = cms.untracked.double(MIN_MZ),
            max_mz = cms.untracked.double(MAX_MZ),
            ),
        cms.untracked.PSet(
            name = cms.untracked.string("ET-ET"),
            cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>20", "eg_medium"),
            cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>20", "eg_medium"),
            min_mz = cms.untracked.double(MIN_MZ),
            max_mz = cms.untracked.double(MAX_MZ),
            ),
        cms.untracked.PSet(
            name = cms.untracked.string("ET-EB"),
            cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>20", "eg_medium"),
            cuts1 = cms.untracked.vstring("acc(ALL)", "acc(EB)", "pt>20", "eg_loose"),
            min_mz = cms.untracked.double(MIN_MZ),
            max_mz = cms.untracked.double(MAX_MZ),
            ),
        cms.untracked.PSet(
            name = cms.untracked.string("All-EB"),
            cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ALL)"),
            cuts1 = cms.untracked.vstring("acc(ALL)", "acc(EB)"),
            min_mz = cms.untracked.double(MIN_MZ),
            max_mz = cms.untracked.double(MAX_MZ),
            ),
        cms.untracked.PSet(
            name = cms.untracked.string("ET-EE"),
            cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "acc(ET)", "pt>20", "eg_medium"),
            cuts1 = cms.untracked.vstring("acc(ALL)", "acc(EE)", "!acc(NT)", "pt>20", "eg_loose"),
            min_mz = cms.untracked.double(MIN_MZ),
            max_mz = cms.untracked.double(MAX_MZ),
            ),
        cms.untracked.PSet(
            name = cms.untracked.string("All-EE"),
            cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ALL)", "acc(ALL)"),
            cuts1 = cms.untracked.vstring("acc(ALL)", "acc(EE)", "!acc(NT)"),
            min_mz = cms.untracked.double(MIN_MZ),
            max_mz = cms.untracked.double(MAX_MZ),
            ),
        cms.untracked.PSet(
            name = cms.untracked.string("ET-NT"),
            cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>20", "eg_medium"),
            cuts1 = cms.untracked.vstring("acc(ALL)", "acc(NT)", "pt>20", "nt_loose"),
            min_mz = cms.untracked.double(MIN_MZ),
            max_mz = cms.untracked.double(MAX_MZ),
            ),
        cms.untracked.PSet(
            name = cms.untracked.string("All-NT"),
            cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ALL)"),
            cuts1 = cms.untracked.vstring("acc(ALL)", "acc(NT)"),
            min_mz = cms.untracked.double(MIN_MZ),
            max_mz = cms.untracked.double(MAX_MZ),
            ),
        cms.untracked.PSet(
            name = cms.untracked.string("ET-HF"),
            cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>20", "eg_medium"),
            cuts1 = cms.untracked.vstring("acc(ALL)", "acc(HF)", "pt>20", "hf_2dloose"),
            min_mz = cms.untracked.double(MIN_MZ),
            max_mz = cms.untracked.double(MAX_MZ),
            ),
        cms.untracked.PSet(
            name = cms.untracked.string("All-HF"),
            cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ALL)"),
            cuts1 = cms.untracked.vstring("acc(ALL)", "acc(HF)"),
            min_mz = cms.untracked.double(MIN_MZ),
            max_mz = cms.untracked.double(MAX_MZ),
            )
        )
