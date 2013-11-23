# ZFinder

## Setting up a working area

To set up a working area, run the following commands:

```bash
scram project CMSSW CMSSW_5_3_13
cd CMSSW_5_3_13
git clone https://github.com/UMN-CMS/ZFinder.git src/
cmsenv
cd src
scram build -j 4
```
