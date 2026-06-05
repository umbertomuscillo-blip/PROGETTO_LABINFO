#include "GameScene.h"
#include "SceneManager.h"
#include "MenuScene.h"
#include "EndGameScene.h"
#include "SoundManager.h"
#include "Database.h"
#include <iostream>

using namespace std;

GameScene::GameScene(SceneManager* mgr, vector<Giocatore> giocatori, bool multi, bool server, 
                     std::shared_ptr<ServerRete> srv, std::shared_ptr<ClientRete> clt) 
    : Scene(mgr), isMultiplayer(multi), isServer(server), mioServer(srv), mioClient(clt), 
      btnAbbandona(mgr->getFont()), btnSalva(mgr->getFont()), btnContesta(mgr->getFont()),
      testoIndicatore(mgr->getFont()), testoFeedback(mgr->getFont()), testoBotLog(mgr->getFont()), testoLegenda(mgr->getFont()),
      testoUno(mgr->getFont()), testoPopUp(mgr->getFont()), testoAnimazioneUno(mgr->getFont()),
      testoMazzo(mgr->getFont())
{
    sf::Font& font = manager->getFont();
    gioco = make_unique<Partita>(giocatori);
    gioco->setupIniziale();

    idLocale = isMultiplayer ? (isServer ? 0 : 1) : 0;
    isSimulazione = (giocatori[0].getIsBot());
    
    speedMultiplier = 1.0f;
    avvisoInCorso = false; botPensando = false; modalitaSceltaColore = false; 
    haDettoUno = false; penalitaInflitta = false; indiceJollyInSospeso = -1; 
    tempoAttesaCalcolato = 1.0f; idDimenticone = -1; animazioneUnoAttiva = false;

    btnAbbandona.setup("ABBANDONA", 20.f, 90.f, 150.f, 45.f, sf::Color(100, 50, 50, 200), sf::Color(180, 50, 50));
    btnSalva.setup("SALVA ED ESCI", 190.f, 90.f, 180.f, 45.f, sf::Color(50, 100, 50, 200), sf::Color(50, 180, 50));
    btnContesta.setup("CONTESTA UNO!", 700.f, 400.f, 250.f, 55.f, sf::Color(200, 20, 20, 220), sf::Color(255, 50, 50));

    sfondoPrincipale.setSize({1024.f, 768.f});
    
    tramaSfondo.setPrimitiveType(sf::PrimitiveType::Lines);
    for(int i = -1024; i < 2048; i += 40) {
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i, 0.f), sf::Color(0, 0, 0, 40)});
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i + 768.f, 768.f), sf::Color(0, 0, 0, 40)});
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i, 768.f), sf::Color(0, 0, 0, 40)});
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i + 768.f, 0.f), sf::Color(0, 0, 0, 40)});
    }
    
    // Un tavolo ovale/arrotondato bello grande e proporzionato
    tavoloCentrale.setSize({650.f, 360.f}); 
    tavoloCentrale.setPosition({187.f, 204.f}); 
    tavoloCentrale.setOutlineThickness(8.f); 
    tavoloCentrale.setOutlineColor(sf::Color(0, 0, 0, 150));
    
    mazzoOmbra.setSize({115.f, 150.f}); mazzoOmbra.setPosition({306.f, 316.f}); mazzoOmbra.setFillColor(sf::Color(0, 0, 0, 150)); 
    
    // Invece delle vecchie forme geometriche, usiamo la bellissima texture del retro!
    mazzoGrafico.setSize({115.f, 150.f});
    mazzoGrafico.setPosition({300.f, 310.f});
    mazzoGrafico.setOutlineThickness(2.f); 
    mazzoGrafico.setOutlineColor(sf::Color::White);
    
    bordinoMazzo.setSize({105.f, 140.f});
    bordinoMazzo.setPosition({305.f, 315.f}); 
    bordinoMazzo.setFillColor(sf::Color::Transparent); 
    bordinoMazzo.setOutlineThickness(1.5f); 
    bordinoMazzo.setOutlineColor(sf::Color(255, 255, 255, 120));
    
    ovaleMazzo.setRadius(40.f); 
    ovaleMazzo.setScale({0.85f, 1.2f}); 
    ovaleMazzo.setOrigin({40.f, 40.f}); 
    ovaleMazzo.setPosition({300.f + 115.f/2.f, 310.f + 150.f/2.f}); 
    ovaleMazzo.setRotation(sf::degrees(-20.f)); 
    ovaleMazzo.setOutlineThickness(2.f); 
    ovaleMazzo.setOutlineColor(sf::Color::White);
    
    testoMazzo.setFont(font); 
    testoMazzo.setString("UNO\nFLIP"); 
    testoMazzo.setCharacterSize(20); 
    testoMazzo.setFillColor(sf::Color::Yellow); 
    testoMazzo.setOutlineThickness(2.f); 
    testoMazzo.setOutlineColor(sf::Color::Red);
    sf::FloatRect boundsM = testoMazzo.getLocalBounds(); 
    testoMazzo.setOrigin({boundsM.position.x + boundsM.size.x / 2.f, boundsM.position.y + boundsM.size.y / 2.f}); 
    testoMazzo.setPosition({300.f + 115.f/2.f, 310.f + 150.f/2.f}); 
    testoMazzo.setRotation(sf::degrees(-20.f));
    
    indicatoreColore.setRadius(20.f); indicatoreColore.setPosition({660.f, 320.f}); indicatoreColore.setOutlineThickness(3.f); indicatoreColore.setOutlineColor(sf::Color::White);
    testoIndicatore.setFont(font); testoIndicatore.setString("Colore\nAttuale"); testoIndicatore.setCharacterSize(14); testoIndicatore.setFillColor(sf::Color::White); testoIndicatore.setPosition({652.f, 280.f});
    
    testoFeedback.setFont(font); testoFeedback.setCharacterSize(24); testoFeedback.setFillColor(sf::Color::White); testoFeedback.setOutlineThickness(2.f); testoFeedback.setOutlineColor(sf::Color::Black); testoFeedback.setPosition({20.f, 20.f});
    testoBotLog.setFont(font); testoBotLog.setCharacterSize(18); testoBotLog.setFillColor(sf::Color(255, 200, 50)); testoBotLog.setOutlineThickness(1.5f); testoBotLog.setOutlineColor(sf::Color::Black); testoBotLog.setPosition({20.f, 55.f});
    testoLegenda.setFont(font); testoLegenda.setCharacterSize(14); testoLegenda.setFillColor(sf::Color(200, 200, 200, 220)); testoLegenda.setPosition({20.f, 150.f});
    testoLegenda.setString("LEGENDA CARTE:\nINV = Inverti giro\nSAL = Salta turno\nALL = Salta tutti\nFLP = Flip (Gira tavolo)\n+1/+2/+5 = Pesca carte\nJOL = Jolly\nCOL = Scegli Colore");

    btnUno.setSize({100.f, 50.f}); btnUno.setPosition({900.f, 700.f}); btnUno.setFillColor(sf::Color::Red); btnUno.setOutlineThickness(3.f);
    testoUno.setFont(font); testoUno.setString("UNO!"); testoUno.setCharacterSize(20); testoUno.setPosition({920.f, 712.f});
    
    btnColori.resize(4); 
    for(int i=0; i<4; i++) { 
        btnColori[i].setSize({60.f, 60.f}); 
        btnColori[i].setPosition({372.f + (i*70.f), 480.f}); 
        btnColori[i].setOutlineThickness(2.f); 
    }
    
    btnVelocita.resize(4); string etichetteVelocita[] = {"1x", "2x", "4x", "MAX"};
    for(int i=0; i<4; i++) { 
        btnVelocita[i].setSize({45.f, 30.f}); btnVelocita[i].setPosition({780.f + (i*55.f), 20.f}); btnVelocita[i].setOutlineThickness(2.f); 
        sf::Text tv(font, etichetteVelocita[i], 16); tv.setPosition({788.f + (i*55.f), 25.f});
        txtVelocita.push_back(tv);
    }

    overlayScurito.setSize({1024.f, 768.f}); overlayScurito.setFillColor(sf::Color(0, 0, 0, 180));
    testoPopUp.setFont(font); testoPopUp.setCharacterSize(34); testoPopUp.setFillColor(sf::Color::Yellow); testoPopUp.setOutlineColor(sf::Color::Red); testoPopUp.setOutlineThickness(3.f);

    testoAnimazioneUno.setFont(font); testoAnimazioneUno.setString("UNO!"); testoAnimazioneUno.setCharacterSize(150); testoAnimazioneUno.setFillColor(sf::Color::Yellow); testoAnimazioneUno.setOutlineColor(sf::Color::Red); testoAnimazioneUno.setOutlineThickness(8.f);
    sf::FloatRect boundsUnoAnim = testoAnimazioneUno.getLocalBounds(); testoAnimazioneUno.setOrigin({boundsUnoAnim.position.x + boundsUnoAnim.size.x / 2.f, boundsUnoAnim.position.y + boundsUnoAnim.size.y / 2.f}); testoAnimazioneUno.setPosition({512.f, 384.f});
}

