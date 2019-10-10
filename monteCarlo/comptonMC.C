#include "TMath.h"
#include "TRandom.h"
#include "TFile.h"
#include "TH1D.h"
#include "TF1.h"
#include "TCanvas.h"
#include <iostream>

using namespace TMath;

//Função usada para descobrir o máximo da seção de choque diferencial do efeito compton
Double_t max_c(Double_t k) {
   Double_t m = 0.511;
   Double_t alpha = 1/137.035999911;
   TF1 *f = new TF1("f", "sin(x)*( ([0]^2)/(2*[1]^2)*((1/(1+([2]/[1])*(1 - cos(x))))^2)*(1/(1+([2]/[1])*(1 - cos(x))) + (1+([2]/[1])*(1-cos(x))) - sin(x)^2) )",0,Pi());
   f->SetParameters(alpha, m, k);
   return f->GetMaximum();
}

//Simulação do espalhamento compton, n é o número de eventos simulados, k é a energia inicial do fóton em MeV
void comptonMC(Int_t n = 10000, Double_t k = 5e-3) 
{
   //Parâmetros de interesse para a função
   Double_t m = 0.511;
   Double_t alpha = 1/137.035999911;
   Double_t kf, th, y, sigma;
   Int_t cnt;
	
   TH1D *hcomp = new TH1D("hcomp", "Compton differential cross section", 100, 0, 3.5);

   for (auto i = 0; i != n; ++i) {
      cnt = 0;
      while (true) {
         if ((++cnt) % 500 == 0) std::cout << "i: " << i << " cnt: " << cnt <<std::endl; 
         
         //Geração de números aleatórios
         th = Pi()*gRandom->Rndm();
         y  = max_c(k)*gRandom->Rndm();
         
         //Definição da seção de choque
         kf = k/(1 + (k/m)*(1-Cos(th)));
         sigma = Power(alpha,2)/(2*Power(m,2))*Power(kf/k,2)*((kf/k) + (k/kf) - Power(Sin(th),2))*Sin(th);
         
         //Teste de Rejeição
         if (y <= sigma){
            hcomp->Fill(th);
            break;
         }
      }
   }

   TCanvas *c = new TCanvas("c","c");
   hcomp->GetXaxis()->SetTitle("#theta (rad)");
   hcomp->GetYaxis()->SetTitle("Differential cross section");
   hcomp->Draw();

   c->SaveAs(Form("plots/Compton_%1.2e_MeV.png",k)); 
}


