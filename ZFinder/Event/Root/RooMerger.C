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

void RooMerger(const std::string dirname, int njobs, std::string baseoutputfile, const std::string selectionname = ""){
  int start=0;
  int entriesmax=600000;
  stringstream ss;
  ss<<0;
  std::string num = ss.str();
  std::string zero = ss.str();

  while (start<njobs-1){

    std::string filename="";
    stringstream sstemp;
    sstemp<<start;
    if (start<10){filename= dirname+zero+zero+sstemp.str()+".root";}
    else if (start<100){filename= dirname+zero+sstemp.str()+".root";}
    else {filename= dirname+sstemp.str()+".root";}

    std::cout<<filename<<std::endl;
    const char *fzero=filename.c_str();
    TFile * f= new TFile(fzero, "READ");
    std::string workspacedirname="ZFinder/"+selectionname+"/workspace";
    const char *workspacedir=workspacedirname.c_str();

    RooWorkspace* w =(RooWorkspace*) f->Get(workspacedir);

    RooDataSet&  roo_dataset= *((RooDataSet*) w->data("roo_dataset") );
    f->Close();
    for (int i=start+1; i<njobs && roo_dataset.numEntries()<entriesmax;i++){
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
      w =(RooWorkspace*) f->Get(workspacedir);
      RooDataSet&  roo_dataset_temp= *((RooDataSet*) w->data("roo_dataset") );
      roo_dataset.append(roo_dataset_temp);
      cout<<roo_dataset.numEntries()<<endl;
      f->Close();
      start=i;
    }
    std::string outputfile=baseoutputfile;
    outputfile+="_";
    outputfile+=sstemp.str();
    outputfile+=".root";
    RooWorkspace* w_merged =new RooWorkspace("w_merged","workspace");
    w_merged->import(roo_dataset);
    w_merged->writeToFile(outputfile.c_str());
  }
}
