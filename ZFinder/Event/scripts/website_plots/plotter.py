from distutils.spawn import find_executable
from subprocess import call
from sys import exit


#COLORS
COLORS = {
        "kRed" : 632,
        "kBlue" : 600,
        }


class Plotter(object):
    """ A class to interface with C++ plotters on the command line. """

    def __init__(self, input_file, tmp_dir):
        self.input_file = input_file
        self.tmp_dir = tmp_dir

    def __check_exe(self):
        """ Check to make sure the specified exe exists. """
        # Check if the exe exists exists
        if find_executable(self.exe) is None:
            print("Can not find", self.exe)
            exit(2)

    def make_plots(self):
        """ Make the plots using call to pass arguments on the command line.
        """
        # Call the plotter
        for command in self.commands:
            retcode = call(command)
            # 0 is ok, 2 is "no plot to make"
            if retcode not in (0, 2):
                print("Error from", self.exe)
                exit(retcode)

    def __construct_command(self):
        """ A command to be fed into subprocess.call to make the plots """
        self.commands = None


class CutByCut(Plotter):

    def __init__(self, input_file, tmp_dir):
        # Set the exe
        self.exe = "./plotters/cut_by_cut_plotter.exe"

        # Call the parent constructor
        super(CutByCut, self).__init__(input_file, tmp_dir)

        # Set up the command to make the plots
        self.__construct_command()

    def __construct_command(self):
        # Otherwise make the plots
        normal_plot = self.tmp_dir + "cut_by_cut_normal.png"
        ratio_plot = self.tmp_dir + "cut_by_cut_ratio.png"

        # Put together the plotting command
        self.commands = (
                [
                    self.exe,
                    self.input_file,
                    normal_plot,
                    ratio_plot,
                ],
            )


class BasicPlotter(Plotter):

    def __init__(self, input_file, tmp_dir, directory, plot_mc=False):
        # Set the exe
        self.exe = "./plotters/basic_plotter.exe"

        # The directory in the TFile to open
        self.directory = directory

        # If plot_mc, use the MC quantities
        self.plot_mc = plot_mc

        # Call the parent constructor
        super(BasicPlotter, self).__init__(input_file, tmp_dir)

        # Set up the command to make the plots
        self.__plot_dictionaries()
        self.__construct_command()

    def __construct_command(self):
        # Put together the plotting command
        self.commands = []
        for d in self.plot_dictionaries:
            # Set up the name of the output png
            plot_file = self.tmp_dir + d["png_name"]
            if self.plot_mc:
                plot_file += "_mc"
            plot_file += ".png"

            # Set up the name of the histogram in the input file
            histogram_name = self.directory + d["histo_name"]

            # Set up colors
            color = str(COLORS["kBlue"])
            if self.plot_mc:
                color = str(COLORS["kRed"])

            self.commands.append(
                [
                    self.exe,
                    self.input_file,
                    plot_file,
                    histogram_name,
                    color,
                ]
            )

    def __plot_dictionaries(self):
        """ Set up the horrible hardcoded information. """
        self.plot_dictionaries = [
                {
                    "histo_name" : "Z0 Mass: All",
                    "png_name" : "z_mass_all",
                },
                {
                    "histo_name" : "Z0 Mass: Coarse",
                    "png_name" : "z_mass_coarse",
                },
                {
                    "histo_name" : "Z0 Mass: Fine",
                    "png_name" : "z_mass_fine",
                },
                {
                    "histo_name" : "Z0 Rapidity",
                    "png_name" : "z_rapidity",
                },
                {
                    "histo_name" : "Z0 p_{T}",
                    "png_name" : "z_pt",
                },
                {
                    "histo_name" : "p_{T,e_{0}}",
                    "png_name" : "e0_pt",
                },
                {
                    "histo_name" : "p_{T,e_{0},Naked}",
                    "png_name" : "e0_pt_naked",
                },
                {
                    "histo_name" : "p_{T,e_{0},Born}",
                    "png_name" : "e0_pt_born",
                },
                {
                    "histo_name" : "p_{T,e_{1}}",
                    "png_name" : "e1_pt",
                },
                {
                    "histo_name" : "p_{T,e_{1},Naked}",
                    "png_name" : "e1_pt_naked",
                },
                {
                    "histo_name" : "p_{T,e_{1},Born}",
                    "png_name" : "e1_pt_born",
                },
                {
                    "histo_name" : "#eta_{e_{0}}",
                    "png_name" : "e0_eta",
                },
                {
                    "histo_name" : "#eta_{e_{0},Naked}",
                    "png_name" : "e0_eta_nake",
                },
                {
                    "histo_name" : "#eta_{e_{0},Born}",
                    "png_name" : "e0_eta_born",
                },
                {
                    "histo_name" : "#eta_{e_{1}}",
                    "png_name" : "e1_eta",
                },
                {
                    "histo_name" : "#eta_{e_{1},Naked}",
                    "png_name" : "e1_eta_naked",
                },
                {
                    "histo_name" : "#eta_{e_{1},Born}",
                    "png_name" : "e1_eta_born",
                },
                #{
                #    "histo_name" : "#phi_{e_{0}}",
                #    "png_name" : "e0_phi",
                #},
                #{
                #    "histo_name" : "#phi_{e_{0},Naked}",
                #    "png_name" : "e0_phi_naked",
                #},
                #{
                #    "histo_name" : "#phi_{e_{0},Born}",
                #    "png_name" : "e0_phi_born",
                #},
                #{
                #    "histo_name" : "#phi_{e_{1}}",
                #    "png_name" : "e1_phi",
                #},
                #{
                #    "histo_name" : "#phi_{e_{1},Naked}",
                #    "png_name" : "e1_phi_naked",
                #},
                #{
                #    "histo_name" : "#phi_{e_{1},Born}",
                #    "png_name" : "e1_phi_born",
                #},
                #{
                #    "histo_name" : "charge_{e_{0}}",
                #    "png_name" : "e0_charge",
                #},
                #{
                #    "histo_name" : "charge_{e_{1}}",
                #    "png_name" : "e1_charge",
                #},
                {
                    "histo_name" : "#phi*",
                    "png_name" : "phistar",
                },
                {
                    "histo_name" : "Born #phi*",
                    "png_name" : "phistar_born",
                },
                {
                    "histo_name" : "Naked #phi*",
                    "png_name" : "phistar_naked",
                },
                {
                    "histo_name" : "Other #phi*",
                    "png_name" : "phistar_other",
                },
                {
                    "histo_name" : "Other Rapidity",
                    "png_name" : "z_rapidity_other",
                },
                {
                    "histo_name" : "N_{Vertices}",
                    "png_name" : "vertices",
                },
                {
                    "histo_name" : "N_{e}",
                    "png_name" : "number_of_electrons",
                },
                {
                    "histo_name" : "Base Weight",
                    "png_name" : "base_weight",
                },
                {
                    "histo_name" : "Full Weight",
                    "png_name" : "full_weight",
                },
                {
                    "histo_name" : "p_{T,e_{0}} Vs. Trigger",
                    "png_name" : "e0_pt_vs_trigger",
                },
                {
                    "histo_name" : "p_{T,e_{1}} Vs. Trigger",
                    "png_name" : "e1_pt_vs_trigger",
                },
                {
                    "histo_name" : "#phi*: Reco Vs. Truth",
                    "png_name" : "phistar_reco_vs_truth",
                },
                {
                    "histo_name" : "#DeltaR",
                    "png_name" : "delta_r",
                },
                ]
