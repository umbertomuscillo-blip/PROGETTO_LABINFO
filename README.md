# PROGETTO_LABINFO
🃏 Progetto Laboratorio di Informatica: UNO Flip! (Edizione 30 e Lode)

Questo repository contiene il caso di studio "UNO" esteso con le regole ufficiali di "UNO Flip".
Il progetto è stato sviluppato per soddisfare i requisiti avanzati del corso, includendo logica Object-Oriented in C++, Interfaccia Grafica 2D (SFML), Intelligenza Artificiale, salvataggio persistente su Database e architettura di Rete Client-Server.

---

## 🌳 Architettura del Software (Pattern MVC)

Il codice è stato progettato applicando rigorosamente il Design Pattern **Model-View-Controller (MVC)** e il principio dell'**Incapsulamento**. La logica pura del gioco è totalmente disaccoppiata dall'interfaccia visiva.

Di seguito il diagramma a blocchi delle dipendenze:

```mermaid
graph TD
    subgraph Base ["1. ENTITÀ DI BASE (Model)"]
        C(Carta <br/> Lato Chiaro/Oscuro)
        M(Mazzo <br/> Genera e Mescola)
        G(Giocatore <br/> Gestisce la Mano)
    end

    subgraph Motore ["2. MOTORE DEL GIOCO (Controller)"]
        P{Partita <br/> Direttore d'orchestra}
        P -->|Controlla| M
        P -->|Gestisce i turni di| G
    end

    subgraph Architettura ["3. ARCHITETTURA AVANZATA (View & Rete)"]
        DB[(Database CSV <br/> Salva Statistiche)]
        S[Server Socket <br/> Valida la logica]
        CL([Client SFML <br/> Interfaccia Grafica])
        GB(Bot IA <br/> Intelligenza Artificiale)
        
        CL <-->|Invia Mosse / Riceve Dati| S
        S <-->|Salva/Carica| DB
        S -->|Gestisce| P
        P -->|Innesca| GB
    end


🚀 Milestone di Sviluppo
[x] 1. Logica di base Carta e Mazzo: Generazione, mescolamento e doppia faccia.

[x] 2. Logica Giocatore: Gestione memoria dinamica (std::vector) della mano.

[x] 3. Controller Partita: Applicazione millimetrica del regolamento ufficiale.

[x] 4. Interfaccia Grafica (GUI): Integrazione completa libreria SFML 3.

[x] 5. Integrazione DBMS: Salvataggio e lettura persistente delle statistiche.

[x] 6. Intelligenza Artificiale: Bot autonomo integrato nel Game Loop.

[ ] 7. Rete Client-Server: Socket per multiplayer su macchine diverse (In Lavorazione).

✨ Funzionalità Tecniche Implementate (Current Build)
Attualmente il gioco è un'applicazione desktop completa, robusta e responsiva.

⚙️ Motore di Gioco (Core Engine)

Generazione Dinamica: 112 carte generate assemblando casualmente "Mezze Carte" chiare e oscure, garantendo combinazioni fronte/retro uniche ad ogni nuova partita.

Eccezioni Ufficiali 1vs1: Le carte "Inverti" e "Salta" sono calcolate matematicamente per bloccare il turno dell'avversario nelle partite a 2 giocatori.

Validazione Assoluta: Il sistema impedisce mosse illegali, riconoscendo colori, numeri e applicando automaticamente le penitenze (+1, +5, Pesca Colore).

🖥️ Grafica e UI (SFML)

Macchina a Stati (FSM): Gestione fluida delle transizioni tra Menu Principale, Game Loop e Schermata di Fine Partita (senza memory leaks tramite allocazione/deallocazione dinamica della Partita).

Letterboxing e Resize: Adattamento automatico della telecamera (sf::View) in caso di ridimensionamento della finestra, mantenendo intatto l'aspect ratio e la precisione dei click del mouse.

Collision Detection Adattiva: Algoritmo matematico per l'overlapping delle carte in mano. Se il giocatore accumula troppe carte, l'interfaccia le sovrappone verso il centro calcolando il click in base allo Z-Index visivo (cliccando sempre la carta in cima).

Meccanica "UNO!": Implementato un pulsante a schermo. Se il giocatore gioca la penultima carta senza prima aver premuto "UNO!", il motore gli infligge dinamicamente 2 carte di penalità.

🧠 IA e Persistenza Dati

Bot Greedy: L'avversario CPU analizza la propria mano ed esegue mosse valide (inclusa la scelta strategica del colore sui Jolly) simulando un tempo di pensiero umano (Delay Timer).

Database (CSV): Utilizzo della libreria fstream (ifstream/ofstream e stringstream) per caricare lo storico vittorie all'avvio e aggiornare il database locale a partita conclusa.