# Running with PDF uncertainties

To compile you'll need to do the following:

1. scram setup lhapdffull
2. touch $CMSSW_BASE/src/ElectroWeakAnalysis/Utilities/BuildFile.xml
3. cmsenv
4. scram b -j

After this everything should compile.

More information can be found on the twiki:
https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideEWKUtilities#PDF_SYSTEMATICS_PDFWeightProduce

## Running on CRAB

The twiki states:

> Jobs should run via CRAB if you are using the "lhapdffull" libraries. But as
> of today, CRAB does not pass properly the necessary environment. We have
> checked that a way to circumvent this is to add the following two commands to
> the CMSSW.sh executable in the local crab_.../job/ subdirectory before
> submitting the jobs: 
>
> scramv1 setup lhapdffull
> scramv1 b
>
> You can add them for instance after the 'eval `scramv1 runtime -sh ...`'
> command.

This works fine.
