#!/usr/bin/env python

from sys import argv, exit
from os.path import isfile
from math import log10, floor

class EfficiencyFormatter:

    def __init__(self, file_name):
        """ Set up the class and read in the data. """
        # Read the file
        if not isfile(file_name):
            exit("Input file isn't a valid file!")
        else:
            with open(file_name) as f:
                data = f.read()
                f.close
            self.lines = data.splitlines()
        # The object to store the data in
        self.table = {}
        self.__get_eta_bins()
        self.__fill_table()

    def __get_eta_bins(self):
        """ Set up the top level keys of self.table to be (lower_eta_edge,
        higher_eta_edge) """
        lines = (l for l in self.lines if l.startswith("Eta Bin Edges: "))
        for line in lines:
            eta_key = self.__return_key(line)
            self.table[eta_key] = {}

    def __return_key(self, line):
        """ Gets the eta key from an eta line. """
        numbers = line.split(":")[1].strip()
        snumbers = numbers.split(",")
        low = snumbers[0].strip()
        high = snumbers[1].strip()
        return (low, high)

    def __return_efficiency(self, line):
        """ Returns a tuple containing:
            (Eff, high_err, low_err)
        """
        sline = line.split()
        eff = float(sline[0].strip())
        pos = float(sline[2].strip())
        neg = float(sline[4].strip())
        return self.__round_results(eff, pos, neg)

    def __round_results(self, eff, pos, neg):
        """ Find the furthest right of the most significant digit of pos and
        neg, and then round all three numbers to that precision and return it
        as a string. """
        if pos:
            pos_digit = -int(floor(log10(pos)))
        else:
            pos_digit = 0
        if neg:
            neg_digit = -int(floor(log10(neg)))
        else:
            neg_digit = 0

        round_digit = max(pos_digit, neg_digit)

        # Dynamically format the number of digits to keep in the string. We
        # must do this so that we display 0.1 as 0.100 if we have that much
        # accuracy.
        format_string = "{number:.{digit}f}"

        rounded_eff = format_string.format(digit=round_digit, number=eff)
        rounded_pos = format_string.format(digit=round_digit, number=pos)
        rounded_neg = format_string.format(digit=round_digit, number=neg)

        return (rounded_eff, rounded_pos, rounded_neg)

    def __fill_table(self):
        """ Fill self.table from self.lines """
        current_eta_key = None
        current_pt_key = None
        for line in self.lines:
            # Eta bin line
            if line.startswith("Eta Bin Edges: "):
                current_eta_key = self.__return_key(line)
            # PT bin line
            elif current_eta_key and line.startswith("Pt Bin Edges: "):
                current_pt_key = self.__return_key(line)
            # Efficiency Line
            elif current_eta_key and current_pt_key:
                (eff, pos, neg) = self.__return_efficiency(line)
                self.table[current_eta_key][current_pt_key] = (eff, pos, neg)

    def to_tex(self, skip_last_pt = True):
        """ Output a LaTeX table. """
        # Get a list of keys sorted
        sorted_eta_keys = sorted(self.table.keys(), key=lambda a:float(a[0]))
        pt_keys = []
        for key in sorted_eta_keys:
            pt_keys += self.table[key].keys()
        uniq_pt_keys = list(set(pt_keys))
        sorted_pt_keys = sorted(uniq_pt_keys, key=lambda a:float(a[0]))

        # Make the Header
        header = "$\eta$ "
        for key in sorted_pt_keys:
            # Sometimes we want to skip the last bin
            if skip_last_pt:
                if key == sorted_pt_keys[-1]:
                    continue
            header += "& %s---%s \GeV "%key
        header += " \\\\ \hline"

        # Make the Rows
        rows = []
        for key in sorted_eta_keys:
            table = self.table[key]
            row = "$\\numrange{%s}{%s}$ "%key
            for pt_key in sorted_pt_keys:
                # Sometimes we want to skip the last bin
                if skip_last_pt:
                    if pt_key == sorted_pt_keys[-1]:
                        continue

                row += "& $%s^{+%s}_{-%s}$ "%(table[pt_key])
            row += " \\\\ \hline"
            rows.append(row)
        return [header] + rows

    def to_cpp(self):
        """ Output c++ code for ZTriggerEfficiencies.cc """
        rows = []
        for eta_key in self.table:
            for pt_key in self.table[eta_key]:
                (eff, pos, neg) = self.table[eta_key][pt_key]
                format_str = ".AddEfficiency({low_pt}, {high_pt}, {low_eta}, {high_eta}, {eff}, {pos}, {neg});"
                row = format_str.format( low_pt = pt_key[0], high_pt = pt_key[1], low_eta = eta_key[0], high_eta = eta_key[1], eff = eff, pos = pos, neg = neg)
                rows.append(row)
        return sorted(rows)

if __name__ == "__main__":
    formatter = EfficiencyFormatter(argv[1])
    print '\n'.join(formatter.to_cpp())
