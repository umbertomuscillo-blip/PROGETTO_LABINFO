/**
 * @file Mazzo.cpp
 * @brief Implementazione della classe Mazzo.
 * Gestisce l'algoritmo di generazione delle carte a doppia faccia (Flip), 
 * il mescolamento e le dinamiche di estrazione (pesca/scarto).
 */

#include "Mazzo.h"
#include <iostream>
#include <algorithm> // Necessario per std::shuffle
#include <random>    // Necessario per std::default_random_engine
#include <chrono>    // Necessario per generare un 'seed' basato sul tempo reale

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
 * Serve a modellare una singola "faccia" di una carta prima che venga incollata
 * fisicamente al suo dorso.
 */
struct MezzaCarta {
    Colore c;
    Valore v;
};

/**
 * @brief Genera l'intero mazzo di 112 carte di UNO Flip.
 * Algoritmo: Poiché in UNO Flip il lato oscuro non corrisponde in modo predefinito 
 * al lato chiaro, l'algoritmo genera due pool di "mezze carte" (uno chiaro e uno oscuro) 
 * separatamente. Mescola poi solo il pool oscuro e incolla le facce a coppie, 
 * garantendo che ad ogni nuova partita le combinazioni fronte/retro siano uniche e imprevedibili.
 */
void Mazzo::inizializzaMazzo() {
    carteDaPescare.clear();
    carteScartate.clear();

    // Vettori temporanei per stoccare le due facce separate
    std::vector<MezzaCarta> latiChiari;
    std::vector<MezzaCarta> latiOscuri;

    // Array costanti per iterare agevolmente sui colori
    Colore coloriChiari[] = {ROSSO, GIALLO, VERDE, BLU};
    Colore coloriOscuri[] = {ARANCIONE, VIOLA, VERDE_ACQUA, ROSA};

    // --- FASE 1: GENERAZIONE DEI LATI ---
    for (int c = 0; c < 4; c++) {
        // Generazione delle carte numeriche (Da 1 a 9, due copie per colore)
        for (int v = UNO; v <= NOVE; v++) {
            latiChiari.push_back({coloriChiari[c], static_cast<Valore>(v)});
            latiChiari.push_back({coloriChiari[c], static_cast<Valore>(v)});
            
            latiOscuri.push_back({coloriOscuri[c], static_cast<Valore>(v)});
            latiOscuri.push_back({coloriOscuri[c], static_cast<Valore>(v)});
        }
        // Generazione delle carte speciali colorate (Due copie per colore)
        for (int i = 0; i < 2; i++) {
            // Effetti Lato Chiaro
            latiChiari.push_back({coloriChiari[c], PESCA_UNO});
            latiChiari.push_back({coloriChiari[c], INVERTI});
            latiChiari.push_back({coloriChiari[c], SALTA});
            latiChiari.push_back({coloriChiari[c], FLIP});

            // Effetti Lato Oscuro
            latiOscuri.push_back({coloriOscuri[c], PESCA_CINQUE});
            latiOscuri.push_back({coloriOscuri[c], INVERTI});
            latiOscuri.push_back({coloriOscuri[c], SALTA_TUTTI});
            latiOscuri.push_back({coloriOscuri[c], FLIP});
        }
    }

    // Generazione carte Jolly (Non colorate, 4 per tipo)
    for (int i = 0; i < 4; i++) {
        latiChiari.push_back({NERO, JOLLY});
        latiChiari.push_back({NERO, JOLLY_PESCA_DUE});

        latiOscuri.push_back({NERO, JOLLY});
        latiOscuri.push_back({NERO, JOLLY_PESCA_COLORE});
    }

    // --- FASE 2: RANDOMIZZAZIONE DEGLI ABBINAMENTI ---
    // Generiamo un "seme" basato sui millisecondi dell'orologio di sistema 
    // per garantire una mescolata sempre diversa ad ogni esecuzione.
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    
    // Mescoliamo SOLO il vettore dei lati oscuri. I lati chiari restano ordinati.
    std::shuffle(latiOscuri.begin(), latiOscuri.end(), std::default_random_engine(seed));

    // --- FASE 3: INCOLLAGGIO (Costruzione degli oggetti Carta finali) ---
    for (int i = 0; i < latiChiari.size(); i++) {
        // Creiamo l'oggetto Carta vero e proprio unendo il lato chiaro[i] con il lato oscuro[i]
        carteDaPescare.push_back(Carta(
            latiChiari[i].c, latiChiari[i].v, 
            latiOscuri[i].c, latiOscuri[i].v
        ));
    }
}

/**
 * @brief Mescola l'intero mazzo finito (Le carte già incollate).
 * Viene chiamata all'inizio della partita per determinare l'ordine di pesca.
 */
void Mazzo::mescola() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(carteDaPescare.begin(), carteDaPescare.end(), std::default_random_engine(seed));
}

/**
 * @brief Getter per conoscere quante carte ci sono ancora nel mazzo.
 */
int Mazzo::carteRimanenti() {
    return carteDaPescare.size();
}

/**
 * @brief Estrae la carta in cima al mazzo per darla a un giocatore.
 * In C++, la fine del vettore (back) rappresenta la cima della pila, 
 * in quanto è l'operazione più veloce per estrarre elementi da un vector.
 * @return La carta estratta.
 */
Carta Mazzo::pesca() {
    // 1. Facciamo una copia della carta in cima (l'ultimo elemento)
    Carta pescata = carteDaPescare.back(); 
    // 2. Rimuoviamo fisicamente la carta dal vettore
    carteDaPescare.pop_back();             
    // 3. Restituiamo la copia al chiamante
    return pescata;
}

/**
 * @brief Aggiunge una carta alla pila degli scarti sul tavolo.
 * @param c La carta appena giocata.
 */
void Mazzo::scarta(Carta c) {
    carteScartate.push_back(c);
}