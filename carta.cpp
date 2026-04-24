#include "Carta.h"

// Implementazione del costruttore: assegna i valori passati alle variabili della carta
Carta::Carta(Colore cChiaro, Valore vChiaro, Colore cOscuro, Valore vOscuro) {
    coloreChiaro = cChiaro;
    valoreChiaro = vChiaro;
    coloreOscuro = cOscuro;
    valoreOscuro = vOscuro;
}

// Funzione "nascosta" di supporto per tradurre il Colore in testo
std::string coloreToString(Colore c) {
    switch(c) {
        case ROSSO: return "Rosso";
        case GIALLO: return "Giallo";
        case VERDE: return "Verde";
        case BLU: return "Blu";
        case ROSA: return "Rosa";
        case VERDE_ACQUA: return "Verde Acqua";
        case ARANCIONE: return "Arancione";
        case VIOLA: return "Viola";
        case NERO: return "(Nessun colore base)";
        default: return "Sconosciuto";
    }
}

// Funzione "nascosta" di supporto per tradurre il Valore in testo
std::string valoreToString(Valore v) {
    switch(v) {
        case UNO: return "1"; case DUE: return "2"; case TRE: return "3";
        case QUATTRO: return "4"; case CINQUE: return "5"; case SEI: return "6";
        case SETTE: return "7"; case OTTO: return "8"; case NOVE: return "9";
        case SALTA: return "Salta Turno";
        case SALTA_TUTTI: return "Salta Tutti";
        case INVERTI: return "Inverti";
        case FLIP: return "Flip";
        case PESCA_UNO: return "Pesca Uno";
        case PESCA_CINQUE: return "Pesca Cinque";
        case JOLLY: return "Jolly";
        case JOLLY_PESCA_DUE: return "Jolly Pesca Due";
        case JOLLY_PESCA_COLORE: return "Jolly Pesca Colore";
        default: return "Sconosciuto";
    }
}

// I metodi pubblici per la stampa a schermo
std::string Carta::getDescrizioneChiara() {
    return valoreToString(valoreChiaro) + " " + coloreToString(coloreChiaro);
}

std::string Carta::getDescrizioneOscura() {
    return valoreToString(valoreOscuro) + " " + coloreToString(coloreOscuro);
}

// I metodi pubblici che restituiscono i veri valori alla classe Partita per i controlli
Colore Carta::getColore(bool latoOscuro) {
    return latoOscuro ? coloreOscuro : coloreChiaro; // Se è oscuro restituisce oscuro, sennò chiaro
}

Valore Carta::getValore(bool latoOscuro) {
    return latoOscuro ? valoreOscuro : valoreChiaro; // Stessa cosa per il valore
}