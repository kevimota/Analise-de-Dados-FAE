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
    
    RooWorkspace w("w");

    w.factory("Gaussian::g(x[-10,10], mu[1,-1000,1000], s[2,0,1000])");

    RooAbsPdf *g = w.pdf("g");
    RooRealVar *x = w.var("x");
    
    RooDataSet *data = g->generate(*x,n);
    data->SetName("Gaussian data");

    RooPlot *pl = x->frame();
    pl->SetTitle("Gaussian model");
    data->plotOn(pl);
   
    g->fitTo(*data);

    g->plotOn(pl);
    pl->Draw();

    w.import(*data);
    
    w.Print();

    w.writeToFile("Gaussian_data.root", true);
}