/* Esse programa cria uma GUI para apresentação de dados, a GUI é composta de um Main Frame (fMain),
onde serão colocados um Embedded Canvas (fEcanvas) e um Horizontal Frame no qual serão colocados 4
botões.
1º botão - Apresenta os dados da Turma de Fisica Geral 2001.2 contidos no livro Estimativas e Erros
em experimentos de Física.
2º botão - Faz um grafico de dispersão do número de caras de uma moeda pelo número de jogadas conforme
feito no livro Métodos Estatísticos em Física Experiemental.
3º botão - Salva uma imagem do desenho feito no Canvas.
4º botão - Fecha o programa. É interessante usar esse botão para fechar e não o da janela, pois ele
finaliza toda a sessão sem erros. */

#include <TGClient.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TRandom.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <RQ_OBJECT.h>
#include "Dist_Moeda.h"
#include <TSystem.h>
#include <TImage.h>
#include "DadosFisGeral.h"
#include "TLine.h"
#include "TVirtualPad.h"

using namespace std;

//Definindo a Classe para criação da janela
class MyMainFrame {
   RQ_OBJECT("MyMainFrame");
private:
   TGMainFrame         *fMain;
   TRootEmbeddedCanvas *fEcanvas;
   TString save_name;
public:
   MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
   virtual ~MyMainFrame();
   void DesenhaFisGeral();
   void DesenhaMoeda(); 
   void Save();
   void SetSaveName(const char* name) {this->save_name = name;}
};

MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) {
   // Criando frame principal
   fMain = new TGMainFrame(p,w,h);
   
   //Chamando as funções que organizam os dados a serem apresentados
   DadosFisGeral();
   Dist_Moeda();

   // Criando o Canvas e colocando no frame principal
   fEcanvas = new TRootEmbeddedCanvas("Ecanvas",fMain,800,600);
   fMain->AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10,10,10,1));

   // Criando o frame horizontal e seus botões
   TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain,300,40);
   TGTextButton *draw = new TGTextButton(hframe,"Dados Fisica Geral");
   TGTextButton *draw2 = new TGTextButton(hframe,"Lancamento Moedas");
   TGTextButton *save = new TGTextButton(hframe, "Salvar");
   TGTextButton *exit = new TGTextButton(hframe,"Sair","gApplication->Terminate(0)");
   
   // Associando os Botões as suas respectivas funções 
   draw->Connect("Clicked()","MyMainFrame",this,"DesenhaFisGeral()");
   draw2->Connect("Clicked()","MyMainFrame",this,"DesenhaMoeda()");
   save->Connect("Clicked()","MyMainFrame",this,"Save()");

   // Adcionando os botões ao frame horizontal
   hframe->AddFrame(draw, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
   hframe->AddFrame(draw2, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
   hframe->AddFrame(save, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
   hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,5,5,3,4));

   //Adcionando o frame horizontal ao frame 
   fMain->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,2,2,2,2));

   // Colocando o nome da janela
   fMain->SetWindowName("Exercicio");

   // Mapeando as subjanelas do frame principal
   fMain->MapSubwindows();

   // Definindo o tamanho da janela
   fMain->Resize(1200,800);

   // Mapeando o frame principal
   fMain->MapWindow();
}

