/**
 * @file Giocatore.cpp
 * @brief Implementazione dei metodi della classe Giocatore.
 * Gestisce l'identità del giocatore (umano o bot) e le operazioni 
 * sulla sua mano di carte (pescare, giocare, visualizzare).
 */

#include "Giocatore.h"
#include <iostream>

using namespace std;

/**
 * @brief Costruttore: inizializza l'identità del giocatore.
 * La mano di carte (il vector 'mano') non ha bisogno di essere inizializzata
 * qui perché i costruttori standard del C++ creano automaticamente un vector vuoto.
 * * @param nomeGiocatore Stringa contenente il nome scelto.
 * @param bot Booleano: true se il giocatore è controllato dall'Intelligenza Artificiale.
 */
Giocatore::Giocatore(string nomeGiocatore, bool bot) {
    nome = nomeGiocatore;
    isBot = bot;
}

// ==============================================================================
// METODI GETTER (Accessori)
// Implementano il principio dell'Incapsulamento: permettono alle altre classi 
// di leggere lo stato del giocatore senza poterne alterare le variabili private.
// ==============================================================================

string Giocatore::getNome() { return nome; }

int Giocatore::getNumeroCarte() { return mano.size(); }

/**
 * @brief Controlla se il giocatore ha vinto.
 * Sfrutta la funzione integrata empty() del vector che è semanticamente più 
 * chiara e più veloce rispetto a controllare se size() == 0.
 */
bool Giocatore::haFinitoLeCarte() { return mano.empty(); }

bool Giocatore::getIsBot() { return isBot; }

vector<Carta> Giocatore::getMano() { return mano; }


// ==============================================================================
// AZIONI DEL GIOCATORE
// ==============================================================================

/**
 * @brief Aggiunge una carta alla mano del giocatore.
 * Utilizza la funzione push_back() della libreria <vector>, che inserisce 
 * dinamicamente l'elemento in coda riallocando la memoria se necessario.
 * * @param c L'oggetto Carta pescato dal mazzo.
 */
void Giocatore::pescaCarta(Carta c) { 
    mano.push_back(c); 
}

/**
 * @brief Estrae una carta dalla mano per poterla giocare sul tavolo.
 * Questa operazione fa due cose: salva temporaneamente la carta, la cancella 
 * fisicamente dalla memoria della mano del giocatore, e poi la restituisce.
 * * @param indice La posizione (0-based) della carta selezionata nella mano.
 * @return La Carta estratta, pronta per essere messa sul tavolo.
 */
Carta Giocatore::giocaCarta(int indice) {
    // 1. Salviamo una copia della carta prima di cancellarla
    Carta cartaGiocata = mano[indice]; 
    
    // 2. Rimuoviamo la carta dal vettore. 
    // Il metodo erase() richiede un 'Iteratore', non un semplice numero intero.
    // mano.begin() è l'iteratore che punta al primo elemento, a cui sommiamo l'indice.
    mano.erase(mano.begin() + indice); 
    
    // 3. Restituiamo la copia alla Partita
    return cartaGiocata;
}

/**
 * @brief Stampa formattata sul terminale di tutte le carte possedute.
 * (Metodo utilizzato per la versione testuale del gioco o per il debug).
 * * @param latoOscuroAttivo Indica quale faccia della carta deve essere stampata.
 */
void Giocatore::mostraMano(bool latoOscuroAttivo) {
    cout << "\n--- Carte di " << nome << " ---" << endl;
    for (int i = 0; i < mano.size(); i++) {
        cout << "[" << i << "] ";
        if (latoOscuroAttivo) cout << mano[i].getDescrizioneOscura() << endl;
        else cout << mano[i].getDescrizioneChiara() << endl;
    }
    cout << "-----------------------" << endl;
}