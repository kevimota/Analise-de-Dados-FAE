#include <iostream>
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TF1.h"
#include "TLatex.h"
#include "TLegend.h"

using namespace TMath;

Double_t signal(Double_t *x, Double_t *par) {
   //a simple gaussian
   return par[0]*exp(-0.5*TMath::Power(((x[0]-par[1])/(par[2])),2)); 
}

Double_t backgr(Double_t *x, Double_t *par) {
   //a simple polynomial
   return par[0]+par[1]*x[0]+par[2]*x[0]*x[0];
}

Double_t fitfun(Double_t *x, Double_t *par) {
   //the total PDF function, sum of the above
   return signal(x,par) + backgr(x,&par[3]); 
}

void Zanalysis()
{
   TFile *f = new TFile("Zdimion_data.root", "RECREATE");
   TTree *t = new TTree("t","Z->dimuon");

   //Enchendo a tree com os dados do arquivo Zdimuon.dat
   Int_t nevt = t->ReadFile("Zdimuon.dat","Q1/I:pT1/D:px1:py1:pz1:E1:phi1:eta1:Q2/I:pT2/D:px2:py2:pz2:E2:phi2:eta2", ',');
   
   //Definição dos histogramas
   TH1D *hZm = new TH1D("hZm", "Simulated #mu^{+}#mu^{-} invariant mass distribuition", 450, 0., 130.);
   TH1D *hZmcut = new TH1D("hZmcut", "Simulated #mu^{+}#mu^{-} invariant mass distribuition", 100, 80, 100);
   TH1D *hmu1pT = new TH1D("hmu1pT", "Simulated #mu^{-} p_T distribuition", 100, 0., 100.);
   TH1D *hmu1phi = new TH1D("hmu1phi", "Simulated #mu^{-} #phi distribuition", 100, -3.5, 3.5);
   TH1D *hmu1eta = new TH1D("hmu1eta", "Simulated #mu^{-} #eta distribuition", 100, -10., 10.);
   TH1D *hmu2pT = new TH1D("hmu2pT", "Simulated #mu^{+} p_T distribuition", 100, 0., 100.);
   TH1D *hmu2phi = new TH1D("hmu2phi", "Simulated #mu^{+} #phi distribuition", 100, -3.5, 3.5);
   TH1D *hmu2eta = new TH1D("hmu2eta", "Simulated #mu^{+} #eta distribuition", 100, -10., 10.);

   //Definição das variáveis necessárias para o histograma
   Double_t pT1, px1, py1, pz1, E1, phi1, eta1, pT2, px2, py2, pz2, E2, phi2, eta2;

   t->SetBranchAddress("pT1", &pT1);
   t->SetBranchAddress("px1", &px1);
   t->SetBranchAddress("py1", &py1);
   t->SetBranchAddress("pz1", &pz1);
   t->SetBranchAddress("E1", &E1);
   t->SetBranchAddress("phi1", &phi1);
   t->SetBranchAddress("eta1", &eta1);
   t->SetBranchAddress("pT2", &pT2);
   t->SetBranchAddress("px2", &px2);
   t->SetBranchAddress("py2", &py2);
   t->SetBranchAddress("pz2", &pz2);
   t->SetBranchAddress("E2", &E2);
   t->SetBranchAddress("phi2", &phi2);
   t->SetBranchAddress("eta2", &eta2);

   //loop para encher os histogramas   
   for (auto i = 0; i != nevt; ++i) {
      t->GetEntry(i);
      hZm->Fill(Sqrt(Power(E1 + E2,2) - Power(px1 + px2,2) - Power(py1 + py2,2) - Power(pz1 + pz2,2)));
      hZmcut->Fill(Sqrt(Power(E1 + E2,2) - Power(px1 + px2,2) - Power(py1 + py2,2) - Power(pz1 + pz2,2)));
      hmu1pT->Fill(pT1);
      hmu1phi->Fill(phi1);
      hmu1eta->Fill(eta1);
      hmu2pT->Fill(pT2);
      hmu2phi->Fill(phi2);
      hmu2eta->Fill(eta2);
   }

   //Configurando os titulos dos eixos dos plots
   hZm->GetXaxis()->SetTitle("m_{#mu #mu} (GeV)");
   hZm->GetYaxis()->SetTitle(Form("Events/%.2f GeV", hZm->GetBinWidth(0)));
   hZmcut->GetXaxis()->SetTitle("m_{#mu #mu} (GeV)");
   hZmcut->GetYaxis()->SetTitle(Form("Events/%.2f GeV", hZmcut->GetBinWidth(0)));
   hmu1pT->GetXaxis()->SetTitle("p_{T} (GeV)");
   hmu1pT->GetYaxis()->SetTitle(Form("Events/%.2f GeV", hmu1pT->GetBinWidth(0)));
   hmu1pT->GetYaxis()->SetTitleSize(0.03);
   hmu1phi->GetXaxis()->SetTitle("#phi (rad)");
   hmu1phi->GetYaxis()->SetTitle(Form("Events/%.2f rad", hmu1phi->GetBinWidth(0)));
   hmu1phi->GetYaxis()->SetTitleSize(0.03);
   hmu1eta->GetXaxis()->SetTitle("#eta");
   hmu1eta->GetYaxis()->SetTitle(Form("Events/%.2f", hmu1eta->GetBinWidth(0)));
   hmu1eta->GetYaxis()->SetTitleSize(0.03);
   hmu2pT->GetXaxis()->SetTitle("p_{T} (GeV)");
   hmu2pT->GetYaxis()->SetTitle(Form("Events/%.2f GeV", hmu2pT->GetBinWidth(0)));
   hmu2pT->GetYaxis()->SetTitleSize(0.03);
   hmu2phi->GetXaxis()->SetTitle("#phi (rad)");
   hmu2phi->GetYaxis()->SetTitle(Form("Events/%.2f rad", hmu2phi->GetBinWidth(0)));
   hmu2phi->GetYaxis()->SetTitleSize(0.03);
   hmu2eta->GetXaxis()->SetTitle("#eta");
   hmu2eta->GetYaxis()->SetTitle(Form("Events/%.2f", hmu2eta->GetBinWidth(0)));
   hmu2eta->GetYaxis()->SetTitleSize(0.03);

   f->Write();
   f->Close();

}

