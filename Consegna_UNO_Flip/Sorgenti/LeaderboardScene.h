#ifndef LEADERBOARDSCENE_H
#define LEADERBOARDSCENE_H

#include "Scene.h"
#include "UIUtils.h"
#include <vector>

class LeaderboardScene : public Scene {
private:
    sf::RectangleShape pannelloClassifica;
    sf::Text testoTitoloClassifica;
    sf::VertexArray tramaSfondo;
    Bottone btnIndietro;

    std::vector<RigaClassifica> datiClassificaRecenti;

public:
    LeaderboardScene(SceneManager* mgr);
    
    void handleInput(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
};

#endif
