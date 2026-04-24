#ifndef CARTA_H
#define CARTA_H

#include <string>

// Definiamo i Colori possibili (sia per il lato chiaro che oscuro)
// NERO serve per le carte Jolly che non hanno un colore di base
enum Colore { ROSSO, GIALLO, VERDE, BLU, ROSA, VERDE_ACQUA, ARANCIONE, VIOLA, NERO };

// Definiamo i Valori possibili delle carte (numeri e speciali per entrambi i lati)
enum Valore { ZERO, UNO, DUE, TRE, QUATTRO, CINQUE, SEI, SETTE, OTTO, NOVE, SALTA, INVERTI, PESCA_UNO, PESCA_CINQUE, SALTA_TUTTI, JOLLY, JOLLY_PESCA };

class Carta {
private:
    // Attributi: i dati che ogni singola carta possiede
    // Lato Chiaro
    Colore coloreChiaro;
    Valore valoreChiaro;
    
    // Lato Oscuro
    Colore coloreOscuro;
    Valore valoreOscuro;

public:
    // Costruttore: il "metodo" speciale che viene chiamato quando creiamo una nuova carta
    Carta(Colore cChiaro, Valore vChiaro, Colore cOscuro, Valore vOscuro);

    // Metodi (per ora dichiariamo solo che esisteranno)
    std::string getDescrizioneChiara();
    std::string getDescrizioneOscura();
};

#endif