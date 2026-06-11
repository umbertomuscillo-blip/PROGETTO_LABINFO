progetto labinfo - uno flip
fatto da umberto e fabio

questo e' il nostro progetto per l'esame di lab. abbiamo rifatto uno flip da zero in c++, ci sta la grafica, il multiplayer e pure un bot per giocare da soli.

come farlo partire:
- se hai il mac: apri il terminale, vai nella cartella Sorgenti e scrivi ./avvia.sh
(se ti da errore di permessi scrivi prima chmod +x avvia.sh e poi riprova)
- se hai windows: apri la cartella Eseguibili, estrai lo zip unoflip_windows_definitivo.zip e fai doppio click su unoflip.exe. non serve installare nient'altro.

come giocare in multiplayer:
uno deve fare da server (ospita partita) e aspettare. vi dira' un ip tipo 192.168...
l'altro clicca su connettiti e mette quell'ip li. fatelo sulla stessa rete wifi o con l'hotspot e siete a posto.

note per il prof sul codice:
abbiamo diviso tutto con il pattern mvc per separare la grafica dalla logica. per la grafica abbiamo usato sfml 3. 
per il multiplayer abbiamo usato i socket tcp non bloccanti cosi il gioco non lagga mentre aspetta i dati.
c'e' stato un bordello per far mischiare le carte allo stesso modo su mac e windows in multiplayer, quindi abbiamo scritto una funzione nostra per i numeri casuali basata sul seed iniziale cosi sono per forza uguali.
per i salvataggi leggiamo e scriviamo su dei file txt e csv (la classifica). 

i file sono tutti commentati cosi si capisce cosa fanno le classi (Carta, Mazzo, Partita, GestoreRete ecc).

buon divertimento!