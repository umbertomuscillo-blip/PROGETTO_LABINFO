#ifndef ENDGAMESCENE_H
#define ENDGAMESCENE_H

#include "Scene.h"
#include "UIUtils.h"
#include <string>
#include <vector>

class EndGameScene : public Scene {
private:
    sf::Text testoVincitore;
    sf::RectangleShape pannelloClassifica;
    sf::RectangleShape pannelloOmbra;
    sf::Text testoTitoloClassifica;
    Bottone btnTornaMenu;
    sf::VertexArray tramaSfondo;
    ParticleSystem fireworks;
    std::vector<RigaClassifica> datiClassificaRecenti;

public:
    EndGameScene(SceneManager* mgr, const std::string& vincitore);
    
    void handleInput(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
};

#endif
