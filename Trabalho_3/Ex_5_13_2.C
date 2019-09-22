#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TSystem.h"
#include "TMath.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include <iostream>

using namespace std;

void Estimador(){
    TString dir = gSystem->UnixPathName(__FILE__);
    dir.ReplaceAll("Ex_5_13_2.C","");

    TFile *file = new TFile(Form("%spendulo.root",dir.Data()), "RECREATE");
    TTree *tree = new TTree("tree", "tree");

    int nevt = tree->ReadFile(Form("%spendulo.dat", dir.Data()),"T/D");

    double T;
    tree->SetBranchAddress("T",&T);
    double mean, err, meanerr, errerr;

    for (int i = 0; i != nevt; ++i){
        tree->GetEntry(i);
        mean += T/nevt;
    }

    for (int i = 0; i != nevt; ++i){
        tree->GetEntry(i);
        err += pow((T-mean),2)/(nevt-1);
    }

    err = sqrt(err);
    meanerr = err/sqrt(nevt);
    errerr = meanerr/sqrt(2*nevt);

    cout << "Média: " << mean << " Erro da medida: " << err << " Erro da média: " << meanerr << " Erro do erro da média: " << errerr << endl;

    file->Write();
    file->Close();    
}