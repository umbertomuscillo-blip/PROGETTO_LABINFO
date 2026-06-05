#include "EndGameScene.h"
#include "SceneManager.h"
#include "MenuScene.h"
#include "SoundManager.h"

EndGameScene::EndGameScene(SceneManager* mgr, const std::string& vincitore) 
    : Scene(mgr), testoVincitore(mgr->getFont()), testoTitoloClassifica(mgr->getFont()), 
      btnTornaMenu(mgr->getFont()), tramaSfondo(sf::PrimitiveType::Lines) 
{
    sf::Font& font = manager->getFont();
    
    testoVincitore.setFont(font);
    testoVincitore.setString("VITTORIA DI: " + vincitore + "!");
    testoVincitore.setCharacterSize(55);
    testoVincitore.setFillColor(sf::Color::Yellow);
    testoVincitore.setOutlineThickness(4.f);
    testoVincitore.setOutlineColor(sf::Color::Red);
    testoVincitore.setPosition({(1024.f - testoVincitore.getLocalBounds().size.x) / 2.f, 90.f}); 

    pannelloOmbra.setSize({600.f, 420.f});
    pannelloOmbra.setPosition({220.f, 188.f});
    pannelloOmbra.setFillColor(sf::Color(0, 0, 0, 150));

    pannelloClassifica.setSize({600.f, 420.f}); 
    pannelloClassifica.setPosition({212.f, 180.f}); 
    pannelloClassifica.setFillColor(sf::Color(20, 20, 40, 200)); 
    pannelloClassifica.setOutlineThickness(2.f); 
    pannelloClassifica.setOutlineColor(sf::Color(100, 200, 255, 150));
    
    testoTitoloClassifica.setFont(font); 
    testoTitoloClassifica.setString("--- HALL OF FAME ---"); 
    testoTitoloClassifica.setCharacterSize(34); 
    testoTitoloClassifica.setFillColor(sf::Color::Yellow); 
    testoTitoloClassifica.setPosition({(1024.f - testoTitoloClassifica.getLocalBounds().size.x)/2.f, 210.f});

    btnTornaMenu.setup("TORNA AL MENU", 312.f, 650.f, 400.f, 55.f, sf::Color(160, 30, 30, 200), sf::Color(220, 50, 50));

    datiClassificaRecenti = leggiClassificaDati();

    for(int i = -1024; i < 2048; i += 40) {
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i, 0.f), sf::Color(0, 0, 0, 40)}); 
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i + 768.f, 768.f), sf::Color(0, 0, 0, 40)});
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i, 768.f), sf::Color(0, 0, 0, 40)}); 
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i + 768.f, 0.f), sf::Color(0, 0, 0, 40)});
    }

    fireworks.init(300, {1024.f, 768.f}, true);

    SoundManager::getInstance().playSound("vittoria");
}

void EndGameScene::handleInput(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
    if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
        sf::Vector2i pixelPos = event->getIf<sf::Event::MouseButtonPressed>()->position; 
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
        
        if (btnTornaMenu.cliccato(mousePos)) {
            manager->changeScene(std::make_unique<MenuScene>(manager));
        }
    }
}

void EndGameScene::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window); 
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
    btnTornaMenu.aggiorna(mousePos, deltaTime);
    fireworks.update(deltaTime, true);
}

void EndGameScene::render(sf::RenderWindow& window) {
    sf::RectangleShape sfondone({1024.f, 768.f});
    sfondone.setFillColor(sf::Color(20, 20, 30));
    window.draw(sfondone);

    fireworks.draw(window);
    window.draw(tramaSfondo);
    window.draw(testoVincitore);
    window.draw(pannelloOmbra);
    window.draw(pannelloClassifica);
    window.draw(testoTitoloClassifica);
    disegnaTabellaClassifica(window, manager->getFont(), datiClassificaRecenti);
    btnTornaMenu.disegna(window);
}
