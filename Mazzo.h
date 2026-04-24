#ifndef MAZZO_H
#define MAZZO_H

#include <vector>
#include "Carta.h"

class Mazzo {
private:
    std::vector<Carta> carteDaPescare;
    std::vector<Carta> carteScartate;

public:
    // Costruttore
    Mazzo();

    // Metodi principali
    void inizializzaMazzo();
    void mescola();
    Carta pesca();
    void scarta(Carta c);
    
    // Utilità per controllare a che punto siamo
    int carteRimanenti();
};

#endif