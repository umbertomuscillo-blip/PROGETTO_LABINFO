#ifndef PARTITA_H
#define PARTITA_H

#include <vector>
#include "Mazzo.h"
#include "Giocatore.h"

class Partita {
private:
    std::vector<Giocatore> giocatori;
    Mazzo mazzo;
    
    // Variabili di stato del gioco
    int turnoCorrente;         // Indice di chi deve giocare
    bool sensoOrario;          // true = orario, false = antiorario
    bool latoOscuroAttivo;     // true = oscuro, false = chiaro
    
    // La carta scoperta sul tavolo e il colore da seguire
    Carta cartaInCima;
    Colore coloreAttivo;

public:
    // Costruttore: gli passiamo i nomi dei giocatori
    Partita(std::vector<std::string> nomiGiocatori);

    // Metodi per il flusso del gioco
    void setupIniziale();
    void eseguiTurno();
    bool partitaTerminata();
    
    // Metodi di controllo regole
    bool mossaValida(Carta c);
    void applicaEffetto(Carta c);
    
    // Utilità
    void passaAlProssimoGiocatore();
    void stampaStatoPartita();
};

#endif