/**
 * @file Partita.cpp
 * @brief Implementazione della logica di gioco di UNO Flip.
 */

#include "Partita.h"
#include <iostream>
#include <algorithm>

using namespace std;

Partita::Partita(vector<Giocatore> listaGiocatori) {
    giocatori = listaGiocatori; turnoCorrente = 0; sensoOrario = true;
    latoOscuroAttivo = false; partitaFinita = false; vincitore = "";
    mostraAvvisoPopup = false; ultimoLogBot = "Partita Iniziata!";
}

void Partita::setupIniziale() {
    mazzo = Mazzo(); mazzo.mescola(); 
    for (int i = 0; i < 7; i++) { for (auto& g : giocatori) { g.pescaCarta(mazzo.pesca()); } }
    Carta primaCarta = mazzo.pesca();
    while (primaCarta.getColore(false) == NERO) { primaCarta = mazzo.pesca(); }
    scarti.push_back(primaCarta); coloreAttivo = primaCarta.getColore(false);
}

void Partita::mostraAvviso(string msg) { messaggioAvviso = msg; mostraAvvisoPopup = true; }

void Partita::passaTurno() {
    if (partitaFinita) return;
    if (sensoOrario) turnoCorrente = (turnoCorrente + 1) % giocatori.size();
    else turnoCorrente = (turnoCorrente - 1 + giocatori.size()) % giocatori.size();
}

bool Partita::mossaValida(Carta c) {
    Carta inCima = scarti.back(); Colore cCima = coloreAttivo; Valore vCima = inCima.getValore(latoOscuroAttivo);
    Colore cMano = c.getColore(latoOscuroAttivo); Valore vMano = c.getValore(latoOscuroAttivo);
    if (cMano == NERO || cMano == cCima || vMano == vCima) return true;
    return false;
}

void Partita::applicaEffettoCarta(Carta c, int coloreScelto) {
    scarti.push_back(c);
    Colore cMano = c.getColore(latoOscuroAttivo); Valore vMano = c.getValore(latoOscuroAttivo);

    if (cMano == NERO) {
        switch (coloreScelto) {
            case 0: coloreAttivo = latoOscuroAttivo ? ROSA : ROSSO; break;
            case 1: coloreAttivo = latoOscuroAttivo ? VERDE_ACQUA : GIALLO; break;
            case 2: coloreAttivo = latoOscuroAttivo ? ARANCIONE : VERDE; break;
            case 3: coloreAttivo = latoOscuroAttivo ? VIOLA : BLU; break;
            default: coloreAttivo = latoOscuroAttivo ? ROSA : ROSSO; break;
        }
    } else { coloreAttivo = cMano; }

    if (giocatori[turnoCorrente].getMano().empty()) {
        partitaFinita = true; vincitore = giocatori[turnoCorrente].getNome(); return;
    }

    if (vMano == INVERTI) {
        if (giocatori.size() == 2) passaTurno(); else sensoOrario = !sensoOrario;
        mostraAvviso("Giro invertito!"); passaTurno();
    } 
    else if (vMano == SALTA) { mostraAvviso("Turno saltato!"); passaTurno(); passaTurno(); }
    else if (vMano == SALTA_TUTTI) { mostraAvviso("TUTTI SALTANO! Ritocca a te!"); }
    else if (vMano == PESCA_UNO) { passaTurno(); giocatori[turnoCorrente].pescaCarta(mazzo.pesca()); mostraAvviso(giocatori[turnoCorrente].getNome() + " pesca 1 carta!"); passaTurno(); }
    else if (vMano == PESCA_CINQUE) { passaTurno(); for(int i=0; i<5; i++) giocatori[turnoCorrente].pescaCarta(mazzo.pesca()); mostraAvviso(giocatori[turnoCorrente].getNome() + " pesca 5 CARTE!"); passaTurno(); }
    else if (vMano == FLIP) { 
        latoOscuroAttivo = !latoOscuroAttivo; 
        mostraAvviso(latoOscuroAttivo ? "LATO OSCURO ATTIVATO!" : "LATO CHIARO ATTIVATO!"); 
        
        Colore nuovoColore = c.getColore(latoOscuroAttivo);
        // FIX BUG JOLLY GRIGIO: Se la carta FLIP ha un Jolly dietro, forziamo un colore a caso per non bloccare il gioco!
        if (nuovoColore == NERO) {
            int r = rand() % 4;
            if (!latoOscuroAttivo) {
                Colore chiari[] = {ROSSO, GIALLO, VERDE, BLU}; coloreAttivo = chiari[r];
            } else {
                Colore oscuri[] = {ROSA, VERDE_ACQUA, ARANCIONE, VIOLA}; coloreAttivo = oscuri[r];
            }
        } else {
            coloreAttivo = nuovoColore; 
        }
        passaTurno(); 
    }
    else if (vMano == JOLLY_PESCA_DUE) { passaTurno(); for(int i=0; i<2; i++) giocatori[turnoCorrente].pescaCarta(mazzo.pesca()); mostraAvviso(giocatori[turnoCorrente].getNome() + " pesca 2 carte!"); passaTurno(); }
    else if (vMano == JOLLY_PESCA_COLORE) {
        passaTurno(); int cartePescate = 0; bool trovato = false;
        while (!trovato) {
            Carta pescata = mazzo.pesca(); giocatori[turnoCorrente].pescaCarta(pescata); cartePescate++;
            if (pescata.getColore(latoOscuroAttivo) == coloreAttivo) trovato = true;
        }
        mostraAvviso(giocatori[turnoCorrente].getNome() + " pesca " + to_string(cartePescate) + " carte!"); passaTurno();
    }
    else { passaTurno(); }
}

