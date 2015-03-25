#include "PlotStyle.h"

// ROOT
#include "TStyle.h"

void SetPlotStyle() {
    TStyle* style = new TStyle("style","Style for P-TDR");

    // For the canvas:
    style->SetCanvasBorderMode(0);
    style->SetCanvasColor(kWhite);
    style->SetCanvasDefX(0);  //Position on screen
    style->SetCanvasDefY(0);

    // For the Pad:
    style->SetPadBorderMode(0);
    style->SetPadColor(kWhite);
    style->SetPadGridX(false);
    style->SetPadGridY(false);
    style->SetGridColor(kBlack);
    style->SetGridStyle(3);
    style->SetGridWidth(1);

    // For the frame:
    style->SetFrameBorderMode(0);
    style->SetFrameBorderSize(1);
    style->SetFrameFillColor(kWhite);
    style->SetFrameFillStyle(0);
    style->SetFrameLineColor(kBlack);
    style->SetFrameLineStyle(1);
    style->SetFrameLineWidth(1);

    // For the histo:
    // style->SetHistFillColor(1);
    style->SetHistFillStyle(0); //
    style->SetHistLineColor(kBlack);
    style->SetHistLineStyle(0);
    style->SetHistLineWidth(1);

    style->SetEndErrorSize(2);
    style->SetErrorX(0.);

    style->SetMarkerStyle(20);

    //For the fit/function:
    style->SetOptFit(1);
    style->SetFitFormat("5.4g");
    style->SetFuncColor(kRed);
    style->SetFuncStyle(1);
    style->SetFuncWidth(1);

    //For the date:
    style->SetOptDate(0);

    // For the statistics box:
    style->SetOptFile(0);
    style->SetOptStat(0);  // To display the mean and RMS: SetOptStat("mr");
    style->SetStatColor(kWhite);
    style->SetStatFont(42);
    style->SetStatFontSize(0.025);
    style->SetStatTextColor(kBlack);
    style->SetStatFormat("6.4g");
    style->SetStatBorderSize(1);
    style->SetStatH(0.1);
    style->SetStatW(0.15);

    // For the Global title:
    style->SetOptTitle(0);
    style->SetTitleFont(42);
    style->SetTitleColor(kBlack);
    style->SetTitleTextColor(kBlack);
    style->SetTitleFillColor(kWhite);  //10 is roughly kWhite, 10% grey?
    style->SetTitleFontSize(0.05);

    // For the axis titles:
    style->SetTitleColor(kBlack, "XYZ");
    style->SetTitleFont(42, "XYZ");
    style->SetTitleSize(0.06, "XYZ");
    style->SetTitleXOffset(0.9);
    style->SetTitleYOffset(1.25);

    // For the axis labels:
    style->SetLabelColor(kBlack, "XYZ");
    style->SetLabelFont(42, "XYZ");
    style->SetLabelOffset(0.007, "XYZ");
    style->SetLabelSize(0.05, "XYZ");

    // For the axis:
    style->SetAxisColor(kBlack, "XYZ");
    style->SetStripDecimals(true);
    style->SetTickLength(0.03, "XYZ");
    style->SetNdivisions(510, "XYZ");
    style->SetPadTickX(true);  // To get tick marks on the opposite side of the frame
    style->SetPadTickY(true);

    // Change for log plots:
    style->SetOptLogx(false);
    style->SetOptLogy(false);
    style->SetOptLogz(false);

    // Set the style
    style->cd();
}
