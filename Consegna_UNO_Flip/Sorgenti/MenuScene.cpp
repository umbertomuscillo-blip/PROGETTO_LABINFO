#include "MenuScene.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "LeaderboardScene.h"
#include "LobbyScene.h"
#include "Giocatore.h"
#include "Partita.h"
#include "SoundManager.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

MenuScene::MenuScene(SceneManager* mgr) : Scene(mgr), 
    titoloOmbra(mgr->getFont()), titolo(mgr->getFont()), benvenuto(mgr->getFont()),
    btnGioca2(mgr->getFont()), btnGioca4(mgr->getFont()), btnSimul(mgr->getFont()), 
    btnMulti(mgr->getFont()), btnClassificaMenu(mgr->getFont()), btnCarica(mgr->getFont()),
    spriteSfondo(mgr->getMenuBgTexture()), timeElapsed(0.0f)
{
    sf::Font& font = manager->getFont();
    
    titoloOmbra.setFont(font); titoloOmbra.setString("UNO FLIP!"); titoloOmbra.setCharacterSize(80);
    titoloOmbra.setPosition({325.f, 55.f}); titoloOmbra.setFillColor(sf::Color(0,0,0,180));
    
    titolo.setFont(font); titolo.setString("UNO FLIP!"); titolo.setCharacterSize(80);
    titolo.setPosition({320.f, 50.f}); titolo.setFillColor(sf::Color::Yellow); 
    titolo.setOutlineThickness(4.f); titolo.setOutlineColor(sf::Color::Red);

    benvenuto.setFont(font); benvenuto.setString("Bentornato, " + manager->getPlayerName() + "!");
    benvenuto.setCharacterSize(22); benvenuto.setFillColor(sf::Color(150, 255, 255));
    benvenuto.setPosition({(1024.f - benvenuto.getLocalBounds().size.x)/2.f, 25.f});

    btnGioca2.setup("GIOCA LOCALE (1 VS 1)", 312.f, 180.f, 400.f, 55.f, sf::Color(20, 120, 40, 200), sf::Color(40, 180, 60));
    btnGioca4.setup("GIOCA LOCALE (4 GIOCATORI)", 312.f, 255.f, 400.f, 55.f, sf::Color(20, 80, 160, 200), sf::Color(40, 120, 220));
    btnSimul.setup("SIMULAZIONE (4 BOT)", 312.f, 330.f, 400.f, 55.f, sf::Color(160, 80, 20, 200), sf::Color(220, 120, 40));
    btnMulti.setup("MULTIPLAYER ONLINE", 312.f, 405.f, 400.f, 55.f, sf::Color(160, 30, 30, 200), sf::Color(220, 50, 50));
    btnClassificaMenu.setup("CONSULTA CLASSIFICA GLOBALE", 312.f, 480.f, 400.f, 55.f, sf::Color(100, 20, 160, 200), sf::Color(150, 40, 220));

    std::ifstream in("salvataggio.txt");
    salvataggioDisponibile = in.good();
    in.close();
    if (salvataggioDisponibile) {
        btnCarica.setup("RIPRENDI PARTITA SALVATA", 312.f, 555.f, 400.f, 55.f, sf::Color(50, 180, 50, 200), sf::Color(80, 220, 80));
    } else {
        btnCarica.setup("NESSUN SALVATAGGIO", 312.f, 555.f, 400.f, 55.f, sf::Color(100, 100, 100, 150), sf::Color(100, 100, 100, 150));
    }

    spriteSfondo.setTexture(manager->getMenuBgTexture());
    // Scale image to fit the window (1024x768)
    sf::Vector2u size = manager->getMenuBgTexture().getSize();
    spriteSfondo.setScale({1024.f / size.x, 768.f / size.y});
    spriteSfondo.setColor(sf::Color(255, 255, 255, 180)); // Slightly darkened
    
    particelleSfondo.init(100, {1024.f, 768.f}, false);
}

void MenuScene::handleInput(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
    if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
        sf::Vector2i pixelPos = event->getIf<sf::Event::MouseButtonPressed>()->position;
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
        
        if (btnMulti.cliccato(mousePos)) {
            manager->changeScene(std::make_unique<LobbyScene>(manager));
        }
        else if (btnClassificaMenu.cliccato(mousePos)) {
            SoundManager::getInstance().playSound("click");
            manager->changeScene(std::make_unique<LeaderboardScene>(manager));
        }
        else if (salvataggioDisponibile && btnCarica.cliccato(mousePos)) {
            SoundManager::getInstance().playSound("click");
            auto partitaRiportata = Partita::caricaPartita("salvataggio.txt");
            if (partitaRiportata) {
                auto game = std::make_unique<GameScene>(manager, partitaRiportata->getGiocatori(), false, false, nullptr, nullptr);
                game->setPartita(std::move(partitaRiportata));
                manager->changeScene(std::move(game));
            }
        }    
        else if (btnGioca2.cliccato(mousePos) || btnGioca4.cliccato(mousePos) || btnSimul.cliccato(mousePos)) {
            srand(time(NULL)); 
            vector<Giocatore> giocatori;
            if (btnGioca2.rect.getGlobalBounds().contains(mousePos)) { 
                giocatori.push_back(Giocatore(manager->getPlayerName(), false)); 
                giocatori.push_back(Giocatore("Prof. Vessio", true)); 
            } 
            else if (btnGioca4.rect.getGlobalBounds().contains(mousePos)) { 
                giocatori.push_back(Giocatore(manager->getPlayerName(), false)); 
                giocatori.push_back(Giocatore("Bot Luigi", true)); 
                giocatori.push_back(Giocatore("Prof. Vessio", true)); 
                giocatori.push_back(Giocatore("Bot Mario", true)); 
            }
            else { 
                giocatori.push_back(Giocatore("Bot Alpha", true)); 
                giocatori.push_back(Giocatore("Bot Beta", true)); 
                giocatori.push_back(Giocatore("Bot Gamma", true)); 
                giocatori.push_back(Giocatore("Bot Delta", true)); 
            }
            manager->changeScene(std::make_unique<GameScene>(manager, giocatori, false, false, nullptr, nullptr));
        }
    }
}

void MenuScene::update(float deltaTime, sf::RenderWindow& window) {
    timeElapsed += deltaTime;
    particelleSfondo.update(deltaTime, false);

    float offsetY = sin(timeElapsed * 2.5f) * 8.0f;
    titolo.setPosition({(1024.f - titolo.getLocalBounds().size.x) / 2.f, 80.f + offsetY});
    titoloOmbra.setPosition({(1024.f - titoloOmbra.getLocalBounds().size.x) / 2.f + 4.f, 84.f + offsetY});

    sf::Vector2i pixelPos = sf::Mouse::getPosition(window); 
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
    btnGioca2.aggiorna(mousePos, deltaTime);
    btnGioca4.aggiorna(mousePos, deltaTime);
    btnSimul.aggiorna(mousePos, deltaTime);
    btnMulti.aggiorna(mousePos, deltaTime);
    btnClassificaMenu.aggiorna(mousePos, deltaTime);
    if (salvataggioDisponibile) btnCarica.aggiorna(mousePos, deltaTime);
}

void MenuScene::render(sf::RenderWindow& window) {
    window.draw(spriteSfondo);
    particelleSfondo.draw(window);

    window.draw(titoloOmbra);
    window.draw(titolo);
    window.draw(benvenuto);
    btnGioca2.disegna(window);
    btnGioca4.disegna(window);
    btnSimul.disegna(window);
    btnMulti.disegna(window);
    btnClassificaMenu.disegna(window);
    btnCarica.disegna(window);
}