GameScene::~GameScene() {}

void GameScene::addCardAnimation(Carta c, sf::Vector2f start, sf::Vector2f end, float duration) {
    animazioni.push_back({c, start, start, end, 0.0f, duration, true});
}

void GameScene::handleInput(const std::optional<sf::Event>& event, sf::RenderWindow& window) {
    if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
        sf::Vector2i pixelPos = event->getIf<sf::Event::MouseButtonPressed>()->position; 
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
        
        if (isSimulazione) { 
            if (btnVelocita[0].getGlobalBounds().contains(mousePos)) speedMultiplier = 1.0f; 
            else if (btnVelocita[1].getGlobalBounds().contains(mousePos)) speedMultiplier = 2.0f; 
            else if (btnVelocita[2].getGlobalBounds().contains(mousePos)) speedMultiplier = 4.0f; 
            else if (btnVelocita[3].getGlobalBounds().contains(mousePos)) speedMultiplier = 100.0f; 
        }

        if (btnAbbandona.cliccato(mousePos)) {
            SoundManager::getInstance().playSound("click");
            if (isMultiplayer) { isServer ? mioServer->inviaMessaggio("QUIT") : mioClient->inviaMessaggio("QUIT"); }
            manager->changeScene(std::make_unique<MenuScene>(manager));
            return;
        }

        if (btnSalva.cliccato(mousePos)) {
            SoundManager::getInstance().playSound("click");
            if (!isMultiplayer) gioco->salvaPartita("salvataggio.txt"); // Disabilita salvataggio in multiplayer
            if (isMultiplayer) { isServer ? mioServer->inviaMessaggio("QUIT") : mioClient->inviaMessaggio("QUIT"); }
            manager->changeScene(std::make_unique<MenuScene>(manager));
            return;
        }

        if (idDimenticone != -1 && idDimenticone != idLocale && btnContesta.cliccato(mousePos) && !isSimulazione) {
            gioco->assegnaPenalitaUno(idDimenticone);
            SoundManager::getInstance().playSound("errore");
            if (isMultiplayer) { string m = "CONTESTA|" + to_string(idDimenticone); isServer ? mioServer->inviaMessaggio(m) : mioClient->inviaMessaggio(m); }
            idDimenticone = -1;
        }

        if (!avvisoInCorso && !animazioneUnoAttiva) { 
            int turno = gioco->getTurnoCorrente(); bool latoOscuro = gioco->getLatoOscuroAttivo();
            if ((isMultiplayer && turno == idLocale) || (!isMultiplayer && !gioco->getGiocatori()[turno].getIsBot())) {
                vector<Carta> miaMano = gioco->getGiocatori()[turno].getMano();
                if (modalitaSceltaColore) {
                    for(int c=0; c<4; c++) { 
                        if (btnColori[c].getGlobalBounds().contains(mousePos)) { 
                            gioco->mossaUmano(indiceJollyInSospeso, c, haDettoUno); 
                            SoundManager::getInstance().playSound("gioca");
                            addCardAnimation(miaMano[indiceJollyInSospeso], sf::Vector2f(512.f, 600.f), sf::Vector2f(480.f, 310.f));
                            if (isMultiplayer) { string m = "GIOCA|" + to_string(indiceJollyInSospeso) + "|" + to_string(c) + "|" + to_string(haDettoUno); isServer ? mioServer->inviaMessaggio(m) : mioClient->inviaMessaggio(m); }
                            modalitaSceltaColore = false; indiceJollyInSospeso = -1; haDettoUno = false; btnUno.setFillColor(sf::Color::Red); 
                        } 
                    }
                } else {
                    if (miaMano.size() == 2 && btnUno.getGlobalBounds().contains(mousePos) && !haDettoUno) { 
                        haDettoUno = true; btnUno.setFillColor(sf::Color::Green); penalitaInflitta = false; 
                        animazioneUnoAttiva = true; timerAnimazioneUno.restart(); 
                        SoundManager::getInstance().playSound("uno");
                    }
                    
                    if (mazzoGrafico.getGlobalBounds().contains(mousePos)) { 
                        gioco->mossaUmano(-1, 0, haDettoUno); idDimenticone = -1; 
                        SoundManager::getInstance().playSound("pesca");
                        addCardAnimation(gioco->getGiocatori()[turno].getMano().back(), sf::Vector2f(300.f, 310.f), sf::Vector2f(512.f, 600.f));
                        if (isMultiplayer) isServer ? mioServer->inviaMessaggio("PESCA") : mioClient->inviaMessaggio("PESCA");
                    }
                    
                    float spaziatura = min(120.f, 800.f / max(1.f, (float)miaMano.size())); float startX = (1024.f - ((miaMano.size() - 1) * spaziatura + 115.f)) / 2.f;
                    for(int i = miaMano.size() - 1; i >= 0; i--) {
                        sf::FloatRect bounds({startX + (i * spaziatura), 600.f}, {115.f, 150.f});
                        if (bounds.contains(mousePos)) {
                            if (gioco->mossaValida(miaMano[i])) {
                                bool avevaDue = (miaMano.size() == 2);
                                if (miaMano[i].getColore(latoOscuro) == NERO) { modalitaSceltaColore = true; indiceJollyInSospeso = i; } 
                                else { 
                                    gioco->mossaUmano(i, 0, haDettoUno); 
                                    SoundManager::getInstance().playSound("gioca");
                                    addCardAnimation(miaMano[i], sf::Vector2f(startX + (i * spaziatura), 600.f), sf::Vector2f(480.f, 310.f));
                                    if (avevaDue && !haDettoUno) idDimenticone = turno; else idDimenticone = -1;
                                    if (isMultiplayer) { string m = "GIOCA|" + to_string(i) + "|0|" + to_string(haDettoUno); isServer ? mioServer->inviaMessaggio(m) : mioClient->inviaMessaggio(m); }
                                    haDettoUno = false; btnUno.setFillColor(sf::Color::Red); 
                                }
                            } break; 
                        }
                    }
                }
            }
        }
    }
}

