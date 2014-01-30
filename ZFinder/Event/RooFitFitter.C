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

const double phistarBins[] = {0, 0.5,2};
const double etaBins[] = {-5.0,-2.5,-2.0,-1.5,-1.0,-0.5,0.0,0.5,1.0,1.5,2.0,2.5,5.0};

void RooFitFitter(const char* filename){
  RooRealVar Zmass("Zmass","Zmass",0,300);
  RooRealVar Zeta("Zeta","Zeta",-20,20);
  RooRealVar Zy("Zy","Zy",-1000,1000);
  RooRealVar Zpt("Zpt","Zpt",0,10000);
  RooRealVar Zphistar("Zphistar","Zphistar",-0.00001,100);
  RooRealVar Weight("Weight","Weight",0,100);
  RooRealVar Pass("Pass","Pass",-2,2);
  RooArgSet ZEventArgSet(Zmass, Zphistar, Zpt, Zeta, Zy, Weight, Pass);

  TFile * f= new TFile(filename, "READ");
  f->ls();
  f->cd("ZFinder");
  f->ls();
  cout<<"hoi"<<endl;
  RooWorkspace* w =(RooWorkspace*) f->Get("w");
  cout<<"hoi2"<<endl;
  RooDataSet& MC_reco = *((RooDataSet*) w->data("MC_reco") );
  RooDataSet& Data_reco = *((RooDataSet*) w->data("Data_reco") );
  RooDataSet& MC_true_all = *((RooDataSet*) w->data("MC_true_all") );
  cout<<"hoi3"<<endl;

  int neta=(sizeof(etaBins)/sizeof(etaBins[0]))-1;
  int nphistar=(sizeof(phistarBins)/sizeof(phistarBins[0]))-1;

  RooRealVar x("x","x",50,150);
  RooRealVar alpha("alpha","alpha",60.,0.1,100000.);
  RooRealVar gamma("gamma","gamma",0.01,0.0001,0.3);
  RooRealVar delta("delta","delta",10.,3.,80.);
  RooFormulaVar var1("var1","(alpha-x)/delta",RooArgSet(alpha,x,delta));
  RooFormulaVar var2("var2","gamma*x",RooArgSet(gamma,x));
  RooGenericPdf MyBackgroundPdf("MyBackgroundPdf","ROOT::Math::erfc(var1)*exp(var2)",RooArgSet(var1, var2));

  double acceptance[neta*nphistar];
  double nsignal[neta*nphistar];

  int bin=0;

  for (int ieta=0; ieta<neta; ieta++){
    TString cuteta = "Zeta>";
    cuteta += double(etaBins[ieta]);
    cuteta +="&&Zeta<";
    cuteta += double(etaBins[ieta+1]);
    RooDataSet* MC_reco_etabin = (RooDataSet*) MC_reco.reduce(ZEventArgSet,cuteta);
    RooDataSet* Data_reco_etabin = (RooDataSet*) Data_reco.reduce(ZEventArgSet,cuteta);
    RooDataSet* MC_true_all_etabin = (RooDataSet*) MC_true_all.reduce(ZEventArgSet,cuteta);
    for (int iphistar=0; iphistar<nphistar; iphistar++){
      TString cutphistar = "Zphistar>";
      cutphistar += double(phistarBins[iphistar]);
      cutphistar +="&&Zphistar<";
      cutphistar += double(phistarBins[iphistar+1]);
      RooDataSet* MC_reco_phistarbin = (RooDataSet*) MC_reco_etabin->reduce(ZEventArgSet,cutphistar);
      RooDataSet* Data_reco_phistarbin = (RooDataSet*) Data_reco_etabin->reduce(ZEventArgSet,cutphistar);
      RooDataSet* MC_true_all_phistarbin = (RooDataSet*) MC_true_all_etabin->reduce(ZEventArgSet,cutphistar);

      acceptance[bin]=0;
      if (MC_true_all_phistarbin->sumEntries()>0) acceptance[bin]=MC_reco_phistarbin->sumEntries()/MC_true_all_phistarbin->sumEntries();
      TCanvas* c=new TCanvas();
      c->cd();
      RooPlot* fitFrame = Zmass.frame();
      MC_reco_phistarbin->plotOn(fitFrame);
      fitFrame->Draw();

      cout<<acceptance[bin]<<endl;

      bin++;
    }
  }
}





