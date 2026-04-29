/**
 * @file Partita.h
 * @brief Definizione della classe Partita. Gestisce le regole, i turni e gli effetti di UNO Flip.
 */

#ifndef PARTITA_H
#define PARTITA_H

#include <vector>
#include <string>
#include "Giocatore.h"
#include "Mazzo.h"

class Partita {
private:
    std::vector<Giocatore> giocatori;
    Mazzo mazzo;
    std::vector<Carta> scarti;
    
    int turnoCorrente;
    bool sensoOrario;
    bool latoOscuroAttivo;
    Colore coloreAttivo;
    
    bool partitaFinita;
    std::string vincitore;
    
    std::string messaggioAvviso;
    bool mostraAvvisoPopup;
    std::string ultimoLogBot;

    void passaTurno();
    void applicaEffettoCarta(Carta c, int coloreScelto = 0);
    void mostraAvviso(std::string msg);

public:
    Partita(std::vector<Giocatore> listaGiocatori);
    
    void setupIniziale();
    
    bool mossaValida(Carta c);
    
    // --- METODI DI INTERAZIONE ---
    void mossaUmano(int indiceCarta, int coloreScelto, bool dettoUno);
    void mossaBot();
    void mossaRete(int indiceCarta, int coloreScelto, bool dettoUno); // NUOVO: Per il Multiplayer!

    // --- GETTERS ---
    std::vector<Giocatore> getGiocatori() const;
    Carta getCartaInCima() const;
    Colore getColoreAttivo() const;
    bool getLatoOscuroAttivo() const;
    int getTurnoCorrente() const;
    bool getSensoOrario() const;
    bool partitaTerminata() const;
    std::string getVincitore() const;
    
    std::string getMessaggioAvviso() const;
    bool getMostraAvviso() const;
    void resetAvviso();
    std::string getUltimoLogBot() const;
};

#endif