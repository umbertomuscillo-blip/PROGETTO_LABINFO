#include <iostream>
#include "Carta.h"
#include "Mazzo.h"  // Novità: diciamo al main che esiste anche il Mazzo!

using namespace std;

int main() {
    cout << "--- AVVIO UNO FLIP ---" << endl;
    
    // 1. Creazione dell'oggetto Mazzo
    Mazzo mazzoDiGioco;
    
    cout << "Mazzo generato. Carte totali: " << mazzoDiGioco.carteRimanenti() << endl;

    // 2. Mescolamento
    mazzoDiGioco.mescola();
    cout << "Mazzo mescolato!" << endl;

    // 3. Estrazione di 3 carte
    cout << "\nPrime 3 carte in cima al mazzo:" << endl;
    for(int i = 0; i < 3; i++) {
        Carta c = mazzoDiGioco.pesca();
        cout << i+1 << ") " << c.getDescrizioneChiara() << " || (Dietro: " << c.getDescrizioneOscura() << ")" << endl;
    }

    // 4. Controllo finale
    cout << "\nCarte rimanenti nel mazzo: " << mazzoDiGioco.carteRimanenti() << endl;

    return 0;
}
