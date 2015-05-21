#!/usr/bin/env python

from style import set_plot_style
import ROOT
from sys import argv

set_plot_style()

input_file = argv[1]

# Open the toy MC file and get the histograms
in_tfile = ROOT.TFile(input_file, "READ")
unsmeared_phistar = in_tfile.unsmeared_phistar
unsmeared_phistar.Sumw2()
smeared_phistar = in_tfile.smeared_phistar
smeared_phistar.Sumw2()
unsmeared_pt = in_tfile.unsmeared_pt
unsmeared_pt.Sumw2()
smeared_pt = in_tfile.smeared_pt
smeared_pt.Sumw2()

def redraw_border():
   ROOT.gPad.Update();
   ROOT.gPad.RedrawAxis();

def make_plot(UNSMEARED, SMEARED, OUT, RATIO_OFFSET, X_TITLE, Y_TITLE, LOG=False):

    UNSMEARED.Scale(1, "width")
    SMEARED.Scale(1, "width")

    canvas = ROOT.TCanvas("canvas", "canvas", 800, 800)
    canvas.Divide(1,2)

    # Top plot
    canvas.cd(1)
    ROOT.gPad.SetPad(0, 0.30, 1., 1.)
    ROOT.gPad.SetBottomMargin(0.01)
    if LOG:
        ROOT.gPad.SetLogy(True)
        ROOT.gPad.SetLogx(True)

    UNSMEARED.SetLineColor(ROOT.kBlue)
    UNSMEARED.GetYaxis().SetTitle(Y_TITLE)
    UNSMEARED.GetYaxis().SetTitleOffset(1.5)
    UNSMEARED.SetLineWidth(2)
    SMEARED.SetMarkerStyle(ROOT.kFullCircle)

    UNSMEARED.Draw("Hist")
    SMEARED.Draw("SAME E")

    X1 = 0.75
    X2 = 0.92
    Y1 = 0.72
    Y2 = 0.92
    legend = ROOT.TLegend(X1, Y1, X2, Y2)
    NO_BORDER = 0
    legend.SetBorderSize(NO_BORDER)
    NO_FILL = 0
    legend.SetFillStyle(NO_FILL)
    legend.AddEntry(UNSMEARED, "Truth", "l")
    legend.AddEntry(SMEARED, "Reco", "p")
    legend.Draw()

    cms_latex = ROOT.TLatex(0.10 + 0.015, 0.95 + 0.01, "Toy MC");
    cms_latex.SetNDC(True)
    cms_latex.SetTextSize(0.05);
    cms_latex.Draw()
    redraw_border()

    # Ratio
    canvas.cd(2)
    ROOT.gPad.SetPad(0, 0.1, 1., 0.30)
    ROOT.gPad.SetTopMargin(0)
    if LOG:
        ROOT.gPad.SetLogx(True)

    ratio = SMEARED.Clone("ratio")
    ratio.Divide(UNSMEARED)
    ratio.SetMarkerStyle(ROOT.kFullCircle)
    ratio.GetXaxis().SetTitle(X_TITLE)
    ratio.GetYaxis().SetTitle("Reco / Truth")
    ratio.GetXaxis().SetLabelSize(0.1)
    ratio.GetXaxis().SetTitleSize(0.2)
    ratio.GetXaxis().SetTitleOffset(0.8)
    ratio.GetYaxis().SetLabelSize(0.1)
    ratio.GetYaxis().SetTitleSize(0.1)
    ratio.GetYaxis().SetTickLength(0.01)
    OFFSET = RATIO_OFFSET + 0.03
    ratio.SetMaximum(1. + OFFSET)
    ratio.SetMinimum(1. - OFFSET)

    ratio_line = ROOT.TF1("line", "pol0", ratio.GetBinLowEdge(1), ratio.GetBinLowEdge(ratio.GetNbinsX()) + ratio.GetBinWidth(ratio.GetNbinsX()))
    ratio_line.SetParameter(0, 1)
    ratio_line.SetLineColor(ROOT.kRed)

    ratio.Draw("E")
    ratio_line.Draw("SAME")
    ratio.Draw("E SAME")
    redraw_border()

    # Save a PDF
    canvas.Print(OUT, "pdf")

# Make the plots
make_plot(unsmeared_phistar, smeared_phistar, "toy_phistar.pdf", 0.4, "#phi*", "Events / Unit #phi*", LOG=True)
make_plot(unsmeared_pt, smeared_pt, "toy_pt.pdf", 0.4, "P_{T} [GeV]", "Events / GeV")
