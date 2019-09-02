#include "TArrow.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLine.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TLatex.h"
#include "TFile.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TRandom3.h"

#include <iostream>
using namespace std;

//===========
// Histogram
//===========



//-----------------------------------------
void Histogram_example_05(int Irebin = 20){
//-----------------------------------------
//-------------------------------------------------------------------------
//-- Goal: o Read 2 histograms from a file (signal and background)
//           Rebin them with a factor 20
//         o Prepare stacked histogram (sig+bgr)
//         o print number of events in a region close to 125 GeV
//         o show combined plot
//------------------------------------------------------------------------

  //----------------------------------------------------------------------
  //-- [1] Get the Higgs signal, background and data histogram from a file 
  //       Prepare sig+bgr distribution and plot them
  //----------------------------------------------------------------------
  TDirectory* dir = gDirectory;   
  TFile *file = new TFile("Histograms_fake.root", "READ");
  dir->cd();
  TH1D *h_sig  = (TH1D*) file->Get("h_m4l_Higgs125_fake")->Clone("h_sig");     
  TH1D *h_bgr  = (TH1D*) file->Get("h_m4l_ZZ_fake")->Clone("h_bgr");     
  TH1D *h_data = (TH1D*) file->Get("h_m4l_data_fake")->Clone("h_fake");     
  file->Close();  

  //-- Rebin histograms (only for plotting)
  h_sig->Rebin(Irebin);
  h_bgr->Rebin(Irebin);
  h_data->Rebin(Irebin);
  printf("\n  INFO: Rebinning the histograms with a factor %d. Binwidth is now %5.2f GeV\n\n", Irebin, h_data->GetBinWidth(1));

  //-- Prepare cumulative histogram for signal + background 
  TH1D *h_sig_plus_bgr = (TH1D* ) h_bgr->Clone("h_sig_plus_bgr"); 
  h_sig_plus_bgr->Reset();
  for (int i_bin = 1; i_bin < h_bgr->GetNbinsX(); i_bin++){
       h_sig_plus_bgr->SetBinContent( i_bin, h_sig->GetBinContent(i_bin) + h_bgr->GetBinContent(i_bin));
  }

  //-- print mass and number of events in a 10 GeV mass window around 125 GeV
  double mass_bin = 0.;
  double Nevt_bin_sig = 0.;
  double Nevt_bin_bgr = 0.;
  double Nevt_bin_data = 0.;
  for(int i_bin = 1; i_bin< h_bgr->GetNbinsX(); i_bin++){
    mass_bin = h_data->GetBinCenter(i_bin);
    Nevt_bin_sig  = h_sig->GetBinContent(i_bin);
    Nevt_bin_bgr  = h_bgr->GetBinContent(i_bin);
    Nevt_bin_data = h_data->GetBinContent(i_bin);
    if(fabs(mass_bin-125)<10.){
      printf("  Bin %3d: mass = %5.2f | Nsig = %5.2f  Nbgr = %5.2f and Ndata = %5.2f\n",i_bin, mass_bin, Nevt_bin_sig, Nevt_bin_bgr, Nevt_bin_data);
    }
  }
  
  //----------------------
  //-- [2] Plot histograms
  //----------------------

  //-- Prepare canvas
  TCanvas * canvas1 = new TCanvas( "canvas1","Standard Canvas",700,500);  
  canvas1->cd(); 

  //-- plot histograms (sig+bgrin higgs color and a restricted mass range on x-axis)
  h_sig->SetFillColor(7); 
  h_sig_plus_bgr->SetFillColor(7); 
  h_sig_plus_bgr->SetAxisRange(0.,25.,"Y");
  h_sig_plus_bgr->SetAxisRange(50.,400.,"X");
  h_sig_plus_bgr->Draw("hist");  

  //-- add the background (plot it over the sig+bgr distribution)
  h_bgr->SetFillColor(2); 
  h_bgr->Draw("same");  
  h_bgr->Draw("axis same");  
  h_data->Draw("e same");  

  //-- add axes
  h_sig_plus_bgr->SetXTitle("4-lepton invariant mass [GeV]");
  h_sig_plus_bgr->SetYTitle(Form("Number of events / %3.1f GeV",h_bgr->GetBinWidth(1)));

  //-- create a legend
  TLegend *legend = new TLegend(0.65,0.65,0.90,0.80);
  legend->SetBorderSize(0); 
  legend->SetFillColor(0);
  TLegendEntry *legend_entry1 = legend->AddEntry(h_sig," Higgs", "f");  
  TLegendEntry *legend_entry2 = legend->AddEntry(h_bgr," ZZ(SM)", "f");  
  legend_entry1->SetTextSize(0.05);
  legend_entry2->SetTextSize(0.05);
  legend->Draw();

  //-- prepare gif
  canvas1->Print("./Histogram_example_5.gif");

  return;

 } // end Histogram_example_05()


