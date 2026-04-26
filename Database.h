/**
 * @file Database.h
 * @brief Definizione della classe Database e della struttura dati associata.
 * Gestisce l'interfaccia per la persistenza dei dati (salvataggio e caricamento)
 * relativi alle statistiche dei giocatori a fine partita.
 */

#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>

/**
 * @struct Statistiche
 * @brief Struttura dati (POD - Plain Old Data) per raggruppare le informazioni di un singolo giocatore.
 * Utilizziamo una struct invece di una classe perché questo oggetto ha il solo 
 * scopo di trasportare dati passivamente, senza logica o metodi complessi.
 */
struct Statistiche {
    std::string nome;
    int partiteGiocate;
    int vittorie;
};

/**
 * @class Database
 * @brief Gestisce il salvataggio e il caricamento delle statistiche su file CSV.
 * Funge da "cache" in memoria: carica i dati dal file nel vettore all'avvio, 
 * li modifica in RAM durante l'esecuzione e li riversa sul file al momento del salvataggio.
 */
class Database {
private:
    std::string nomeFile;             ///< Il percorso/nome del file su disco (es. "classifica.csv")
    std::vector<Statistiche> records; ///< Cache in RAM contenente i dati di tutti i giocatori

    /**
     * @brief Cerca sequenzialmente un giocatore all'interno della cache.
     * Metodo privato perché è un'utility interna: l'utente della classe non 
     * ha bisogno di sapere come il Database indicizza i giocatori.
     * @param nome Il nome testuale da cercare.
     * @return L'indice nel vettore 'records', oppure -1 se non trovato.
     */
    int trovaIndiceGiocatore(std::string nome);

public:
    /**
     * @brief Costruttore della classe.
     * @param file Il nome del file da agganciare. Ha un valore di default ("classifica.csv").
     */
    Database(std::string file = "classifica.csv");

    /**
     * @brief Carica i dati dal disco alla memoria (RAM).
     */
    void caricaDati();

    /**
     * @brief Riversa i dati dalla memoria (RAM) al disco.
     */
    void salvaDati();

    /**
     * @brief Aggiorna le metriche di tutti i partecipanti e del vincitore.
     * @param partecipanti Lista di stringhe con i nomi di chi ha giocato.
     * @param nomeVincitore Stringa con il nome di chi ha vinto.
     */
    void aggiornaStatistiche(std::vector<std::string> partecipanti, std::string nomeVincitore);

    /**
     * @brief Stampa la classifica generale sul terminale.
     */
    void stampaClassifica();
};

#endif