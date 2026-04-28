/**
 * @file Partita.cpp
 * @brief Implementazione del motore logico del gioco.
 */

#include "Partita.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Partita::Partita(vector<Giocatore> listaGiocatori) : cartaInCima(ROSSO, UNO, ROSSO, UNO) {
    giocatori = listaGiocatori; 
    turnoCorrente = 0;             
    sensoOrario = true;            
    latoOscuroAttivo = false;      
    ultimoLogBot = "In attesa...";
    mostraAvviso = false;
    messaggioAvviso = "";
}

void Partita::setupIniziale() {
    mazzo.mescola();
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < giocatori.size(); j++) giocatori[j].pescaCarta(mazzo.pesca());
    }
    cartaInCima = mazzo.pesca();
    coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
}

void Partita::stampaStatoPartita() { }

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

std::string Partita::getMessaggioAvviso() { return messaggioAvviso; }
bool Partita::getMostraAvviso() { return mostraAvviso; }
void Partita::impostaAvviso(std::string msg) { 
    messaggioAvviso = msg; 
    mostraAvviso = true; 
}
void Partita::resetAvviso() { mostraAvviso = false; }


void Partita::applicaEffetto(Carta c) {
    Valore v = c.getValore(latoOscuroAttivo);
    int prossimo = sensoOrario ? (turnoCorrente + 1) % giocatori.size() : (turnoCorrente - 1 + giocatori.size()) % giocatori.size();
    
    string nomeAttaccante = giocatori[turnoCorrente].getNome();
    string nomeVittima = giocatori[prossimo].getNome();

    if (v == INVERTI) {
        sensoOrario = !sensoOrario;
        if (giocatori.size() == 2) {
            impostaAvviso(nomeAttaccante + " inverte e mantiene il turno!");
            passaAlProssimoGiocatore(); 
        } else {
            impostaAvviso("Giro invertito!");
        }
    } 
    else if (v == SALTA || v == SALTA_TUTTI) {
        impostaAvviso(nomeAttaccante + " fa saltare il turno a " + nomeVittima + "!");
        passaAlProssimoGiocatore(); 
    }
    else if (v == FLIP) {
        latoOscuroAttivo = !latoOscuroAttivo; 
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
        impostaAvviso("FLIP! Il tavolo si e' capovolto!");
    }
    else if (v == PESCA_UNO) {
        giocatori[prossimo].pescaCarta(mazzo.pesca());
        impostaAvviso(nomeAttaccante + " lancia un +1!\n" + nomeVittima + " pesca e salta!");
        passaAlProssimoGiocatore(); 
    }
    else if (v == PESCA_CINQUE) {
        for(int i = 0; i < 5; i++) giocatori[prossimo].pescaCarta(mazzo.pesca());
        impostaAvviso(nomeAttaccante + " lancia un +5!\n" + nomeVittima + " pesca 5 e salta!");
        passaAlProssimoGiocatore();
    }
    else if (v == JOLLY_PESCA_DUE) {
        for(int i = 0; i < 2; i++) giocatori[prossimo].pescaCarta(mazzo.pesca());
        impostaAvviso(nomeAttaccante + " gioca Jolly +2!\n" + nomeVittima + " pesca 2 e salta!");
        passaAlProssimoGiocatore();
    }
    else if (v == JOLLY_PESCA_COLORE) {
        bool trovato = false;
        int contatore = 0;
        while (!trovato) {
            Carta pescata = mazzo.pesca();
            giocatori[prossimo].pescaCarta(pescata);
            contatore++;
            if (pescata.getColore(latoOscuroAttivo) == coloreAttivo) trovato = true;
        }
        impostaAvviso(nomeAttaccante + " gioca Jolly Colore!\n" + nomeVittima + " ha pescato " + to_string(contatore) + " carte!");
        passaAlProssimoGiocatore();
    }
}

Carta Partita::getCartaInCima() { return cartaInCima; }
Colore Partita::getColoreAttivo() { return coloreAttivo; }
bool Partita::getLatoOscuroAttivo() { return latoOscuroAttivo; }
vector<Giocatore> Partita::getGiocatori() { return giocatori; }
int Partita::getTurnoCorrente() { return turnoCorrente; }
string Partita::getUltimoLogBot() { return ultimoLogBot; }
bool Partita::getSensoOrario() { return sensoOrario; } // <-- IL GETTER PER LA UI

