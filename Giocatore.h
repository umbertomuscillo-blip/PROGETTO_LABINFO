// Giocatore.h
// Definizione della classe Giocatore.
// Modella l'entità che partecipa alla partita, mantenendo lo stato della
// sua mano di carte e permettendo di distinguere tra un utente Umano e un Bot (Intelligenza Artificiale).

#ifndef GIOCATORE_H
#define GIOCATORE_H

#include <string>
#include <vector>
#include "Carta.h" // Necessario perché usiamo oggetti di tipo Carta all'interno del vector

// Classe Giocatore
// Contiene le informazioni personali e le carte possedute da un singolo giocatore.
// Isola la logica di gestione della mano (aggiunta e rimozione sicura delle carte)
// dalle regole globali della partita.
class Giocatore {
private:
    std::string nome;           ///< Nome testuale del giocatore da visualizzare a schermo
    std::vector<Carta> mano;    ///< Contenitore dinamico delle carte attualmente possedute
    bool isBot;                 ///< Flag (bandierina) booleano: true se mosso dalla CPU, false se Umano

public:
    // Costruttore della classe.
// Parametro: nomeGiocatore La stringa identificativa del giocatore.
// Parametro: bot Parametro opzionale che definisce se è un Bot. Di default è false (Umano).
    Giocatore(std::string nomeGiocatore, bool bot = false);

    // --- Metodi Getter (Accessori) ---
    
    // Restituisce il nome del giocatore.
    std::string getNome();

    // Restituisce il numero di carte attualmente in mano.
    int getNumeroCarte();

    // Verifica se il giocatore ha vinto (0 carte in mano).
    bool haFinitoLeCarte();

    // Indica se questo giocatore deve essere gestito dall'IA.
    bool getIsBot();

    // Restituisce una copia dell'intera mano del giocatore.
// Necessario specialmente per il Bot, per scansionare le carte e decidere la mossa.
    std::vector<Carta> getMano(); 

    // --- Azioni del Giocatore ---

    // Inserisce una nuova carta nella mano del giocatore.
// Parametro: c La carta pescata dal mazzo.
    void pescaCarta(Carta c);

    // Estrae una carta dalla mano per giocarla sul tavolo.
// Parametro: indice La posizione della carta nel vettore della mano.
// Ritorna: L'oggetto Carta rimosso dalla mano.
    Carta giocaCarta(int indice);

    // Stampa la mano di carte a terminale (usato per debug o versione testuale).
// Parametro: latoOscuroAttivo Definisce se stampare i nomi del lato chiaro o oscuro.
    void mostraMano(bool latoOscuroAttivo);
};

#endif