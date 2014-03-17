#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif

#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooGenericPdf.h"
#include "RooDataSet.h"
#include "RooAbsData.h"
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooWorkspace.h"
#include "RooPlot.h"
#include <TH2D.h>
#include <TH1D.h>
#include <TFile.h>
#include <TCanvas.h>

#include <sstream>
#include <string>

#include <iostream>
#include <fstream>
using namespace RooFit;
//using namespace RooStats;

void RooMerger(const std::string dirname, int njobs, const char* outputfile){
  stringstream ss;
  ss<<0;
  std::string num = ss.str();
  std::string zero = ss.str();
  
  std::string filename= dirname+zero+zero+num+".root";
  std::cout<<filename<<std::endl;
  const char *fzero=filename.c_str();
  TFile * f= new TFile(fzero, "READ");
  RooWorkspace* w =(RooWorkspace*) f->Get("ZFinder/w");
  RooDataSet& mc_reco_merged = *((RooDataSet*) w->data("mc_reco_dataset") );
  RooDataSet& data_reco_merged = *((RooDataSet*) w->data("data_reco_dataset") );
  RooDataSet& mc_truth_merged = *((RooDataSet*) w->data("mc_truth_dataset") );
  f->Close();
  for (int i=1; i<njobs;i++){
    //    if (i==56)continue;
    stringstream ssnum;
    ssnum<<i;
    num = ssnum.str();
    filename=dirname;
    if (i<100){filename=filename+zero;}
    if (i<10){filename=filename+zero;}
    filename=filename+num+".root";
    std::cout<<filename<<std::endl;
    const char *fnum=filename.c_str();
    f= new TFile(fnum, "READ");
    w =(RooWorkspace*) f->Get("ZFinder/w");
    RooDataSet& MC_reco = *((RooDataSet*) w->data("mc_reco_dataset") );
    RooDataSet& Data_reco = *((RooDataSet*) w->data("data_reco_dataset") );
    RooDataSet& MC_true_all = *((RooDataSet*) w->data("mc_truth_dataset") );
    mc_reco_merged.append(MC_reco);
    data_reco_merged.append(Data_reco);
    mc_truth_merged.append(MC_true_all);
    f->Close();
  }
  RooWorkspace* w_merged =new RooWorkspace("w_merged","workspace");
  w_merged->import(mc_reco_merged);
  w_merged->import(data_reco_merged);
  w_merged->import(mc_truth_merged);
  w_merged->writeToFile(outputfile);
}
