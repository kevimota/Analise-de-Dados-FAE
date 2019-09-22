#include "TMath.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TH1D.h"
#include "TF1.h"
#include "TStyle.h"
#include "TFitResult.h"
#include "TSystem.h"
#include "TMath.h"
#include <iostream>

using namespace std;

/* Repeat the previous exercise, but by using the Fit Panel GUI */

void ex17(){
    TString dir = gSystem->UnixPathName(__FILE__);
    dir.ReplaceAll("Exercise17.C","");
    
    TFile *file = new TFile(Form("%sdimuon_data.root",dir.Data()));

    TH1D *h1 = (TH1D*)file->Get("h1");
    gStyle->SetOptFit(1111);

    TCanvas *h_canvas = new TCanvas("Hist");
    /* h_canvas->SetLogx();
    h_canvas->SetLogy(); */
    h1->Draw();
    h1->FitPanel();

}