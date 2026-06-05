#ifndef UIUTILS_H
#define UIUTILS_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Carta.h"

class RoundedRectangleShape : public sf::Shape {
private:
    sf::Vector2f m_size;
    float m_radius;
    unsigned int m_cornerPointCount;
public:
    RoundedRectangleShape(const sf::Vector2f& size = {0, 0}, float radius = 0, unsigned int cornerPointCount = 10);
    void setSize(const sf::Vector2f& size);
    void setCornerRadius(float radius);
    virtual std::size_t getPointCount() const override;
    virtual sf::Vector2f getPoint(std::size_t index) const override;
};

// Struttura Bottone
struct Bottone {
public:
    RoundedRectangleShape rect;
    sf::Text testo;
    sf::Color coloreBase;
    sf::Color coloreHover;
    float scaleAttuale;
    float scaleTarget;

    Bottone(const sf::Font& font);
    void setup(std::string etichetta, float x, float y, float width, float height, sf::Color base, sf::Color hover);
    void aggiorna(sf::Vector2f mousePos, float deltaTime = 0.016f);
    void disegna(sf::RenderWindow& window);
    bool cliccato(sf::Vector2f mousePos);
};

// Sistema di Particelle
struct Particella {
    sf::Vector2f pos;
    sf::Vector2f vel;
    float size;
    float alpha;
    float lifetime;
    sf::Color color;
};

class ParticleSystem {
private:
    std::vector<Particella> particelle;
    sf::RectangleShape rectShape;
public:
    void init(int count, sf::Vector2f area, bool isFireworks = false);
    void update(float deltaTime, bool isFireworks = false);
    void draw(sf::RenderWindow& window);
};

// Funzioni di utilità
std::vector<std::string> splittaStringa(std::string s, std::string delimiter);

struct RigaClassifica {
    std::string nome;
    int vittorie;
    int partite;
};

bool ordinaClassifica(const RigaClassifica& a, const RigaClassifica& b);
std::vector<RigaClassifica> leggiClassificaDati();
void disegnaTabellaClassifica(sf::RenderWindow& window, sf::Font& font, std::vector<RigaClassifica>& dati);

std::string ottieniSimboloBreve(Carta c, bool latoOscuro);
void disegnaCartaFronte(sf::RenderWindow& window, Carta cartaDati, bool latoOscuro, sf::Font& font, float x, float y, float scale = 1.0f);
sf::Color getSFMLColor(Colore c);

// Funzione per disegnare un bell'HUD avatar per l'avversario
void disegnaAvatarAvversario(sf::RenderWindow& window, sf::Font& font, std::string nome, int numCarte, float x, float y, bool turnoSuo);

#endif
