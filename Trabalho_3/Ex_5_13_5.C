#include "TMath.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TStyle.h"
#include "TFitResult.h"
#include <iostream>

using namespace TMath;

void LinearAj(){
    double E[5] = {0.07, 0.18, 0.30, 0.45, 0.60};
    double D[5] = {4.9, 6.7, 7.3, 8.1, 9.2};
    double e[5] = {0.3, 0.3, 0.4, 0.4, 0.4};

    for (auto i = 0; i != 5; ++i){
        E[i] = Log(E[i]);
        e[i] = e[i]/D[i];
        D[i] = Log(D[i]);
    }
    
    TGraph *gre = new TGraphErrors(5,E,D,0,e);
    TF1 *f1 = new TF1("f1", "pol1");

    TFitResultPtr re = gre->Fit(f1,"M S");
    gStyle->SetOptFit(1111);
    std::cout << re->Parameter(1) << " +/- " << re->Error(1) << std::endl;
    gre->SetMarkerStyle(20);
    gre->SetMarkerSize(0.8);
    gre->Draw("AP");
}