#ifndef SCENE_H
#define SCENE_H

#include <SFML/Graphics.hpp>
#include <optional>

class SceneManager; // Forward declaration

class Scene {
protected:
    SceneManager* manager;

public:
    Scene(SceneManager* mgr) : manager(mgr) {}
    virtual ~Scene() {}

    virtual void handleInput(const std::optional<sf::Event>& event, sf::RenderWindow& window) = 0;
    virtual void update(float deltaTime, sf::RenderWindow& window) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};

#endif
