#ifndef DIST_MOEDA_H
#define DIST_MOEDA_H

#include <iostream>
#include <cstdlib>
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TAxis.h"

//estrutura de dados que guarda os dados da dispersão e do número de jogadas.
struct jog_moeda
    {
        int disp;
        int n_jog;
    };

//função que entrega o numero de resultados cara de N jogadas de uma moeda;
int Moeda(int N) {
    int res_cara = 0;
    for (auto i = 0; i != N; ++i){
        if (rand() % 2) {
            ++res_cara;
        }
    }
    return res_cara;
}

//Essa função usa os dados da função Moeda, para salvar os dados de dispersão numero de jogadas no arquivo Dist_Moeda.root
void Dist_Moeda(int jogs = 1000){
    jog_moeda moeda;
    //Criando o arquivo e a tree
    TFile *file = new TFile("Dist_Moeda.root","RECREATE");
    TTree *tree = new TTree("Disp_Moeda", "Dispersão de um número de caras jogando moedas.");
    
    //Associando um branch à struct, duas leaves são criadas para os dados de dispersão e número de jogadas
    tree->Branch("moeda",&moeda.disp,"disp/I:n_jog/I");

    //Dados do Branch são calculados e populados 
    for (auto i = 2; i < jogs+2; i = i + 2){
        moeda.n_jog = i;
        moeda.disp = Moeda(moeda.n_jog) - moeda.n_jog/2;
        tree->Fill();
    }

    //Arquivo é escrito e fechado
    file->Write();
    file->Close();
}

//Essa função lê os dados do arquivo e os desenha em um grafico de disp. x n_jog
void PrintData(){
    //Criando a struct, o arquivo e a tree
    jog_moeda moeda;
    TFile *file = new TFile("Dist_Moeda.root");
    TTree *tree = (TTree*) file->Get("Disp_Moeda");
    
    //Associando os endereços do Branch à struct
    tree->SetBranchAddress("moeda",&moeda.disp);
    
    //O número de entradas da tree e os arrays são criados (para o gráfico)
    auto nentries = tree->GetEntries();
    int x[nentries], y[nentries];
    
    //Os valores das arrays são populadas
    for (auto i = 0; i != nentries; ++i){
        tree->GetEntry(i);
        x[i] = moeda.n_jog;
        y[i] = moeda.disp;
    }   
    
    //Gráfico criado
    TGraph *gr = new TGraph(nentries,x,y);
    
    //Perfumando o Gráfico e desenhado-o no Canvas
    gr->SetTitle("Disp. x N. de jogadas");
    gr->GetXaxis()->SetTitle("N. de jogadas");
    gr->GetXaxis()->SetRangeUser(0,2*nentries);
    gr->GetYaxis()->SetTitle("Disp.");
    gr->SetLineColor(4);
    gr->SetLineWidth(2);
    gr->Draw();

}

#endif