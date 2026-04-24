#include <iostream>
#include <vector>
#include "Partita.h"

using namespace std;

int main() {
    cout << "--- BENVENUTI A UNO FLIP ---" << endl;
    
    // Creiamo una lista di nomi
    vector<string> giocatori;
    giocatori.push_back("Umberto");
    giocatori.push_back("Prof. Vessio");

    // Avviamo il "cervello" del gioco
    Partita gioco(giocatori);

    // Diciamo alla partita di fare mescolamento e distribuzione
    gioco.setupIniziale();

    // Mostriamo il tavolo pronto per il primo turno!
        // Il cuore del gioco: un ciclo che continua finché la partita non finisce!
    while (!gioco.partitaTerminata()) {
        gioco.eseguiTurno();
    }

    return 0;
}


   