import FWCore.ParameterSet.Config as cms
MIN_MZ = 60.
MAX_MZ = 120.

zdefs = cms.untracked.VPSet(
    cms.untracked.PSet(
        name = cms.untracked.string("All Electrons"),
        cuts0 = cms.untracked.vstring("acc(ALL)"),
        cuts1 = cms.untracked.vstring("acc(ALL)"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    # Cuts for the combined result, but using generator quantities and no
    # ID cuts
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Gen Cuts Combined Double"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "gpt>20", "geta<2.4", "geta>-2.4"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "gpt>20", "geta<2.4", "geta>-2.4"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(True)
        ),
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Gen Cuts Combined Single"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "gpt>30", "geta<2.4", "geta>-2.4"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "gpt>10", "geta<2.4", "geta>-2.4"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(True)
        ),
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Reco Cuts Combined Double"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>20", "eta<2.4", "eta>-2.4"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>20", "eta<2.4", "eta>-2.4"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Reco Cuts Combined Single"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>30", "eta<2.4", "eta>-2.4"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>10", "eta<2.4", "eta>-2.4"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    # Cuts for the combined result. Some assume a double electron trigger (and
    # hence use eg_medium on both legs) and some assume a single electron
    # trigger (and hence use eg_tight) on one leg
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Combined Double"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "trig(et_et_tight)", "pt>20", "eta<2.4", "eta>-2.4", "eg_medium"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "trig(et_et_loose)", "pt>20", "eta<2.4", "eta>-2.4", "eg_medium"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Combined Single"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "trig(single_ele)", "pt>30", "eta<2.4", "eta>-2.4", "eg_tight"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "acc(ET)",          "pt>10", "eta<2.4", "eta>-2.4", "eg_medium"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    # Cuts for the full rapidity measurement, where one electron is required in
    # the ECAL Tracked (ET) region and have pt>30, and the other electron is
    # required to have pt>20.
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Rapidity"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "trig(et_et_tight)", "pt>30", "eg_medium"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "trig(et_et_loose)", "pt>20", "eg_loose"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    cms.untracked.PSet(
        name = cms.untracked.string("ET-NT Rapidity"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "trig(et_nt_etleg)", "pt>30", "eg_medium"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(NT)", "acc(NT)",           "pt>20", "nt_loose"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    cms.untracked.PSet(
        name = cms.untracked.string("ET-HF Rapidity"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "trig(et_hf_tight)", "pt>30", "eg_medium"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(HF)", "trig(hf_loose)",    "pt>20", "hf_2dloose"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    )

zdefs_no_trigger = cms.untracked.VPSet(
    cms.untracked.PSet(
        name = cms.untracked.string("All Electrons"),
        cuts0 = cms.untracked.vstring("acc(ALL)"),
        cuts1 = cms.untracked.vstring("acc(ALL)"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    # Cuts for the combined result using kinematics only
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Gen Cuts Combined Double"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "gpt>20", "geta<2.4", "geta>-2.4"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "gpt>20", "geta<2.4", "geta>-2.4"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(True)
        ),
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Gen Cuts Combined Single"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "gpt>30", "geta<2.4", "geta>-2.4"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "gpt>10", "geta<2.4", "geta>-2.4"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(True)
        ),
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Reco Cuts Combined Double"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>20", "eta<2.4", "eta>-2.4"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>20", "eta<2.4", "eta>-2.4"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Reco Cuts Combined Single"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>30", "eta<2.4", "eta>-2.4"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>10", "eta<2.4", "eta>-2.4"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Reco Cuts Combined Double"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>20", "eta<2.4", "eta>-2.4"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>20", "eta<2.4", "eta>-2.4"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Reco Cuts Combined Single"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>30", "eta<2.4", "eta>-2.4"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>10", "eta<2.4", "eta>-2.4"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    # Cuts for the combined result. Some assume a double electron trigger (and
    # hence use eg_medium on both legs) and some assume a single electron
    # trigger (and hence use eg_tight) on one leg
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Combined Double"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>20", "eta<2.4", "eta>-2.4", "eg_medium"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>20", "eta<2.4", "eta>-2.4", "eg_medium"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Combined Single"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>30", "eta<2.4", "eta>-2.4", "eg_tight"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>10", "eta<2.4", "eta>-2.4", "eg_medium"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    # Cuts for the full rapidity measurement, where one electron is required in
    # the ECAL Tracked (ET) region and have pt>30, and the other electron is
    # required to have pt>20.
    cms.untracked.PSet(
        name = cms.untracked.string("ET-ET Rapidity"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>30", "eg_medium"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>20", "eg_loose"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    cms.untracked.PSet(
        name = cms.untracked.string("ET-NT Rapidity"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>30", "eg_medium"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(NT)", "pt>20", "nt_loose"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    cms.untracked.PSet(
        name = cms.untracked.string("ET-HF Rapidity"),
        cuts0 = cms.untracked.vstring("acc(ALL)", "acc(ET)", "pt>30", "eg_medium"),
        cuts1 = cms.untracked.vstring("acc(ALL)", "acc(HF)", "pt>20", "hf_2dloose"),
        min_mz = cms.untracked.double(MIN_MZ),
        max_mz = cms.untracked.double(MAX_MZ),
        use_truth_mass = cms.untracked.bool(False)
        ),
    )
