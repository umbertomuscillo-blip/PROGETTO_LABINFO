#include <SFML/Graphics.hpp>
#include <iostream>
#include "SceneManager.h"
#include "LoginScene.h"

int main() {
    sf::ContextSettings settings;
    sf::RenderWindow window(sf::VideoMode({1024, 768}), "UNO Flip! - Premium Edition", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(60);

    SceneManager manager;
    if (!manager.initialize()) {
        std::cerr << "Errore inizializzazione SceneManager (font mancante?)" << std::endl;
        return -1;
    }

    // Imposta la scena iniziale
    manager.changeScene(std::make_unique<LoginScene>(&manager));

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (event->is<sf::Event::Resized>()) {
                // Gestione ridimensionamento: Letterboxing per mantenere l'aspect ratio
                auto size = event->getIf<sf::Event::Resized>()->size;
                float windowRatio = size.x / (float)size.y;
                float viewRatio = 1024.f / 768.f;
                float sizeX = 1.f;
                float sizeY = 1.f;
                float posX = 0.f;
                float posY = 0.f;

                if (windowRatio < viewRatio) {
                    sizeX = viewRatio / windowRatio;
                    posX = (1.f - sizeX) / 2.f;
                } else {
                    sizeY = windowRatio / viewRatio;
                    posY = (1.f - sizeY) / 2.f;
                }

                sf::View view({0.f, 0.f}, {1024.f, 768.f});
                view.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
                window.setView(view);
            }
            manager.handleInput(event, window);
        }

        manager.update(deltaTime, window);

        window.clear(sf::Color::Black);
        manager.render(window);
        window.display();
    }

    return 0;
}