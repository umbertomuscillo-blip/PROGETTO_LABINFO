/**
 * @file main.cpp
 * @brief Punto di ingresso per la versione testuale/console del gioco.
 * Utile per il debugging rapido della logica di base (Partita, Carte, Database)
 * senza dover caricare le librerie grafiche SFML.
 */

#include <iostream>
#include <vector>
#include "Partita.h"
#include "Database.h" // Importiamo il sistema di persistenza dei dati

using namespace std;

int main() {
    cout << "--- BENVENUTI A UNO FLIP ---" << endl;
    
    // ==============================================================================
    // 1. CARICAMENTO DEL DATABASE (Fase di Inizializzazione)
    // ==============================================================================
    Database db;
    // Legge i dati dal file "classifica.csv" caricandoli nella RAM
    db.caricaDati();
    // Mostra a terminale lo stato attuale dei punteggi prima di giocare
    db.stampaClassifica(); 
    
    // ==============================================================================
    // 2. SETUP DEI GIOCATORI E DELLE STRUTTURE DATI
    // ==============================================================================
    vector<Giocatore> giocatori;
    giocatori.push_back(Giocatore("Umberto", false));        // Giocatore Umano
    giocatori.push_back(Giocatore("Prof. Vessio", true));    // Giocatore controllato dalla CPU (Bot)

    // Creiamo un vettore parallelo solo con i nomi per passarlo comodamente al Database a fine partita
    vector<string> nomiPartecipanti;
    nomiPartecipanti.push_back("Umberto");
    nomiPartecipanti.push_back("Prof. Vessio");

    // ==============================================================================
    // 3. AVVIO DELLA PARTITA
    // ==============================================================================
    // Istanziazione dell'oggetto centrale del gioco
    Partita gioco(giocatori);
    
    // Mescola il mazzo, distribuisce le 7 carte iniziali e gira la prima carta sul tavolo
    gioco.setupIniziale();

    // ==============================================================================
    // 4. GAME LOOP (Ciclo Principale di Gioco)
    // ==============================================================================
    // Continua a far girare i turni finché la condizione di vittoria non si verifica
    while (!gioco.partitaTerminata()) {
        gioco.eseguiTurno();
    }

    // ==============================================================================
    // 5. SALVATAGGIO STATISTICHE A FINE PARTITA (Teardown)
    // ==============================================================================
    // Estrae il nome di chi ha svuotato la mano per primo
    string vincitore = gioco.getVincitore();
    
    // Modifica le statistiche in RAM (aggiunge 1 giocata a tutti e 1 vittoria al vincitore)
    db.aggiornaStatistiche(nomiPartecipanti, vincitore);
    
    // Riversa le modifiche dalla RAM al file fisico CSV
    db.salvaDati();
    
    cout << "\nStatistiche salvate correttamente nel database." << endl;
    
    // Stampa la classifica finale aggiornata
    db.stampaClassifica(); 

    return 0;
}