void GameScene::handleMultiplayerMessages() {
    if (isMultiplayer && gioco->getTurnoCorrente() != idLocale) {
        string msgRicevuto = isServer ? mioServer->riceviMessaggio() : mioClient->riceviMessaggio();
        if (msgRicevuto != "") {
            vector<string> dati = splittaStringa(msgRicevuto, "|");
            if (dati[0] == "GIOCA") { 
                int tAvv = gioco->getTurnoCorrente(); bool avevaDue = (gioco->getGiocatori()[tAvv].getMano().size() == 2); bool haDettoU = stoi(dati[3]);
                gioco->mossaRete(stoi(dati[1]), stoi(dati[2]), haDettoU); 
                SoundManager::getInstance().playSound("gioca");
                if (avevaDue && !haDettoU) idDimenticone = tAvv; else idDimenticone = -1;
            } 
            else if (dati[0] == "PESCA") { 
                gioco->mossaRete(-1, 0, false); idDimenticone = -1; 
                SoundManager::getInstance().playSound("pesca");
            }
            else if (dati[0] == "QUIT") {
                gioco->forzaFinePartita(gioco->getGiocatori()[idLocale].getNome());
                avvisoInCorso = true; timerAvviso.restart(); testoPopUp.setString("L'AVVERSARIO E' SCAPPATO!");
            }
            else if (dati[0] == "CONTESTA") { 
                gioco->assegnaPenalitaUno(stoi(dati[1])); idDimenticone = -1; 
                SoundManager::getInstance().playSound("errore");
            }
        }
    }
}

