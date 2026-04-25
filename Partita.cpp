#include "Partita.h"
#include <iostream>
#include <cstdlib> // Serve per rand()

using namespace std;

Partita::Partita(vector<Giocatore> listaGiocatori) : cartaInCima(ROSSO, UNO, ROSSO, UNO) {
    giocatori = listaGiocatori; 
    turnoCorrente = 0;
    sensoOrario = true;
    latoOscuroAttivo = false; 
}

void Partita::setupIniziale() {
    mazzo.mescola();
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < giocatori.size(); j++) giocatori[j].pescaCarta(mazzo.pesca());
    }
    cartaInCima = mazzo.pesca();
    coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
}

void Partita::stampaStatoPartita() {
    // Questo ora lo fa l'interfaccia grafica, ma lo lasciamo qui per non rompere il vecchio main.cpp
}

bool Partita::mossaValida(Carta c) {
    Colore coloreCarta = c.getColore(latoOscuroAttivo);
    Valore valoreCarta = c.getValore(latoOscuroAttivo);
    if (coloreCarta == NERO) return true;
    if (coloreCarta == coloreAttivo || valoreCarta == cartaInCima.getValore(latoOscuroAttivo)) return true;
    return false; 
}

void Partita::passaAlProssimoGiocatore() {
    if (sensoOrario) turnoCorrente = (turnoCorrente + 1) % giocatori.size();
    else turnoCorrente = (turnoCorrente - 1 + giocatori.size()) % giocatori.size();
}

void Partita::applicaEffetto(Carta c) {
    Valore v = c.getValore(latoOscuroAttivo);
    int prossimo = sensoOrario ? (turnoCorrente + 1) % giocatori.size() : (turnoCorrente - 1 + giocatori.size()) % giocatori.size();

    if (v == INVERTI) {
        sensoOrario = !sensoOrario;
        if (giocatori.size() == 2) passaAlProssimoGiocatore(); 
    } 
    else if (v == SALTA || v == SALTA_TUTTI) {
        passaAlProssimoGiocatore(); 
    }
    else if (v == FLIP) {
        latoOscuroAttivo = !latoOscuroAttivo; 
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
    }
    else if (v == PESCA_UNO) {
        giocatori[prossimo].pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore();
    }
    else if (v == PESCA_CINQUE) {
        for(int i = 0; i < 5; i++) giocatori[prossimo].pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore();
    }
    else if (v == JOLLY_PESCA_DUE) {
        for(int i = 0; i < 2; i++) giocatori[prossimo].pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore();
    }
    else if (v == JOLLY_PESCA_COLORE) {
        bool trovato = false;
        while (!trovato) {
            Carta pescata = mazzo.pesca();
            giocatori[prossimo].pescaCarta(pescata);
            if (pescata.getColore(latoOscuroAttivo) == coloreAttivo) trovato = true;
        }
        passaAlProssimoGiocatore();
    }
}

// ==========================================
// METODI PER LA GRAFICA (E PER L'IA)
// ==========================================

Carta Partita::getCartaInCima() { return cartaInCima; }
bool Partita::getLatoOscuroAttivo() { return latoOscuroAttivo; }
vector<Giocatore> Partita::getGiocatori() { return giocatori; }
int Partita::getTurnoCorrente() { return turnoCorrente; }

// Questa funzione sostituisce il vecchio eseguiTurno() per il giocatore umano.
// Prende l'indice della carta che il mouse ha cliccato (oppure -1 se clicca il mazzo)
void Partita::mossaUmano(int scelta) {
    Giocatore& g = giocatori[turnoCorrente];
    
    // Ha cliccato il mazzo
    if (scelta == -1) {
        g.pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore();
        return;
    }
    
    // Ha cliccato una carta
    Carta cartaScelta = g.giocaCarta(scelta);
    if (mossaValida(cartaScelta)) {
        cartaInCima = cartaScelta;
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
        
        // Se è un Jolly, per ora assegniamo un colore a caso (implementeremo la scelta visiva dopo)
        if (coloreAttivo == NERO) {
            if (!latoOscuroAttivo) coloreAttivo = static_cast<Colore>(rand() % 4); 
            else coloreAttivo = static_cast<Colore>((rand() % 4) + 4);
        }
        
        applicaEffetto(cartaScelta);
        passaAlProssimoGiocatore();
    } else {
        // Se non era valida (controllo di sicurezza), la rimettiamo in mano e il turno non passa
        g.pescaCarta(cartaScelta); 
    }
}

// Questa funzione fa scattare l'Intelligenza Artificiale all'istante
void Partita::mossaBot() {
    Giocatore& g = giocatori[turnoCorrente];
    int scelta = -1;
    vector<Carta> manoBot = g.getMano();
    
    // Cerca mossa
    for (int i = 0; i < manoBot.size(); i++) {
        if (mossaValida(manoBot[i])) { scelta = i; break; }
    }
    
    if (scelta == -1) {
        g.pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore();
    } else {
        Carta cartaScelta = g.giocaCarta(scelta);
        cartaInCima = cartaScelta;
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
        if (coloreAttivo == NERO) {
            if (!latoOscuroAttivo) coloreAttivo = static_cast<Colore>(rand() % 4); 
            else coloreAttivo = static_cast<Colore>((rand() % 4) + 4);
        }
        applicaEffetto(cartaScelta);
        passaAlProssimoGiocatore();
    }
}

// METODI VECCHI (Lasciati vuoti o invariati per far compilare il vecchio main.cpp testuale se serve)
void Partita::eseguiTurno() { }

bool Partita::partitaTerminata() {
    for (int i = 0; i < giocatori.size(); i++) {
        if (giocatori[i].haFinitoLeCarte()) return true;
    }
    return false;
}

string Partita::getVincitore() {
    for (int i = 0; i < giocatori.size(); i++) {
        if (giocatori[i].haFinitoLeCarte()) return giocatori[i].getNome();
    }
    return "";
}