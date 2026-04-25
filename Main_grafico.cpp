#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include "Partita.h"

using namespace std;

void impostaGraficaCarta(sf::RectangleShape& rect, sf::Text& testo, Carta cartaDati, bool latoOscuro, sf::Font& font, float x, float y) {
    rect.setSize({100.f, 150.f});
    rect.setPosition({x, y});
    rect.setFillColor(sf::Color::White);
    rect.setOutlineThickness(3.f);
    rect.setOutlineColor(sf::Color::Black);
    testo.setFont(font);
    testo.setString(latoOscuro ? cartaDati.getDescrizioneOscura() : cartaDati.getDescrizioneChiara());
    testo.setCharacterSize(14);
    testo.setFillColor(sf::Color::Black);
    testo.setPosition({x + 5.f, y + 60.f}); 
}

int main()
{
    vector<Giocatore> giocatori;
    giocatori.push_back(Giocatore("Umberto", false)); 
    giocatori.push_back(Giocatore("Prof. Vessio", true)); 
    
    Partita gioco(giocatori);
    gioco.setupIniziale(); 

    sf::RenderWindow window(sf::VideoMode({1024, 768}), "UNO Flip! - Edizione 30 e Lode");
    sf::RectangleShape sfondoTavolo({1024.f, 768.f});
    sf::Font font;
    if (!font.openFromFile("font.ttf")) return -1;

    sf::RectangleShape mazzoGrafico({100.f, 150.f});
    mazzoGrafico.setPosition({250.f, 250.f});
    mazzoGrafico.setOutlineThickness(3.f);
    mazzoGrafico.setOutlineColor(sf::Color::Black);
    
    sf::Text testoMazzo(font);
    testoMazzo.setString("MAZZO");
    testoMazzo.setCharacterSize(14);
    testoMazzo.setFillColor(sf::Color::White);
    testoMazzo.setPosition({265.f, 310.f});

    sf::RectangleShape cartaCentroShape;
    sf::Text testoCartaCentro(font); // CORRETTO QUI
    
    sf::Text testoFeedback(font);
    testoFeedback.setCharacterSize(24);
    testoFeedback.setFillColor(sf::Color::White);
    testoFeedback.setPosition({100.f, 50.f});

    // --- GAME LOOP ---
    while (window.isOpen())
    {
        bool latoOscuro = gioco.getLatoOscuroAttivo();
        vector<Carta> miaMano = gioco.getGiocatori()[0].getMano();
        int turno = gioco.getTurnoCorrente();
        
        sfondoTavolo.setFillColor(latoOscuro ? sf::Color(20, 20, 20) : sf::Color(34, 139, 34));
        mazzoGrafico.setFillColor(latoOscuro ? sf::Color(128, 0, 128) : sf::Color::Blue);

        impostaGraficaCarta(cartaCentroShape, testoCartaCentro, gioco.getCartaInCima(), latoOscuro, font, 437.f, 250.f);

        vector<sf::RectangleShape> formeCarteMano;
        vector<sf::Text> testiCarteMano;
        float startX = max(50.f, 512.f - (miaMano.size() * 55.f)); 
        for(int i = 0; i < miaMano.size(); i++) {
            sf::RectangleShape rect;
            sf::Text testo(font); // CORRETTO QUI
            impostaGraficaCarta(rect, testo, miaMano[i], latoOscuro, font, startX + (i * 110.f), 550.f);
            formeCarteMano.push_back(rect);
            testiCarteMano.push_back(testo);
        }

        if (gioco.partitaTerminata()) {
            testoFeedback.setString("PARTITA FINITA! Ha vinto: " + gioco.getVincitore());
        } else {
            testoFeedback.setString(turno == 0 ? "Tuo turno! Gioca o Pesca." : "Il Bot sta pensando...");
        }

        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();

            if (turno == 0 && !gioco.partitaTerminata() && event->is<sf::Event::MouseButtonPressed>())
            {
                if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos(event->getIf<sf::Event::MouseButtonPressed>()->position.x, event->getIf<sf::Event::MouseButtonPressed>()->position.y);

                    if (mazzoGrafico.getGlobalBounds().contains(mousePos)) {
                        gioco.mossaUmano(-1); // Pesca
                    }
                    for(int i = 0; i < formeCarteMano.size(); i++) {
                        if (formeCarteMano[i].getGlobalBounds().contains(mousePos)) {
                            if (gioco.mossaValida(miaMano[i])) {
                                gioco.mossaUmano(i); // Gioca
                            }
                        }
                    }
                }
            }
        }

        // Se è il turno del Bot, fa la sua mossa
        if (turno == 1 && !gioco.partitaTerminata()) {
            sf::sleep(sf::milliseconds(800)); // Pausa realistica
            gioco.mossaBot();
        }

        window.clear();               
        window.draw(sfondoTavolo);    
        window.draw(mazzoGrafico);
        window.draw(testoMazzo);
        window.draw(cartaCentroShape);     
        window.draw(testoCartaCentro);
        for(int i = 0; i < formeCarteMano.size(); i++) {
            window.draw(formeCarteMano[i]);
            window.draw(testiCarteMano[i]);
        }
        window.draw(testoFeedback);
        window.display();             
    }

    return 0;
}