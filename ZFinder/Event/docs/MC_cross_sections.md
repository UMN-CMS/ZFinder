# MC Cross Sections

PREP gives a LO cross section based on the config file, and example of such is
here:
http://cms.cern.ch/iCMS/prep/requestmanagement?dsn=WW_TuneZ2star_8TeV_pythia6_tauola

Otherwise the Twiki gives NLO or NNLO cross sections:
https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat8TeV

## Various Cross Sections

| MC                                                            | Us/Joe's WR (pb) | AN-14-050 (pb) | Twiki (pb)            | PREP LO Only (pb) |
|---------------------------------------------------------------|------------------|----------------|-----------------------|-------------------|
| DYToTauTau_M-20_CT10_TuneZ2star_v2_8TeV-powheg-tauola-pythia6 | 1966.7           | 1915           | 1966.7                | 1871              |
| TTJets_FullLeptMGDecays_8TeV-madgraph                         | 23.64            | —              | 252.89 (all decays!!) | 13.43             |
| WW_TuneZ2star_8TeV_pythia6_tauola                             | 54.8             | 54.8           | 56.0                  | 33.61             |
| WZ_TuneZ2star_8TeV_pythia6_tauola                             | 33.2             | 33.2           | 33.21                 | 12.63             |
| ZZ_TuneZ2star_8TeV_pythia6_tauola                             | 17.7             | 17.7           | 17.0                  | 5.196             |
| T_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola                 | 11.1             | 11.1           | 11.1                  | 10.7              |
| Tbar_tW-channel-DR_TuneZ2star_8TeV-powheg-tauola              | 11.1             | 11.1           | 11.1                  | 10.7              |
| WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball                   | 37509            | 36255.04       | —                     | 30400             |
| DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball              | 3531.9           | —              | 1177.3 * 3            | 2950              |
| DYToEE_M-20_CT10_TuneZ2star_8TeV-powheg-pythia6               | 1966.7           | 1915           | 1966.7                | 1871              |

### TTJets calculations

This is the configuration file used:
https://github.com/cms-sw/genproductions/blob/master/python/EightTeV/Hadronizer_MgmMatchTuneZ2star_8TeV_madgraph_cff.py

The starting cross section is 252.89 pb from the Twiki assuming m(top)=172.5
GeV, default scales muF=muR=mt, and using PDF MSTW2008 NNLO.

The decay we care about it tt -> wwqq -> llnunuqq where l is e, mu, or tau. I
ran a sample through and looked at the genparticles, there are a lot of taus
so it must have this as a decay mode.

We know that t goes to W b 0.91 compared to all w q. So two t going this way
is 0.91^2 = 0.8281. w goes to e, mu, tau at
10.71, 10.63, 11.38. The sum is 0.3272. To have two w go this way is
(0.3272^2) = 0.10705984.

Than the combined tt -> wwbb -> llnunubb = 0.1071 * 0.8281 = 0.0887

Then the cross section is 252.89 * 0.0887 = 22.43 pb

If we instead assume t -> wq = 1.0, then the total cross section is 0.1071 *
252.89 = 27.08 pb.

If instead we use 245.8 pb (mt=173.3), then we get 21.80 and 26.33
respectively.
