#ifndef GIOCATORE_H
#define GIOCATORE_H

#include <string>
#include <vector>
#include "Carta.h"

class Giocatore {
private:
    std::string nome;
    std::vector<Carta> mano;
    bool isBot; // Flag per riconoscere l'IA

public:
    Giocatore(std::string nomeGiocatore, bool bot = false);

    std::string getNome();
    int getNumeroCarte();
    bool haFinitoLeCarte();
    bool getIsBot();
    std::vector<Carta> getMano(); // Serve a far "leggere" la mano al Bot

    void pescaCarta(Carta c);
    Carta giocaCarta(int indice);
    void mostraMano(bool latoOscuroAttivo);
};

#endif