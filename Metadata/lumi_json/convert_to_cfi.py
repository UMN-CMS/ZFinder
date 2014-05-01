#!/usr/bin/env python

from sys import argv
from json import load

input_file = argv[1]

with open(input_file) as open_file:
    lumi_json = load(open_file)

print lumi_json
for run in lumi_json:
    for (start_lumi, end_lumi) in lumi_json[run]:
        print '"%i:%i-%i:%i",' % (int(run), start_lumi, int(run), end_lumi)
