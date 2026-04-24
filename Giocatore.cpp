#include "Giocatore.h"
#include <iostream>

using namespace std;

Giocatore::Giocatore(string nomeGiocatore, bool bot) {
    nome = nomeGiocatore;
    isBot = bot;
}

string Giocatore::getNome() { return nome; }
int Giocatore::getNumeroCarte() { return mano.size(); }
bool Giocatore::haFinitoLeCarte() { return mano.empty(); }
bool Giocatore::getIsBot() { return isBot; }
vector<Carta> Giocatore::getMano() { return mano; }

void Giocatore::pescaCarta(Carta c) { mano.push_back(c); }

Carta Giocatore::giocaCarta(int indice) {
    Carta cartaGiocata = mano[indice]; 
    mano.erase(mano.begin() + indice); 
    return cartaGiocata;
}

void Giocatore::mostraMano(bool latoOscuroAttivo) {
    cout << "\n--- Carte di " << nome << " ---" << endl;
    for (int i = 0; i < mano.size(); i++) {
        cout << "[" << i << "] ";
        if (latoOscuroAttivo) cout << mano[i].getDescrizioneOscura() << endl;
        else cout << mano[i].getDescrizioneChiara() << endl;
    }
    cout << "-----------------------" << endl;
}