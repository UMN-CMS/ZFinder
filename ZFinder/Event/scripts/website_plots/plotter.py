from distutils.spawn import find_executable
from subprocess import call
from sys import exit


class Plotter(object):
    """ A class to interface with C++ plotters on the command line. """

    def __init__(self, input_file, tmp_dir):
        self.input_file = input_file
        self.tmp_dir = tmp_dir
        self.plots = None

    def __check_exe(self):
        """ Check to make sure the specificied exe exists. """
        # Check if the exe exists exists
        if find_executable(self.exe) is None:
            print("Can not find", self.exe)
            exit(2)

    def make_plots(self):
        """ Make the plots using call to pass arguments on the command line.
        """
        # Call the plotter
        retcode = call(self.command)
        if retcode != 0:
            print("Error from", self.exe)
            exit(retcode)

    def __construct_command(self):
        """ A command to be fed into subprocess.call to make the plots """
        self.command = None


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
        self.command = [
                self.exe,
                self.input_file,
                normal_plot,
                ratio_plot,
                ]

        self.plots = [normal_plot, ratio_plot]
