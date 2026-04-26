/**
 * @file Mazzo.h
 * @brief Definizione della classe Mazzo.
 * Modella i due mucchi di carte presenti fisicamente sul tavolo da gioco: 
 * il mazzo coperto da cui pescare e la pila degli scarti scoperta.
 */

#ifndef MAZZO_H
#define MAZZO_H

#include <vector>
#include "Carta.h" // Il Mazzo è un contenitore di oggetti Carta

/**
 * @class Mazzo
 * @brief Gestisce il ciclo di vita delle carte durante la partita.
 * Si occupa di generare le carte all'inizio, mescolarle e simulare il flusso
 * fisico delle carte che passano dal mazzo principale ai giocatori, e dai giocatori agli scarti.
 */
class Mazzo {
private:
    /** * @brief Pila delle carte ancora da pescare (coperte).
     * Implementata come vettore dinamico per facilitare mescolamento ed estrazione rapida (pop_back).
     */
    std::vector<Carta> carteDaPescare;
    
    /** * @brief Pila delle carte già giocate (scoperte sul tavolo).
     * Mantiene lo storico delle giocate e permette il riciclo del mazzo se le carte da pescare finiscono.
     */
    std::vector<Carta> carteScartate;

public:
    /**
     * @brief Costruttore della classe.
     * All'istanziazione dell'oggetto, genera automaticamente le 112 carte del gioco.
     */
    Mazzo();

    // --- Metodi di Gestione del Mazzo ---

    /**
     * @brief Genera e popola il mazzo con tutte le combinazioni previste dal regolamento.
     * Metodo separato dal costruttore per permettere di rigenerare il mazzo
     * nel caso si voglia iniziare una nuova partita senza distruggere l'oggetto.
     */
    void inizializzaMazzo();

    /**
     * @brief Applica un algoritmo di randomizzazione al vettore delle carte da pescare.
     */
    void mescola();

    /**
     * @brief Rimuove la carta in cima (l'ultima del vettore) dal mazzo e la restituisce.
     * @return La Carta pescata.
     */
    Carta pesca();

    /**
     * @brief Riceve una carta giocata e la inserisce nella pila degli scarti.
     * @param c La carta appena messa in tavola da un giocatore.
     */
    void scarta(Carta c);
    
    // --- Metodi di Interrogazione ---

    /**
     * @brief Restituisce il numero esatto di carte ancora disponibili nel mazzo coperto.
     * @return Un intero che rappresenta la dimensione (size) del vettore carteDaPescare.
     */
    int carteRimanenti();
};

#endif