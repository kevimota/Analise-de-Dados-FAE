#include <iostream>
#include "TRandom3.h"
#include "TH1D.h"
#include "TMath.h"

using namespace std;

// Experimento de Buffon para definição do valor de pi
Double_t pimc(int n=1000) 
{
   gRandom->SetSeed(time(NULL) % 65539);
   Double_t d = 1, l = 1, p, th;
   Int_t m = 0;

   for (auto i = 0; i != n; ++i) {
      //Geração de números aleatórios
      th = TMath::Pi()*gRandom->Rndm();
      p = d/2*gRandom->Rndm();

      //Teste de Rejeição
      if (p <= (l/2)*TMath::Sin(th)) {
         m += 1;
      }    
   }

   return 2*n*l/m/d;
}