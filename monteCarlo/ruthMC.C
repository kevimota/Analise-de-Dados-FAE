#include "TMath.h"
#include "TRandom.h"
#include "TFile.h"
#include "TH1D.h"
#include "TF1.h"
#include "TCanvas.h"
#include <iostream>

using namespace TMath;

//Função usada para descobrir o máximo da seção de choque diferencial do efeito compton
Double_t max_r(Double_t k) {
   Double_t alpha = 1/137.035999911;
   TF1 *f = new TF1("f", "( [0]^2/(16*[1]^2*sin(x/2)^4)*sin(x) )", 0.1*Pi(), 0.9*Pi());
   f->SetParameters(alpha, k);
   return f->GetMaximum();
}

//Simulação do espalhamento rutherford, n é o número de eventos simulados, E é a energia cinética inicial do elétron em eV
void ruthMC(Int_t n = 10000, Double_t E = 2) 
{
   //Definição dos parâmetros de interesse
   Double_t alpha = 1/137.035999911;
   Double_t th, y, sigma;
   Int_t cnt;

   TH1D *hruth = new TH1D("hruth", "Rutherford Scattering differential cross section", 100, 0, 3.5);
   //std::cout << max_v(E) << std::endl;
   for (auto i = 0; i != n; ++i) {
      cnt = 0;
      while (true) {
         if ((++cnt) % 500 == 0) std::cout << "i: " << i << " cnt: " << cnt <<std::endl; 

         //Geração de números aleatórios
         th = gRandom->Uniform(0.1*Pi(),0.9*Pi());
         y  = max_r(E)*gRandom->Rndm();

         //Definição da função
         sigma = Sin(th)*Power(alpha,2)/(16*Power(E,2)*Power(Sin(th/2),4));
         
         //Teste de Rejeição
         if (y <= sigma){
            hruth->Fill(th);
            break;
         }
      }
   }
   
   TCanvas *c = new TCanvas("c","c");

   hruth->GetXaxis()->SetTitle("#theta (rad)");
   hruth->GetYaxis()->SetTitle("Differential cross section");
   hruth->Draw(); 

   c->SaveAs(Form("plots/Rutherford_%1.2e_eV.png",E)); 
}


