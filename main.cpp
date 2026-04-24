#include <iostream>
#include "Carta.h" // Importiamo il "menu" della nostra carta

using namespace std;

int main() {
    cout << "Avvio simulazione UNO Flip..." << endl;

    // Creiamo una carta: Lato chiaro = 1 Rosso, Lato oscuro = Pesca Cinque Arancione
    Carta primaCarta(ROSSO, UNO, ARANCIONE, PESCA_CINQUE);

    cout << "Ho creato la mia prima carta in memoria!" << endl;

    return 0;
}