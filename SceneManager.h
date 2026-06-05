#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include <memory>
#include <string>

class SceneManager {
private:
    std::unique_ptr<Scene> currentScene;
    
    // Globals shared across scenes
    std::string playerName;
    sf::Font globalFont;
    float fadeAlpha;

    sf::Texture menuBgTexture;
    sf::Texture tableBgTexture;
    sf::Texture cardBackTexture;

public:
    SceneManager();
    ~SceneManager();

    bool initialize();
    void changeScene(std::unique_ptr<Scene> newScene);
    
    void handleInput(const std::optional<sf::Event>& event, sf::RenderWindow& window);
    void update(float deltaTime, sf::RenderWindow& window);
    void render(sf::RenderWindow& window);

    // Shared data accessors
    void setPlayerName(const std::string& name) { playerName = name; }
    std::string getPlayerName() const { return playerName; }
    sf::Font& getFont() { return globalFont; }
    sf::Texture& getMenuBgTexture() { return menuBgTexture; }
    sf::Texture& getTableBgTexture() { return tableBgTexture; }
    sf::Texture& getCardBackTexture() { return cardBackTexture; }
};

#endif
