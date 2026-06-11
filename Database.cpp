// Database.cpp
// Implementazione dei metodi per la gestione della persistenza dei dati.
// Permette di salvare e caricare le statistiche dei giocatori su un file CSV locale.

#include "Database.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Costruttore: inizializza il gestore del database.
// Parametro: file Il nome (o percorso) del file CSV da utilizzare (es. "classifica.csv").
Database::Database(string file) {
    nomeFile = file;
}

// Cerca un giocatore nel database caricato in memoria.
// Esegue una ricerca lineare all'interno del vettore dei record.
// * Parametro: nome Il nome del giocatore da cercare.
// Ritorna: L'indice del giocatore nel vettore, oppure -1 se il giocatore non esiste.
int Database::trovaIndiceGiocatore(string nome) {
    for (int i = 0; i < records.size(); i++) {
        if (records[i].nome == nome) return i;
    }
    return -1; // -1 è il valore standard informatico per indicare "Non Trovato"
}

// Carica i dati dal file CSV alla memoria (nel vettore records).
// Se il file non esiste (es. prima partita in assoluto), la funzione termina in modo sicuro.
void Database::caricaDati() {
    records.clear(); // Svuota la memoria prima di caricare, per evitare duplicati
    
    // ifstream (Input File Stream) apre il file in modalità lettura
    ifstream fileLettura(nomeFile);
    
    if (!fileLettura.is_open()) {
        // Nessun errore: è normale la prima volta che si lancia il gioco!
        return; 
    }

    string riga, nome, temp;
    int giocate, vittorie;

    // Leggiamo il file CSV riga per riga fino alla fine del file
    while (getline(fileLettura, riga)) {
        // stringstream permette di trattare la riga di testo come se fosse uno stream di input (come cin)
        stringstream ss(riga);
        
        // Estraiamo i campi separati dalla virgola (',')
        getline(ss, nome, ',');
        
        getline(ss, temp, ',');
        giocate = stoi(temp); // stoi (String TO Integer) converte il testo in numero intero
        
        getline(ss, temp, ',');
        vittorie = stoi(temp);

        // Creiamo e inseriamo il record nel vettore in memoria
        records.push_back({nome, giocate, vittorie});
    }
    fileLettura.close(); // Buona pratica: chiudere sempre i file al termine dell'operazione
}

// Salva i dati dalla memoria (vettore records) al file CSV.
// Sovrascrive il file precedente aggiornandolo con le nuove statistiche.
void Database::salvaDati() {
    // ofstream (Output File Stream) apre il file in scrittura. Di default lo sovrascrive.
    ofstream fileScrittura(nomeFile); 
    
    if (fileScrittura.is_open()) {
        for (int i = 0; i < records.size(); i++) {
            // Scriviamo nel file separando i valori con le virgole e andando a capo
            fileScrittura << records[i].nome << "," 
                          << records[i].partiteGiocate << "," 
                          << records[i].vittorie << "\n";
        }
        fileScrittura.close();
    } else {
        cout << "Errore: impossibile salvare il file di database!" << endl;
    }
}

// Aggiorna le statistiche a fine partita, incrementando giocate e vittorie.
// Se un giocatore ha partecipato per la prima volta, viene creato un nuovo record.
// * Parametro: partecipanti Vettore contenente i nomi di chi ha giocato.
// Parametro: nomeVincitore Nome di chi ha vinto (stringa vuota in caso di pareggio/errore).
void Database::aggiornaStatistiche(vector<string> partecipanti, string nomeVincitore) {
    // 1. Aggiorniamo le "Partite Giocate" per TUTTI i partecipanti
    for (int i = 0; i < partecipanti.size(); i++) {
        int indice = trovaIndiceGiocatore(partecipanti[i]);
        
        if (indice == -1) {
            // Giocatore nuovo! Lo aggiungiamo alla lista con 1 partita e 0 vittorie
            records.push_back({partecipanti[i], 1, 0});
        } else {
            // Giocatore esistente, incrementiamo il contatore delle partite
            records[indice].partiteGiocate++;
        }
    }

    // 2. Assegniamo la vittoria al vincitore
    if (nomeVincitore != "") {
        int indice = trovaIndiceGiocatore(nomeVincitore);
        if (indice != -1) {
            records[indice].vittorie++;
        }
    }
}

// Stampa la classifica formattata (utilizzata principalmente per il debug su terminale).
void Database::stampaClassifica() {
    cout << "\n=====================================" << endl;
    cout << "        CLASSIFICA GLOBALE           " << endl;
    cout << "=====================================" << endl;
    if (records.empty()) {
        cout << "Nessuna partita giocata finora." << endl;
    } else {
        for (int i = 0; i < records.size(); i++) {
            cout << records[i].nome 
                 << " -> Vittorie: " << records[i].vittorie 
                 << " / Giocate: " << records[i].partiteGiocate << endl;
        }
    }
    cout << "=====================================\n" << endl;
}