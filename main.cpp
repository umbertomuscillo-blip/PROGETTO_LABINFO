#include <iostream>
#include "Carta.h"

using namespace std;

int main() {
    // Creiamo una carta "Flip"
    Carta miaCarta(ROSSO, FLIP, ARANCIONE, FLIP);

    cout << "--- TEST CARTA ---" << endl;
    cout << "Lato Chiaro: " << miaCarta.getDescrizioneChiara() << endl;
    cout << "Lato Oscuro: " << miaCarta.getDescrizioneOscura() << endl;

    return 0;
}



