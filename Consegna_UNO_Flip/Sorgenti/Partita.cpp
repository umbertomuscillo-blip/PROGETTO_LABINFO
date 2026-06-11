// Partita.cpp
// Implementazione della logica di gioco di UNO Flip.

#include "Partita.h"
#include "SoundManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
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
            int r = Mazzo::ottieniNumeroCasuale() % 4;
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
        Carta c = mano[indiceGiocabile]; int coloreScelto = Mazzo::ottieniNumeroCasuale() % 4;
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

bool Partita::salvaPartita(string filename) {
    ofstream out(filename);
    if (!out) return false;
    
    out << turnoCorrente << " " << sensoOrario << " " << latoOscuroAttivo << " " << coloreAttivo << " " << partitaFinita << "\n";
    string v = vincitore.empty() ? "NESSUNO" : vincitore;
    out << v << "\n";
    
    out << scarti.size() << "\n";
    for (Carta c : scarti) {
        out << c.getColore(false) << " " << c.getValore(false) << " " << c.getColore(true) << " " << c.getValore(true) << "\n";
    }
    
    vector<Carta> daPescare = mazzo.getCarteDaPescare();
    out << daPescare.size() << "\n";
    for (Carta c : daPescare) {
        out << c.getColore(false) << " " << c.getValore(false) << " " << c.getColore(true) << " " << c.getValore(true) << "\n";
    }
    
    out << giocatori.size() << "\n";
    for (Giocatore g : giocatori) {
        out << g.getNome() << "\n";
        out << g.getIsBot() << "\n";
        vector<Carta> mano = g.getMano();
        out << mano.size() << "\n";
        for (Carta c : mano) {
            out << c.getColore(false) << " " << c.getValore(false) << " " << c.getColore(true) << " " << c.getValore(true) << "\n";
        }
    }
    
    out.close();
    return true;
}

std::unique_ptr<Partita> Partita::caricaPartita(string filename) {
    ifstream in(filename);
    if (!in) return nullptr;
    
    int tCorr, cAttivo;
    bool sOrario, lOscuro, pFinita;
    in >> tCorr >> sOrario >> lOscuro >> cAttivo >> pFinita;
    string v;
    std::getline(in >> std::ws, v);
    
    int numScarti;
    in >> numScarti;
    vector<Carta> scartiP;
    for(int i=0; i<numScarti; i++) {
        int cc, vc, co, vo;
        in >> cc >> vc >> co >> vo;
        scartiP.push_back(Carta((Colore)cc, (Valore)vc, (Colore)co, (Valore)vo));
    }
    
    int numPescare;
    in >> numPescare;
    vector<Carta> daPescare;
    for(int i=0; i<numPescare; i++) {
        int cc, vc, co, vo;
        in >> cc >> vc >> co >> vo;
        daPescare.push_back(Carta((Colore)cc, (Valore)vc, (Colore)co, (Valore)vo));
    }
    
    int numGiocatori;
    in >> numGiocatori;
    vector<Giocatore> gioca;
    for(int i=0; i<numGiocatori; i++) {
        string nome;
        std::getline(in >> std::ws, nome);
        bool isBot;
        in >> isBot;
        Giocatore g(nome, isBot);
        int numCarte;
        in >> numCarte;
        for(int k=0; k<numCarte; k++) {
            int cc, vc, co, vo;
            in >> cc >> vc >> co >> vo;
            g.pescaCarta(Carta((Colore)cc, (Valore)vc, (Colore)co, (Valore)vo));
        }
        gioca.push_back(g);
    }
    in.close();
    
    auto p = std::make_unique<Partita>(gioca);
    p->turnoCorrente = tCorr;
    p->sensoOrario = sOrario;
    p->latoOscuroAttivo = lOscuro;
    p->coloreAttivo = (Colore)cAttivo;
    p->partitaFinita = pFinita;
    p->vincitore = (v == "NESSUNO") ? "" : v;
    
    p->mazzo.setMazzoDaStato(daPescare, scartiP);
    // Populate the base class vector (we overwrite it since the constructor used the new list but without cards)
    p->giocatori = gioca;
    p->scarti = scartiP; // Actually scarti is not strictly used since getCartaInCima relies on mazzo or Partita scarti. Wait! Partita has its own `scarti` vector?
    // Let me check Partita.h: `std::vector<Carta> scarti;` Yes, we need to populate it.
    p->scarti = scartiP;
    
    return p;
}