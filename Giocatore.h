/**
 * @file Giocatore.h
 * @brief Definizione della classe Giocatore.
 * Modella l'entità che partecipa alla partita, mantenendo lo stato della 
 * sua mano di carte e permettendo di distinguere tra un utente Umano e un Bot (Intelligenza Artificiale).
 */

#ifndef GIOCATORE_H
#define GIOCATORE_H

#include <string>
#include <vector>
#include "Carta.h" // Necessario perché usiamo oggetti di tipo Carta all'interno del vector

/**
 * @class Giocatore
 * @brief Contiene le informazioni personali e le carte possedute da un singolo giocatore.
 * Isola la logica di gestione della mano (aggiunta e rimozione sicura delle carte) 
 * dalle regole globali della partita.
 */
class Giocatore {
private:
    std::string nome;           ///< Nome testuale del giocatore da visualizzare a schermo
    std::vector<Carta> mano;    ///< Contenitore dinamico delle carte attualmente possedute
    bool isBot;                 ///< Flag (bandierina) booleano: true se mosso dalla CPU, false se Umano

public:
    /**
     * @brief Costruttore della classe.
     * @param nomeGiocatore La stringa identificativa del giocatore.
     * @param bot Parametro opzionale che definisce se è un Bot. Di default è false (Umano).
     */
    Giocatore(std::string nomeGiocatore, bool bot = false);

    // --- Metodi Getter (Accessori) ---
    
    /**
     * @brief Restituisce il nome del giocatore.
     */
    std::string getNome();

    /**
     * @brief Restituisce il numero di carte attualmente in mano.
     */
    int getNumeroCarte();

    /**
     * @brief Verifica se il giocatore ha vinto (0 carte in mano).
     */
    bool haFinitoLeCarte();

    /**
     * @brief Indica se questo giocatore deve essere gestito dall'IA.
     */
    bool getIsBot();

    /**
     * @brief Restituisce una copia dell'intera mano del giocatore.
     * Necessario specialmente per il Bot, per scansionare le carte e decidere la mossa.
     */
    std::vector<Carta> getMano(); 

    // --- Azioni del Giocatore ---

    /**
     * @brief Inserisce una nuova carta nella mano del giocatore.
     * @param c La carta pescata dal mazzo.
     */
    void pescaCarta(Carta c);

    /**
     * @brief Estrae una carta dalla mano per giocarla sul tavolo.
     * @param indice La posizione della carta nel vettore della mano.
     * @return L'oggetto Carta rimosso dalla mano.
     */
    Carta giocaCarta(int indice);

    /**
     * @brief Stampa la mano di carte a terminale (usato per debug o versione testuale).
     * @param latoOscuroAttivo Definisce se stampare i nomi del lato chiaro o oscuro.
     */
    void mostraMano(bool latoOscuroAttivo);
};

#endif