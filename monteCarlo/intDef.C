#include "TMath.h"
#include "TRandom3.h"
#include "TF1.h"
#include <iostream>

// Integral definida, utilizando método da rejeição
Double_t intDef(Double_t a, Double_t b, TString func, Int_t n = 10000) 
{
   // Gerador de números aleatórios
   TRandom3 *r = new TRandom3();
   r->SetSeed(time(NULL));

   // Definição do máximo da função
   TF1 *f = new TF1("f",func, a, b);
   Double_t max = f->GetMaximum(a,b), x = 0, y = 0;
   Int_t m1 = 0;
   
   for (auto i = 0; i != n; ++i) {
      //Geração de números aleatórios
      x = r->Uniform(a,b);
      y = r->Uniform(0,max);
      
      // teste da rejeição
      if (y <= f->Eval(x)) m1 += 1;
   }

   Double_t A = (b - a)*max;
   return A*m1/n;
}