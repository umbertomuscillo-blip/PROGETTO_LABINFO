#include "Mazzo.h"
#include <iostream>
#include <algorithm> 
#include <random>    
#include <chrono>    

Mazzo::Mazzo() {
    inizializzaMazzo();
}

// Struttura di supporto temporanea solo per la generazione
struct MezzaCarta {
    Colore c;
    Valore v;
};

void Mazzo::inizializzaMazzo() {
    carteDaPescare.clear();
    carteScartate.clear();

    // Creiamo due liste temporanee per i lati separati
    std::vector<MezzaCarta> latiChiari;
    std::vector<MezzaCarta> latiOscuri;

    Colore coloriChiari[] = {ROSSO, GIALLO, VERDE, BLU};
    Colore coloriOscuri[] = {ARANCIONE, VIOLA, VERDE_ACQUA, ROSA};

    // GENERIAMO I DUE LATI SEPARATAMENTE
    for (int c = 0; c < 4; c++) {
        // Numeri
        for (int v = UNO; v <= NOVE; v++) {
            latiChiari.push_back({coloriChiari[c], static_cast<Valore>(v)});
            latiChiari.push_back({coloriChiari[c], static_cast<Valore>(v)});
            
            latiOscuri.push_back({coloriOscuri[c], static_cast<Valore>(v)});
            latiOscuri.push_back({coloriOscuri[c], static_cast<Valore>(v)});
        }
        // Carte speciali colorate
        for (int i = 0; i < 2; i++) {
            latiChiari.push_back({coloriChiari[c], PESCA_UNO});
            latiChiari.push_back({coloriChiari[c], INVERTI});
            latiChiari.push_back({coloriChiari[c], SALTA});
            latiChiari.push_back({coloriChiari[c], FLIP});

            latiOscuri.push_back({coloriOscuri[c], PESCA_CINQUE});
            latiOscuri.push_back({coloriOscuri[c], INVERTI});
            latiOscuri.push_back({coloriOscuri[c], SALTA_TUTTI});
            latiOscuri.push_back({coloriOscuri[c], FLIP});
        }
    }

    // Jolly (4 per tipo)
    for (int i = 0; i < 4; i++) {
        latiChiari.push_back({NERO, JOLLY});
        latiChiari.push_back({NERO, JOLLY_PESCA_DUE});

        latiOscuri.push_back({NERO, JOLLY});
        latiOscuri.push_back({NERO, JOLLY_PESCA_COLORE});
    }

    // MESCOLIAMO SOLO I LATI OSCURI per creare abbinamenti casuali e imprevedibili!
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(latiOscuri.begin(), latiOscuri.end(), std::default_random_engine(seed));

    // INCOLLIAMO LE DUE METÀ per creare le carte definitive
    for (int i = 0; i < latiChiari.size(); i++) {
        carteDaPescare.push_back(Carta(
            latiChiari[i].c, latiChiari[i].v, 
            latiOscuri[i].c, latiOscuri[i].v
        ));
    }
}

void Mazzo::mescola() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(carteDaPescare.begin(), carteDaPescare.end(), std::default_random_engine(seed));
}

int Mazzo::carteRimanenti() {
    return carteDaPescare.size();
}

Carta Mazzo::pesca() {
    Carta pescata = carteDaPescare.back(); 
    carteDaPescare.pop_back();             
    return pescata;
}

void Mazzo::scarta(Carta c) {
    carteScartate.push_back(c);
}