#ifndef DADOSFISGERAL_C
#define DADOSFISGERAL_C

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TCanvas.h"

/*
Recebendo um arquivo csv (DadosFisGeral2001_2.csv) e salvando no arquivo "FisGeral2001_2.root".
*/
void DadosFisGeral(){
    //Criamos o arquivo e a tree
    TFile *f = new TFile("FisGeral2001_2.root","RECREATE");
    TTree *tree = new TTree("fisgeral","Dados da turma de fisica geral 2001.2");
    
    //A tree recebe os valores contidos no arquivo, os valores são delimitados por uma vírgula.
    tree->ReadFile("DadosFisGeral2001_2.csv","idade/I,massa/D,altura/D",',');
    
    //Inicializando valores para associar aos branches da tree
    int idade;
    double massa, altura;
    tree->SetBranchAddress("idade",&idade);
    tree->SetBranchAddress("massa",&massa);
    tree->SetBranchAddress("altura",&altura);

    //Criando os histogramas para a partir dos valores lidos da tree
    TH1I *h_idade = new TH1I("hist_idade","Histograma idade", 25,15.,40.);
    TH1D *h_massa = new TH1D("hist_massa","Histograma massa", 20,50.,130.);
    TH1D *h_altura = new TH1D("hist_altura","Histograma altura", 21,158.,200.);
        
    auto n = tree->GetEntries();
    
    for (auto i = 0; i != n; ++i){
        tree->GetEntry(i);
        h_idade->Fill(idade);
        h_massa->Fill(massa);
        h_altura->Fill(altura);
    }
    
    //Arquivo é escrito com a tree e os histogramas e depois fechado
    f->Write();
    f->Close();
}

#endif