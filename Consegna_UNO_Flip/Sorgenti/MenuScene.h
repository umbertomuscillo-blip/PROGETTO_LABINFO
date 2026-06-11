#ifndef MENUSCENE_H
#define MENUSCENE_H

#include "Scene.h"
#include "UIUtils.h"

class MenuScene : public Scene {
private:
    sf::Text titoloOmbra;
    sf::Text titolo;
    sf::Text benvenuto;
    sf::Sprite spriteSfondo;
    ParticleSystem particelleSfondo;
    float timeElapsed;

    Bottone btnGioca2;
    Bottone btnGioca4;
    Bottone btnSimul;
    Bottone btnMulti;
    Bottone btnClassificaMenu;
    Bottone btnCarica;
    bool salvataggioDisponibile;

public:
    MenuScene(SceneManager* mgr);
    
    void handleInput(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
};

#endif
