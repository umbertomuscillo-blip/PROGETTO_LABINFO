#ifndef GIOCATORE_H
#define GIOCATORE_H

#include <string>
#include <vector>
#include "Carta.h"

class Giocatore {
private:
    std::string nome;
    std::vector<Carta> mano; // La lista dinamica delle carte che ha in mano

public:
    // Costruttore
    Giocatore(std::string nomeGiocatore);

    // Metodi base
    std::string getNome();
    int getNumeroCarte();
    bool haFinitoLeCarte();

    // Metodi per interagire con le carte
    void pescaCarta(Carta c);
    Carta giocaCarta(int indice);
    
    // Metodo per vedere le carte (deve sapere se il mazzo è girato sul lato oscuro!)
    void mostraMano(bool latoOscuroAttivo);
};

#endif