#include "TTree.h"
#include "TSystem.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include <iostream>

using namespace RooFit;

/* The aim of this exercise is to learn how to build a composite model in RooFit made of two p.d.f, one representing a signal and one a background
distributions. We want to determine the number of signal events. For this we need to perform an extended maximum likelihood fit, where the signal
events is one of the fit parameter.

Create first the composite model formed by a Gaussian signal over a falling exponential background. Then read the data (in text format) from the
attached file, Hgg.txt and create a RooDataSet class with all the data. Perform then an extended unbinned fit to the data to extract the Higgs
signal strength. Plot the resulting fit function from the fit with separate signal and background components. */

void ex20() {
    //Recenbendo os dados do arquivo Hgg.txt e salvando em uma tree
    TString dir = gSystem->UnixPathName(__FILE__);
    dir.ReplaceAll("Exercise20.C","");
    TTree *t = new TTree("t","tree");
    int nevt = t->ReadFile(Form("%sHgg.txt",dir.Data()),"x/D");
    
    //criando o workspace com as variáveis necessárias
    RooWorkspace w("w");
    w.factory("nbg[10000,0,10000]");
    w.var("nbg")->setMin(0.1*nevt);
    w.var("nbg")->setMax(10*nevt);

    w.factory("x[110,160]");
    
    //função que modelará o background, exp(-(a1*x/100 + a2*(x/100)^2))
    w.factory("a1[7.5, -500, 500]");
    w.factory("a2[-1.5, -500, 500]");
    w.factory("expr::z('-(a1*x/100 + a2*(x/100)^2)', a1, a2, x)");
    w.factory("Exponential::bg(z, 1)");
    
    //função que modelará o sinal, gaussiana
    w.factory("nsignal[100, 0.0, 1000.0]");
    w.factory("mass[130, 110, 150]");
    w.factory("width[1, 0.5, 5]");
    w.factory("Gaussian::signal(x, mass, width)");

    //criando uma pdf como uma soma do background e sinal
    w.factory("SUM::model(nbg*bg, nsignal*signal)");

    //Recebendo as pdfs do modelo e do sinal
    RooAbsPdf *signal = w.pdf("signal");
    RooAbsPdf *model = w.pdf("model");

    RooRealVar *x = w.var("x");

    //importando os dados da tree para um RooDataSet
    RooDataSet data("data", "data", *x, Import(*t));

    //fazendo um fit dos dados
    RooFitResult *r = model->fitTo(data);

    //fazendo o plot dos dados, da pdf e suas componentes (background e signal).
    RooPlot *p = x->frame();
    data.plotOn(p);
    model->plotOn(p);
    model->plotOn(p, Components("bg"),LineStyle(kDashed));
    model->plotOn(p, Components("signal"),LineColor(kRed));

    p->Draw();

}