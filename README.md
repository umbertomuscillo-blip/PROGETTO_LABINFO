# PROGETTO_LABINFO
🃏 Progetto Laboratorio di Informatica: UNO Flip

Questo repository contiene il caso di studio "UNO" esteso con le regole di "UNO Flip".
Il progetto è strutturato per soddisfare i requisiti avanzati (30 e lode), includendo classi C++, architettura Client-Server, interfaccia grafica (GUI) e salvataggio su Database (DBMS).

🌳 Architettura del Software

Di seguito è riportato il diagramma a blocchi che illustra le dipendenze e la struttura del nostro codice:

[mermaid]
graph TD
    subgraph Base ["1. ENTITÀ DI BASE (I Mattoncini)"]
        C(Carta <br/> Lato Chiaro/Oscuro)
        M(Mazzo <br/> Genera e Mescola)
        G(Giocatore <br/> Gestisce la Mano)
        GU(Giocatore Umano <br/> Input Tastiera/Mouse)
        GB(Giocatore Bot <br/> Intelligenza Artificiale)
        
        G --- GU
        G --- GB
    end

    subgraph Motore ["2. MOTORE DEL GIOCO (Il Cervello)"]
        P{Partita <br/> Direttore d'orchestra}
        P -->|Controlla| M
        P -->|Gestisce i turni di| G
    end

    subgraph Architettura ["3. ARCHITETTURA 30 E LODE"]
        DB[(Database DBMS <br/> Salva Statistiche)]
        S[Server <br/> Valida la logica]
        CL([Client GUI <br/> Interfaccia Grafica])
        
        CL <-->|Invia Mosse| S
        S <-->|Salva/Carica| DB
        S -->|Gestisce| P
    end


🚀 Milestone di Sviluppo

1. [Completata] Logica di base Carta e Mazzo

2. [In corso] Logica Giocatore (Umano e Bot)

3. [Da fare] Controller Partita

4. [Da fare] Integrazione DBMS

5. [Da fare] Rete Client-Server e GUI
