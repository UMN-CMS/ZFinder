import FWCore.ParameterSet.Config as cms

MIN_MZ = cms.untracked.double(60.)
MAX_MZ = cms.untracked.double(120.)

# Define common PSets

# All events with at least 2 electrons
all_electrons = cms.untracked.PSet(
        name = cms.untracked.string("All Electrons"),
        cuts0 = cms.untracked.vstring("acc(ALL)"),
        cuts1 = cms.untracked.vstring("acc(ALL)"),
        min_mz = MIN_MZ,
        max_mz = MAX_MZ,
        use_truth_mass = cms.untracked.bool(False),
        )

# Only the kinematic cuts for the combined muon result, using generator
# quantities
combined_gen_cuts = cms.untracked.PSet(
        name = cms.untracked.string("Combined Gen Cuts"),
        cuts0 = cms.untracked.vstring("gaeta<2.1", "gpt>30"),
        cuts1 = cms.untracked.vstring("gaeta<2.4", "gpt>20"),
        min_mz = MIN_MZ,
        max_mz = MAX_MZ,
        use_truth_mass = cms.untracked.bool(True),
        )

# Only the kinematic cuts for the combined muon result, using reco quantities
combined_reco_cuts = cms.untracked.PSet(
        name = cms.untracked.string("Combined Reco Cuts"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(MUON_TIGHT)", "pt>30"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(MUON_LOOSE)", "pt>20"),
        min_mz = MIN_MZ,
        max_mz = MAX_MZ,
        use_truth_mass = cms.untracked.bool(False),
        )

# Cuts for the combined muon result assuming a single electron trigger
combined_single = cms.untracked.PSet(
        name = cms.untracked.string("Combined Single"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(MUON_TIGHT)", "trig(single_ele)", "pt>30", "eg_tight"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(MUON_LOOSE)", "acc(MUON_LOOSE)",  "pt>20", "eg_medium"),
        min_mz = MIN_MZ,
        max_mz = MAX_MZ,
        use_truth_mass = cms.untracked.bool(False),
        )

combined_single_no_trigger = combined_single.clone(
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(MUON_TIGHT)", "pt>30", "eg_tight"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(MUON_LOOSE)", "pt>20", "eg_medium"),
        )

# Cuts for the electron only analysis
ecal_ecal = cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Rapidity"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "trig(single_ele)", "pt>30", "eg_medium"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "acc(ALL)",         "pt>20", "eg_loose"),
        min_mz = MIN_MZ,
        max_mz = MAX_MZ,
        use_truth_mass = cms.untracked.bool(False),
        )

ecal_ecal_no_trigger = ecal_ecal.clone(
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>30", "eg_medium"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>20", "eg_loose"),
        )

ecal_nt = cms.untracked.PSet(
        name = cms.untracked.string("ET-NT Rapidity"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "trig(single_ele)", "pt>30", "eg_medium"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(NT)", "acc(NT)",          "pt>20", "nt_loose"),
        min_mz = MIN_MZ,
        max_mz = MAX_MZ,
        use_truth_mass = cms.untracked.bool(False),
        )

ecal_nt_no_trigger = ecal_nt.clone(
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>30", "eg_medium"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(NT)", "pt>20", "nt_loose"),
        )

ecal_hf = cms.untracked.PSet(
        name = cms.untracked.string("ET-HF Rapidity"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "trig(single_ele)", "pt>30", "eg_medium"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(HF)", "acc(ALL)",         "pt>20", "hf_2dloose"),
        min_mz = MIN_MZ,
        max_mz = MAX_MZ,
        use_truth_mass = cms.untracked.bool(False),
        )

ecal_hf_no_trigger = ecal_hf.clone(
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>30", "eg_medium"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(HF)", "pt>20", "hf_2dloose"),
        )

# The ZDefinition for use on data for the extended electron result
zdefs_extended_data = cms.untracked.VPSet(
        all_electrons,
        ecal_ecal,
        ecal_nt,
        ecal_hf,
        )

# The ZDefinition for use on MC for the extended electron result
zdefs_extended_mc = cms.untracked.VPSet(
        all_electrons,
        ecal_ecal_no_trigger,
        ecal_nt_no_trigger,
        ecal_hf_no_trigger,
        )

# The ZDefinition for use on data for the combined result
zdefs_combined_data = cms.untracked.VPSet(
        all_electrons,
        combined_reco_cuts,
        combined_single,
        )

# The ZDefinition for use on MC for the combined result
zdefs_combined_mc = cms.untracked.VPSet(
        all_electrons,
        combined_gen_cuts,
        combined_reco_cuts,
        combined_single_no_trigger,
        )
