# ZDefinition

## Using ZDefinition

### Electron Cuts

Most of the power in ZDefinitions is actually derived from the cuts placed in
ZFinderElectrons by SetterBase objects (and ZFindeEvent, which sets a few cuts
when it creates electrons). Any cut that doesn't match a Comparison String
(detailed below) is assumed to be a cut on an electron that is checked with the
ZFinderElectron.CutPassed method.

##### Possible Cuts

###### Acceptance

Acceptance cuts make selections of electrons based on the geometric acceptance
of the detector.

| Cut Name | Explanation                    |
|----------|--------------------------------|
| acc(ALL) | In any of the defined regions. |
| acc(EB)  | -1.4442 < eta < 1.4442         |
| acc(EB+) | 0 < eta < 1.4442               |
| acc(EB-) | -1.4442 < eta < 0              |
| acc(EE)  | 1.566 < abs(eta) < 2.850       |
| acc(EE+) | 1.566 < eta < 2.850            |
| acc(EE-) | -2.850 < eta < -1.566          |
| acc(ET)  | In acc(EB) or acc(EE)          |
| acc(ET+) | In acc(EB+) or acc(EE+)        |
| acc(ET-) | In acc(EB-) or acc(EE-)        |
| acc(NT)  | 2.5 < abs(eta) < 2.850         |
| acc(NT+) | 2.5 < eta < 2.850              |
| acc(NT-) | -2.850 < eta < -2.5            |
| acc(HF)  | 3.1 < abs(eta) < 4.6           |
| acc(HF+) | 3.1 < eta < 4.6                |
| acc(HF-) | -4.6 < eta < -3.1              |

###### Trigger

Trigger cuts are used to match electrons to specific legs of triggers. They are
true if an electron from that leg of the trigger matches our reco electrons
with dR < 0.3, they are false otherwise.

| Cut Name          | Trigger Leg                                              | Trigger Filter                                                 |
|-------------------|----------------------------------------------------------|----------------------------------------------------------------|
| trig(hf_loose)    | Loose HF leg                                             | hltHFEMTightFilter                                             |
| trig(hf_tight)    | Tight HF leg                                             | hltHFEMPt30TightFilter                                         |
| trig(et_et_tight) | Tight leg of double ET trigger                           | hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoFilter       |
| trig(et_et_loose) | Loose leg of double ET trigger                           | hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoDoubleFilter |
| trig(et_et_dz)    | dz vertex match filter                                   | hltEle17TightIdLooseIsoEle8TightIdLooseIsoTrackIsoDZ           |
| trig(et_nt_etleg) | Tight ET leg of ET-NT trigger                            | hltEle27TightIdLooseIsoTrackIsoFilter                          |
| trig(et_hf_tight) | Tight ET leg of ET-HF trigger (paired with loose HF leg) | hltEle27TightIdLooseIsoTrackIsoFilter                          |
| trig(et_hf_loose) | Loose ET leg of ET-HF trigger (paired with tight HF leg) | hltEle23TightIdLooseIsoTrackIsoFilter                          |
| trig(single_ele)  | The Single Electron trigger (HLT_Ele27_WP80)             | hltEle27WP80TrackIsoFilter                                     |

###### Type

Type cuts select electrons based on the objects which were used to create the
ZFinderElectron.

| Cut Name           | Explanation                  |
|--------------------|------------------------------|
| type_gsf           | From reco::GsfElectron       |
| type_gen           | From reco::GenParticle       |
| type_ecalcandidate | From reco::RecoEcalCandidate |
| type_photon        | From reco::Photon            |
| type_hlt           | From trigger::TriggerObject  |

###### Quality

Quality cuts select electrons that have passed various quality criteria.

| Cut Name     | Explanation                                                                                     |
|--------------|-------------------------------------------------------------------------------------------------|
| eg_veto      | Cuts as defined here: https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification |
| eg_loose     |                                                                                                 |
| eg_medium    |                                                                                                 |
| eg_tight     |                                                                                                 |
| eg_eop_cut   |                                                                                                 |
| eg_trigtight |                                                                                                 |
| eg_trigwp70  |                                                                                                 |

| Cut Name    | Explanation                     |
|-------------|---------------------------------|
| hf_e9e25    | e9e25 > 0.94                    |
| hf_2dloose  | (eCe9 - (eSeL * 0.475)) > 0.815 |
| hf_2dmedium | (eCe9 - (eSeL * 0.275)) > 0.875 |
| hf_2dtight  | (eCe9 - (eSeL * 0.20)) > 0.92   |

| Cut Name | Explanation                                              |
|----------|----------------------------------------------------------|
| nt_loose | Many cuts on HCSC variables, see code in ZFinderEvent.cc |

#### Comparison Strings

The ZDefinition code allows some cuts to be made on basic kinematic variables
in a semi-free-form string. These strings take the following form:

```python
# General Form
"${Variable}${Opperator}${Value}"

# Some examples
"pt>20"      # Require pt > 20
"!gtp<30"    # Reject truth pt > 30
"charge=1"   # Require charge +1
"teta>=0"    # Require trigger eta greater than or equal to 0
"phi=<3.14"  # Require phi less than or equal to 3.14
```

Any string string with '!' inverts the result, where as a quantity that starts
with 'g' makes the cut on the Truth Level info (and always fails for real
data), and a quantity that starts with 't' makes the cut on the Trigger Level
info.

##### Allowed Values

A list of all allowed strings for `${Variable}`, `${Opperator}`, and `${Value}`
are given in the following table:

| `${Variable}` | `${Opperator}` | `${Value}`                                   |
|---------------|----------------|----------------------------------------------|
| pt            | =              | Any Double or Int, including negative values |
| eta           | >              |                                              |
| phi           | <              |                                              |
| charge        | >=             |                                              |
| gpt           | <=             |                                              |
| geta          |                |                                              |
| gphi          |                |                                              |
| gcharge       |                |                                              |

Note that this is a menu; pick one from each column!
