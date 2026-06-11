#include <SFML/Graphics.hpp>
#include <optional> // Obbligatorio in SFML 3 per gli eventi

int main()
{
    // SFML 3: Ora richiede le parentesi graffe {} per le dimensioni (Vector2u)
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Test Grafico UNO Flip!");

    // Crea un cerchio verde
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    // SFML 3: Anche le posizioni richiedono le parentesi graffe {} (Vector2f)
    shape.setPosition({350.f, 250.f});

    // Il loop principale del gioco
    while (window.isOpen())
    {
        // SFML 3: Il nuovo e più sicuro modo di leggere gli eventi
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            // Controlla se hai cliccato la X per chiudere la finestra
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();          // Pulisce lo schermo di nero
        window.draw(shape);      // Disegna il cerchio
        window.display();        // Mostra tutto a schermo
    }

    return 0;
}