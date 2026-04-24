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
    
    // --- LA SOLUZIONE AL TUO PROBLEMA UX ---
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
    
    // Mostriamo la mano del giocatore di turno
    giocatori[turnoCorrente].mostraMano(latoOscuroAttivo);
}

// --- FUNZIONI DI GIOCO ---

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

    // Controllo di sicurezza: l'indice inserito è valido?
    if (scelta < 0 || scelta >= giocatoreAttuale.getNumeroCarte()) {
        cout << "\n❌ SCELTA NON VALIDA! Inserisci un numero tra 0 e " << giocatoreAttuale.getNumeroCarte() - 1 << "." << endl;
        return; // Ripete il turno
    }

    // Caso 2: Il giocatore prova a giocare una carta
    // Caso 2: Il giocatore prova a giocare una carta
    Carta cartaScelta = giocatoreAttuale.giocaCarta(scelta);

    if (mossaValida(cartaScelta)) {
        cout << "\n-> Mossa accettata! Hai giocato: ";
        if (latoOscuroAttivo) cout << cartaScelta.getDescrizioneOscura() << endl;
        else cout << cartaScelta.getDescrizioneChiara() << endl;

        // Aggiorniamo il tavolo
        cartaInCima = cartaScelta;
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
        
        // --- NOVITA': SCELTA DEL COLORE SE E' UN JOLLY ---
        if (coloreAttivo == NERO) {
            int sceltaColore;
            if (!latoOscuroAttivo) {
                cout << "\n🎨 Hai giocato un Jolly! Scegli il nuovo colore:" << endl;
                cout << "0: Rosso, 1: Giallo, 2: Verde, 3: Blu\nScelta: ";
                cin >> sceltaColore;
                Colore coloriChiari[] = {ROSSO, GIALLO, VERDE, BLU};
                // Usiamo "% 4" per sicurezza, così se inserisci 9 prende comunque un colore valido
                coloreAttivo = coloriChiari[sceltaColore % 4]; 
            } else {
                cout << "\n🎨 Hai giocato un Jolly Oscuro! Scegli il nuovo colore:" << endl;
                cout << "0: Arancione, 1: Viola, 2: Verde Acqua, 3: Rosa\nScelta: ";
                cin >> sceltaColore;
                Colore coloriOscuri[] = {ARANCIONE, VIOLA, VERDE_ACQUA, ROSA};
                coloreAttivo = coloriOscuri[sceltaColore % 4];
            }
        }
        
        // APPLICHIAMO L'EFFETTO DELLA CARTA APPENA GIOCATA!
        applicaEffetto(cartaScelta);
        
        passaAlProssimoGiocatore();
    } else {
        // Se ha barato o sbagliato, gli ridiamo la carta in mano e gli facciamo ripetere il turno
        cout << "\n❌ MOSSA NON VALIDA! La carta non combacia. Riprova." << endl;
        giocatoreAttuale.pescaCarta(cartaScelta); 
    }
}

bool Partita::partitaTerminata() {
    for (int i = 0; i < giocatori.size(); i++) {
        if (giocatori[i].haFinitoLeCarte()) {
            cout << "\n🏆🏆🏆 VITTORIA! 🏆🏆🏆" << endl;
            cout << "Il giocatore " << giocatori[i].getNome() << " ha vinto la partita!" << endl;
            return true;
        }
    }
    return false; // Nessuno ha ancora vinto, il gioco continua
}

void Partita::applicaEffetto(Carta c) {
    Valore v = c.getValore(latoOscuroAttivo);

    // Calcoliamo chi è il prossimo giocatore (colui che subirà le eventuali penalità)
    int prossimo = sensoOrario ? (turnoCorrente + 1) % giocatori.size() : (turnoCorrente - 1 + giocatori.size()) % giocatori.size();

   if (v == INVERTI) {
        cout << "🔄 CAMBIO GIRO! Il verso di gioco e' stato invertito." << endl;
        sensoOrario = !sensoOrario;
        
        // --- FIX REGOLA UFFICIALE 2 GIOCATORI ---
        if (giocatori.size() == 2) {
            cout << "⚡ Regola 2 Giocatori: L'Inversione agisce come un Salta Turno!" << endl;
            passaAlProssimoGiocatore(); // Salta l'avversario!
        }
    }
    else if (v == SALTA || v == SALTA_TUTTI) {
        cout << "🚫 SALTA TURNO! Il prossimo giocatore viene saltato." << endl;
        passaAlProssimoGiocatore(); 
    }
    else if (v == FLIP) {
        cout << "\n🌌 *WUSH* 🌌 IL MAZZO SI GIRA!!!" << endl;
        latoOscuroAttivo = !latoOscuroAttivo; 
        coloreAttivo = cartaInCima.getColore(latoOscuroAttivo);
    }
    // --- NUOVE PENALITA' COMPLETATE ---
    else if (v == PESCA_UNO) {
        cout << "🃏 PESCA 1! " << giocatori[prossimo].getNome() << " pesca 1 carta e salta il turno." << endl;
        giocatori[prossimo].pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore(); // Salta il turno
    }
    else if (v == PESCA_CINQUE) {
        cout << "🃏 PESCA 5! " << giocatori[prossimo].getNome() << " pesca 5 carte e salta il turno." << endl;
        for(int i = 0; i < 5; i++) giocatori[prossimo].pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore();
    }
    else if (v == JOLLY_PESCA_DUE) {
        cout << "🃏 JOLLY PESCA 2! " << giocatori[prossimo].getNome() << " pesca 2 carte e salta il turno." << endl;
        for(int i = 0; i < 2; i++) giocatori[prossimo].pescaCarta(mazzo.pesca());
        passaAlProssimoGiocatore();
    }
    else if (v == JOLLY_PESCA_COLORE) {
        cout << "🃏 JOLLY PESCA COLORE! " << giocatori[prossimo].getNome() << " pesca finche' non trova il colore giusto!" << endl;
        bool trovato = false;
        int cartePescate = 0;
        
        while (!trovato) {
            Carta pescata = mazzo.pesca();
            giocatori[prossimo].pescaCarta(pescata);
            cartePescate++;
            // Se la carta appena pescata ha il colore che ci serve, ci fermiamo!
            if (pescata.getColore(latoOscuroAttivo) == coloreAttivo) {
                trovato = true;
            }
        }
        cout << "-> " << giocatori[prossimo].getNome() << " ha pescato " << cartePescate << " carte!" << endl;
        passaAlProssimoGiocatore();
    }
}