/**
 * @file Partita.cpp
 * @brief Implementazione del motore logico del gioco.
 * Gestisce i turni, le regole di piazzamento, gli effetti speciali delle carte
 * e le decisioni basilari dell'Intelligenza Artificiale (Bot).
 */

#include "Partita.h"
#include <iostream>
#include <cstdlib>

using namespace std;

/**
 * @brief Costruttore della Partita.
 * Inizializza i parametri base. Notare l'uso della Initialization List 
 * per creare una cartaInCima fittizia iniziale (verrà sovrascritta nel setup).
 * @param listaGiocatori Vettore contenente i giocatori che parteciperanno.
 */
Partita::Partita(vector<Giocatore> listaGiocatori) : cartaInCima(ROSSO, UNO, ROSSO, UNO) {
    giocatori = listaGiocatori; 
    turnoCorrente = 0;             // Inizia sempre il giocatore di indice 0
    sensoOrario = true;            // Il senso di gioco base
    latoOscuroAttivo = false;      // Si parte sempre dal lato chiaro
    ultimoLogBot = "In attesa...";
}

/**
 * @brief Fase di preparazione prima dell'inizio del primo turno.
 * Mescola il mazzo, distribuisce 7 carte a ogni giocatore e rivela 
 * la prima carta sul tavolo per dare il via al gioco.
 */
void Partita::setupIniziale() {
    mazzo.mescola();
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < giocatori.size(); j++) giocatori[j].pescaCarta(mazzo.pesca());
    }
    cartaInCima = mazzo.pesca();
    coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
}

void Partita::stampaStatoPartita() { }

/**
 * @brief Algoritmo di validazione delle mosse.
 * Controlla se la carta selezionata dal giocatore può essere legalmente posata.
 * @param c La carta che il giocatore vorrebbe giocare.
 * @return true se la mossa è legale, false altrimenti.
 */
bool Partita::mossaValida(Carta c) {
    Colore coloreCarta = c.getColore(latoOscuroAttivo);
    Valore valoreCarta = c.getValore(latoOscuroAttivo);
    
    // I Jolly (colore NERO) possono essere giocati in qualsiasi momento
    if (coloreCarta == NERO) return true;
    
    // Una carta normale è valida se corrisponde per Colore OPPURE per Valore
    if (coloreCarta == coloreAttivo || valoreCarta == cartaInCima.getValore(latoOscuroAttivo)) return true;
    
    return false; 
}

/**
 * @brief Aggiorna l'indice del turno corrente calcolando chi deve giocare.
 * Usa l'aritmetica modulare (l'operatore %) per creare un ciclo circolare (es. da 3 torna a 0).
 */
void Partita::passaAlProssimoGiocatore() {
    if (sensoOrario) turnoCorrente = (turnoCorrente + 1) % giocatori.size();
    else turnoCorrente = (turnoCorrente - 1 + giocatori.size()) % giocatori.size();
}

/**
 * @brief Il cuore del regolamento: applica gli effetti speciali se la carta li possiede.
 * Identifica chi è il "bersaglio" dell'effetto (il prossimo giocatore) e modifica 
 * lo stato della partita (es. invertendo il senso o attivando il lato oscuro).
 * @param c La carta appena giocata e validata.
 */
void Partita::applicaEffetto(Carta c) {
    Valore v = c.getValore(latoOscuroAttivo);
    
    // Calcoliamo a chi toccherebbe SUBITO DOPO il giocatore attuale
    int prossimo = sensoOrario ? (turnoCorrente + 1) % giocatori.size() : (turnoCorrente - 1 + giocatori.size()) % giocatori.size();

    // ==========================================================
    // ECCEZIONE REGOLAMENTO 2 GIOCATORI: 
    // Giocare "Inverti" o "Salta" in una partita a 2 giocatori 
    // annulla il turno dell'avversario. Quindi il turno non avanza!
    // ==========================================================
    
    if (v == INVERTI) {
        sensoOrario = !sensoOrario;
        if (giocatori.size() == 2) passaAlProssimoGiocatore(); // Salta l'avversario
    } 
    else if (v == SALTA || v == SALTA_TUTTI) {
        passaAlProssimoGiocatore(); // Avanza di uno extra (salta il prossimo)
    }
    else if (v == FLIP) {
        // La meccanica iconica del gioco: si inverte lo stato globale!
        latoOscuroAttivo = !latoOscuroAttivo; 
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
    }
    else if (v == PESCA_UNO) {
        giocatori[prossimo].pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore(); // Oltre a pescare, il malcapitato perde il turno
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
        // Effetto devastante del Lato Oscuro: si pesca a oltranza finché 
        // non esce una carta del colore appena imposto dal Jolly.
        bool trovato = false;
        while (!trovato) {
            Carta pescata = mazzo.pesca();
            giocatori[prossimo].pescaCarta(pescata);
            if (pescata.getColore(latoOscuroAttivo) == coloreAttivo) trovato = true;
        }
        passaAlProssimoGiocatore();
    }
}

