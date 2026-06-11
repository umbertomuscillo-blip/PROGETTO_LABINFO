#include "SceneManager.h"
#include <iostream>

SceneManager::SceneManager() : currentScene(nullptr), playerName("Giocatore1"), fadeAlpha(255.f) {}

SceneManager::~SceneManager() {}

bool SceneManager::initialize() {
    if (!globalFont.openFromFile("font.ttf")) {
        std::cerr << "Errore: Impossibile caricare font.ttf" << std::endl;
        return false;
    }
    menuBgTexture.loadFromFile("menu_bg.png");
    tableBgTexture.loadFromFile("table_bg.png");
    cardBackTexture.loadFromFile("card_back.png");
    
    // Smooth li rende più premium
    menuBgTexture.setSmooth(true);
    tableBgTexture.setSmooth(true);
    cardBackTexture.setSmooth(true);

    return true;
}

void SceneManager::changeScene(std::unique_ptr<Scene> newScene) {
    currentScene = std::move(newScene);
    fadeAlpha = 255.f;
}

void SceneManager::handleInput(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
    if (currentScene) {
        currentScene->handleInput(event, window);
    }
}

void SceneManager::update(float deltaTime, sf::RenderWindow& window) {
    if (currentScene) {
        currentScene->update(deltaTime, window);
    }
    if (fadeAlpha > 0.f) {
        fadeAlpha -= deltaTime * 400.f;
        if (fadeAlpha < 0.f) fadeAlpha = 0.f;
    }
}

void SceneManager::render(sf::RenderWindow& window) {
    if (currentScene) {
        currentScene->render(window);
    }
    if (fadeAlpha > 0.f) {
        sf::RectangleShape fadeRect({(float)window.getSize().x, (float)window.getSize().y});
        fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(fadeAlpha)));
        window.draw(fadeRect);
    }
}
