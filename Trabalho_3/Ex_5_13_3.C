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

    int n = 6;

    double ddp[6] = {1.44, 1.48, 1.42, 1.44, 1.50, 1.46};
    double t = 1.11;
    double mean, err, meanerr, errerr;

    for (int i = 0; i != n; ++i){
        mean += ddp[i]/n;
    }

    for (int i = 0; i != n; ++i){
        err += pow((ddp[i]-mean),2)/(n-1);
    }

    err = sqrt(err);
    meanerr = err/sqrt(n);
    errerr = meanerr/sqrt(2*n);

    cout << "Média: " << mean << " Erro da medida: " << err << " Erro da média: " << meanerr << " Erro do erro da média: " << errerr << endl; 
    cout << "Considerando fator de cobertura " << t << " a ddp é: " << mean << " +/- " << meanerr*t << endl;
}