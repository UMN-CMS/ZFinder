#!/usr/bin/env python

#  Copyright (C) 2015  Alexander Gude - gude@physics.umn.edu
#
#  This program is free software; you can redistribute it and/or modify it
#  under the terms of the GNU General Public License as published by the Free
#  Software Foundation; either version 3 of the License, or (at your option)
#  any later version.
#
#  This program is distributed in the hope that it will be useful, but WITHOUT
#  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
#  more details.
#
#  You should have received a copy of the GNU General Public License along with
#  this program; if not, write to the:
#
#  Free Software Foundation, Inc.,
#  59 Temple Place - Suite 330,
#  Boston, MA  02111-1307, USA.

def get_file_number(file):
    end = file.split("_")[-1]
    number = end.split('.')[0]
    return int(number)

if __name__ == '__main__':
    from optparse import OptionParser
    from os import listdir
    from os.path import isfile
    from sys import exit
    import ROOT

    # Allows command line options to be parsed.
    usage = "usage: %prog directory_1 directory_2"
    parser = OptionParser(usage=usage, version="")
    (options, args) = parser.parse_args()

    if len(args) != 2:
        print "Wrong number of arguments! This script takes exactly two!"
        exit(2)

    # Find the files
    files = [{}, {}]
    for i in (0, 1):
        directory = args[i]
        items = listdir(directory)
        for item in items:
            full_path = directory + "/" + item
            if isfile(full_path) and item.endswith(".root") and "hadded" not in item:
                number = get_file_number(item)
                files[i][number] = full_path

    # Loop over the files in the first directory and compare them to the
    # corresponding file in the second one
    print "Working on: "
    for key, file_0 in files[0].iteritems():
        file_1 = files[1].get(key)
        if file_1:
            print key
            # Open the files and compare the various histograms
            tfile_0 = ROOT.TFile(file_0, "READ")
            tfile_1 = ROOT.TFile(file_1, "READ")
            histo_0 = tfile_0.Get("ZFinder/Combined Single Reco/7 60 < M_{ee} < 120/Z0 Mass: All")
            histo_1 = tfile_1.Get("ZFinder/Combined Single Reco/7 60 < M_{ee} < 120/Z0 Mass: All")
            if histo_0 and histo_1:
                number_0 = histo_0.GetEntries()
                number_1 = histo_1.GetEntries()
                if number_0 != number_1:
                    print "Found mismatch!"
                    print file_0, "has", number_0, "entries"
                    print file_1, "has", number_1, "entries"
            else:
                print "FAILURE", file_0, file_1
