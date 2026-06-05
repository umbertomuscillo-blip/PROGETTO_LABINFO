#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "UIUtils.h"
#include "Partita.h"
#include "GestoreRete.h"
#include <memory>
#include <vector>
#include <string>

struct CardAnim {
    Carta carta;
    sf::Vector2f startPos;
    sf::Vector2f currentPos;
    sf::Vector2f endPos;
    float time;
    float duration;
    bool active;
};

class GameScene : public Scene {
private:
    std::unique_ptr<Partita> gioco;
    
    bool isMultiplayer;
    bool isServer;
    std::shared_ptr<ServerRete> mioServer;
    std::shared_ptr<ClientRete> mioClient;
    int idLocale;
    bool isSimulazione;

    sf::RectangleShape sfondoPrincipale;
    sf::RectangleShape tavoloCentrale;
    sf::VertexArray tramaSfondo;

    sf::RectangleShape mazzoGrafico;
    sf::RectangleShape mazzoOmbra;
    sf::RectangleShape bordinoMazzo;
    sf::CircleShape ovaleMazzo;
    sf::Text testoMazzo;
    sf::CircleShape indicatoreColore;
    sf::Text testoIndicatore;
    sf::Text testoFeedback;
    sf::Text testoBotLog;
    sf::Text testoLegenda;

    sf::RectangleShape btnUno;
    sf::Text testoUno;
    std::vector<sf::RectangleShape> btnColori;

    Bottone btnAbbandona;
    Bottone btnSalva;
    Bottone btnContesta;

    float speedMultiplier;
    std::vector<sf::RectangleShape> btnVelocita;
    std::vector<sf::Text> txtVelocita;

    sf::Clock timerBot;
    sf::Clock timerAvviso;
    bool avvisoInCorso;
    bool botPensando;
    bool modalitaSceltaColore;
    bool haDettoUno;
    bool penalitaInflitta;
    int indiceJollyInSospeso;
    float tempoAttesaCalcolato;
    int idDimenticone;

    sf::RectangleShape overlayScurito;
    sf::Text testoPopUp;

    bool animazioneUnoAttiva;
    sf::Clock timerAnimazioneUno;
    sf::Text testoAnimazioneUno;
    
    // Animazioni
    std::vector<CardAnim> animazioni;

    void handleMultiplayerMessages();
    void updateBotLogic();
    bool checkGameEnd();
    void addCardAnimation(Carta c, sf::Vector2f start, sf::Vector2f end, float duration = 0.3f);

public:
    GameScene(SceneManager* mgr, std::vector<Giocatore> giocatori, bool multi, bool server, 
              std::shared_ptr<ServerRete> srv, std::shared_ptr<ClientRete> clt);
    ~GameScene();
    
    void setPartita(std::unique_ptr<Partita> p) { gioco = std::move(p); }
    
    void handleInput(const std::optional<sf::Event>& event, sf::RenderWindow& window) override;
    void update(float deltaTime, sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
};

#endif
