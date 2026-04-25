#include "Database.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Database::Database(string file) {
    nomeFile = file;
}

int Database::trovaIndiceGiocatore(string nome) {
    for (int i = 0; i < records.size(); i++) {
        if (records[i].nome == nome) return i;
    }
    return -1; // Ritorna -1 se il giocatore è nuovo
}

void Database::caricaDati() {
    records.clear();
    ifstream fileLettura(nomeFile);
    
    if (!fileLettura.is_open()) {
        // È normale la prima volta: il file non esiste ancora!
        return; 
    }

    string riga, nome, temp;
    int giocate, vittorie;

    // Leggiamo il CSV riga per riga (Formato: Nome,Giocate,Vittorie)
    while (getline(fileLettura, riga)) {
        stringstream ss(riga);
        getline(ss, nome, ',');
        
        getline(ss, temp, ',');
        giocate = stoi(temp); // Trasforma la stringa in numero
        
        getline(ss, temp, ',');
        vittorie = stoi(temp);

        records.push_back({nome, giocate, vittorie});
    }
    fileLettura.close();
}

void Database::salvaDati() {
    ofstream fileScrittura(nomeFile); // Apre il file (lo crea se non esiste, o lo sovrascrive)
    
    if (fileScrittura.is_open()) {
        for (int i = 0; i < records.size(); i++) {
            fileScrittura << records[i].nome << "," 
                          << records[i].partiteGiocate << "," 
                          << records[i].vittorie << "\n";
        }
        fileScrittura.close();
    } else {
        cout << "Errore: impossibile salvare il file di database!" << endl;
    }
}

void Database::aggiornaStatistiche(vector<string> partecipanti, string nomeVincitore) {
    // 1. Aggiorniamo le "Partite Giocate" per TUTTI i partecipanti
    for (int i = 0; i < partecipanti.size(); i++) {
        int indice = trovaIndiceGiocatore(partecipanti[i]);
        
        if (indice == -1) {
            // Giocatore nuovo! Lo aggiungiamo alla lista con 1 partita e 0 vittorie iniziali
            records.push_back({partecipanti[i], 1, 0});
        } else {
            // Giocatore esistente, aumentiamo le partite
            records[indice].partiteGiocate++;
        }
    }

    // 2. Assegniamo la vittoria a chi ha vinto
    if (nomeVincitore != "") {
        int indice = trovaIndiceGiocatore(nomeVincitore);
        if (indice != -1) {
            records[indice].vittorie++;
        }
    }
}

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