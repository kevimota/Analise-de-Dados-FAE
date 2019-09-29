#include "TSystem.h"
#include "TFile.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include <iostream>

using namespace RooFit;

/* Open the file you have just created in the previous exercise and get the RooWorkspace object ffrom the file. Get a pointer to the p.d.f describing
your model, and a pointer to the data. Re-fit the data, but this time in the range [0,10] and plot the result. */

void ex19() {
    //Criando o TFile que receberá o arquivo criado exercício anterior
    TString dir = gSystem->UnixPathName(__FILE__);
    dir.ReplaceAll("Exercise19.C","");
    TFile *file = new TFile(Form("%sGaussian_data.root",dir.Data()));
    
    //Retirando o workspace de dentro o arquivo
    RooWorkspace *w = (RooWorkspace*)file->Get("w");
    
    //definindo as variáveis que estão dentro do workspace
    RooAbsPdf *g = w->pdf("g");
    RooAbsData *data = w->data("Gaussian data");
    RooRealVar *x = w->var("x");
    
    //definindo o plot e colocando os dados dentro do plot
    RooPlot *p = x->frame();
    data->plotOn(p);
    
    //fazendo o fit apenas na região de (0,10)
    RooFitResult *r = g->fitTo(*data, Save(true), Minimizer("Minuit2", "Migrad"), Range(0.,10.));
    g->plotOn(p);
    g->paramOn(p);
    p->Draw();
}