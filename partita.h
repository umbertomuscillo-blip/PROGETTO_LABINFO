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
    std::string ultimoLogBot;

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

    // Metodi Grafica
    Carta getCartaInCima();
    Colore getColoreAttivo();
    bool getLatoOscuroAttivo();
    std::vector<Giocatore> getGiocatori();
    int getTurnoCorrente();
    std::string getUltimoLogBot();
    
    // Aggiornato con scelta colore e flag UNO
    void mossaUmano(int scelta, int coloreScelto = 0, bool haDettoUno = false);
    void mossaBot();
};

#endif