#include "LeaderboardScene.h"
#include "SceneManager.h"
#include "MenuScene.h"

LeaderboardScene::LeaderboardScene(SceneManager* mgr) : Scene(mgr), 
    testoTitoloClassifica(mgr->getFont()), btnIndietro(mgr->getFont()), tramaSfondo(sf::PrimitiveType::Lines) {
    sf::Font& font = manager->getFont();
    
    pannelloClassifica.setSize({600.f, 420.f}); 
    pannelloClassifica.setPosition({212.f, 180.f}); 
    pannelloClassifica.setFillColor(sf::Color(20, 20, 30, 220)); 
    pannelloClassifica.setOutlineThickness(3.f); 
    pannelloClassifica.setOutlineColor(sf::Color(100, 200, 255));
    
    testoTitoloClassifica.setFont(font); 
    testoTitoloClassifica.setString("--- HALL OF FAME ---"); 
    testoTitoloClassifica.setCharacterSize(34); 
    testoTitoloClassifica.setFillColor(sf::Color::Yellow); 
    testoTitoloClassifica.setPosition({(1024.f - testoTitoloClassifica.getLocalBounds().size.x)/2.f, 210.f});

    btnIndietro.setup("INDIETRO", 312.f, 650.f, 400.f, 55.f, sf::Color(80, 80, 80, 200), sf::Color(130, 130, 130));

    datiClassificaRecenti = leggiClassificaDati();

    for(int i = -1024; i < 2048; i += 40) {
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i, 0.f), sf::Color(0, 0, 0, 40)}); 
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i + 768.f, 768.f), sf::Color(0, 0, 0, 40)});
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i, 768.f), sf::Color(0, 0, 0, 40)}); 
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i + 768.f, 0.f), sf::Color(0, 0, 0, 40)});
    }
}

void LeaderboardScene::handleInput(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
    if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
        sf::Vector2i pixelPos = event->getIf<sf::Event::MouseButtonPressed>()->position; 
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
        
        if (btnIndietro.cliccato(mousePos)) {
            manager->changeScene(std::make_unique<MenuScene>(manager));
        }
    }
}

void LeaderboardScene::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window); 
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
    btnIndietro.aggiorna(mousePos, deltaTime);
}

void LeaderboardScene::render(sf::RenderWindow& window) {
    window.draw(tramaSfondo);
    window.draw(pannelloClassifica);
    window.draw(testoTitoloClassifica);
    disegnaTabellaClassifica(window, manager->getFont(), datiClassificaRecenti);
    btnIndietro.disegna(window);
}
