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





✅ FASE 1 COMPLETATA: Il Motore di Gioco Centrale (Core Engine)

Abbiamo un gioco solido, testato e funzionante nel terminale.

Carta & Mazzo: Generazione dinamica di 112 carte con lati chiari e oscuri mescolati in modo asincrono (niente abbinamenti scontati).

Giocatori: Gestione dinamica della mano di carte (pesca, scarta, visualizzazione adattiva lato chiaro/oscuro).

Partita & Turni: Flusso circolare perfetto, gestione dell'inversione di marcia e controllo di chiusura (vittoria).

Regole & Validazione: Impossibile barare. Il gioco riconosce colori e numeri.

Effetti Speciali: Il mazzo "Flippa", i Jolly fanno scegliere il colore (con comunicazione a schermo al prossimo giocatore), e le penalità (Pesca 1, Pesca 5, Pesca Colore, Salta) vengono inflitte correttamente all'avversario.

Eccezioni Ufficiali: Inverti che vale come Salta per 2 giocatori.