void Partita::assegnaPenalitaUno(int idGiocatore) {
    if(partitaFinita) return;
    giocatori[idGiocatore].pescaCarta(mazzo.pesca());
    giocatori[idGiocatore].pescaCarta(mazzo.pesca());
    mostraAvviso(giocatori[idGiocatore].getNome() + " BECCATO SENZA UNO! +2 CARTE!");
}

void Partita::forzaFinePartita(string nomeVincitore) {
    partitaFinita = true;
    vincitore = nomeVincitore;
}

void Partita::mossaUmano(int indiceCarta, int coloreScelto, bool dettoUno) {
    if (partitaFinita) return;
    if (indiceCarta == -1) {
        giocatori[turnoCorrente].pescaCarta(mazzo.pesca());
        passaTurno(); // FIX: Ora se peschi, il turno passa sempre (evita il bug doppio click pesca infinita)
        return;
    }
    Carta c = giocatori[turnoCorrente].getMano()[indiceCarta];
    giocatori[turnoCorrente].giocaCarta(indiceCarta);
    applicaEffettoCarta(c, coloreScelto);
}

void Partita::mossaBot() {
    if (partitaFinita) return;
    Giocatore& bot = giocatori[turnoCorrente]; vector<Carta> mano = bot.getMano(); 
    if (mano.size() == 2) ultimoLogBot = bot.getNome() + " grida: UNO!";
    int indiceGiocabile = -1;
    for (int i=0; i<mano.size(); i++) { if (mossaValida(mano[i])) { Valore v = mano[i].getValore(latoOscuroAttivo); if (v==PESCA_CINQUE || v==SALTA_TUTTI || v==JOLLY_PESCA_COLORE) { indiceGiocabile = i; break; } } }
    if (indiceGiocabile == -1) { for (int i=0; i<mano.size(); i++) { if (mossaValida(mano[i])) { indiceGiocabile = i; break; } } }
    if (indiceGiocabile != -1) {
        Carta c = mano[indiceGiocabile]; int coloreScelto = rand() % 4;
        ultimoLogBot = bot.getNome() + " ha giocato una carta.";
        bot.giocaCarta(indiceGiocabile); applicaEffettoCarta(c, coloreScelto);
    } else {
        ultimoLogBot = bot.getNome() + " non ha carte. Pesca."; bot.pescaCarta(mazzo.pesca()); passaTurno();
    }
}

void Partita::mossaRete(int indiceCarta, int coloreScelto, bool dettoUno) {
    if (partitaFinita) return;
    if (indiceCarta == -1) {
        giocatori[turnoCorrente].pescaCarta(mazzo.pesca());
        passaTurno(); 
        return;
    }
    Carta c = giocatori[turnoCorrente].getMano()[indiceCarta];
    giocatori[turnoCorrente].giocaCarta(indiceCarta);
    applicaEffettoCarta(c, coloreScelto);
}

vector<Giocatore> Partita::getGiocatori() const { return giocatori; }
Carta Partita::getCartaInCima() const { return scarti.back(); }
Colore Partita::getColoreAttivo() const { return coloreAttivo; }
bool Partita::getLatoOscuroAttivo() const { return latoOscuroAttivo; }
int Partita::getTurnoCorrente() const { return turnoCorrente; }
bool Partita::getSensoOrario() const { return sensoOrario; }
bool Partita::partitaTerminata() const { return partitaFinita; }
string Partita::getVincitore() const { return vincitore; }
string Partita::getMessaggioAvviso() const { return messaggioAvviso; }
bool Partita::getMostraAvviso() const { return mostraAvvisoPopup; }
void Partita::resetAvviso() { mostraAvvisoPopup = false; }
string Partita::getUltimoLogBot() const { return ultimoLogBot; }