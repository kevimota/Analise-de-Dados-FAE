#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include <iostream>

using namespace RooFit;

/* We will start with a similar exercise we did for Root fitting. We will create a Gaussian model from which we will generate a pseudo-data set and
then we will fit this data set.

Start directly creating the Gaussian model using the workspace factory, thus the syntax introduced in the lecture slides. One you have created the
model, use the generate() method of the RooAbsPdf class to generate 1000 events. Try to plot the data set using RooPlot as shown in the lecture slides

After, fit the model to the data and show the resulting fitted function.

At the end save the RooWorkspace object in a file, but before remember to import, by calling RooWorkspace::import(data), the data set you have generated
in the workspace. The workspace does not contains only the model, but also the data, allowing then to re-perform the analysis later on. */

void ex18(unsigned n = 1000) {
    //Definindo o workspace
    RooWorkspace w("w");
    //Criando a pdf e as variáveis usando o factory
    w.factory("Gaussian::g(x[-10,10], mu[1,-1000,1000], s[2,0,1000])");
    //Retirando de dentro do workspace a variável x e a pdf g
    RooAbsPdf *g = w.pdf("g");
    RooRealVar *x = w.var("x");
    
    //Usando o método generate para gerar dados a partir de uma gaussiana
    RooDataSet *data = g->generate(*x,n);
    data->SetName("Gaussian data");

    //Criando o plot e definindo que os dados de data serão plotados.
    RooPlot *pl = x->frame();
    pl->SetTitle("Gaussian model");
    data->plotOn(pl);
    
    //fazendo o fit
    g->fitTo(*data);

    // plotando a pdf
    g->plotOn(pl);
    pl->Draw();
    
    //importando o dataset para dentro do workspace
    w.import(*data);   
    w.Print();

    //escrevendo o workspace dentro do arquivo Gaussian_data.root
    w.writeToFile("Gaussian_data.root", true);
}