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
    gioco.setupIniziale(); 

    Carta cartaCentroDati = gioco.getCartaInCima();
    bool latoOscuro = gioco.getLatoOscuroAttivo();
    vector<Carta> miaMano = gioco.getGiocatori()[0].getMano(); 

    // --- SETUP GRAFICA ---
    sf::RenderWindow window(sf::VideoMode({1024, 768}), "UNO Flip! - Edizione 30 e Lode");
    sf::RectangleShape sfondoTavolo({1024.f, 768.f});
    sfondoTavolo.setFillColor(sf::Color(34, 139, 34)); 

    sf::Font font;
    if (!font.openFromFile("font.ttf")) {
        cout << "ERRORE: File font.ttf non trovato!" << endl;
        return -1;
    }

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
    testoCartaCentro.setPosition({445.f, 290.f}); 

    // 2. Creiamo le forme per le tue Carte in Mano
    vector<sf::RectangleShape> formeCarteMano;
    vector<sf::Text> testiCarteMano;
    
    float startX = 100.f; 
    for(int i = 0; i < miaMano.size(); i++) {
        sf::RectangleShape rect({100.f, 150.f});
        rect.setPosition({startX + (i * 115.f), 550.f}); 
        rect.setFillColor(sf::Color::White);
        rect.setOutlineThickness(3.f);
        rect.setOutlineColor(sf::Color::Black);
        formeCarteMano.push_back(rect);

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

            // --- RILEVAMENTO CLICK DEL MOUSE (NOVITÀ!) ---
            if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>())
            {
                // Controlliamo se è stato premuto il tasto Sinistro del mouse
                if (mouseClick->button == sf::Mouse::Button::Left)
                {
                    // Prendiamo le coordinate X e Y del click
                    sf::Vector2f mousePos(mouseClick->position.x, mouseClick->position.y);

                    // Scansioniamo tutte le carte della nostra mano per vedere se ne abbiamo colpita una
                    for(int i = 0; i < formeCarteMano.size(); i++) {
                        // getGlobalBounds() crea il rettangolo di collisione della carta
                        if (formeCarteMano[i].getGlobalBounds().contains(mousePos)) {
                            
                            string nomeCarta = latoOscuro ? miaMano[i].getDescrizioneOscura() : miaMano[i].getDescrizioneChiara();
                            cout << "-> HAI CLICCATO LA CARTA: " << nomeCarta << " (Indice " << i << ")" << endl;
                            
                            // Un piccolo feedback visivo: coloriamo di giallo la carta cliccata!
                            formeCarteMano[i].setFillColor(sf::Color::Yellow);
                        }
                    }
                }
            }
        }

        // --- FASE DI DISEGNO ---
        window.clear();               
        window.draw(sfondoTavolo);    
        window.draw(cartaCentroShape);     
        window.draw(testoCartaCentro);

        for(int i = 0; i < formeCarteMano.size(); i++) {
            window.draw(formeCarteMano[i]);
            window.draw(testiCarteMano[i]);
        }

        window.display();             
    }

    return 0;
}