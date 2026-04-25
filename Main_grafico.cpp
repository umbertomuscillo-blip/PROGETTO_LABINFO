#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include "Partita.h"
#include "Database.h" 

using namespace std;

int main()
{
    cout << "Avvio motore logico..." << endl;
    vector<Giocatore> giocatori;
    giocatori.push_back(Giocatore("Umberto", false));
    giocatori.push_back(Giocatore("Prof. Vessio", true));
    
    Partita gioco(giocatori);
    gioco.setupIniziale(); // Dà le 7 carte a testa

    // Carichiamo lo stato attuale del gioco per la grafica
    Carta cartaCentroDati = gioco.getCartaInCima();
    bool latoOscuro = gioco.getLatoOscuroAttivo();
    vector<Carta> miaMano = gioco.getGiocatori()[0].getMano(); // Le carte di Umberto

    // --- SETUP GRAFICA ---
    sf::RenderWindow window(sf::VideoMode({1024, 768}), "UNO Flip! - Edizione 30 e Lode");
    sf::RectangleShape sfondoTavolo({1024.f, 768.f});
    sfondoTavolo.setFillColor(sf::Color(34, 139, 34)); 

    // Caricamento del Font che abbiamo clonato dal Mac
    sf::Font font;
    if (!font.openFromFile("font.ttf")) {
        cout << "ERRORE: File font.ttf non trovato!" << endl;
        return -1;
    }

    // --- CREAZIONE ELEMENTI GRAFICI ---
    
    // 1. La Carta in Cima al Tavolo
    sf::RectangleShape cartaCentroShape({150.f, 220.f});
    cartaCentroShape.setPosition({437.f, 200.f}); 
    cartaCentroShape.setFillColor(sf::Color::White);
    cartaCentroShape.setOutlineThickness(5.f);
    cartaCentroShape.setOutlineColor(sf::Color::Black);

    sf::Text testoCartaCentro(font);
    testoCartaCentro.setString(latoOscuro ? cartaCentroDati.getDescrizioneOscura() : cartaCentroDati.getDescrizioneChiara());
    testoCartaCentro.setCharacterSize(20);
    testoCartaCentro.setFillColor(sf::Color::Black);
    testoCartaCentro.setPosition({445.f, 290.f}); // Posizionato al centro della carta bianca

    // 2. Le tue Carte in Mano (Le disegniamo una accanto all'altra in basso)
    vector<sf::RectangleShape> formeCarteMano;
    vector<sf::Text> testiCarteMano;
    
    float startX = 100.f; // Punto di partenza da sinistra
    for(int i = 0; i < miaMano.size(); i++) {
        // Disegna il rettangolo della carta
        sf::RectangleShape rect({100.f, 150.f});
        rect.setPosition({startX + (i * 115.f), 550.f}); // 115 di spazio tra una carta e l'altra
        rect.setFillColor(sf::Color::White);
        rect.setOutlineThickness(3.f);
        rect.setOutlineColor(sf::Color::Black);
        formeCarteMano.push_back(rect);

        // Disegna il testo della carta
        sf::Text testo(font);
        testo.setString(latoOscuro ? miaMano[i].getDescrizioneOscura() : miaMano[i].getDescrizioneChiara());
        testo.setCharacterSize(14);
        testo.setFillColor(sf::Color::Black);
        testo.setPosition({startX + (i * 115.f) + 5.f, 610.f});
        testiCarteMano.push_back(testo);
    }

    // --- GAME LOOP ---
    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        window.clear();               
        window.draw(sfondoTavolo);    
        
        // Disegna il tavolo
        window.draw(cartaCentroShape);     
        window.draw(testoCartaCentro);

        // Disegna la tua mano
        for(int i = 0; i < formeCarteMano.size(); i++) {
            window.draw(formeCarteMano[i]);
            window.draw(testiCarteMano[i]);
        }

        window.display();             
    }

    return 0;
}