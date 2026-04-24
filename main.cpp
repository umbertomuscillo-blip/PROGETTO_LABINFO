#include <iostream>
#include <vector>
#include "Partita.h"

using namespace std;

int main() {
    cout << "--- BENVENUTI A UNO FLIP ---" << endl;
    
    vector<Giocatore> giocatori;
    giocatori.push_back(Giocatore("Umberto", false));   // Umberto è Umano (false)
    giocatori.push_back(Giocatore("Prof. Vessio", true)); // Il Prof. Vessio è controllato dal BOT (true)

    Partita gioco(giocatori);
    gioco.setupIniziale();

    while (!gioco.partitaTerminata()) {
        gioco.eseguiTurno();
    }

    return 0;
}


   