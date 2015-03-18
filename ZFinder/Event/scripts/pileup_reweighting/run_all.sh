#!/bin/bash

# Normal
./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012ABCD.json --inputLumiJSON ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt --minBiasXsec 69400 --calcMode true --numPileupBins 60 --maxPileupBin 60 output_2012ABCD_v2.root
./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012A.json --inputLumiJSON ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt --minBiasXsec 69400 --calcMode true --numPileupBins 60 --maxPileupBin 60 output_2012A_v2.root
./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012B.json --inputLumiJSON ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt --minBiasXsec 69400 --calcMode true --numPileupBins 60 --maxPileupBin 60 output_2012B_v2.root
./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012C.json --inputLumiJSON ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt --minBiasXsec 69400 --calcMode true --numPileupBins 60 --maxPileupBin 60 output_2012C_v2.root
./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012D.json --inputLumiJSON ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt --minBiasXsec 69400 --calcMode true --numPileupBins 60 --maxPileupBin 60 output_2012D_v2.root

# Plus 5%
./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012ABCD.json --inputLumiJSON ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt --minBiasXsec 72870 --calcMode true --numPileupBins 60 --maxPileupBin 60 output_2012ABCD_p5_v2.root
./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012A.json --inputLumiJSON ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt --minBiasXsec 72870 --calcMode true --numPileupBins 60 --maxPileupBin 60 output_2012A_p5_v2.root
./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012B.json --inputLumiJSON ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt --minBiasXsec 72870 --calcMode true --numPileupBins 60 --maxPileupBin 60 output_2012B_p5_v2.root
./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012C.json --inputLumiJSON ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt --minBiasXsec 72870 --calcMode true --numPileupBins 60 --maxPileupBin 60 output_2012C_p5_v2.root
./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012D.json --inputLumiJSON ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt --minBiasXsec 72870 --calcMode true --numPileupBins 60 --maxPileupBin 60 output_2012D_p5_v2.root

# Minus 5%
./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012ABCD.json --inputLumiJSON ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt --minBiasXsec 65930 --calcMode true --numPileupBins 60 --maxPileupBin 60 output_2012ABCD_m5_v2.root
./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012A.json --inputLumiJSON ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt --minBiasXsec 65930 --calcMode true --numPileupBins 60 --maxPileupBin 60 output_2012A_m5_v2.root
./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012B.json --inputLumiJSON ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt --minBiasXsec 65930 --calcMode true --numPileupBins 60 --maxPileupBin 60 output_2012B_m5_v2.root
./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012C.json --inputLumiJSON ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt --minBiasXsec 65930 --calcMode true --numPileupBins 60 --maxPileupBin 60 output_2012C_m5_v2.root
./pileupCalc.py -i ../../../../Metadata/lumi_json/Run2012D.json --inputLumiJSON ../../../../Metadata/pileup_json/pileup_JSON_DCSONLY_190389-208686_All_2012_pixelcorr_v2.txt --minBiasXsec 65930 --calcMode true --numPileupBins 60 --maxPileupBin 60 output_2012D_m5_v2.root
