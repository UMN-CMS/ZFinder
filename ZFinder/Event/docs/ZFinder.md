# ZFinder

ZFinder selects events that have electrons, makes plots of their quantities,
and save the results in a RooWorkspace.

## ZFinder

[ZFinder](../src/ZFinder.cc) is the main analyzer. Each edm::Event is passed to
the ZFinderEvent constructor which extracts the relevant quantities.

Here is, step by step, how it is set up and runs:

### ZFinder::ZFinder (Constructor)

This reads in the parameter set from the Python [configuration file](../python/zfinder_cfi.py).
It sets up two Cut Setters, AcceptanceSetter and TruthMatchSetter (if running
on MC) which set cuts on each ZFinderElectron. How these cuts work are
described in the ZFinderElectron section.

The "Unweighted Event Count" histogram is also created here. It is used to
count each event looked at, which is useful for when we read back an MC file as
we need the total number of events to normalize.

The ZDefinitions are also set up here after they are read in from the
[configuration file](../python/zdefinitions_cfi.py). For each ZDefinition, a
ZDefinitionPlotter and ZDefinitionWorkspace are also made (these are discussed
in their sections below). If running on MC, a "MC" version of each definition
is also made that uses the truth value of the Z Mass for cuts and plots the
truth values in the plotter.

### ZFinder::analyze

Each time analyze is called, a new ZFinderEvent (discussed in its section
below) is created. If this event is good (Mass > -1, and two electrons at
least) then the rest of the code proceeds, otherwise we skip to the next event.

All of the cut setters are called on this event to set the cuts in the
electrons. We then set the weights of each events based on scale factors and
pileup reweighting. We then check the ZDefinitions and see which of them the
event passes (this must be done after setting the electron cuts as they are
used to determine if an event passes a definition). After this we pass the
event to the ZDefinitionPlotters and ZDefinitionWorkspaces which plot the event
and save it as a RooWorkspace.

## ZFinderEvent

[ZFinderEvent](../src/ZFinderEvent.cc) takes a edm::Event, and edm::EventSetup,
and a edm::ParameterSet.  It contains variables describing the event include
the beam spot, number of vertices, event idea, and various information about
the Z.

It contains various pieces of information, but the most important are:
* A list of ZFinderElectrons, and two special pointers (e0, e1) to the two
  electrons in this list used to make the reco_z.
* In MC only, two pointers to the two truth electrons (e0_truth, e1_truth)
* A Z "object" and a truth Z, called reco_z and (in MC only) a truth_Z

The list of (data, not MC) electrons are filled in the various "Init"
functions: 
* ZFinderEvent::InitGSFElectrons()
* ZFinderEvent::InitHFElectrons()
* ZFinderEvent::InitNTElectrons()

It is best to read the code to figure out what these functions do.

MC electrons are set in ZFinderEvent::InitTruth() (where their Z is also set).

### reco_z, truth_z

These objects are created from the electron pointers (reco_z is made from e0,
e1; truth_z is made from e0_truth, e1_truth).

These objects are defined as follows:

```C++
struct ZData{
    double m;
    double pt;
    double y;
    double phistar;
    double eta;
    double deltaR;
} reco_z, truth_z;
```

They are created from the list of ZFinderElectrons in ZFinderEvent::InitZ() for
reco_z, and in ZFinderEvent::InitTruth() for truth_z.

## ZFinderElectron

[ZFinderElectron](../src/ZFinderElectron.cc) is a class that is created from
one of the CMS physics object types (GsfElectron, GenParticle,
RecoEcalCandidate, Photon, or TriggerObject).  It provides access to pt, eta,
phi, charge via its methods, but more importantly it has a map that maps a
string to a structure containing a bool, a weight, and a name. These are called
"CutResults" and are used to store whether the electron passed a specific cut.

An example cutresult entry might look like this:

```c++
cutresult["eg_medium"] = { passed = true, name = "eg_medium", weight = 0.6 };
```

This indicates that the ZFinderElectron has passed "eg_medium", and has a
"weight" of 0.6.

Some of these cut results are set in the ZFinderEvent (eg_medium for example),
and others are set in "Setter" Classes like AcceptanceSetter.cc or
TruthMatchSetter.cc. The power of the cut results is that they can be any
string, and they're used by ZDefinitions to select events.

## ZFinderPlotter

[ZFinderPlotter](../src/ZFinderPlotter.cc) creates histograms from
ZFinderEvents. They can be initialized manually by giving them a
TFileDirectory, but more commonly they are created by ZDefinition Plotter.
There is a variable "USE_MC" set in the constructor that selects between
plotting Reco quantities and truth quantities.

## ZDefinition

[ZDefintion](../src/ZDefinition.cc) applies a definition of what a "good event"
is and uses that to tell us if an event passes or not. Think of them as
'cutresults' (from ZFinderElectron) but applied to the entire ZFinderEvent.
That is, there is a map that maps a string (the name of the definition) to a
list of the different cut levels. If all the cutlevels are true, then the event
has passed the definition.

Additional information about setting up ZDefinitions can be found
[here](ZDefinition.md).

## ZDefinitionPlotter

A [ZDefinitionPlotter](../src/ZDefinitionPlotter.cc) is initialized with a
ZDefinition. After that they plot the events that pass each level of the cuts
using ZFinderPlotter.

## ZDefinitionWorkspace

[ZDefinitionWorkspace](../src/ZDefinitionWorkspace.cc) is initialized with a
ZDefinition and uses that to select events to save in a RooWorkspace. These are
rather complicated objects which are detailed [here](ZDefinitionWorkspace.md).
