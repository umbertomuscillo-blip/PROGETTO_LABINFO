#include "Carta.h"

// Costruttore della classe Carta.
// Inizializza una carta con i parametri per entrambi i lati (Chiaro e Oscuro).
// Questa struttura a 4 attributi è fondamentale per implementare la meccanica "Flip" del gioco.
Carta::Carta(Colore cChiaro, Valore vChiaro, Colore cOscuro, Valore vOscuro) {
    coloreChiaro = cChiaro;
    valoreChiaro = vChiaro;
    coloreOscuro = cOscuro;
    valoreOscuro = vOscuro;
}

// ==============================================================================
// FUNZIONI HELPER (Di supporto)
// Queste funzioni non fanno parte della classe (non hanno Carta::), ma servono 
// internamente a questo file per tradurre i tipi Enum in stringhe leggibili.
// ==============================================================================

// Traduce l'enumerazione Colore in una stringa di testo.
// Utile per l'interfaccia grafica e per la stampa a terminale.
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
        case NERO: return "(Nessun colore base)"; // Caso specifico per i Jolly
        default: return "Sconosciuto";
    }
}

// Traduce l'enumerazione Valore in una stringa di testo.
std::string valoreToString(Valore v) {
    switch(v) {
        case UNO: return "1"; case DUE: return "2"; case TRE: return "3";
        case QUATTRO: return "4"; case CINQUE: return "5"; case SEI: return "6";
        case SETTE: return "7"; case OTTO: return "8"; case NOVE: return "9";
        case SALTA: return "SAL";
        case SALTA_TUTTI: return "ALL";
        case INVERTI: return "INV";
        case FLIP: return "FLP";
        case PESCA_UNO: return "+1";
        case PESCA_CINQUE: return "+5";
        case JOLLY: return "JOL";
        case JOLLY_PESCA_DUE: return "JOL +2";
        case JOLLY_PESCA_COLORE: return "COL";
        default: return "Sconosciuto";
    }
}

// ==============================================================================
// METODI PUBBLICI DELLA CLASSE CARTA
// ==============================================================================

// Restituisce il nome completo della carta per il Lato Chiaro (es. "7 Rosso").
std::string Carta::getDescrizioneChiara() {
    // Concatena il valore e il colore convertiti in stringa
    return valoreToString(valoreChiaro) + " " + coloreToString(coloreChiaro);
}

// Restituisce il nome completo della carta per il Lato Oscuro (es. "Pesca Cinque Viola").
std::string Carta::getDescrizioneOscura() {
    return valoreToString(valoreOscuro) + " " + coloreToString(coloreOscuro);
}

// Getter per il Colore della carta in base allo stato attuale della partita.
// Parametro: latoOscuro Booleano che indica se il tavolo è attualmente girato sul lato oscuro.
// Ritorna: Il colore corrispondente alla faccia visibile.
Colore Carta::getColore(bool latoOscuro) {
    // Utilizzo dell'operatore ternario (condizione ? se_vero : se_falso)
    // È una forma compatta e molto elegante per evitare un costrutto if-else
    return latoOscuro ? coloreOscuro : coloreChiaro; 
}

// Getter per il Valore della carta in base allo stato attuale della partita.
// Parametro: latoOscuro Booleano che indica se il tavolo è attualmente girato sul lato oscuro.
// Ritorna: Il valore corrispondente alla faccia visibile.
Valore Carta::getValore(bool latoOscuro) {
    return latoOscuro ? valoreOscuro : valoreChiaro; 
}