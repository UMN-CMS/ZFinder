import ROOT

def set_plot_style():
    style = ROOT.TStyle("style", "Style for P-TDR")

    # For the canvas:
    style.SetCanvasBorderMode(0)
    style.SetCanvasColor(ROOT.kWhite)
    style.SetCanvasDefX(0)  #Position on screen
    style.SetCanvasDefY(0)

    # For the Pad:
    style.SetPadBorderMode(0)
    style.SetPadColor(ROOT.kWhite)
    style.SetPadGridX(False)
    style.SetPadGridY(False)
    style.SetGridColor(ROOT.kBlack)
    style.SetGridStyle(3)
    style.SetGridWidth(1)

    # For the frame:
    style.SetFrameBorderMode(0)
    style.SetFrameBorderSize(1)
    style.SetFrameFillColor(ROOT.kWhite)
    style.SetFrameFillStyle(0)
    style.SetFrameLineColor(ROOT.kBlack)
    style.SetFrameLineStyle(1)
    style.SetFrameLineWidth(1)

    # For the histo:
    # style.SetHistFillColor(1)
    style.SetHistFillStyle(0) #
    style.SetHistLineColor(ROOT.kBlack)
    style.SetHistLineStyle(0)
    style.SetHistLineWidth(1)

    style.SetEndErrorSize(2)
    style.SetErrorX(0.)

    style.SetMarkerStyle(20)

    #For the fit/function:
    style.SetOptFit(1)
    style.SetFitFormat("5.4g")
    style.SetFuncColor(ROOT.kRed)
    style.SetFuncStyle(1)
    style.SetFuncWidth(1)

    #For the date:
    style.SetOptDate(0)

    # For the statistics box:
    style.SetOptFile(0)
    style.SetOptStat(0)  # To display the mean and RMS: SetOptStat("mr")
    style.SetStatColor(ROOT.kWhite)
    style.SetStatFont(42)
    style.SetStatFontSize(0.025)
    style.SetStatTextColor(ROOT.kBlack)
    style.SetStatFormat("6.4g")
    style.SetStatBorderSize(1)
    style.SetStatH(0.1)
    style.SetStatW(0.15)

    # Margins:
    style.SetPadTopMargin(1 - 0.95)
    style.SetPadBottomMargin(0.35)
    style.SetPadLeftMargin(0.12)
    style.SetPadRightMargin(1 - 0.96)

    # For the Global title:
    style.SetOptTitle(0)
    style.SetTitleFont(42)
    style.SetTitleColor(ROOT.ROOT.kBlack)
    style.SetTitleTextColor(ROOT.kBlack)
    style.SetTitleFillColor(ROOT.kWhite)  #10 is roughly ROOT.kWhite, 10% grey?
    style.SetTitleFontSize(0.05)

    # For the axis titles:
    style.SetTitleColor(ROOT.kBlack, "XYZ")
    style.SetTitleFont(42, "XYZ")
    style.SetTitleSize(0.06, "XYZ")
    style.SetTitleXOffset(0.9)
    style.SetTitleYOffset(1.25)

    # For the axis labels:
    style.SetLabelColor(ROOT.kBlack, "XYZ")
    style.SetLabelFont(42, "XYZ")
    style.SetLabelOffset(0.007, "XYZ")
    style.SetLabelSize(0.05, "XYZ")

    # For the axis:
    style.SetAxisColor(ROOT.kBlack, "XYZ")
    style.SetStripDecimals(True)
    style.SetTickLength(0.03, "XYZ")
    style.SetNdivisions(510, "XYZ")
    style.SetPadTickX(True)  # To get tick marks on the opposite side of the frame
    style.SetPadTickY(True)

    # Change for log plots:
    style.SetOptLogx(False)
    style.SetOptLogy(False)
    style.SetOptLogz(False)

    # Set the style
    style.cd()

    return style
