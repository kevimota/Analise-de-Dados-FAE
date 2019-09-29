#include "TMath.h"
#include "TRandom3.h"
#include "TFile.h"
#include "TH1D.h"
#include <iostream>

using namespace TMath;

void ruthMC(Int_t n = 10000) 
{ 
   //TFile *f = new TFile("hist_ruther.root","RECREATE");
   TH1D *h1 = new TH1D("h1", "Rutherford Scattering", 1000, -Pi(), Pi());
   TRandom3 *r = new TRandom3();
   r->SetSeed(time(NULL));

   Double_t ruther = 0, th = 0, y = 30;
   Int_t m = 0;
   Double_t E = 0;
   Double_t e = Sqrt(4*Pi()/137);

   for (Int_t i = 0; i != n; ++i) {
      th = Pi()*r->Uniform(-1,+1);
      E = r->Gaus(2.662,.2);
      ruther = Power(Power(e,2)/(8*Pi()*E),2)/Power(Sin(th/2),4);
      if (y <= ruther) h1->Fill(th);
   }

   h1->Draw();
}