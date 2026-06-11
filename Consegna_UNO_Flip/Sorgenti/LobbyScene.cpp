#include "LobbyScene.h"
#include "SceneManager.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "SoundManager.h"
#include "Mazzo.h"
#include <ctime>

using namespace std;

LobbyScene::LobbyScene(SceneManager* mgr) : Scene(mgr), 
    titoloLobby(mgr->getFont()), testoIstruzioniIP(mgr->getFont()), testoIP(mgr->getFont()), testoAttesa(mgr->getFont()),
    btnOspita(mgr->getFont()), btnUnisciti(mgr->getFont()), btnIndietro(mgr->getFont()), 
    ipInput("127.0.0.1"), inputIpAttivo(false), inAttesaConnessione(false), isServer(false), tramaSfondo(sf::PrimitiveType::Lines) 
{
    mioServer = std::make_shared<ServerRete>();
    mioClient = std::make_shared<ClientRete>();
    
    sf::Font& font = manager->getFont();
    
    titoloLobby.setFont(font); titoloLobby.setString("SALA D'ATTESA MULTIPLAYER"); 
    titoloLobby.setCharacterSize(40); titoloLobby.setPosition({250.f, 100.f}); 
    titoloLobby.setFillColor(sf::Color::Yellow);
    
    btnOspita.setup("OSPITA PARTITA (Server)", 312.f, 250.f, 400.f, 55.f, sf::Color(20, 120, 40, 200), sf::Color(40, 180, 60));
    btnUnisciti.setup("UNISCITI A PARTITA (Client)", 312.f, 325.f, 400.f, 55.f, sf::Color(20, 80, 160, 200), sf::Color(40, 120, 220));
    
    testoIstruzioniIP.setFont(font); testoIstruzioniIP.setString("IP a cui connettersi (se Client):"); 
    testoIstruzioniIP.setCharacterSize(20); testoIstruzioniIP.setPosition({312.f, 410.f});
    
    testoIP.setFont(font); testoIP.setString(ipInput); 
    testoIP.setCharacterSize(30); testoIP.setPosition({312.f, 440.f}); 
    testoIP.setFillColor(sf::Color::Cyan); 
    
    btnIndietro.setup("INDIETRO", 312.f, 650.f, 400.f, 55.f, sf::Color(80, 80, 80, 200), sf::Color(130, 130, 130));

    testoAttesa.setFont(font); testoAttesa.setString("IN ATTESA DELL'AVVERSARIO SULLA PORTA 53000...");
    testoAttesa.setCharacterSize(30); testoAttesa.setPosition({150.f, 300.f});
    testoAttesa.setFillColor(sf::Color::Yellow); testoAttesa.setOutlineThickness(2.f); 
    testoAttesa.setOutlineColor(sf::Color::Black);

    for(int i = -1024; i < 2048; i += 40) {
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i, 0.f), sf::Color(0, 0, 0, 40)}); 
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i + 768.f, 768.f), sf::Color(0, 0, 0, 40)});
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i, 768.f), sf::Color(0, 0, 0, 40)}); 
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i + 768.f, 0.f), sf::Color(0, 0, 0, 40)});
    }
}

void LobbyScene::handleInput(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
    if (event->is<sf::Event::TextEntered>()) {
        if (inputIpAttivo && !inAttesaConnessione) {
            uint32_t unicode = event->getIf<sf::Event::TextEntered>()->unicode;
            if (unicode == '\b' && !ipInput.empty()) {
                ipInput.pop_back();
            } else if ((unicode >= '0' && unicode <= '9') || unicode == '.') { 
                if (ipInput.size() < 15) ipInput += static_cast<char>(unicode); 
            }
            testoIP.setString(ipInput + "_");
        }
    }
    
    if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
        sf::Vector2i pixelPos = event->getIf<sf::Event::MouseButtonPressed>()->position; 
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
        
        if (btnIndietro.cliccato(mousePos)) {
            if (inAttesaConnessione) mioServer->scollega();
            manager->changeScene(std::make_unique<MenuScene>(manager));
            return;
        }
        
        if (!inAttesaConnessione) {
            if (testoIP.getGlobalBounds().contains(mousePos) || testoIstruzioniIP.getGlobalBounds().contains(mousePos)) { 
                inputIpAttivo = true; testoIP.setString(ipInput + "_"); 
            } else { 
                inputIpAttivo = false; testoIP.setString(ipInput); 
            }
            
            if (btnOspita.cliccato(mousePos)) { 
                if (mioServer->preparaServer()) { 
                    isServer = true; inAttesaConnessione = true; 
                } 
            }
            
            if (btnUnisciti.cliccato(mousePos)) {
                if (mioClient->connetti(ipInput)) {
                    isServer = false; 
                    mioClient->inviaMessaggio("NOME|" + manager->getPlayerName()); 
                    bool partitaRicevuta = false;
                    while (!partitaRicevuta && window.isOpen()) {
                        string msg = mioClient->riceviMessaggio();
                        if (msg != "") { 
                            vector<string> dati = splittaStringa(msg, "|");
                            if (dati[0] == "INIZIO") { 
                                srand(stoi(dati[2])); 
                                Mazzo::impostaSeedRete(stoi(dati[2]));
                                vector<Giocatore> giocatori = {Giocatore(dati[1], false), Giocatore(manager->getPlayerName(), false)};
                                
                                manager->changeScene(std::make_unique<GameScene>(manager, giocatori, true, false, nullptr, mioClient));
                                partitaRicevuta = true;
                            }
                        }
                    }
                }
            }
        }
    }
}

void LobbyScene::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window); 
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
    
    btnIndietro.aggiorna(mousePos, deltaTime);
    if (!inAttesaConnessione) {
        btnOspita.aggiorna(mousePos, deltaTime);
        btnUnisciti.aggiorna(mousePos, deltaTime);
    }
    
    if (inAttesaConnessione) {
        if (mioServer->attendiClient()) {
            bool nomeRicevuto = false; string nomeAvversario = "Avversario";
            while (!nomeRicevuto && window.isOpen()) {
                string msg = mioServer->riceviMessaggio();
                if (msg != "") { 
                    vector<string> dati = splittaStringa(msg, "|"); 
                    if (dati[0] == "NOME") { nomeAvversario = dati[1]; nomeRicevuto = true; } 
                }
            }
            int randomSeed = time(NULL); 
            srand(randomSeed); 
            Mazzo::impostaSeedRete(randomSeed);
            mioServer->inviaMessaggio("INIZIO|" + manager->getPlayerName() + "|" + to_string(randomSeed));
            vector<Giocatore> giocatori = {Giocatore(manager->getPlayerName(), false), Giocatore(nomeAvversario, false)};
            
            manager->changeScene(std::make_unique<GameScene>(manager, giocatori, true, true, mioServer, nullptr));
        }
    }
}

void LobbyScene::render(sf::RenderWindow& window) {
    window.draw(tramaSfondo);
    if (inAttesaConnessione) {
        window.draw(testoAttesa);
        btnIndietro.disegna(window);
    } else {
        window.draw(titoloLobby);
        btnOspita.disegna(window);
        btnUnisciti.disegna(window);
        window.draw(testoIstruzioniIP);
        window.draw(testoIP);
        btnIndietro.disegna(window);
    }
}
