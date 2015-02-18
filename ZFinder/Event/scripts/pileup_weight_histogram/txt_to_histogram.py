#!/usr/bin/env python

import ROOT
from sys import argv


input_text_file = argv[1]
input_root_file = argv[2]
output_file = argv[3]

# Open India's histogram
input_tfile = ROOT.TFile(input_root_file, "READ")
india_histo = input_tfile.Get("h1")

# Open the output file
output_tfile = ROOT.TFile(output_file, "RECREATE")
output_tfile.cd()

# Make our histo and fill it
our_histo = ROOT.TH1D("our_weights", "our_weights", 60, 0, 60);
our_values = []
with open(input_text_file) as f:
    for line in f:
        weight = float(line.split()[1])
        our_values.append(weight)

for i in xrange(0, 59):
    root_bin = i+1
    our_histo.SetBinContent(root_bin, our_values[i])

# Save everything
canvas = ROOT.TCanvas("Canvas")
canvas.cd()
our_histo.Draw("E")
india_histo.Draw("E same")
canvas.Write()

output_tfile.Write()
output_tfile.Close()
