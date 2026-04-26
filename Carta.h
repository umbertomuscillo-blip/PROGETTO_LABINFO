/**
 * @file Carta.h
 * @brief Definizione della classe Carta e delle enumerazioni di base (Colore, Valore).
 * Questo file rappresenta la struttura fondamentale di una carta di UNO Flip,
 * caratterizzata dal possedere due facce giocabili (Lato Chiaro e Lato Oscuro).
 */

#ifndef CARTA_H
#define CARTA_H

#include <string>

/**
 * @enum Colore
 * @brief Rappresenta tutti i colori possibili assumibili da una carta.
 * Include i 4 colori classici (Lato Chiaro), i 4 colori alternativi (Lato Oscuro)
 * e un colore neutro (NERO) riservato alle carte Jolly.
 */
enum Colore { 
    ROSSO, GIALLO, VERDE, BLU,           // Colori Lato Chiaro
    ROSA, VERDE_ACQUA, ARANCIONE, VIOLA, // Colori Lato Oscuro
    NERO                                 // Colore neutro iniziale per le carte speciali (Jolly)
};

/**
 * @enum Valore
 * @brief Rappresenta tutti i numeri e i poteri speciali delle carte.
 * Nota di Game Design: Nel gioco "UNO Flip!" originale non esiste la carta numero ZERO.
 */
enum Valore { 
    UNO, DUE, TRE, QUATTRO, CINQUE, SEI, SETTE, OTTO, NOVE, 
    SALTA,               // Lato Chiaro: Salta il turno del giocatore successivo
    SALTA_TUTTI,         // Lato Oscuro: Salta il turno di tutti gli altri giocatori
    INVERTI,             // Entrambi i lati: Inverte il senso di gioco
    FLIP,                // Entrambi i lati: La carta chiave che capovolge il mazzo e le mani
    PESCA_UNO,           // Lato Chiaro: Fa pescare 1 carta al successivo
    PESCA_CINQUE,        // Lato Oscuro: Fa pescare 5 carte al successivo
    JOLLY,               // Entrambi i lati: Permette di cambiare il colore attivo
    JOLLY_PESCA_DUE,     // Lato Chiaro: Cambia colore e fa pescare 2 carte
    JOLLY_PESCA_COLORE   // Lato Oscuro: Cambia colore e fa pescare carte finché non esce il colore scelto
};

/**
 * @class Carta
 * @brief Modella una singola carta fisica del gioco UNO Flip.
 * Ogni istanza di questa classe memorizza permanentemente le informazioni 
 * per entrambe le sue facce. In questo modo, quando si verifica un FLIP, 
 * la carta mantiene coerente la propria identità.
 */
class Carta {
private:
    // --- Attributi Lato Chiaro ---
    Colore coloreChiaro;
    Valore valoreChiaro;
    
    // --- Attributi Lato Oscuro ---
    Colore coloreOscuro;
    Valore valoreOscuro;

public:
    /**
     * @brief Costruttore della Carta.
     * @param cChiaro Colore della faccia chiara.
     * @param vChiaro Valore/Effetto della faccia chiara.
     * @param cOscuro Colore della faccia oscura.
     * @param vOscuro Valore/Effetto della faccia oscura.
     */
    Carta(Colore cChiaro, Valore vChiaro, Colore cOscuro, Valore vOscuro);

    /**
     * @brief Restituisce la descrizione formattata per la stampa (Lato Chiaro).
     * @return Stringa nel formato "Valore Colore" (es. "7 Verde").
     */
    std::string getDescrizioneChiara();

    /**
     * @brief Restituisce la descrizione formattata per la stampa (Lato Oscuro).
     * @return Stringa nel formato "Valore Colore" (es. "Pesca Cinque Rosa").
     */
    std::string getDescrizioneOscura();

    /**
     * @brief Interroga la carta per conoscerne il colore attualmente giocabile.
     * @param latoOscuro Booleano globale della Partita (true se il gioco è capovolto).
     * @return Il colore della faccia attualmente visibile.
     */
    Colore getColore(bool latoOscuro);

    /**
     * @brief Interroga la carta per conoscerne il valore/potere attualmente giocabile.
     * @param latoOscuro Booleano globale della Partita (true se il gioco è capovolto).
     * @return Il valore della faccia attualmente visibile.
     */
    Valore getValore(bool latoOscuro);
};

#endif