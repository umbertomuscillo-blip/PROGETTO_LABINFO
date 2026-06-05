#ifndef LOBBYSCENE_H
#define LOBBYSCENE_H

#include "Scene.h"
#include "UIUtils.h"
#include "GestoreRete.h"

class LobbyScene : public Scene {
private:
    sf::Text titoloLobby;
    sf::Text testoIstruzioniIP;
    sf::Text testoIP;
    sf::Text testoAttesa;
    sf::VertexArray tramaSfondo;

    Bottone btnOspita;
    Bottone btnUnisciti;
    Bottone btnIndietro;

    std::string ipInput;
    bool inputIpAttivo;
    bool inAttesaConnessione;
    
    std::shared_ptr<ServerRete> mioServer;
    std::shared_ptr<ClientRete> mioClient;
    bool isServer;

public:
    LobbyScene(SceneManager* mgr);
    
    void handleInput(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
};

#endif
