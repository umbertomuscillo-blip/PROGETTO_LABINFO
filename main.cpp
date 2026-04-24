#include <iostream>
#include "Carta.h"
#include "Mazzo.h"
#include "Giocatore.h"

using namespace std;

int main() {
    cout << "--- AVVIO UNO FLIP ---" << endl;
    
    Mazzo mazzoDiGioco;
    mazzoDiGioco.mescola();
    
    // Creiamo due giocatori
    Giocatore g1("Umberto");
    Giocatore g2("Prof. Vessio");

    // Distribuiamo 7 carte a testa come da regole ufficiali
    cout << "\nDistribuzione carte..." << endl;
    for(int i = 0; i < 7; i++) {
        g1.pescaCarta(mazzoDiGioco.pesca());
        g2.pescaCarta(mazzoDiGioco.pesca());
    }

    // Mostriamo la mano di Umberto (Lato Chiaro, come all'inizio della partita)
    g1.mostraMano(false);

    // Simuliamo che qualcuno giochi un FLIP! Mostriamo di nuovo la mano, ma al rovescio
    cout << "\n*** E' STATA GIOCATA LA CARTA FLIP! ***" << endl;
    g1.mostraMano(true);

    return 0;
}