// --- Getter per l'Interfaccia Grafica ---
Carta Partita::getCartaInCima() { return cartaInCima; }
Colore Partita::getColoreAttivo() { return coloreAttivo; }
bool Partita::getLatoOscuroAttivo() { return latoOscuroAttivo; }
vector<Giocatore> Partita::getGiocatori() { return giocatori; }
int Partita::getTurnoCorrente() { return turnoCorrente; }
string Partita::getUltimoLogBot() { return ultimoLogBot; }

/**
 * @brief Gestisce l'azione intrapresa dall'utente tramite l'interfaccia.
 * Intercetta sia l'azione "Pesca" sia l'azione "Gioca Carta", validando la scelta
 * e gestendo la meccanica dell'UNO (con relativa penalità se dimenticato).
 * @param scelta Indice della carta scelta (-1 se l'utente ha deciso di pescare dal mazzo).
 * @param coloreScelto Se ha giocato un Jolly, contiene il nuovo colore selezionato dalla UI (0-3).
 * @param haDettoUno Flag gestito dall'interfaccia grafica (true se il pulsante è stato premuto).
 */
void Partita::mossaUmano(int scelta, int coloreScelto, bool haDettoUno) {
    Giocatore& g = giocatori[turnoCorrente];
    
    // CASO 1: L'utente ha cliccato sul Mazzo per pescare
    if (scelta == -1) {
        g.pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore();
        cout << "[LOGICA] Umano ha pescato. Prossimo turno: " << turnoCorrente << endl;
        return;
    }
    
    // CASO 2: L'utente ha cliccato su una Carta
    Carta cartaScelta = g.giocaCarta(scelta);
    if (mossaValida(cartaScelta)) {
        cartaInCima = cartaScelta;
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
        
        // Risoluzione speciale per i Jolly (Conversione dal menu visivo all'enum interno)
        if (coloreAttivo == NERO) {
            if (!latoOscuroAttivo) coloreAttivo = static_cast<Colore>(coloreScelto); 
            else coloreAttivo = static_cast<Colore>(coloreScelto + 4);
        }
        
        // GESTIONE PENALITÀ "UNO!"
        // Se, tolta la carta giocata, l'utente rimane con esattamente 1 carta,
        // ma l'interfaccia ci dice che non ha premuto il bottone, scatta la penitenza (+2).
        if (g.getMano().size() == 1 && !haDettoUno) {
            cout << "[LOGICA] Umano non ha detto UNO! Penalita' di 2 carte." << endl;
            g.pescaCarta(mazzo.pesca());
            g.pescaCarta(mazzo.pesca());
        }
        
        applicaEffetto(cartaScelta);
        passaAlProssimoGiocatore();
        cout << "[LOGICA] Umano ha giocato. Prossimo turno: " << turnoCorrente << " (Ricorda: Salta/Inverti ti fanno rigiocare in 2!)" << endl;
    } else {
        // Se per un glitch o un errore la mossa non era valida, si restituisce la carta al giocatore
        g.pescaCarta(cartaScelta); 
    }
}

/**
 * @brief Intelligenza Artificiale basilare per il Bot.
 * L'algoritmo attuale è di tipo "Greedy" (Ingordo): il Bot analizza la propria mano
 * da sinistra a destra e gioca la prima carta legale che trova. Se non ne ha, pesca.
 */
void Partita::mossaBot() {
    Giocatore& g = giocatori[turnoCorrente];
    int scelta = -1;
    vector<Carta> manoBot = g.getMano();
    
    // Trova la prima carta giocabile
    for (int i = 0; i < manoBot.size(); i++) {
        if (mossaValida(manoBot[i])) { scelta = i; break; }
    }
    
    // Se non ha trovato nulla
    if (scelta == -1) {
        g.pescaCarta(mazzo.pesca());
        ultimoLogBot = "Il Bot ha pescato una carta.";
        passaAlProssimoGiocatore();
        cout << "[LOGICA] Bot ha pescato. Prossimo turno: " << turnoCorrente << endl;
    } 
    // Se ha trovato una mossa valida
    else {
        Carta cartaScelta = g.giocaCarta(scelta);
        cartaInCima = cartaScelta;
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
        
        string nomeC = latoOscuroAttivo ? cartaScelta.getDescrizioneOscura() : cartaScelta.getDescrizioneChiara();
        
        // Se il Bot gioca un Jolly, sceglie un colore a caso
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

/**
 * @brief Verifica la condizione di fine partita globale (Qualcuno è rimasto a 0 carte).
 */
bool Partita::partitaTerminata() {
    for (int i = 0; i < giocatori.size(); i++) if (giocatori[i].haFinitoLeCarte()) return true;
    return false;
}

/**
 * @brief Estrapola il nome di chi ha causato il trigger di Fine Partita.
 */
string Partita::getVincitore() {
    for (int i = 0; i < giocatori.size(); i++) if (giocatori[i].haFinitoLeCarte()) return giocatori[i].getNome();
    return "";
}