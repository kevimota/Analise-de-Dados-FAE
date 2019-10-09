#include "TMath.h"
#include "TRandom.h"
#include "TFile.h"
#include "TH1D.h"
#include <iostream>

using namespace TMath;

void ruthMC(Int_t n = 10000) 
{ 
   Double_t m = 0.511;
   Double_t k;
   Int_t cont = 0;

   TFile *f = new TFile("hist_ruth.root","RECREATE");
   TH1D *h1 = new TH1D("5 keV","5 keV",100,0,Pi());

   for (auto j = 0; j != n; ++j){
      for (auto i = 0; i < 1200; ++i){
         //std::cout << i << std::endl;
         Double_t zz = 1.076*(gRandom->Rndm());
         Double_t x = Pi()*(gRandom->Rndm());
         if (zz <= ((Power((1/(1+0.01*(1-Cos(x)))),2))*(((1/(1+0.01*(1-Cos(x)))))+(1+0.01*(1-Cos(x))))-Sin(x)*Sin(x))){
            i=1200;
         }
      h1->Fill(x);
      }

   }
   f->Write();
   f->Close();
}

