#include "Pythia8/Pythia.h"
#include <iostream>

using namespace Pythia8;

int main() {
  // Setando as configurações do Pythia
  Pythia pythia;
  pythia.readFile("Zdimuon.cmnd");

  // Setando o seed para geração de números aleatórios
  long now = time(NULL);
  pythia.readString("Random:setSeed = on");
  pythia.readString("Random:seed = " + std::to_string(now % 900000000)); 
  pythia.init();

  // Lendo as configurações pertinentes para a geração de eventos
  int nEvent = pythia.mode("Main:numberOfEvents");
  int nAbort = pythia.mode("Main:timesAllowErrors");
  int iAbort = 0;
  
  int cnt = 0;

  // Criação de arquivo que será usado posteriormente na análise de dados
  ofstream out("Zdimuon.dat");
  out << "#Q1:pT1:px1:py1:pz1:E1:phi1:eta1:Q2:pT2:px2:py2:pz2:E2:phi2:eta2" << endl;

  // Geração de eventos
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
    if (!pythia.next()) {
      if (++iAbort < nAbort) continue;
      std::cout << " Event generation aborted prematurely, owing to error!\n";
      break;
    }
    
    /* Análise de cada evento, observando se a partícula é o Z0 (id = 23) caso seja,
    é testado se o resultado de seu decaimento são dois muons. Os dados de cada
    muon é guardado no arquivo Zdimuon.dat */
    for (int i = 0; i < pythia.event.size(); ++i) {
      if (pythia.event[i].id() != 23) continue;
      vector<int> daughters = pythia.event[i].daughterList(); //lista com os resultados do decaimento
      //criação das variáveis pertinentes.
      double Q1, Q2, pT1, pT2, px1, px2, py1, py2, pz1, pz2, E1, E2, phi1, phi2, eta1, eta2;
      
      // Testando se são dois muons
      if (daughters.size() == 2) {
        if (pythia.event[daughters[0]].idAbs() == 13 && pythia.event[daughters[1]].idAbs() == 13 ) {
          pT1 = pythia.event[daughters[0]].pT();
          pT2 = pythia.event[daughters[1]].pT();
          Q1 = pythia.event[daughters[0]].charge();
          Q2 = pythia.event[daughters[1]].charge();
          px1 = pythia.event[daughters[0]].px();
          px2 = pythia.event[daughters[1]].px();
          py1 = pythia.event[daughters[0]].py();
          py2 = pythia.event[daughters[1]].py();
          pz1 = pythia.event[daughters[0]].pz();
          pz2 = pythia.event[daughters[1]].pz();
          E1 = pythia.event[daughters[0]].e();
          E2 = pythia.event[daughters[1]].e();
          phi1 = pythia.event[daughters[0]].phi();
          phi2 = pythia.event[daughters[1]].phi();
          eta1 = pythia.event[daughters[0]].eta();
          eta2 = pythia.event[daughters[1]].eta();
          ++cnt;

          // Escrevendo no arquivo as variáveis.
          out << Q1 << ',' << pT1 << ',' << px1 << ',' << py1 << ',' << pz1 << ',' << E1 << ',' << phi1 << ','
              << eta1 << ',' << Q2 << ',' << pT2 << ',' << px2 << ',' << py2 << ',' << pz2 << ',' << E2 << ','
              << phi2 << ',' << eta2 << endl; 
        }
        // Fim do loop para escrita dos parâmentros dos muons
      }
    }
  //Fim do loop de geração de eventos
  }
  out.close(); // Fechar os arquivos

  cout << endl << "--------  Z -> dimuon decay events = " << cnt << "  -------------------------------------" << endl;
  pythia.stat();

  return 0;
}