void Zplot()
{
   Zanalysis();

   TCanvas *c = new TCanvas("c", "c", 1440, 900);
   TFile *f = new TFile("Zdimion_data.root");
   
   TH1D *hZm = (TH1D*)f->Get("hZm");
   TH1D *hZmcut = (TH1D*)f->Get("hZmcut");
   TH1D *hmu1pT = (TH1D*)f->Get("hmu1pT");
   TH1D *hmu1phi = (TH1D*)f->Get("hmu1phi");
   TH1D *hmu1eta = (TH1D*)f->Get("hmu1eta");
   TH1D *hmu2pT = (TH1D*)f->Get("hmu2pT");
   TH1D *hmu2phi = (TH1D*)f->Get("hmu2phi");
   TH1D *hmu2eta = (TH1D*)f->Get("hmu2eta");

   //Fazendo o fit do gráfico da massa invariante do Z0
   const Int_t nfitpar = 6; 
   TF1 *f1 = new TF1("f1", fitfun, 80,100, nfitpar);
   f1->SetParameters(100,90,0.5,0,0,0);
   hZmcut->Fit(f1);

   Double_t par[nfitpar];
   f1->GetParameters(par);

   c->Clear();
   hZmcut->SetStats(0);
   hZmcut->SetMarkerStyle(21);
   hZmcut->SetMarkerSize(0.8);
   hZmcut->Fit("f1","V+","ep");

   // Pegar as funções separadamente para representação
   TF1 *signalFcn = new TF1("signalFcn",signal, 80,100,3);
   signalFcn->SetLineColor(kBlue);
   signalFcn->SetNpx(500);
   TF1 *backFcn = new TF1("backFcn",backgr,80,100,3);
   backFcn->SetLineColor(kGray);
   backFcn->SetLineStyle(2);

   signalFcn->SetParameters(par);
   signalFcn->Draw("same");
   
   backFcn->SetParameters(&par[3]);
   backFcn->Draw("same");

   // Desenhar a legenda
   TLegend *legend = new TLegend(0.7,0.65,0.88,0.85);
   legend->SetBorderSize(0);
   legend->SetTextFont(40);
   legend->SetTextSize(0.03);
   legend->AddEntry(hZmcut,"Data","lpe");
   legend->AddEntry(backFcn,"Background fit","l");
   legend->AddEntry(signalFcn,"Signal fit","l");
   legend->AddEntry(f1,"Global Fit","l");
   legend->Draw("same");

   // Apresentar dados referentes à massa do Z.
   TLatex L;
   L.SetNDC();
   L.SetTextSize(0.03);
   L.DrawLatex(0.15,0.70,Form("mass: %5.3f #pm %5.3f GeV/c^{2}",
               par[1], f1->GetParErrors()[1]));
   L.DrawLatex(0.15,0.65,Form("width: %5.3f #pm %5.3f GeV/c^{2}", 
               par[2], f1->GetParErrors()[2]));
   c->SaveAs("plots/Zfit.png");
   
   // Desenhar o restante dos gráficos.
   c->Clear();
   hZm->Draw();
   c->SaveAs("plots/invariant_mass.png");

   c->Clear();
   c->Divide(2,1);
   c->cd(1);
   hmu1pT->Draw();
   c->cd(2);
   hmu2pT->Draw();
   c->SaveAs("plots/muons_pT.png");
   
   c->Clear();
   c->Divide(2,1);
   c->cd(1);
   hmu1phi->Draw();
   c->cd(2);
   hmu2phi->Draw();
   c->SaveAs("plots/muons_phi.png");
   
   c->Clear();
   c->Divide(2,1);
   c->cd(1);
   hmu1eta->Draw();
   c->cd(2);
   hmu2eta->Draw();
   c->SaveAs("plots/muons_eta.png");
   c->Close();

}