void GameScene::updateBotLogic() {
    if (!isMultiplayer && gioco->getGiocatori()[gioco->getTurnoCorrente()].getIsBot() && !modalitaSceltaColore && !avvisoInCorso) {
        if (!botPensando) { 
            timerBot.restart(); botPensando = true; int carteRimaste = gioco->getGiocatori()[gioco->getTurnoCorrente()].getMano().size();
            if (carteRimaste <= 2) tempoAttesaCalcolato = 2.5f + (rand() % 15) / 10.0f; else if (carteRimaste <= 4) tempoAttesaCalcolato = 1.5f + (rand() % 10) / 10.0f; else tempoAttesaCalcolato = 0.6f + (rand() % 6) / 10.0f; tempoAttesaCalcolato = tempoAttesaCalcolato / speedMultiplier;
        } else if (timerBot.getElapsedTime().asSeconds() > tempoAttesaCalcolato) { 
            int cartePrima = gioco->getGiocatori()[gioco->getTurnoCorrente()].getNumeroCarte();
            gioco->mossaBot(); 
            int carteDopo = gioco->getGiocatori()[gioco->getTurnoCorrente()].getNumeroCarte();
            
            if (carteDopo > cartePrima) {
                SoundManager::getInstance().playSound("pesca");
            } else {
                SoundManager::getInstance().playSound("gioca");
            }
            botPensando = false; 
        }
    }
}

