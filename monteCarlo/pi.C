#include <iostream>
#include "TRandom3.h"
#include "TH1D.h"
#include "TMath.h"

using namespace std;

int pimc(int n=1000) 
{
   TH1D *h1 = new TH1D("h","h",100,0,TMath::Pi());
   TRandom3 *r = new TRandom3();
   r->SetSeed(time(NULL));
   Double_t d = 1, l = 1, p, th;
   Int_t m = 0;
   for (auto i = 0; i != n; ++i) {
      th = TMath::Pi()*r->Rndm();
      p = d/2*r->Rndm();
      if (p <= (l/2)*TMath::Sin(th)) {
         m += 1;
      }
      h1->Fill(th);      
   }

   std::cout << 2*n*l/m/d << std::endl;
   h1->GetYaxis()->SetRangeUser(0,n/50 > 5 ? n/50 : 5);
   h1->Draw();

   return 0;
}