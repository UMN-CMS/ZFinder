#!/usr/bin/env python

from sys import argv

lines = []

def print_csv_line(lines):
    event_number = int(lines[0].split()[1])
    pt_eta_phi0 = lines[1].split()[3].split('/')
    pt_eta_phi1 = lines[2].split()[3].split('/')
    pt0 = float(pt_eta_phi0[0])
    eta0 = float(pt_eta_phi0[1])
    phi0 = float(pt_eta_phi0[2])
    pt1 = float(pt_eta_phi1[0])
    eta1 = float(pt_eta_phi1[1])
    phi1 = float(pt_eta_phi1[2])

    print str(event_number)+",",
    print str(pt0)+",",
    print str(eta0)+",",
    print str(phi0)+",",
    print str(pt1)+",",
    print str(eta1)+",",
    print str(phi1)+",",
    print str(-1)  # India does not include the mass

# Open the file and convert
with open(argv[1], 'r') as f:
    first_dashes = True
    for line in f:
        # Starting a new block
        if line.startswith("--"):
            # If we have lines, then print the information contained in them
            if lines:
                print_csv_line(lines)
            # Clear our list, we're about to enter a new block of information
            lines = []
        # All other lines contain information, so save them
        else:
            lines.append(line)
