#include "Giocatore.h"
#include <iostream>

using namespace std;

// Costruttore
Giocatore::Giocatore(string nomeGiocatore) {
    nome = nomeGiocatore;
}

string Giocatore::getNome() {
    return nome;
}

int Giocatore::getNumeroCarte() {
    return mano.size();
}

bool Giocatore::haFinitoLeCarte() {
    return mano.empty(); // Ritorna 'true' se la mano è vuota
}

void Giocatore::pescaCarta(Carta c) {
    mano.push_back(c); // Aggiunge la carta alla fine della lista
}

Carta Giocatore::giocaCarta(int indice) {
    // Salviamo la carta che il giocatore vuole giocare
    Carta cartaGiocata = mano[indice]; 
    
    // Rimuoviamo la carta dalla mano (la funzione erase richiede un "iteratore")
    mano.erase(mano.begin() + indice); 
    
    return cartaGiocata;
}

void Giocatore::mostraMano(bool latoOscuroAttivo) {
    cout << "\n--- Carte di " << nome << " ---" << endl;
    
    for (int i = 0; i < mano.size(); i++) {
        cout << "[" << i << "] ";
        if (latoOscuroAttivo) {
            // Se siamo nel lato oscuro, mostriamo solo quello!
            cout << mano[i].getDescrizioneOscura() << endl;
        } else {
            // Altrimenti mostriamo il lato chiaro
            cout << mano[i].getDescrizioneChiara() << endl;
        }
    }
    cout << "-----------------------" << endl;
}