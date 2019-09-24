#include <iostream>
#include "TRandom3.h"
#include "TMath.h"

using namespace TMath;

int pimc(int n=10000) 
{
   TRandom3 *r = new TRandom3();
   r->SetSeed(time(NULL));
   Double_t d = 1, l = 1, pi = 0, x, y, z, p, th;
   Int_t m = 0;
   for (auto i = 0; i != n; ++i) {
      r->Sphere(x, y, z, 1);
      p = r->Rndm();
      th = x/Sqrt(Power(x,2)+Power(y,2));
      if (x <= l/2*Sin(th)) {
         m += 1;
      }      
   }

   std::cout << 2*n*l/m/d << std::endl;

   return 0;
}