void Partita::mossaUmano(int scelta, int coloreScelto, bool haDettoUno) {
    Giocatore& g = giocatori[turnoCorrente];
    
    if (scelta == -1) {
        g.pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore();
        return;
    }
    
    Carta cartaScelta = g.giocaCarta(scelta);
    if (mossaValida(cartaScelta)) {
        cartaInCima = cartaScelta;
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
        
        if (coloreAttivo == NERO) {
            Colore chiari[] = {ROSSO, GIALLO, VERDE, BLU};
            Colore oscuri[] = {ROSA, VERDE_ACQUA, ARANCIONE, VIOLA};
            if (!latoOscuroAttivo) coloreAttivo = chiari[coloreScelto]; 
            else coloreAttivo = oscuri[coloreScelto];
        }
        
        if (g.getMano().size() == 1 && !haDettoUno) {
            g.pescaCarta(mazzo.pesca());
            g.pescaCarta(mazzo.pesca());
            impostaAvviso("Hai dimenticato di dire UNO!\nPenalita' di 2 carte!");
        }
        
        applicaEffetto(cartaScelta);
        passaAlProssimoGiocatore();
    } else {
        g.pescaCarta(cartaScelta); 
    }
}

void Partita::mossaBot() {
    Giocatore& g = giocatori[turnoCorrente];
    int scelta = -1;
    vector<Carta> manoBot = g.getMano();
    
    for (int i = 0; i < manoBot.size(); i++) {
        if (mossaValida(manoBot[i])) { scelta = i; break; }
    }
    
    if (scelta == -1) {
        g.pescaCarta(mazzo.pesca());
        ultimoLogBot = "Il Bot ha pescato una carta.";
        passaAlProssimoGiocatore();
    } else {
        Carta cartaScelta = g.giocaCarta(scelta);
        cartaInCima = cartaScelta;
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
        
        string nomeC = latoOscuroAttivo ? cartaScelta.getDescrizioneOscura() : cartaScelta.getDescrizioneChiara();
        
        if (coloreAttivo == NERO) {
            Colore chiari[] = {ROSSO, GIALLO, VERDE, BLU};
            Colore oscuri[] = {ROSA, VERDE_ACQUA, ARANCIONE, VIOLA};
            int r = rand() % 4; 
            if (!latoOscuroAttivo) coloreAttivo = chiari[r]; 
            else coloreAttivo = oscuri[r];
            
            string nomeNuovoColore = "";
            if (!latoOscuroAttivo) {
                if (r == 0) nomeNuovoColore = "Rosso"; else if (r == 1) nomeNuovoColore = "Giallo";
                else if (r == 2) nomeNuovoColore = "Verde"; else nomeNuovoColore = "Blu";
            } else {
                if (r == 0) nomeNuovoColore = "Rosa"; else if (r == 1) nomeNuovoColore = "Verde Acqua";
                else if (r == 2) nomeNuovoColore = "Arancione"; else nomeNuovoColore = "Viola";
            }
            nomeC += " (Nuovo colore: " + nomeNuovoColore + ")";
            if (cartaScelta.getValore(latoOscuroAttivo) == JOLLY) {
                impostaAvviso(g.getNome() + " cambia colore!\nAdesso si gioca sul " + nomeNuovoColore);
            }
        }
        
        ultimoLogBot = "Giocata: " + nomeC;
        applicaEffetto(cartaScelta);
        passaAlProssimoGiocatore();
    }
}

void Partita::eseguiTurno() { }
bool Partita::partitaTerminata() {
    for (int i = 0; i < giocatori.size(); i++) if (giocatori[i].haFinitoLeCarte()) return true;
    return false;
}
string Partita::getVincitore() {
    for (int i = 0; i < giocatori.size(); i++) if (giocatori[i].haFinitoLeCarte()) return giocatori[i].getNome();
    return "";
}