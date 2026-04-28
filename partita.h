/**
 * @file Partita.h
 * @brief Definizione della classe centrale Partita.
 */

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
    
    std::string messaggioAvviso;
    bool mostraAvviso;

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

    Carta getCartaInCima();
    Colore getColoreAttivo();
    bool getLatoOscuroAttivo();
    std::vector<Giocatore> getGiocatori();
    int getTurnoCorrente();
    std::string getUltimoLogBot();
    bool getSensoOrario(); // <-- NUOVO METODO!
    
    std::string getMessaggioAvviso();
    bool getMostraAvviso();
    void impostaAvviso(std::string msg);
    void resetAvviso();

    void mossaUmano(int scelta, int coloreScelto = 0, bool haDettoUno = false);
    void mossaBot();
};

#endif