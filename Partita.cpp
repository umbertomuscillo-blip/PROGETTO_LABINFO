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

void Partita::applicaEffetto(Carta c) {
    Valore v = c.getValore(latoOscuroAttivo);
    int prossimo = sensoOrario ? (turnoCorrente + 1) % giocatori.size() : (turnoCorrente - 1 + giocatori.size()) % giocatori.size();

    // REGOLA 2 GIOCATORI: Inverti e Salta mantengono il turno allo stesso giocatore!
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

Carta Partita::getCartaInCima() { return cartaInCima; }
Colore Partita::getColoreAttivo() { return coloreAttivo; }
bool Partita::getLatoOscuroAttivo() { return latoOscuroAttivo; }
vector<Giocatore> Partita::getGiocatori() { return giocatori; }
int Partita::getTurnoCorrente() { return turnoCorrente; }
string Partita::getUltimoLogBot() { return ultimoLogBot; }

void Partita::mossaUmano(int scelta, int coloreScelto, bool haDettoUno) {
    Giocatore& g = giocatori[turnoCorrente];
    
    if (scelta == -1) {
        g.pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore();
        cout << "[LOGICA] Umano ha pescato. Prossimo turno: " << turnoCorrente << endl;
        return;
    }
    
    Carta cartaScelta = g.giocaCarta(scelta);
    if (mossaValida(cartaScelta)) {
        cartaInCima = cartaScelta;
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
        
        if (coloreAttivo == NERO) {
            if (!latoOscuroAttivo) coloreAttivo = static_cast<Colore>(coloreScelto); 
            else coloreAttivo = static_cast<Colore>(coloreScelto + 4);
        }
        
        // --- PENALITA' UNO ---
        // Se, DOPO aver giocato questa carta, il giocatore rimane con 1 carta
        // e non ha premuto il pulsante, pesca 2 carte di penalità!
        if (g.getMano().size() == 1 && !haDettoUno) {
            cout << "[LOGICA] Umano non ha detto UNO! Penalita' di 2 carte." << endl;
            g.pescaCarta(mazzo.pesca());
            g.pescaCarta(mazzo.pesca());
        }
        
        applicaEffetto(cartaScelta);
        passaAlProssimoGiocatore();
        cout << "[LOGICA] Umano ha giocato. Prossimo turno: " << turnoCorrente << " (Ricorda: Salta/Inverti ti fanno rigiocare in 2!)" << endl;
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
        cout << "[LOGICA] Bot ha pescato. Prossimo turno: " << turnoCorrente << endl;
    } else {
        Carta cartaScelta = g.giocaCarta(scelta);
        cartaInCima = cartaScelta;
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
        
        string nomeC = latoOscuroAttivo ? cartaScelta.getDescrizioneOscura() : cartaScelta.getDescrizioneChiara();
        
        if (coloreAttivo == NERO) {
            int r = rand() % 4;
            if (!latoOscuroAttivo) coloreAttivo = static_cast<Colore>(r); 
            else coloreAttivo = static_cast<Colore>(r + 4);
            nomeC += " (Scelto nuovo colore)";
        }
        
        ultimoLogBot = "Il Bot ha giocato: " + nomeC;
        applicaEffetto(cartaScelta);
        passaAlProssimoGiocatore();
        cout << "[LOGICA] Bot ha giocato " << nomeC << ". Prossimo turno: " << turnoCorrente << endl;
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