bool GameScene::checkGameEnd() {
    if (gioco->partitaTerminata()) {
        Database db; db.caricaDati(); vector<string> nomiPartecipanti; string nomeVincitoreUmano = ""; 
        for (Giocatore g : gioco->getGiocatori()) { 
            if (!g.getIsBot()) { 
                nomiPartecipanti.push_back(g.getNome()); 
                if (g.getNome() == gioco->getVincitore()) nomeVincitoreUmano = g.getNome(); 
            } 
        }
        db.aggiornaStatistiche(nomiPartecipanti, nomeVincitoreUmano); db.salvaDati();
        manager->changeScene(std::make_unique<EndGameScene>(manager, gioco->getVincitore()));
        return true;
    }
    return false;
}

void GameScene::update(float deltaTime, sf::RenderWindow& window) {
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window); 
    sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

    btnAbbandona.aggiorna(mousePos, deltaTime);
    if (!isMultiplayer) btnSalva.aggiorna(mousePos, deltaTime);
    if(idDimenticone != -1 && idDimenticone != idLocale && !isSimulazione) btnContesta.aggiorna(mousePos, deltaTime);

    // Update animations
    for (auto it = animazioni.begin(); it != animazioni.end();) {
        it->time += deltaTime;
        if (it->time >= it->duration) {
            it = animazioni.erase(it);
        } else {
            float t = it->time / it->duration;
            // Simple ease-out
            t = t * (2 - t); 
            it->currentPos = it->startPos + (it->endPos - it->startPos) * t;
            ++it;
        }
    }

    if (animazioneUnoAttiva) {
        if (timerAnimazioneUno.getElapsedTime().asSeconds() >= 1.0f) {
            animazioneUnoAttiva = false;
        }
    }

    if (avvisoInCorso) {
        if (timerAvviso.getElapsedTime().asSeconds() > 2.5f) { 
            avvisoInCorso = false; gioco->resetAvviso(); 
        }
        return; // Pause game logic during alert
    }

    if (checkGameEnd()) return;
    
    if (gioco->getMostraAvviso()) {
        if (!isMultiplayer && speedMultiplier >= 100.f) { 
            gioco->resetAvviso(); 
        } else {
            avvisoInCorso = true; timerAvviso.restart(); 
            testoPopUp.setString(gioco->getMessaggioAvviso()); 
            testoPopUp.setPosition({(1024.f - testoPopUp.getLocalBounds().size.x) / 2.f, 350.f}); 
        }
    }

    handleMultiplayerMessages();
    updateBotLogic();
}