/* Método que desenha no canvas os histogramas e o grafico de correlação dos dados da 
turma de Física Geral 2001.2 */
void MyMainFrame::DesenhaFisGeral() {
   //Esse método seta o nome e a extensão em que a figura será salva ao apertar o botão salvar
   this->SetSaveName("Histograms.gif");
   
   //Um canvas é criado como o canvas embutido, é limpo e dividido
   TCanvas *fCanvas = fEcanvas->GetCanvas();
   fCanvas->Clear();
   fCanvas->Divide(2,2);
   
   // Aqui é feito todo o processo para leitura dos dados no arquivo
   TString dir = gSystem->UnixPathName(__FILE__);
   dir.ReplaceAll("GUI.C","");
   
   TFile *file = new TFile(Form("%sFisGeral2001_2.root", dir.Data()));
   TTree *tree = (TTree*) file->Get("fisgeral")->Clone();
   
   //Os histogramas retirados do file
   TH1I *h_idade = (TH1I*)file->Get("hist_idade")->Clone();
   TH1D *h_massa = (TH1D*)file->Get("hist_massa")->Clone();
   TH1D *h_altura = (TH1D*)file->Get("hist_altura")->Clone();
   
   h_idade->SetFillColor(kBlue);
   h_idade->GetXaxis()->SetTitle("idade [anos]");
   h_massa->SetFillColor(kBlue);
   h_massa->GetXaxis()->SetTitle("massa [anos]");
   h_altura->SetFillColor(kBlue);
   h_altura->GetXaxis()->SetTitle("altura [anos]");

   //Os dados de massa e altura são copiados para duas arrays para criação do gráfico
   double massa, altura;
   tree->SetBranchAddress("massa",&massa);
   tree->SetBranchAddress("altura",&altura);
   int n = tree->GetEntries();
   double x[n], y[n];

   for (auto i = 0; i != n; ++i){
      tree->GetEntry(i);
      x[i] = massa;
      y[i] = altura;
   }

   //Gráfico criado e estilizado
   TGraph *gr = new TGraph(n, x, y);
   gr->SetMarkerStyle(20);
   gr->SetMarkerSize(.8);
   gr->SetTitle("Diagrama de Dispers#tilde{a}o altura x massa");
   gr->GetXaxis()->SetTitle("Massa [kg]");
   gr->GetYaxis()->SetTitle("Altura [cm]");
   
   //as médias dos dados de altura e massa para desenhar linhas nos gráficos
   auto x_mean = h_massa->GetMean();
   auto y_mean = h_altura->GetMean();
   
   //Desenhado os histogramas e gráfico em cada uma dos subpads do canvas
   fCanvas->cd(1);
   h_idade->Draw();
   fCanvas->cd(2);
   h_massa->Draw();
   fCanvas->cd(3);
   h_altura->Draw();
   fCanvas->cd(4);
   gr->Draw("AP");

   //linhas das médias são criadas e desenhadas no gráfico
   TLine *line_x = new TLine(x_mean, gr->GetYaxis()->GetXmax(), x_mean, gr->GetYaxis()->GetXmin());
   TLine *line_y = new TLine(gr->GetXaxis()->GetXmax(), y_mean, gr->GetXaxis()->GetXmin(), y_mean);
   line_x->Draw();
   line_y->Draw();

   //canvas é selecionado e atualizado
   fCanvas->cd();
   fCanvas->Update();
}

//Esse método desenha o gráfico da dispersão de caras de várias jogadas de uma moeda
void MyMainFrame::DesenhaMoeda() {
   // Selecionando o nome do arquivo para salvar
   this->SetSaveName("Disp_Moeda.gif");

   //Canvas criado, associado ao canvas embutido, limpo e selecionado
   TCanvas *fCanvas = fEcanvas->GetCanvas();
   fCanvas->Clear();
   fCanvas->cd();
   
   //Função desenha o gráfico de disp x n. de jogadas
   PrintData();

   // canvas é atualizado
   fCanvas->Update();
}

//Esse método salva uma figura com o nome determinado pela propriedade save_name
void MyMainFrame::Save() {
   /* A propriedade save_name começa o programa como uma string vazia, o if observa isso e
   caso nenhuma figura tenha sido desenhada no canvas, não salva o canvas, isso previne que
   uma figura vazia seja salva */
   if (save_name != ""){
      TCanvas *fCanvas = fEcanvas->GetCanvas();
      fCanvas->Print(save_name);
   }
}

// Método destrutor, limpa todos os widgets e deleta a janela
MyMainFrame::~MyMainFrame() {
   fMain->Cleanup();
   delete fMain;
}

// Cria uma nova janela.
void GUI() {
   new MyMainFrame(gClient->GetRoot(),800,600);
}

