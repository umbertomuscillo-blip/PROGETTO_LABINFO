/**
 * @file Mazzo.cpp
 * @brief Implementazione della classe Mazzo.
 * Gestisce l'algoritmo di generazione delle carte a doppia faccia (Flip), 
 * il mescolamento e le dinamiche di estrazione (pesca/scarto).
 */

#include "Mazzo.h"
#include <iostream>
#include <algorithm>
#include <random>

static uint32_t seed_rete = 12345;

void Mazzo::impostaSeedRete(uint32_t seed) {
    seed_rete = seed;
}

uint32_t Mazzo::ottieniNumeroCasuale() {
    seed_rete = seed_rete * 1664525 + 1013904223;
    return seed_rete;
}


/**
 * @brief Costruttore del Mazzo. 
 * Invoca immediatamente la funzione di inizializzazione per preparare le carte.
 */
Mazzo::Mazzo() {
    inizializzaMazzo();
}

/**
 * @struct MezzaCarta
 * @brief Struttura dati temporanea usata ESCLUSIVAMENTE durante la generazione del mazzo.
 */
struct MezzaCarta {
    Colore c;
    Valore v;
};

/**
 * @brief Genera l'intero mazzo di 112 carte di UNO Flip.
 */
void Mazzo::inizializzaMazzo() {
    carteDaPescare.clear();
    carteScartate.clear();

    std::vector<MezzaCarta> latiChiari;
    std::vector<MezzaCarta> latiOscuri;

    Colore coloriChiari[] = {ROSSO, GIALLO, VERDE, BLU};
    Colore coloriOscuri[] = {ARANCIONE, VIOLA, VERDE_ACQUA, ROSA};

    // --- FASE 1: GENERAZIONE DEI LATI ---
    for (int c = 0; c < 4; c++) {
        for (int v = UNO; v <= NOVE; v++) {
            latiChiari.push_back({coloriChiari[c], static_cast<Valore>(v)});
            latiChiari.push_back({coloriChiari[c], static_cast<Valore>(v)});
            
            latiOscuri.push_back({coloriOscuri[c], static_cast<Valore>(v)});
            latiOscuri.push_back({coloriOscuri[c], static_cast<Valore>(v)});
        }
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

    for (int i = 0; i < 4; i++) {
        latiChiari.push_back({NERO, JOLLY});
        latiChiari.push_back({NERO, JOLLY_PESCA_DUE});

        latiOscuri.push_back({NERO, JOLLY});
        latiOscuri.push_back({NERO, JOLLY_PESCA_COLORE});
    }

    // --- FASE 2: RANDOMIZZAZIONE DEGLI ABBINAMENTI (ORA SINCRONIZZATA DALLA RETE) ---
    // Usiamo il nostro generatore pseudo-casuale per garantire che la sequenza
    // sia identica byte per byte su compilatori e OS diversi (Mac vs Windows).
    for (size_t i = latiOscuri.size() - 1; i > 0; --i) {
        size_t j = ottieniNumeroCasuale() % (i + 1);
        std::swap(latiOscuri[i], latiOscuri[j]);
    }

    // --- FASE 3: INCOLLAGGIO ---
    for (int i = 0; i < latiChiari.size(); i++) {
        carteDaPescare.push_back(Carta(
            latiChiari[i].c, latiChiari[i].v, 
            latiOscuri[i].c, latiOscuri[i].v
        ));
    }
}

/**
 * @brief Mescola l'intero mazzo finito (Le carte già incollate).
 */
void Mazzo::mescola() {
    // Usiamo il nostro generatore custom per lo shuffle, per evitare che 
    // std::shuffle produca risultati diversi tra libc++ (Mac) e libstdc++ (Windows).
    for (size_t i = carteDaPescare.size() - 1; i > 0; --i) {
        size_t j = ottieniNumeroCasuale() % (i + 1);
        std::swap(carteDaPescare[i], carteDaPescare[j]);
    }
}

/**
 * @brief Getter per conoscere quante carte ci sono ancora nel mazzo.
 */
int Mazzo::carteRimanenti() {
    return carteDaPescare.size();
}

/**
 * @brief Estrae la carta in cima al mazzo per darla a un giocatore.
 */
Carta Mazzo::pesca() {
    Carta pescata = carteDaPescare.back(); 
    carteDaPescare.pop_back();             
    return pescata;
}

/**
 * @brief Aggiunge una carta alla pila degli scarti sul tavolo.
 */
void Mazzo::scarta(Carta c) {
    carteScartate.push_back(c);
}

std::vector<Carta> Mazzo::getCarteDaPescare() const {
    return carteDaPescare;
}

std::vector<Carta> Mazzo::getCarteScartate() const {
    return carteScartate;
}

void Mazzo::setMazzoDaStato(std::vector<Carta> daPescare, std::vector<Carta> scartate) {
    carteDaPescare = daPescare;
    carteScartate = scartate;
}