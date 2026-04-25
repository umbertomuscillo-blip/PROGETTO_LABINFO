#include "Partita.h"
#include <iostream>
#include <cstdlib> // Serve per i numeri casuali del Bot

using namespace std;

Partita::Partita(vector<Giocatore> listaGiocatori) : cartaInCima(ROSSO, UNO, ROSSO, UNO) {
    giocatori = listaGiocatori; // Salviamo i giocatori (incluso il Bot)
    turnoCorrente = 0;
    sensoOrario = true;
    latoOscuroAttivo = false; 
}

void Partita::setupIniziale() {
    cout << "Avvio preparazione partita..." << endl;
    mazzo.mescola();
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < giocatori.size(); j++) giocatori[j].pescaCarta(mazzo.pesca());
    }
    cartaInCima = mazzo.pesca();
    coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
}

void Partita::stampaStatoPartita() {
    cout << "\n=====================================" << endl;
    cout << "CARTA SUL TAVOLO: ";
    if (latoOscuroAttivo) cout << cartaInCima.getDescrizioneOscura();
    else cout << cartaInCima.getDescrizioneChiara();
    
    cout << "\nCOLORE DA GIOCARE: ";
    switch(coloreAttivo) {
        case ROSSO: cout << "🔴 Rosso"; break;
        case GIALLO: cout << "🟡 Giallo"; break;
        case VERDE: cout << "🟢 Verde"; break;
        case BLU: cout << "🔵 Blu"; break;
        case ARANCIONE: cout << "🟠 Arancione"; break;
        case VIOLA: cout << "🟣 Viola"; break;
        case VERDE_ACQUA: cout << "💧 Verde Acqua"; break;
        case ROSA: cout << "🌸 Rosa"; break;
        case NERO: cout << "Nessuno"; break;
    }

    cout << "\nVerso di gioco: " << (sensoOrario ? "Orario ->" : "<- Antiorario");
    cout << "\nE' IL TURNO DI: " << giocatori[turnoCorrente].getNome() << endl;
    cout << "=====================================\n" << endl;
    
    // Nascondiamo la mano del Bot per non "barare", mostriamo solo la nostra
    if (!giocatori[turnoCorrente].getIsBot()) {
        giocatori[turnoCorrente].mostraMano(latoOscuroAttivo);
    } else {
        cout << "(Il Bot ha " << giocatori[turnoCorrente].getNumeroCarte() << " carte in mano)" << endl;
    }
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

void Partita::eseguiTurno() {
    stampaStatoPartita();
    Giocatore& giocatoreAttuale = giocatori[turnoCorrente];
    int scelta = -1;

    // --- IL CERVELLO DEL BOT ---
    if (giocatoreAttuale.getIsBot()) {
        cout << "\n🤖 Il Bot sta pensando..." << endl;
        vector<Carta> manoBot = giocatoreAttuale.getMano();
        
        // Cerca la prima carta giocabile
        for (int i = 0; i < manoBot.size(); i++) {
            if (mossaValida(manoBot[i])) {
                scelta = i;
                break;
            }
        }
    } 
    // --- TURNO UMANO ---
    else {
        cout << "Scegli il numero della carta da giocare (oppure scrivi -1 per pescare): ";
        cin >> scelta;
    }

    // Caso 1: Pesca
    if (scelta == -1) {
        cout << "\n-> " << giocatoreAttuale.getNome() << " pesca una carta dal mazzo!" << endl;
        giocatoreAttuale.pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore();
        return;
    }

    if (scelta < 0 || scelta >= giocatoreAttuale.getNumeroCarte()) {
        cout << "\n❌ SCELTA NON VALIDA! Inserisci un numero corretto." << endl;
        return; 
    }

    // Caso 2: Gioca
    Carta cartaScelta = giocatoreAttuale.giocaCarta(scelta);

    if (mossaValida(cartaScelta)) {
        cout << "\n-> Mossa accettata! " << giocatoreAttuale.getNome() << " ha giocato: ";
        if (latoOscuroAttivo) cout << cartaScelta.getDescrizioneOscura() << endl;
        else cout << cartaScelta.getDescrizioneChiara() << endl;

        cartaInCima = cartaScelta;
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
        
        // Se si gioca un Jolly
        if (coloreAttivo == NERO) {
            int sceltaColore = 0;
            if (giocatoreAttuale.getIsBot()) {
                sceltaColore = rand() % 4; // Il Bot sceglie a caso
                cout << "🤖 Il Bot ha scelto un nuovo colore!" << endl;
            } else {
                if (!latoOscuroAttivo) {
                    cout << "\n🎨 Scegli colore (0: Rosso, 1: Giallo, 2: Verde, 3: Blu): ";
                } else {
                    cout << "\n🎨 Scegli colore (0: Arancione, 1: Viola, 2: Verde Acqua, 3: Rosa): ";
                }
                cin >> sceltaColore;
            }
            
            if (!latoOscuroAttivo) {
                Colore coloriChiari[] = {ROSSO, GIALLO, VERDE, BLU};
                coloreAttivo = coloriChiari[sceltaColore % 4]; 
            } else {
                Colore coloriOscuri[] = {ARANCIONE, VIOLA, VERDE_ACQUA, ROSA};
                coloreAttivo = coloriOscuri[sceltaColore % 4];
            }
        }
        
        applicaEffetto(cartaScelta);
        passaAlProssimoGiocatore();
    } else {
        if (!giocatoreAttuale.getIsBot()) cout << "\n❌ MOSSA NON VALIDA! Riprova." << endl;
        giocatoreAttuale.pescaCarta(cartaScelta); 
    }
}

bool Partita::partitaTerminata() {
    for (int i = 0; i < giocatori.size(); i++) {
        if (giocatori[i].haFinitoLeCarte()) {
            cout << "\n🏆 VITTORIA! " << giocatori[i].getNome() << " ha vinto!" << endl;
            return true;
        }
    }
    return false;
}

void Partita::applicaEffetto(Carta c) {
    Valore v = c.getValore(latoOscuroAttivo);
    int prossimo = sensoOrario ? (turnoCorrente + 1) % giocatori.size() : (turnoCorrente - 1 + giocatori.size()) % giocatori.size();

    if (v == INVERTI) {
        cout << "🔄 CAMBIO GIRO!" << endl;
        sensoOrario = !sensoOrario;
        if (giocatori.size() == 2) passaAlProssimoGiocatore(); 
    } 
    else if (v == SALTA || v == SALTA_TUTTI) {
        cout << "🚫 SALTA TURNO!" << endl;
        passaAlProssimoGiocatore(); 
    }
    else if (v == FLIP) {
        cout << "\n🌌 *WUSH* 🌌 IL MAZZO SI GIRA!!!" << endl;
        latoOscuroAttivo = !latoOscuroAttivo; 
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
    }
    else if (v == PESCA_UNO) {
        cout << "🃏 PESCA 1 per " << giocatori[prossimo].getNome() << "!" << endl;
        giocatori[prossimo].pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore();
    }
    else if (v == PESCA_CINQUE) {
        cout << "🃏 PESCA 5 per " << giocatori[prossimo].getNome() << "!" << endl;
        for(int i = 0; i < 5; i++) giocatori[prossimo].pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore();
    }
    else if (v == JOLLY_PESCA_DUE) {
        cout << "🃏 JOLLY PESCA 2 per " << giocatori[prossimo].getNome() << "!" << endl;
        for(int i = 0; i < 2; i++) giocatori[prossimo].pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore();
    }
    else if (v == JOLLY_PESCA_COLORE) {
        cout << "🃏 PESCA COLORE per " << giocatori[prossimo].getNome() << "!" << endl;
        bool trovato = false;
        while (!trovato) {
            Carta pescata = mazzo.pesca();
            giocatori[prossimo].pescaCarta(pescata);
            if (pescata.getColore(latoOscuroAttivo) == coloreAttivo) trovato = true;
        }
        passaAlProssimoGiocatore();
    }
}
string Partita::getVincitore() {
    for (int i = 0; i < giocatori.size(); i++) {
        if (giocatori[i].haFinitoLeCarte()) {
            return giocatori[i].getNome();
        }
    }
    return ""; // Nessuno ha ancora vinto
}