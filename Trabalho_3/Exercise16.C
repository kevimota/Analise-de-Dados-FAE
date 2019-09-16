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

/* We are going to fit the histogram with a more complicated function. We can use the histogram obtained from the CMS tree data of Exercise 10.
 The aim is to compute the mass and width of the peak (in this case the J/Psi).

Create (or read from the ROOT file obtained in Exercise 10) the tree with the dimuon CMS data between 2 and 5 GeV (text data file).
Fill an histogram with 60 bins between 2 and 5 with the invariant mass for the events when the two muons have opposite charge.
Create a function composed of the gaussian plus the exponential and fit to the histogram. Do the fit works ? What do you need to do to make the fit working ?
Compute the number of peak events, by using the integral of the Gaussian function. Use TF1::IntegralError to compute also its error. */

void Ex16(){
    TString dir = gSystem->UnixPathName(__FILE__);
    dir.ReplaceAll("Exercise16.C","");
    
    TFile *file = new TFile(Form("%sdimuon_data.root",dir.Data()));

    TH1D *h1 = (TH1D*)file->Get("h1");
    gStyle->SetOptFit(1111);

    TF1 *f1 = new TF1("f1", "[0]*TMath::Gaus(x,[1],[2]) + [3]*TMath::Exp([4]*x)");
    f1->SetParameters(43.5672,2.96847,0.27741,535.418,-1.01169);
    h1->Fit(f1);
    h1->Draw();

    TCanvas *c = new TCanvas();
    TF1 *g = new TF1("g", "gaus", 2,5);
    g->SetParameters(f1->GetParameters());
    cout << "O número de eventos é: " << round(g->Integral(2,5)) << endl;
    
    c->cd();
    g->Draw();
}

void Readdimuonfile(){
    //Lendo o arquivo csv para criar a tree
    TString dir = gSystem->UnixPathName(__FILE__);
    dir.ReplaceAll("Exercise16.C","");
    
    TFile *file = new TFile(Form("%sdimuon_data.root",dir.Data()),"RECREATE");
    TTree *dimuon = new TTree("dimuon", "Dimuon CMS data");
    dimuon->ReadFile(Form("%sdimuon_2-5GeV.csv", dir.Data()), "Type/C:RunNo/I:EvNo/I:E1/D:px1:py1:pz1:pt1:eta1:phi1:Q1/I:E2/D:px2:py2:pz2:pt2:eta2:phi2:Q2/I:M/D", ',');

    //O histograma e griado e preenchido apenas com os muons com carga oposta
    TH1D *h1 = new TH1D("h1", "Invariant mass of opposite charge dimuon", 60, 2, 5);

    auto nentries = dimuon->GetEntries();
    int Q1, Q2;
    double M;

    dimuon->SetBranchAddress("Q1", &Q1);
    dimuon->SetBranchAddress("Q2", &Q2);
    dimuon->SetBranchAddress("M", &M);

    for (auto i = 0; i != nentries; ++i) {
        dimuon->GetEntry(i);
        if (Q1*Q2 == -1){
            h1->Fill(M);
        }
    }

    file->Write();
    file->Close();

}

void GetGausParameter(){
    TString dir = gSystem->UnixPathName(__FILE__);
    dir.ReplaceAll("Exercise16.C","");
    
    TFile *file = new TFile(Form("%sdimuon_data.root",dir.Data()));

    TH1D *h1 = (TH1D*)file->Get("h1");

    TF1 *f1 = new TF1("f1", "gaus");
    f1->SetParameters(1,3,0.5);
    h1->Fit(f1,"","",2.7,3.3);
    h1->Draw();
    cout << f1->GetParameter(0) << " " << f1->GetParameter(1) << " " << f1->GetParameter(2) << endl;
}

void GetExpParameter(){
    TString dir = gSystem->UnixPathName(__FILE__);
    dir.ReplaceAll("Exercise16.C","");
    
    TFile *file = new TFile(Form("%sdimuon_data.root",dir.Data()));

    TH1D *h1 = (TH1D*)file->Get("h1");

    TF1 *f1 = new TF1("f1", "[0]*TMath::Exp([1]*x)");
    h1->Fit(f1);
    h1->Draw();
    cout << f1->GetParameter(0) << " " << f1->GetParameter(1) << " " << endl;
}