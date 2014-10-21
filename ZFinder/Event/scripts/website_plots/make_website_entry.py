#!/usr/bin/env python

from tempfile import mkdtemp
from sys import exit
from os import makedirs, listdir
from os.path import isdir, exists, basename, isfile
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

    # The directories used by the basic plots
    basic_data_dir = "ZFinder/Combined Single Reco/7 60 < M_{ee} < 120/"
    basic_mc_dir = basic_data_dir.replace("Reco", "MC")
    plot_mc = True

    # Data plots (and reco MC)
    plotters.append(plotter.BasicPlotter(input_file, tmp_dir, basic_data_dir))

    # MC plots
    if options.is_mc:
        plotters.append(plotter.CutByCut(input_file, tmp_dir))
        plotters.append(plotter.BasicPlotter(input_file, tmp_dir, basic_mc_dir, plot_mc))


    # Run the plotters
    for p in plotters:
        p.make_plots()

    # Make the website directory
    if not exists(output_dir):
        makedirs(output_dir)
    elif not isdir(output_dir):
        print("Output directory already exists and is not a directory!")
        exit(2)

    # Move the plots to the output directory
    plots = [tmp_dir + '/' + p for p in listdir(tmp_dir) if (isfile(tmp_dir + '/' + p) and p.endswith(".png"))]
    for plot in plots:
        move(plot, output_dir + "/" + basename(plot))

    # Clean up our temp directory
    rmtree(tmp_dir)
