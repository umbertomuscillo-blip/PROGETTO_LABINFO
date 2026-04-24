#ifndef PARTITA_H
#define PARTITA_H

#include <vector>
#include "Mazzo.h"
#include "Giocatore.h"

class Partita {
private:
    std::vector<Giocatore> giocatori;
    Mazzo mazzo;
    int turnoCorrente;
    bool sensoOrario;
    bool latoOscuroAttivo;
    Carta cartaInCima;
    Colore coloreAttivo;

public:
    // Ora passiamo direttamente gli oggetti Giocatore, così sappiamo chi è il Bot!
    Partita(std::vector<Giocatore> listaGiocatori);

    void setupIniziale();
    void eseguiTurno();
    bool partitaTerminata();
    bool mossaValida(Carta c);
    void applicaEffetto(Carta c);
    void passaAlProssimoGiocatore();
    void stampaStatoPartita();
};

#endif