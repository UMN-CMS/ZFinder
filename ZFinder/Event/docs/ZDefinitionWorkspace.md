# ZDefinitionWorkspace

ZDefinitionWorkspace makes RooWorkspaces containing the events that pass the
associated ZDefinition. However, there are some deferences that must be kept
in mind.

## Events in the RooWorkspace

The Events saved in the RooWorkspace are all the events that the cut two from
the end of the ZDefinition. For example, if the cut strings are:

|            |                   |                     | Events Selected Here | Numerator   | The Mass Cut is Ignored |
|------------|-------------------|---------------------|----------------------|-------------|-------------------------|
| "acc(ALL)" | "acc(MUON_TIGHT)" | "trig(et_et_tight)" | "pt>30"              | "eg_medium" | Mass Cut                |
| "acc(ALL)" | "acc(MUON_LOOSE)" | "trig(et_et_loose)" | "pt>20"              | "eg_medium" | Mass Cut                |

Then all events which pass the cuts up to the column marked
"Events Selected Here" are in the workspace. Events which also pass the column
marked "Numerator" have the numerator variable set to True. The mass cut is
ignored entirely; it should be applied by hand if desired.

## Variables

The following variables are stored in the workspace.

| Variables  | Explanation                                                                                                                              |
|------------|------------------------------------------------------------------------------------------------------------------------------------------|
| z_mass     | The mass of the Z Boson in GeV.                                                                                                          |
| z_eta      | The pseudorapidity of the Z Boson.                                                                                                       |
| z_y        | The rapidity of the Z Boson.                                                                                                             |
| z_pt       | The pt of the Z Boson.                                                                                                                   |
| phistar    | The Phistar value of the Z Boson.                                                                                                        |
| e0_pt      | The pt value of the 0th electron.                                                                                                        |
| e1_pt      | The pt value of the 1st electron.                                                                                                        |
| e0_eta     | The pseudorapidity value of the 0th electron.                                                                                            |
| e1_eta     | The pseudorapidity value of the 1st electron.                                                                                            |
| e0_charge  | The charge of the 0th electron. Either 1, -1, or 0 indicating unmeasured.                                                                |
| e1_charge  | The charge of the 1st electron.                                                                                                          |
| n_vert     | The number of vertices in the event.                                                                                                     |
| event_num  | The CMS event number.                                                                                                                    |
| data_type  | One of "Truth MC", "Reco MC", or "Data".                                                                                                 |
| numerator  | True if the event passes all cuts (*except* the mass requirement), False if it fails the last cut.                                       |
| degenerate | Takes one of a few values, described below.                                                                                              |
| weight     | The weight of the event, taking into account the efficiency of the cuts applied and any general weight (such as for pileup reweighting). |

### Degenerate

It is sometimes important to know if an event could have passed with e0
and e1 flipped. This variable stores that information as follows:

| Key                                  | Int Value | Explanation                                                                                                                                                                                                   |
|--------------------------------------|-----------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| False                                | 0         | The event would not have passed with e0 and e1 reversed.                                                                                                                                                      |
| Degenerate Denominator               | 1         | The event would pass the denominator cuts (that is, the penultimate set of cuts) with results reversed, but not the numerator set. The Numerator cuts are only passed with e0 and e1 as set in the Workspace. |
| Degenerate Denominator and Numerator | 2         | The event passes the entire cut chain (except the mass requirement) with e0 and e1 flipped.                                                                                                                   |

### Weight

The weight is the product of the initial event weight(often 1,
but sometimes different if pileup reweighting is being used) weights of the
various cuts up to the last one, even for denominator events.
