# Running with PDF uncertainties

You will need to modify the file the tells CMSSW where to find pdfs:

    vi $CMSSW_BASE/config/toolbox/$SCRAM_ARCH/tools/selected/lhapdffull.xml

Change it to read as follows (for CMSSW_5_3):

```
<tool name="lhapdffull" version="5.9.1"> <!-- Note the new version: 5.9.1 -->
  <lib name="LHAPDF"/>
  <client>
    <environment name="LHAPDFFULL_BASE" default="/cvmfs/cms.cern.ch/slc5_amd64_gcc462/external/lhapdf/5.9.1"/> <!-- Note the new version: 5.9.1 -->
    <environment name="LIBDIR" default="$LHAPDFFULL_BASE/full/lib"/>
    <environment name="INCLUDE" default="$LHAPDFFULL_BASE/include"/>
  </client>
  <runtime name="LHAPATH" value="$LHAPDFFULL_BASE/share/lhapdf/PDFsets"/>
  <use name="f77compiler"/>
</tool>

```

Then, to compile you'll need to do the following:

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
