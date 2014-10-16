#!/usr/bin/env python

from tempfile import mkdtemp
from sys import exit
from os import makedirs
from os.path import isdir, exists, basename
from shutil import move, rmtree
import plotter


##### START OF CODE
if __name__ == '__main__':
    # Allows command line options to be parsed.
    from optparse import OptionParser  # Command line parsing

    usage = "usage: %prog -i root_file -o output_directory"
    parser = OptionParser(usage=usage)
    parser.add_option("-i", "--input", action="store", dest="input", default=None, help="the input rootfile containing histograms")
    parser.add_option("-o", "--output", action="store", dest="output", default=None, help="the output directory, if it doesn't exist it will be made")
    parser.add_option("-m", "--is-mc", action="store_true", dest="is_mc", default=False, help="is the file MC")

    (options, args) = parser.parse_args()

    # Get the inputfiles
    input_file = options.input
    output_dir = options.output

    # Make a temporary scratch directory
    tmp_dir = mkdtemp() + '/'
    print(tmp_dir)

    # Store a list of the plots
    plots = []

    # Make the plotters
    plotters = []

    if options.is_mc:
        plotters.append(plotter.CutByCut(input_file, tmp_dir))

    # Run the plotters
    for p in plotters:
        p.make_plots()
        plots += p.plots

    # Make the website directory
    if not exists(output_dir):
        makedirs(output_dir)
    elif not isdir(output_dir):
        print("Output directory already exists and is not a directory!")
        exit(2)

    # Move the plots to the output directory
    for plot in plots:
        move(plot, output_dir + "/" + basename(plot))

    # Clean up our temp directory
    rmtree(tmp_dir)
