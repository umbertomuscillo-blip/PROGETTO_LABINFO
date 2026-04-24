#include "Partita.h"
#include <iostream>

using namespace std;

// Costruttore: riceve i nomi e crea i giocatori
Partita::Partita(vector<string> nomiGiocatori) : cartaInCima(ROSSO, UNO, ROSSO, UNO) {
    for (int i = 0; i < nomiGiocatori.size(); i++) {
        giocatori.push_back(Giocatore(nomiGiocatori[i]));
    }

    // Impostiamo lo stato iniziale
    turnoCorrente = 0;
    sensoOrario = true;
    latoOscuroAttivo = false; // Inizia sempre con il Lato Chiaro
}

void Partita::setupIniziale() {
    cout << "Avvio preparazione partita..." << endl;
    
    // Mescolamento del mazzo
    mazzo.mescola();
    
    // Distribuzione di 7 carte a testa
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < giocatori.size(); j++) {
            giocatori[j].pescaCarta(mazzo.pesca());
        }
    }
    
    // Scopriamo la prima carta sul tavolo
    cartaInCima = mazzo.pesca();
    coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
}

void Partita::stampaStatoPartita() {
    cout << "\n=====================================" << endl;
    cout << "CARTA SUL TAVOLO: ";
    if (latoOscuroAttivo) {
        cout << cartaInCima.getDescrizioneOscura();
    } else {
        cout << cartaInCima.getDescrizioneChiara();
    }
    cout << "\nVerso di gioco: " << (sensoOrario ? "Orario ->" : "<- Antiorario");
    cout << "\nE' IL TURNO DI: " << giocatori[turnoCorrente].getNome() << endl;
    cout << "=====================================\n" << endl;
    
    // Mostriamo la mano del giocatore di turno
    giocatori[turnoCorrente].mostraMano(latoOscuroAttivo);
}

// STUBS: Funzioni per ora "vuote" per evitare errori. Le riempiremo man mano!
void Partita::eseguiTurno() {}
bool Partita::partitaTerminata() { return false; }
bool Partita::mossaValida(Carta c) { return true; }
void Partita::applicaEffetto(Carta c) {}
void Partita::passaAlProssimoGiocatore() {}