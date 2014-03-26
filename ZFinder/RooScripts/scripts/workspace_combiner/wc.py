import ROOT
from ROOT import RooStats
from sys import exit, argv
from os.path import isfile


if __name__ == '__main__':
    # Allows command line options to be parsed.
    from optparse import OptionParser  # Command line parsing

    usage = "usage: %prog [Options] workspace_location output_file input_files"
    version = "%prog\n\nCopyright (C) 2014 Alexander Gude - gude@physics.umn.edu\nThis is free software.  You may redistribute copies of it under the terms of\nthe GNU General Public License <http://www.gnu.org/licenses/gpl.html>.\nThere is NO WARRANTY, to the extent permitted by law.\n\nWritten by Alexander Gude."
    parser = OptionParser(usage=usage, version=version)
    parser.add_option("-v", "--verbose", action="store_const", const=1, dest="verbosity", default=0, help="print some extra status messages to stdout [default false]")
    parser.add_option("-q", "--quite", action="store_const", const=0, dest="verbosity", default=0, help="do not print any status messages to stdout [default true]")
    parser.add_option("-V", "--very-verbose", action="store_const", const=2, dest="verbosity", default=0, help="print everything [default false]")
    parser.add_option("-f", "--force-overwrite", action="store_true", dest="force_overwrite", default=False, help="Overwrite the output file if it exists [default false]")

    (options, args) = parser.parse_args()

    ## Check that we have at least a few files to work on
    in_files = []
    out_file = None
    if len(args) <= 2:
        print "Not enough arguments on the command line. Exiting."
        exit(2)  # Not enough commands
    else:
        ws_loc = args[0]
        out_file = args[1]
        in_files = args[2:]

    # Exit if the output file exists and we have not set overwrite
    if isfile(out_file) and not options.force_overwrite:
        exit(out_file + " already exists! Exiting!")

    # We grab the dataset from the first file
    if options.verbosity >= 1:
        print("Working on first file: ", file)
    first_file = in_files.pop(0)
    tfile = ROOT.TFile(first_file)
    ws = tfile.Get(ws_loc)
    out_dataset = ws.data("roo_dataset")

    for file in in_files:
        if options.verbosity >= 1:
            print("Working on file: ", file)
        tfile = ROOT.TFile(file)
        ws = tfile.Get(ws_loc)
        ds = ws.data("roo_dataset")
        out_dataset.append(ds)

    if options.verbosity >= 2:
        out_dataset.Print("V")

    out_space = ROOT.RooWorkspace("workspace", "Output combined workspace")
    getattr(out_space, 'import')(out_dataset)  # import is a reserved word, so we can't say out_space.import()
    # Save our new workspace to a file
    if options.verbosity >= 1:
        print "Saving file:", out_file
    recreate_file = not options.force_overwrite
    out_space.writeToFile(out_file, recreate_file)