void GameScene::render(sf::RenderWindow& window) {
    bool latoOscuro = gioco->getLatoOscuroAttivo(); int numGiocatori = gioco->getGiocatori().size();
    
    sfondoPrincipale.setFillColor(latoOscuro ? sf::Color(40, 20, 40) : sf::Color(20, 60, 80)); 
    window.draw(sfondoPrincipale); 
    window.draw(tramaSfondo); 
    
    tavoloCentrale.setFillColor(latoOscuro ? sf::Color(25, 10, 25) : sf::Color(20, 100, 40)); 
    window.draw(tavoloCentrale); 
    
    mazzoGrafico.setFillColor(latoOscuro ? sf::Color(80, 0, 120) : sf::Color(10, 30, 150)); 
    ovaleMazzo.setFillColor(latoOscuro ? sf::Color(40, 0, 60) : sf::Color(0, 15, 80));
    
    // Draw deck shadow and graphic
    window.draw(mazzoOmbra); window.draw(mazzoGrafico); 
    window.draw(bordinoMazzo); window.draw(ovaleMazzo); window.draw(testoMazzo);
    
    // Draw current color indicator
    indicatoreColore.setFillColor(getSFMLColor(gioco->getColoreAttivo()));
    
    disegnaCartaFronte(window, gioco->getCartaInCima(), latoOscuro, manager->getFont(), 480.f, 310.f); 
    window.draw(indicatoreColore); window.draw(testoIndicatore);
    
    // Draw Players Hands
    vector<Carta> manoBasso = gioco->getGiocatori()[idLocale].getMano(); 
    float spaziaturaBasso = min(120.f, 800.f / max(1.f, (float)manoBasso.size())); 
    float startXBasso = (1024.f - ((manoBasso.size() - 1) * spaziaturaBasso + 115.f)) / 2.f;
    
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixel);
    
    for(int i = 0; i < manoBasso.size(); i++) { 
        float posX = startXBasso + (i * spaziaturaBasso);
        float posY = 600.f;
        float scale = 1.0f;
        
        // Removed Hearthstone hover
        
        if (isSimulazione) { 
            sf::RectangleShape rect({60.f, 90.f}); rect.setPosition({posX + 27.5f, 650.f}); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); rect.setFillColor(getSFMLColor(manoBasso[i].getColore(!latoOscuro))); window.draw(rect); 
            sf::Text valTxt(manager->getFont(), ottieniSimboloBreve(manoBasso[i], latoOscuro), 22); valTxt.setFillColor(sf::Color::White); valTxt.setOutlineColor(sf::Color::Black); valTxt.setOutlineThickness(2.f); valTxt.setOrigin({valTxt.getLocalBounds().position.x + valTxt.getLocalBounds().size.x / 2.f, valTxt.getLocalBounds().position.y + valTxt.getLocalBounds().size.y / 2.f}); valTxt.setPosition({rect.getPosition().x + 30.f, rect.getPosition().y + 40.f}); window.draw(valTxt); 
        } else {
            disegnaCartaFronte(window, manoBasso[i], latoOscuro, manager->getFont(), posX, posY, scale); 
        }
    }
    
    int turno = gioco->getTurnoCorrente(); 
    
    if (numGiocatori > 1) { 
        if (numGiocatori == 2) { 
            int idAvversario = isMultiplayer ? (isServer ? 1 : 0) : 1; 
            vector<Carta> manoAlto = gioco->getGiocatori()[idAvversario].getMano(); 
            string nomeAlto = gioco->getGiocatori()[idAvversario].getNome();
            if (isSimulazione) {
                float spaziaturaAlto = min(50.f, 600.f / max(1.f, (float)manoAlto.size())); float startXAlto = (1024.f - ((manoAlto.size() - 1) * spaziaturaAlto + 115.f)) / 2.f; 
                for(int i = 0; i < manoAlto.size(); i++) disegnaCartaFronte(window, manoAlto[i], latoOscuro, manager->getFont(), startXAlto + (i * spaziaturaAlto), -30.f); 
            } else { 
                disegnaAvatarAvversario(window, manager->getFont(), nomeAlto, manoAlto.size(), 512.f, 80.f, turno == idAvversario);
            } 
        } else { 
            vector<Carta> manoSx = gioco->getGiocatori()[1].getMano(); 
            string nomeSx = gioco->getGiocatori()[1].getNome();
            if (isSimulazione) { 
                float spaziaturaSx = min(40.f, 400.f / max(1.f, (float)manoSx.size())); float startYSx = (768.f - ((manoSx.size() - 1) * spaziaturaSx + 60.f)) / 2.f; 
                for(int i = 0; i < manoSx.size(); i++) { sf::Transform t; t.translate(sf::Vector2f(80.f, startYSx + (i * spaziaturaSx))); t.rotate(sf::degrees(90.f)); t.translate(sf::Vector2f(-57.f, -75.f)); sf::RectangleShape rect({115.f, 150.f}); rect.setFillColor(getSFMLColor(manoSx[i].getColore(latoOscuro))); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); window.draw(rect, t); sf::Text valTxt(manager->getFont(), ottieniSimboloBreve(manoSx[i], latoOscuro), 22); valTxt.setFillColor(sf::Color::White); valTxt.setOutlineColor(sf::Color::Black); valTxt.setOutlineThickness(2.f); valTxt.setOrigin({valTxt.getLocalBounds().position.x + valTxt.getLocalBounds().size.x/2.f, valTxt.getLocalBounds().position.y + valTxt.getLocalBounds().size.y/2.f}); valTxt.setPosition({80.f, startYSx + (i * spaziaturaSx)}); valTxt.setRotation(sf::degrees(90.f)); window.draw(valTxt); } 
            } else { 
                disegnaAvatarAvversario(window, manager->getFont(), nomeSx, manoSx.size(), 80.f, 384.f, turno == 1);
            } 
        } 
    }
    
    if (numGiocatori > 2) { 
        vector<Carta> manoAlto = gioco->getGiocatori()[2].getMano(); 
        string nomeAlto = gioco->getGiocatori()[2].getNome();
        if (isSimulazione) { 
            float spaziaturaAlto = min(50.f, 600.f / max(1.f, (float)manoAlto.size())); float startXAlto = (1024.f - ((manoAlto.size() - 1) * spaziaturaAlto + 115.f)) / 2.f; 
            for(int i = 0; i < manoAlto.size(); i++) disegnaCartaFronte(window, manoAlto[i], latoOscuro, manager->getFont(), startXAlto + (i * spaziaturaAlto), -30.f); 
        } else { 
            disegnaAvatarAvversario(window, manager->getFont(), nomeAlto, manoAlto.size(), 512.f, 80.f, turno == 2);
        } 
    }
    if (numGiocatori > 3) { 
        vector<Carta> manoDx = gioco->getGiocatori()[3].getMano(); 
        string nomeDx = gioco->getGiocatori()[3].getNome();
        if (isSimulazione) { 
            float spaziaturaDx = min(40.f, 400.f / max(1.f, (float)manoDx.size())); float startYDx = (768.f - ((manoDx.size() - 1) * spaziaturaDx + 60.f)) / 2.f; 
            for(int i = 0; i < manoDx.size(); i++) { sf::Transform t; t.translate(sf::Vector2f(944.f, startYDx + (i * spaziaturaDx))); t.rotate(sf::degrees(-90.f)); t.translate(sf::Vector2f(-57.f, -75.f)); sf::RectangleShape rect({115.f, 150.f}); rect.setFillColor(getSFMLColor(manoDx[i].getColore(latoOscuro))); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); window.draw(rect, t); sf::Text valTxt(manager->getFont(), ottieniSimboloBreve(manoDx[i], latoOscuro), 22); valTxt.setFillColor(sf::Color::White); valTxt.setOutlineColor(sf::Color::Black); valTxt.setOutlineThickness(2.f); valTxt.setOrigin({valTxt.getLocalBounds().position.x + valTxt.getLocalBounds().size.x/2.f, valTxt.getLocalBounds().position.y + valTxt.getLocalBounds().size.y/2.f}); valTxt.setPosition({944.f, startYDx + (i * spaziaturaDx)}); valTxt.setRotation(sf::degrees(-90.f)); window.draw(valTxt); } 
        } else { 
            disegnaAvatarAvversario(window, manager->getFont(), nomeDx, manoDx.size(), 944.f, 384.f, turno == 3);
        } 
    }

    // Draw Animations
    for (const auto& anim : animazioni) {
        disegnaCartaFronte(window, anim.carta, latoOscuro, manager->getFont(), anim.currentPos.x, anim.currentPos.y);
    }
            
    if (isSimulazione) { 
        sf::Text txtL(manager->getFont(), "VELOCITA':", 16); txtL.setPosition({680.f, 25.f}); window.draw(txtL); 
        for(int i=0; i<4; i++) { if ((i==0 && speedMultiplier==1.f) || (i==1 && speedMultiplier==2.f) || (i==2 && speedMultiplier==4.f) || (i==3 && speedMultiplier>=100.f)) btnVelocita[i].setFillColor(sf::Color::Green); else btnVelocita[i].setFillColor(sf::Color(50, 50, 50)); window.draw(btnVelocita[i]); window.draw(txtVelocita[i]); } 
    }
    
    sf::Text testoSenso(manager->getFont(), gioco->getSensoOrario() ? "GIRO: ORARIO >>" : "<< GIRO: ANTIORARIO", 18); 
    testoSenso.setFillColor(gioco->getSensoOrario() ? sf::Color(100, 255, 100) : sf::Color(100, 255, 255)); 
    testoSenso.setOutlineColor(sf::Color::Black); testoSenso.setOutlineThickness(2.f); testoSenso.setPosition({420.f, 240.f}); window.draw(testoSenso);
    
    // Rimuoviamo il vecchio marker del turno perché ora c'è il glow dorato negli avatar!
    if (turno == idLocale) {
        sf::CircleShape markerTurno(15.f, 3); markerTurno.setFillColor(sf::Color::Yellow); markerTurno.setOutlineThickness(3.f); markerTurno.setOutlineColor(sf::Color::Red); markerTurno.setOrigin({15.f, 15.f});
        markerTurno.setPosition({512.f, 570.f}); markerTurno.setRotation(sf::degrees(180.f)); 
        window.draw(markerTurno);
    }
    
    testoBotLog.setString(gioco->getUltimoLogBot()); 
    if (penalitaInflitta) testoFeedback.setString("Penalita' per non aver detto UNO: +2 Carte!"); else { string nomeG = gioco->getGiocatori()[gioco->getTurnoCorrente()].getNome(); if (turno == idLocale) testoFeedback.setString("Tocca a te! Fai la tua mossa."); else { if (tempoAttesaCalcolato > 2.0f && botPensando) testoFeedback.setString(nomeG + " sta pensando intensamente..."); else testoFeedback.setString("In attesa di " + nomeG + "..."); } } 
    window.draw(testoFeedback);
    window.draw(testoBotLog);
    window.draw(testoLegenda);
    
    if (manoBasso.size() == 2 && turno == idLocale) { window.draw(btnUno); window.draw(testoUno); }
    if (modalitaSceltaColore) { 
        if (!latoOscuro) { btnColori[0].setFillColor(sf::Color(220, 20, 20)); btnColori[1].setFillColor(sf::Color(240, 190, 0)); btnColori[2].setFillColor(sf::Color(20, 160, 20)); btnColori[3].setFillColor(sf::Color(20, 50, 220)); } 
        else { btnColori[0].setFillColor(sf::Color(255, 60, 150)); btnColori[1].setFillColor(sf::Color(0, 150, 136)); btnColori[2].setFillColor(sf::Color(255, 100, 0)); btnColori[3].setFillColor(sf::Color(100, 20, 150)); } 
        for(int i=0; i<4; i++) window.draw(btnColori[i]); 
    }
    
    btnAbbandona.disegna(window);
    if (!isMultiplayer) btnSalva.disegna(window); // Mostriamo il salvataggio solo in locale
    if (idDimenticone != -1 && idDimenticone != idLocale && !isSimulazione) btnContesta.disegna(window);

    if (avvisoInCorso) { window.draw(overlayScurito); window.draw(testoPopUp); }
    if (animazioneUnoAttiva) { 
        float elapsed = timerAnimazioneUno.getElapsedTime().asSeconds(); 
        if (elapsed < 1.0f) { 
            window.draw(overlayScurito); float scala = 1.0f + (elapsed * 2.0f); int alpha = 255 - (int)(elapsed * 255.f); 
            testoAnimazioneUno.setScale({scala, scala}); testoAnimazioneUno.setFillColor(sf::Color(255, 255, 0, alpha)); 
            testoAnimazioneUno.setOutlineColor(sf::Color(255, 0, 0, alpha)); window.draw(testoAnimazioneUno); 
        } 
    }
}
