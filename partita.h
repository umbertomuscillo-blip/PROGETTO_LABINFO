/**
 * @file Partita.h
 * @brief Definizione della classe centrale Partita.
 * Rappresenta il "motore" logico (Game Engine) di UNO Flip. Fa comunicare
 * tra loro Mazzo, Carte e Giocatori applicando rigidamente le regole del gioco.
 */

#ifndef PARTITA_H
#define PARTITA_H

#include <vector>
#include <string>
#include "Mazzo.h"
#include "Giocatore.h"

/**
 * @class Partita
 * @brief Coordina lo stato globale del gioco.
 * Mantiene in memoria di chi è il turno, il senso di marcia, il colore da
 * rispettare e gestisce l'interazione tra interfaccia grafica (View) e logica (Model).
 */
class Partita {
private:
    // --- COMPONENTI DEL TAVOLO ---
    std::vector<Giocatore> giocatori; ///< Elenco dei partecipanti alla partita
    Mazzo mazzo;                      ///< Il gestore fisico delle carte (pesca/scarti)
    
    // --- STATO GLOBALE DEL GIOCO ---
    int turnoCorrente;                ///< Indice (0 o 1) del giocatore che deve muovere
    bool sensoOrario;                 ///< Direzione del gioco (influenzata dalle carte Inverti)
    bool latoOscuroAttivo;            ///< Flag globale: se true, si gioca col Lato Oscuro (Flip!)
    
    // --- REGOLE DEL TAVOLO ---
    Carta cartaInCima;                ///< L'ultima carta fisica giocata (visibile sul tavolo)
    Colore coloreAttivo;              ///< Il colore da rispettare (può differire dal colore della carta in cima a causa dei Jolly)
    
    // --- FEEDBACK UI ---
    std::string ultimoLogBot;         ///< Stringa di testo usata per comunicare alla GUI l'azione del Bot

public:
    /**
     * @brief Costruttore: istanzia la partita con i giocatori forniti.
     * @param listaGiocatori Vettore con i giocatori (umani o bot) già inizializzati.
     */
    Partita(std::vector<Giocatore> listaGiocatori);

    // ==========================================================
    // METODI LOGICI GLOBALI
    // ==========================================================

    /**
     * @brief Configura il tavolo prima del turno 0.
     * Mescola il mazzo, dà 7 carte a testa e svela la prima carta.
     */
    void setupIniziale();

    /**
     * @brief Funzione residua della vecchia versione testuale.
     * Nelle interfacce grafiche non viene chiamata perché il loop è gestito dalla libreria SFML.
     */
    void eseguiTurno(); 

    /**
     * @brief Controlla se le condizioni di vittoria sono state raggiunte.
     * @return True se almeno un giocatore ha zero carte in mano.
     */
    bool partitaTerminata();

    /**
     * @brief Recupera il nome di chi ha svuotato la mano.
     * @return Stringa col nome del vincitore.
     */
    std::string getVincitore();

    /**
     * @brief Motore di validazione del regolamento di UNO.
     * @param c La carta che si intende giocare.
     * @return True se i colori o i numeri combaciano (o se è un Jolly).
     */
    bool mossaValida(Carta c);

    /**
     * @brief Applica eventuali poteri speciali (+5, Salta, Flip, ecc.).
     * @param c La carta legalmente giocata di cui applicare gli effetti.
     */
    void applicaEffetto(Carta c);

    /**
     * @brief Avanza il turno rispettando il senso di marcia.
     */
    void passaAlProssimoGiocatore();

    /**
     * @brief Stampa su console testuale la situazione. Usata per debug.
     */
    void stampaStatoPartita();

    // ==========================================================
    // METODI GETTER PER L'INTERFACCIA GRAFICA (VIEW)
    // ==========================================================
    // Questi metodi implementano il pattern MVC. La logica espone 
    // i propri dati in "sola lettura" affinché la grafica possa disegnarli.

    Carta getCartaInCima();
    Colore getColoreAttivo();
    bool getLatoOscuroAttivo();
    std::vector<Giocatore> getGiocatori();
    int getTurnoCorrente();
    std::string getUltimoLogBot();
    
    // ==========================================================
    // RICEZIONE DEGLI INPUT (CONTROLLER)
    // ==========================================================

    /**
     * @brief Elabora i click del mouse tradotti dall'interfaccia grafica.
     * @param scelta L'indice della carta (-1 per pescare).
     * @param coloreScelto Da 0 a 3, inviato dai bottoni a schermo quando si gioca un Jolly.
     * @param haDettoUno True se l'utente ha premuto il tasto rosso GUI per l'UNO.
     */
    void mossaUmano(int scelta, int coloreScelto = 0, bool haDettoUno = false);

    /**
     * @brief Innesca il ragionamento automatico dell'Intelligenza Artificiale.
     */
    void mossaBot();
};

#endif