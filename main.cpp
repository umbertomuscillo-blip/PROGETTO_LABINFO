#include <iostream>
#include <vector>
#include "Partita.h"
#include "Database.h" // Importiamo il nostro nuovo sistema di salvataggio!

using namespace std;

int main() {
    cout << "--- BENVENUTI A UNO FLIP ---" << endl;
    
    // 1. CARICAMENTO DATABASE
    Database db;
    db.caricaDati();
    db.stampaClassifica(); // Mostra subito chi è il più forte!
    
    // 2. SETUP GIOCATORI
    vector<Giocatore> giocatori;
    giocatori.push_back(Giocatore("Umberto", false));
    giocatori.push_back(Giocatore("Prof. Vessio", true)); // Il Bot

    vector<string> nomiPartecipanti;
    nomiPartecipanti.push_back("Umberto");
    nomiPartecipanti.push_back("Prof. Vessio");

    // 3. AVVIO PARTITA
    Partita gioco(giocatori);
    gioco.setupIniziale();

    // 4. CICLO DI GIOCO
    while (!gioco.partitaTerminata()) {
        gioco.eseguiTurno();
    }

    // 5. SALVATAGGIO STATISTICHE FINE PARTITA
    string vincitore = gioco.getVincitore();
    db.aggiornaStatistiche(nomiPartecipanti, vincitore);
    db.salvaDati();
    
    cout << "\nStatistiche salvate correttamente nel database." << endl;
    db.stampaClassifica(); // Mostriamo la classifica aggiornata

    return 0;
}