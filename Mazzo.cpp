#include "Mazzo.h"
#include <iostream>
#include <algorithm> // Necessario per mescolare il mazzo
#include <random>    // Necessario per la casualità
#include <chrono>    // Necessario per avere un seed casuale legato al tempo

// Il costruttore chiama subito la funzione per creare le carte
Mazzo::Mazzo() {
    inizializzaMazzo();
}

void Mazzo::inizializzaMazzo() {
    // Svuotiamo le pile nel caso stessimo resettando la partita
    carteDaPescare.clear();
    carteScartate.clear();

    // Creiamo due array di supporto per abbinare i colori Chiari e Oscuri
    Colore coloriChiari[] = {ROSSO, GIALLO, VERDE, BLU};
    Colore coloriOscuri[] = {ARANCIONE, VIOLA, VERDE_ACQUA, ROSA};

    // 1. GENERAZIONE CARTE COLORATE
    for (int c = 0; c < 4; c++) {
        Colore cChiaro = coloriChiari[c];
        Colore cOscuro = coloriOscuri[c];

        // Numeri da 1 a 9 (in UNO Flip ci sono due copie per ogni numero)
        for (int v = UNO; v <= NOVE; v++) {
            // Inseriamo due copie esatte
            carteDaPescare.push_back(Carta(cChiaro, static_cast<Valore>(v), cOscuro, static_cast<Valore>(v)));
            carteDaPescare.push_back(Carta(cChiaro, static_cast<Valore>(v), cOscuro, static_cast<Valore>(v)));
        }

        // Carte speciali colorate (2 copie ciascuna)
        for (int i = 0; i < 2; i++) {
            carteDaPescare.push_back(Carta(cChiaro, PESCA_UNO, cOscuro, PESCA_CINQUE));
            carteDaPescare.push_back(Carta(cChiaro, INVERTI, cOscuro, INVERTI));
            carteDaPescare.push_back(Carta(cChiaro, SALTA, cOscuro, SALTA_TUTTI));
            carteDaPescare.push_back(Carta(cChiaro, FLIP, cOscuro, FLIP));
        }
    }

    // 2. GENERAZIONE CARTE JOLLY (4 copie per tipo, senza colore base)
    for (int i = 0; i < 4; i++) {
        carteDaPescare.push_back(Carta(NERO, JOLLY, NERO, JOLLY));
        carteDaPescare.push_back(Carta(NERO, JOLLY_PESCA_DUE, NERO, JOLLY_PESCA_COLORE));
    }
}

void Mazzo::mescola() {
    // Generiamo un "seme" basato sull'orologio del computer per avere mescolamenti sempre diversi
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(carteDaPescare.begin(), carteDaPescare.end(), std::default_random_engine(seed));
}

int Mazzo::carteRimanenti() {
    return carteDaPescare.size();
}

Carta Mazzo::pesca() {
    // Prende l'ultima carta in cima alla pila
    Carta pescata = carteDaPescare.back(); 
    // La rimuove fisicamente dal mazzo
    carteDaPescare.pop_back();             
    return pescata;
}

void Mazzo::scarta(Carta c) {
    carteScartate.push_back(c);
}