//   //  F->ls();
//   TCanvas* MC_2d = new TCanvas("MC_2d", "MC_2d", 1280, 640);
//   MC_2d->Divide(2,1);
//   TH2D * MC_2d_true_bin_content_ = (TH2D*)f->Get("MC_2d_true_bin_content_");
//   TH2D * MC_2d_reco_bin_content_ = (TH2D*)f->Get("MC_2d_reco_bin_content_");
//   MC_2d->cd(1);
//   MC_2d_true_bin_content_->Draw("COLZ");
//   MC_2d->cd(2);
//   MC_2d_reco_bin_content_->Draw("COLZ");
//   std::cout<<"hi"<<std::endl;
//   RooRealVar x("x","x",50,150);
//   RooRealVar alpha("alpha","alpha",60.,0.1,100000.);
//   RooRealVar gamma("gamma","gamma",0.01,0.0001,0.3);
//   RooRealVar delta("delta","delta",10.,3.,80.);
//   RooFormulaVar var1("var1","(alpha-x)/delta",RooArgSet(alpha,x,delta));
//   RooFormulaVar var2("var2","gamma*x",RooArgSet(gamma,x));
//   RooGenericPdf MyBackgroundPdf("MyBackgroundPdf","ROOT::Math::erfc(var1)*exp(var2)",RooArgSet(var1, var2));
//   int neta=(sizeof(etaBins)/sizeof(etaBins[0]))-1;
//   int nphistar=(sizeof(phistarBins)/sizeof(phistarBins[0]))-1;
//   const std::string  MC_Zmass_generic_name= "MC_Zmass_";
//   const std::string  Data_Zmass_generic_name= "Data_Zmass_";
//   double acceptance[neta*nphistar];
//   double nsignal[neta*nphistar];
//   int bin=0;
//   for (int ieta=0; ieta<neta; ieta++){
//     for (int iphistar=0; iphistar<nphistar; iphistar++){
//       acceptance[bin]=0;
//       if (MC_2d_true_bin_content_->GetBinContent(ieta,iphistar)>0){
// 	acceptance[bin]=MC_2d_reco_bin_content_->GetBinContent(ieta,iphistar)/MC_2d_true_bin_content_->GetBinContent(ieta,iphistar);
//       }
//       std::ostringstream s;
//       s<<etaBins[ieta]<<"_Eta_"<<etaBins[ieta+1]<<"_"<<phistarBins[iphistar]<<"_PhiStar_"<<phistarBins[iphistar+1];
//       const std::string MC_Zmass_name_string=MC_Zmass_generic_name+s.str();
//       const std::string Data_Zmass_name_string=Data_Zmass_generic_name+s.str();
//       const std::string Canvas_name_string=s.str();
//       const char * MC_Zmass_name = MC_Zmass_name_string.c_str();
//       const char * Data_Zmass_name = Data_Zmass_name_string.c_str();
//       const char * Canvas_name = Canvas_name_string.c_str();
//       TCanvas * c = new TCanvas(Canvas_name,Canvas_name,1280,640);
//       c->Divide(2,1);
//       TH1D * data = (TH1D*)f->Get(Data_Zmass_name);
//       TH1D * mc = (TH1D*)f->Get(MC_Zmass_name);
//       c->cd(1);
//       data->Draw();
//       c->cd(2);
//       mc->Draw();
//       nsignal[bin]=data ->GetEntries();
//     }
//   }
// }
