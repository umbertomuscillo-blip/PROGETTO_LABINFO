#!/bin/bash

echo "🔍 Controllo dei requisiti di sistema..."

# Controlla se Homebrew è installato (serve per installare roba su Mac)
if ! command -v brew &> /dev/null; then
    echo "❌ ATTENZIONE: Homebrew non è installato."
    echo "Apri il terminale e incolla questo comando per installarlo:"
    echo '/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"'
    exit 1
fi

# Controlla se SFML è installato nella cartella di Homebrew
if [ ! -d "/opt/homebrew/include/SFML" ]; then
    echo "⚠️ SFML non trovato! Inizio l'installazione automatica..."
    brew install sfml
    if [ $? -ne 0 ]; then
        echo "❌ Errore durante l'installazione di SFML. Controlla la tua connessione a Internet."
        exit 1
    fi
    echo "✅ SFML installato con successo!"
else
    echo "✅ SFML trovato. Tutto pronto."
fi

echo "🛠️  Compilazione di UNO Flip in corso..."

# Il mega-comando di compilazione
g++ -std=c++17 main_grafico.cpp Carta.cpp Mazzo.cpp Giocatore.cpp Partita.cpp Database.cpp GestoreRete.cpp -o unoflip_grafico -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

# Controlla se la compilazione è andata a buon fine
if [ $? -eq 0 ]; then
    echo "✅ Compilazione perfetta! Avvio del gioco..."
    ./unoflip_grafico
else
    echo "❌ Errore di compilazione! Controlla il codice."
fi