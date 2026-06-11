#include "LoginScene.h"
#include "SceneManager.h"
#include "MenuScene.h"
#include "SoundManager.h"

LoginScene::LoginScene(SceneManager* mgr) : Scene(mgr), nomeInput(""), tramaSfondo(sf::PrimitiveType::Lines),
    testoTitoloLogin(mgr->getFont()), testoIstruzioniLogin(mgr->getFont()), testoNomeInput(mgr->getFont()) {
    sf::Font& font = manager->getFont();

    testoTitoloLogin.setFont(font);
    testoTitoloLogin.setString("CREA O SELEZIONA PROFILO");
    testoTitoloLogin.setCharacterSize(50);
    testoTitoloLogin.setPosition({150.f, 150.f});
    testoTitoloLogin.setFillColor(sf::Color::Yellow);

    testoIstruzioniLogin.setFont(font);
    testoIstruzioniLogin.setString("Scrivi il tuo nome e premi INVIO:");
    testoIstruzioniLogin.setCharacterSize(24);
    testoIstruzioniLogin.setPosition({300.f, 300.f});

    testoNomeInput.setFont(font);
    testoNomeInput.setString("_");
    testoNomeInput.setCharacterSize(40);
    testoNomeInput.setPosition({350.f, 380.f});
    testoNomeInput.setFillColor(sf::Color::Green);

    for(int i = -1024; i < 2048; i += 40) {
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i, 0.f), sf::Color(0, 0, 0, 40)}); 
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i + 768.f, 768.f), sf::Color(0, 0, 0, 40)});
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i, 768.f), sf::Color(0, 0, 0, 40)}); 
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i + 768.f, 0.f), sf::Color(0, 0, 0, 40)});
    }
}

void LoginScene::handleInput(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
    if (event->is<sf::Event::TextEntered>()) {
        uint32_t unicode = event->getIf<sf::Event::TextEntered>()->unicode;
        if (unicode == '\b' && !nomeInput.empty()) {
            nomeInput.pop_back();
            SoundManager::getInstance().playSound("click");
        } else if (unicode == '\r' || unicode == '\n') { 
            if (!nomeInput.empty()) {
                SoundManager::getInstance().playSound("click");
                manager->setPlayerName(nomeInput);
                manager->changeScene(std::make_unique<MenuScene>(manager));
                return; // STOP execution because this scene is destroyed
            } 
        } else if (unicode >= 32 && unicode < 128 && nomeInput.size() < 12) {
            nomeInput += static_cast<char>(unicode);
            SoundManager::getInstance().playSound("click");
        }
        testoNomeInput.setString(nomeInput + "_");
    }
}

void LoginScene::update(float deltaTime, sf::RenderWindow& window) {
    // Nessun aggiornamento frame-per-frame necessario
}

void LoginScene::render(sf::RenderWindow& window) {
    window.draw(tramaSfondo);
    window.draw(testoTitoloLogin);
    window.draw(testoIstruzioniLogin);
    window.draw(testoNomeInput);
}
