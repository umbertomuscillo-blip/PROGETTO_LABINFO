#ifndef CARTA_H
#define CARTA_H

#include <string>

// NERO serve per le carte Jolly che non hanno un colore di base
enum Colore { 
    ROSSO, GIALLO, VERDE, BLU,           // Colori Lato Chiaro
    ROSA, VERDE_ACQUA, ARANCIONE, VIOLA, // Colori Lato Oscuro
    NERO                                 // Colore di partenza per i Jolly
};

// Enum aggiornato con le vere regole di UNO Flip! (Niente numero ZERO)
enum Valore { 
    UNO, DUE, TRE, QUATTRO, CINQUE, SEI, SETTE, OTTO, NOVE, 
    SALTA,               // Lato Chiaro
    SALTA_TUTTI,         // Lato Oscuro
    INVERTI,             // Entrambi i lati
    FLIP,                // Entrambi i lati (la carta che fa girare il mazzo!)
    PESCA_UNO,           // Lato Chiaro
    PESCA_CINQUE,        // Lato Oscuro
    JOLLY,               // Entrambi i lati (cambia solo colore)
    JOLLY_PESCA_DUE,     // Lato Chiaro (cambia colore e fa pescare 2)
    JOLLY_PESCA_COLORE   // Lato Oscuro (cambia colore e fa pescare a oltranza)
};

class Carta {
private:
    Colore coloreChiaro;
    Valore valoreChiaro;
    
    Colore coloreOscuro;
    Valore valoreOscuro;

public:
    Carta(Colore cChiaro, Valore vChiaro, Colore cOscuro, Valore vOscuro);

    std::string getDescrizioneChiara();
    std::string getDescrizioneOscura();
};

#endif