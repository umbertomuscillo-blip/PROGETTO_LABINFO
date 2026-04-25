#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

// Creiamo un "mattoncino" personalizzato per contenere i dati di un singolo giocatore
struct Statistiche {
    std::string nome;
    int partiteGiocate;
    int vittorie;
};

class Database {
private:
    std::string nomeFile;
    std::vector<Statistiche> records; // La lista in memoria di tutti i giocatori

    // Funzione interna per capire se un giocatore esiste già nel file
    int trovaIndiceGiocatore(std::string nome);

public:
    // Costruttore: di default chiamerà il file "classifica.csv"
    Database(std::string file = "classifica.csv");

    // Metodi principali
    void caricaDati();
    void salvaDati();
    void aggiornaStatistiche(std::vector<std::string> partecipanti, std::string nomeVincitore);
    void stampaClassifica();
};

#endif