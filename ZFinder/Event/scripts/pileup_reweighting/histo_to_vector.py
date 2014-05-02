#!/usr/bin/env python

import ROOT
from ROOT import RooStats
from sys import exit, argv

in_file = argv[1]
tfile = ROOT.TFile(in_file)
histo = tfile.Get("pileup")
for i in xrange(1, histo.GetNbinsX()+1):
    print i, histo.GetBinContent(i)

