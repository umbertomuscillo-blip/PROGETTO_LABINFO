#ifndef PARTITA_H
#define PARTITA_H

#include <vector>
#include <string>
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
    Partita(std::vector<Giocatore> listaGiocatori);

    void setupIniziale();
    void eseguiTurno(); 
    bool partitaTerminata();
    std::string getVincitore();
    bool mossaValida(Carta c);
    void applicaEffetto(Carta c);
    void passaAlProssimoGiocatore();
    void stampaStatoPartita();

    // Metodi per la Grafica
    Carta getCartaInCima();
    bool getLatoOscuroAttivo();
    std::vector<Giocatore> getGiocatori();
    int getTurnoCorrente();
    void mossaUmano(int scelta);
    void mossaBot();
};

#endif