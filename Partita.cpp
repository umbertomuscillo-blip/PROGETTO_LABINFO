#include "Partita.h"
#include <iostream>

using namespace std;

// Costruttore: riceve i nomi e crea i giocatori
Partita::Partita(vector<string> nomiGiocatori) : cartaInCima(ROSSO, UNO, ROSSO, UNO) {
    for (int i = 0; i < nomiGiocatori.size(); i++) {
        giocatori.push_back(Giocatore(nomiGiocatori[i]));
    }

    // Impostiamo lo stato iniziale
    turnoCorrente = 0;
    sensoOrario = true;
    latoOscuroAttivo = false; // Inizia sempre con il Lato Chiaro
}

void Partita::setupIniziale() {
    cout << "Avvio preparazione partita..." << endl;
    
    // Mescolamento del mazzo
    mazzo.mescola();
    
    // Distribuzione di 7 carte a testa
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < giocatori.size(); j++) {
            giocatori[j].pescaCarta(mazzo.pesca());
        }
    }
    
    // Scopriamo la prima carta sul tavolo
    cartaInCima = mazzo.pesca();
    coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
}

void Partita::stampaStatoPartita() {
    cout << "\n=====================================" << endl;
    cout << "CARTA SUL TAVOLO: ";
    if (latoOscuroAttivo) {
        cout << cartaInCima.getDescrizioneOscura();
    } else {
        cout << cartaInCima.getDescrizioneChiara();
    }
    cout << "\nVerso di gioco: " << (sensoOrario ? "Orario ->" : "<- Antiorario");
    cout << "\nE' IL TURNO DI: " << giocatori[turnoCorrente].getNome() << endl;
    cout << "=====================================\n" << endl;
    
    // Mostriamo la mano del giocatore di turno
    giocatori[turnoCorrente].mostraMano(latoOscuroAttivo);
}

// --- INIZIO NUOVE FUNZIONI PARTITA.CPP ---

bool Partita::mossaValida(Carta c) {
    Colore coloreCarta = c.getColore(latoOscuroAttivo);
    Valore valoreCarta = c.getValore(latoOscuroAttivo);

    // Se è una carta nera (Jolly), si può sempre giocare!
    if (coloreCarta == NERO) return true;

    // La mossa è valida se combacia il colore OPPURE il valore
    if (coloreCarta == coloreAttivo || valoreCarta == cartaInCima.getValore(latoOscuroAttivo)) {
        return true;
    }

    return false; // Se non combacia niente, mossa illegale
}

void Partita::passaAlProssimoGiocatore() {
    // La funzione "%" (modulo) serve a far "ricominciare il giro" quando si arriva all'ultimo giocatore
    if (sensoOrario) {
        turnoCorrente = (turnoCorrente + 1) % giocatori.size();
    } else {
        turnoCorrente = (turnoCorrente - 1 + giocatori.size()) % giocatori.size();
    }
}

void Partita::eseguiTurno() {
    stampaStatoPartita();
    
    // Usiamo una "reference" (&) per lavorare direttamente sul giocatore di turno
    Giocatore& giocatoreAttuale = giocatori[turnoCorrente];
    
    int scelta;
    cout << "Scegli il numero della carta da giocare (oppure scrivi -1 per pescare): ";
    cin >> scelta;

    // Caso 1: Il giocatore decide (o deve) pescare
    if (scelta == -1) {
        cout << "\n-> " << giocatoreAttuale.getNome() << " pesca una carta dal mazzo!" << endl;
        giocatoreAttuale.pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore();
        return;
    }

    // Caso 2: Il giocatore prova a giocare una carta
    Carta cartaScelta = giocatoreAttuale.giocaCarta(scelta);

    if (mossaValida(cartaScelta)) {
        cout << "\n-> Mossa accettata! Hai giocato: ";
        if (latoOscuroAttivo) cout << cartaScelta.getDescrizioneOscura() << endl;
        else cout << cartaScelta.getDescrizioneChiara() << endl;

        // Aggiorniamo il tavolo
        cartaInCima = cartaScelta;
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
        
        // Qui in futuro chiameremo applicaEffetto(cartaScelta) per le carte speciali!
        
        passaAlProssimoGiocatore();
    } else {
        // Se ha barato o sbagliato, gli ridiamo la carta in mano e gli facciamo ripetere il turno
        cout << "\n❌ MOSSA NON VALIDA! La carta non combacia. Riprova." << endl;
        giocatoreAttuale.pescaCarta(cartaScelta); 
    }
}

// Lasciamo ancora vuoti questi due, li faremo dopo
bool Partita::partitaTerminata() { return false; }
void Partita::applicaEffetto(Carta c) {}