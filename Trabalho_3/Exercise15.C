#include "TMath.h"
#include "TH1D.h"
#include "TF1.h"
#include "TStyle.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include <iostream>

using namespace std;

/* We will start with an exercise where we fin an histogram with a simple function, to get familiar with the fitting options in ROOT.

Start creating an histogram with 50 bins in [-5,5] and fill with 1000 Gaussian distributed number
Fit the histogram using a Gaussian function
Get the value and error of the width of the gaussian
Retrieve the fit result and print the correlation matrix. */

void Ex15(){
    //criando o histograma e enchendo com números de uma distribuição normal
    TH1D *hist = new TH1D("histgaussian", "Gaussian Histogram", 50, -5, 5);
    hist->FillRandom("gaus",10000);
    
    //Criada a função que será usada no fit, inicializando os parâmetros e fitando
    TF1 *f1 = new TF1("f1","gaus");
    f1->SetParameters(1, 0, 1);
    hist->Fit(f1, "P");

    //Setando que os parâmetros do fit serão colocados na legenda e desenhando o histograma com o fit
    gStyle->SetOptFit(1111);
    hist->Draw();

    //os valores do sigma da gaussiana fitada são escritos na tela
    auto width = f1->GetParameter("Sigma");
    auto widthe = f1->GetParError(f1->GetParNumber("Sigma"));
    cout << "=============================================" << endl;
    cout << "The width is: " << width << " with error: " << widthe << endl;
    cout << "=============================================" << endl;

    //Para obter a matriz de correlação, a classe FitResult é utilizada
    TFitResultPtr r = hist->Fit(f1,"S Q");
    TMatrixDSym C = r->GetCorrelationMatrix();
    C.Print();

}