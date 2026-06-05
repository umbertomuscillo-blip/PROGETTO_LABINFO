#ifndef LOGINSCENE_H
#define LOGINSCENE_H

#include "Scene.h"
#include <string>

class LoginScene : public Scene {
private:
    std::string nomeInput;
    sf::Text testoTitoloLogin;
    sf::Text testoIstruzioniLogin;
    sf::Text testoNomeInput;
    sf::VertexArray tramaSfondo;

public:
    LoginScene(SceneManager* mgr);
    
    void handleInput(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
};

#endif
