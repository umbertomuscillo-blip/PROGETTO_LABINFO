/**
 * @file main_grafico.cpp
 * @brief Punto di ingresso (Entry Point) dell'applicazione grafica UNO Flip.
 */

#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "Partita.h"
#include "Database.h" 

using namespace std;

enum StatoGioco { LOGIN, MENU, IN_CORSO, FINE };

void creaBottone(sf::RectangleShape& rect, sf::Text& testo, sf::Font& font, string etichetta, float x, float y, sf::Color colore) {
    rect.setSize({350.f, 60.f}); 
    rect.setPosition({x, y});
    rect.setFillColor(colore);
    rect.setOutlineThickness(3.f);
    rect.setOutlineColor(sf::Color::White);

    testo.setFont(font);
    testo.setString(etichetta);
    testo.setCharacterSize(20);
    testo.setFillColor(sf::Color::White);
    testo.setPosition({x + 20.f, y + 15.f}); 
}

void impostaGraficaCarta(sf::RectangleShape& rect, sf::RectangleShape& ombra, sf::Text& testo, Carta cartaDati, bool latoOscuro, sf::Font& font, float x, float y) {
    ombra.setSize({115.f, 150.f});
    ombra.setPosition({x + 6.f, y + 6.f});
    ombra.setFillColor(sf::Color(0, 0, 0, 100)); 
    rect.setSize({115.f, 150.f});
    rect.setPosition({x, y});
    
    string desc = latoOscuro ? cartaDati.getDescrizioneOscura() : cartaDati.getDescrizioneChiara();
    sf::Color bgColor = sf::Color(30, 30, 30); 
    if (desc.find("Rosso") != string::npos) bgColor = sf::Color(220, 20, 20);
    else if (desc.find("Blu") != string::npos) bgColor = sf::Color(20, 50, 220);
    else if (desc.find("Verde Acqua") != string::npos) bgColor = sf::Color(0, 150, 136);
    else if (desc.find("Verde") != string::npos) bgColor = sf::Color(20, 160, 20);
    else if (desc.find("Giallo") != string::npos) bgColor = sf::Color(240, 190, 0);
    else if (desc.find("Rosa") != string::npos) bgColor = sf::Color(255, 60, 150);
    else if (desc.find("Arancione") != string::npos) bgColor = sf::Color(255, 100, 0);
    else if (desc.find("Viola") != string::npos) bgColor = sf::Color(100, 20, 150);
    
    rect.setFillColor(bgColor);
    rect.setOutlineThickness(2.f);
    rect.setOutlineColor(sf::Color(255, 255, 255, 200));
    
    testo.setFont(font);
    if (desc == "Jolly (Nessun colore base)") desc = "Jolly";
    if (desc == "Jolly Pesca Due (Nessun colore base)") desc = "Jolly\n+2";
    if (desc == "Jolly Pesca Colore (Nessun colore base)") desc = "Jolly\nColore";
    if (desc.find("Salta Turno") != string::npos) desc.replace(desc.find("Turno"), 5, ""); 
    
    testo.setString(desc);
    testo.setCharacterSize(14);
    testo.setFillColor(sf::Color::White);
    testo.setOutlineColor(sf::Color::Black);
    testo.setOutlineThickness(1.5f);
    testo.setPosition({x + 8.f, y + 40.f}); 
}

sf::Color getSFMLColor(Colore c) {
    switch(c) {
        case ROSSO: return sf::Color(220, 20, 20);
        case BLU: return sf::Color(20, 50, 220);
        case VERDE: return sf::Color(20, 160, 20);
        case GIALLO: return sf::Color(240, 190, 0);
        case ROSA: return sf::Color(255, 60, 150);
        case VERDE_ACQUA: return sf::Color(0, 150, 136);
        case ARANCIONE: return sf::Color(255, 100, 0);
        case VIOLA: return sf::Color(100, 20, 150);
        default: return sf::Color(30, 30, 30); 
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({1024, 768}), "UNO Flip! - Premium Edition");
    sf::View view(sf::FloatRect({0.f, 0.f}, {1024.f, 768.f}));
    window.setView(view);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("font.ttf")) return -1;

    StatoGioco statoAttuale = LOGIN; 
    Partita* gioco = nullptr; 
    
    // --- LOGIN PROFILO ---
    string nomeInput = "";
    sf::Text testoTitoloLogin(font, "CREA O SELEZIONA PROFILO", 50);
    testoTitoloLogin.setPosition({150.f, 150.f});
    testoTitoloLogin.setFillColor(sf::Color::Yellow);
    sf::Text testoIstruzioniLogin(font, "Scrivi il tuo nome e premi INVIO:", 24);
    testoIstruzioniLogin.setPosition({300.f, 300.f});
    sf::Text testoNomeInput(font, "_", 40);
    testoNomeInput.setPosition({350.f, 380.f});
    testoNomeInput.setFillColor(sf::Color::Green);

    // --- MENU PRINCIPALE ---
    sf::Text titolo(font, "UNO FLIP!", 80);
    titolo.setPosition({320.f, 80.f});
    titolo.setFillColor(sf::Color::Yellow);
    titolo.setOutlineThickness(5.f);
    titolo.setOutlineColor(sf::Color::Red);

    sf::RectangleShape btnGioca2, btnGioca4, btnSimulazione;
    sf::Text txtGioca2(font), txtGioca4(font), txtSimulazione(font); 
    creaBottone(btnGioca2, txtGioca2, font, "GIOCA (1 VS 1)", 337.f, 280.f, sf::Color(20, 100, 20));
    creaBottone(btnGioca4, txtGioca4, font, "GIOCA (4 GIOCATORI)", 337.f, 360.f, sf::Color(20, 50, 150));
    creaBottone(btnSimulazione, txtSimulazione, font, "SIMULAZIONE (4 BOT)", 337.f, 440.f, sf::Color(150, 80, 20));

    // --- FINE PARTITA ---
    sf::Text testoVincitore(font, "", 50);
    testoVincitore.setFillColor(sf::Color::Yellow);
    testoVincitore.setOutlineThickness(3.f);
    testoVincitore.setOutlineColor(sf::Color::Black);
    sf::Text testoClassifica(font, "CLASSIFICA GENERALE:\n", 20);
    testoClassifica.setFillColor(sf::Color::White);
    sf::RectangleShape btnTornaMenu;
    sf::Text txtTornaMenu(font);
    creaBottone(btnTornaMenu, txtTornaMenu, font, "TORNA AL MENU", 337.f, 600.f, sf::Color(200, 30, 30));

    // --- GIOCO ---
    sf::RectangleShape sfondoTavolo({1024.f, 768.f});
    sf::RectangleShape mazzoGrafico({115.f, 150.f}), mazzoOmbra({115.f, 150.f});
    mazzoOmbra.setPosition({256.f, 316.f}); mazzoOmbra.setFillColor(sf::Color(0, 0, 0, 100));
    mazzoGrafico.setPosition({250.f, 310.f}); mazzoGrafico.setOutlineThickness(2.f); mazzoGrafico.setOutlineColor(sf::Color::White);
    sf::Text testoMazzo(font, "MAZZO", 14);
    testoMazzo.setFillColor(sf::Color::White); testoMazzo.setPosition({280.f, 370.f});
    
    sf::RectangleShape cartaCentroShape, cartaCentroOmbra;
    sf::Text testoCartaCentro(font);
    
    sf::CircleShape indicatoreColore(25.f);
    indicatoreColore.setPosition({600.f, 360.f}); indicatoreColore.setOutlineThickness(3.f); indicatoreColore.setOutlineColor(sf::Color::White);
    sf::Text testoIndicatore(font, "Colore\nAttuale", 14);
    testoIndicatore.setFillColor(sf::Color::White); testoIndicatore.setPosition({590.f, 310.f});

    sf::Text testoFeedback(font, "", 26);
    testoFeedback.setFillColor(sf::Color::White); testoFeedback.setOutlineThickness(2.f); testoFeedback.setOutlineColor(sf::Color::Black); testoFeedback.setPosition({50.f, 30.f});
    
    sf::Text testoBotLog(font, "", 20);
    testoBotLog.setFillColor(sf::Color(255, 200, 50)); testoBotLog.setOutlineThickness(1.5f); testoBotLog.setOutlineColor(sf::Color::Black); testoBotLog.setPosition({50.f, 70.f});

    sf::RectangleShape btnUno({120.f, 60.f});
    btnUno.setPosition({850.f, 650.f}); btnUno.setFillColor(sf::Color::Red); btnUno.setOutlineThickness(3.f);
    sf::Text testoUno(font, "UNO!", 24); testoUno.setPosition({880.f, 665.f});
    
    vector<sf::RectangleShape> btnColori(4);
    for(int i=0; i<4; i++) {
        btnColori[i].setSize({80.f, 80.f});
        btnColori[i].setPosition({332.f + (i*90.f), 450.f});
        btnColori[i].setOutlineThickness(2.f);
    }
    
    sf::Clock timerBot; 
    sf::Clock timerAvviso;
    bool avvisoInCorso = false;
    bool botPensando = false, modalitaSceltaColore = false, haDettoUno = false, penalitaInflitta = false;
    int indiceJollyInSospeso = -1;
    
    sf::RectangleShape overlayScurito({1024.f, 768.f});
    overlayScurito.setFillColor(sf::Color(0, 0, 0, 180));
    sf::Text testoPopUp(font, "", 34);
    testoPopUp.setFillColor(sf::Color::Yellow);
    testoPopUp.setOutlineColor(sf::Color::Red);
    testoPopUp.setOutlineThickness(3.f);

    while (window.isOpen())
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();

            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                float windowRatio = (float)resized->size.x / (float)resized->size.y;
                float viewRatio = 1024.f / 768.f;
                float sizeX = 1.f, sizeY = 1.f, posX = 0.f, posY = 0.f;
                if (windowRatio > viewRatio) { sizeX = viewRatio / windowRatio; posX = (1.f - sizeX) / 2.f; }
                else { sizeY = windowRatio / viewRatio; posY = (1.f - sizeY) / 2.f; }
                view.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
                window.setView(view);
            }

            if (event->is<sf::Event::TextEntered>()) {
                if (statoAttuale == LOGIN) {
                    uint32_t unicode = event->getIf<sf::Event::TextEntered>()->unicode;
                    if (unicode == '\b' && !nomeInput.empty()) { 
                        nomeInput.pop_back();
                    } else if (unicode == '\r' || unicode == '\n') { 
                        if (!nomeInput.empty()) statoAttuale = MENU;
                    } else if (unicode >= 32 && unicode < 128 && nomeInput.size() < 12) {
                        nomeInput += static_cast<char>(unicode);
                    }
                    testoNomeInput.setString(nomeInput + "_");
                }
            }

            if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
            {
                if (statoAttuale == MENU) {
                    if (btnGioca2.getGlobalBounds().contains(mousePos) || btnGioca4.getGlobalBounds().contains(mousePos) || btnSimulazione.getGlobalBounds().contains(mousePos)) {
                        vector<Giocatore> giocatori;
                        
                        if (btnGioca2.getGlobalBounds().contains(mousePos)) {
                            giocatori.push_back(Giocatore(nomeInput, false)); 
                            giocatori.push_back(Giocatore("Prof. Vessio", true)); 
                        } else if (btnGioca4.getGlobalBounds().contains(mousePos)) {
                            giocatori.push_back(Giocatore(nomeInput, false)); 
                            giocatori.push_back(Giocatore("Bot Luigi", true)); 
                            giocatori.push_back(Giocatore("Prof. Vessio", true)); 
                            giocatori.push_back(Giocatore("Bot Mario", true)); 
                        } else {
                            giocatori.push_back(Giocatore("Bot Alpha", true));
                            giocatori.push_back(Giocatore("Bot Beta", true)); 
                            giocatori.push_back(Giocatore("Bot Gamma", true)); 
                            giocatori.push_back(Giocatore("Bot Delta", true)); 
                        }

                        gioco = new Partita(giocatori); 
                        gioco->setupIniziale();
                        
                        modalitaSceltaColore = false; haDettoUno = false; penalitaInflitta = false; botPensando = false; avvisoInCorso = false;
                        statoAttuale = IN_CORSO;
                    }
                }
                else if (statoAttuale == IN_CORSO && gioco != nullptr) {
                    if (!avvisoInCorso) {
                        int turno = gioco->getTurnoCorrente();
                        bool latoOscuro = gioco->getLatoOscuroAttivo();
                        
                        if (!gioco->getGiocatori()[turno].getIsBot() && !gioco->partitaTerminata()) {
                            vector<Carta> miaMano = gioco->getGiocatori()[turno].getMano();

                            if (modalitaSceltaColore) {
                                for(int c=0; c<4; c++) {
                                    if (btnColori[c].getGlobalBounds().contains(mousePos)) {
                                        gioco->mossaUmano(indiceJollyInSospeso, c, haDettoUno); 
                                        modalitaSceltaColore = false; indiceJollyInSospeso = -1; haDettoUno = false; 
                                        btnUno.setFillColor(sf::Color::Red);
                                    }
                                }
                            } else {
                                if (miaMano.size() == 2 && btnUno.getGlobalBounds().contains(mousePos)) {
                                    haDettoUno = true; btnUno.setFillColor(sf::Color::Green); penalitaInflitta = false;
                                }
                                if (mazzoGrafico.getGlobalBounds().contains(mousePos)) {
                                    gioco->mossaUmano(-1, 0, haDettoUno); 
                                    penalitaInflitta = false;
                                }
                                
                                float maxSpazioMano = 800.f;
                                float spaziatura = min(120.f, maxSpazioMano / max(1.f, (float)miaMano.size())); 
                                float startX = (1024.f - ((miaMano.size() - 1) * spaziatura + 115.f)) / 2.f;

                                for(int i = miaMano.size() - 1; i >= 0; i--) {
                                    sf::FloatRect bounds({startX + (i * spaziatura), 600.f}, {115.f, 150.f});
                                    if (bounds.contains(mousePos)) {
                                        if (gioco->mossaValida(miaMano[i])) {
                                            if (miaMano.size() == 2 && !haDettoUno) penalitaInflitta = true;
                                            else penalitaInflitta = false;

                                            if (miaMano[i].getColore(latoOscuro) == NERO) {
                                                modalitaSceltaColore = true; indiceJollyInSospeso = i;
                                            } else {
                                                gioco->mossaUmano(i, 0, haDettoUno); 
                                                if (miaMano.size() > 2) haDettoUno = false; 
                                                btnUno.setFillColor(sf::Color::Red);
                                            }
                                        }
                                        break; 
                                    }
                                }
                            }
                        }
                    }
                }
                else if (statoAttuale == FINE) {
                    if (btnTornaMenu.getGlobalBounds().contains(mousePos)) {
                        delete gioco; gioco = nullptr; 
                        statoAttuale = MENU; 
                    }
                }
            }
        }

        // --- GESTIONE TRANSIZIONI E AVVISI ---
        if (statoAttuale == IN_CORSO && gioco != nullptr) {
            
            if (gioco->partitaTerminata()) {
                statoAttuale = FINE;
                string vincitore = gioco->getVincitore();
                testoVincitore.setString("VINCITORE: " + vincitore);
                testoVincitore.setPosition({(1024.f - testoVincitore.getGlobalBounds().size.x) / 2.f, 150.f}); 
                
                Database db;
                db.caricaDati();
                vector<string> nomiPartecipanti;
                for (Giocatore g : gioco->getGiocatori()) nomiPartecipanti.push_back(g.getNome());
                
                db.aggiornaStatistiche(nomiPartecipanti, vincitore);
                db.salvaDati(); 
                
                string classificaStr = "CLASSIFICA (Aggiornata!):\n";
                ifstream file("classifica.csv");
                if (file.is_open()) { string riga; while(getline(file, riga)) classificaStr += riga + "\n"; file.close(); }
                testoClassifica.setString(classificaStr);
                testoClassifica.setPosition({337.f, 250.f});
            }
            else if (gioco->getMostraAvviso()) {
                if (!avvisoInCorso) {
                    avvisoInCorso = true;
                    timerAvviso.restart();
                    testoPopUp.setString(gioco->getMessaggioAvviso());
                    testoPopUp.setPosition({(1024.f - testoPopUp.getGlobalBounds().size.x) / 2.f, 350.f});
                }
                else if (timerAvviso.getElapsedTime().asSeconds() > 2.5f) {
                    avvisoInCorso = false;
                    gioco->resetAvviso();
                }
            }
            else if (gioco->getGiocatori()[gioco->getTurnoCorrente()].getIsBot() && !modalitaSceltaColore && !avvisoInCorso) {
                if (!botPensando) { timerBot.restart(); botPensando = true; } 
                else if (timerBot.getElapsedTime().asSeconds() > (gioco->getGiocatori()[0].getIsBot() ? 0.3f : 1.5f)) { 
                    gioco->mossaBot(); botPensando = false;
                }
            }
        }

        // --- DISEGNO ---
        window.clear(sf::Color(20, 20, 20));

        if (statoAttuale == LOGIN) {
            window.draw(testoTitoloLogin);
            window.draw(testoIstruzioniLogin);
            window.draw(testoNomeInput);
        }
        else if (statoAttuale == MENU) {
            window.draw(titolo);
            sf::Text benvenuto(font, "Bentornato, " + nomeInput + "!", 24);
            benvenuto.setFillColor(sf::Color::Cyan);
            benvenuto.setPosition({337.f, 200.f});
            window.draw(benvenuto);
            
            window.draw(btnGioca2); window.draw(txtGioca2);
            window.draw(btnGioca4); window.draw(txtGioca4);
            window.draw(btnSimulazione); window.draw(txtSimulazione);
        }
        else if (statoAttuale == IN_CORSO && gioco != nullptr) {
            bool latoOscuro = gioco->getLatoOscuroAttivo();
            int numGiocatori = gioco->getGiocatori().size();

            sfondoTavolo.setFillColor(latoOscuro ? sf::Color(30, 20, 30) : sf::Color(20, 90, 40));
            mazzoGrafico.setFillColor(latoOscuro ? sf::Color(80, 0, 120) : sf::Color(10, 30, 150));
            indicatoreColore.setFillColor(getSFMLColor(gioco->getColoreAttivo()));

            window.draw(sfondoTavolo);    
            window.draw(mazzoOmbra); window.draw(mazzoGrafico); window.draw(testoMazzo);
            
            impostaGraficaCarta(cartaCentroShape, cartaCentroOmbra, testoCartaCentro, gioco->getCartaInCima(), latoOscuro, font, 437.f, 310.f);
            window.draw(cartaCentroOmbra); window.draw(cartaCentroShape); window.draw(testoCartaCentro);
            window.draw(indicatoreColore); window.draw(testoIndicatore);
            
            // ==========================================
            // DISEGNO DELLE MANI (FINO A 4 GIOCATORI)
            // ==========================================
            
            vector<Carta> manoBasso = gioco->getGiocatori()[0].getMano();
            float maxSpazioMano = 800.f;
            float spaziatura = min(120.f, maxSpazioMano / max(1.f, (float)manoBasso.size())); 
            float startX = (1024.f - ((manoBasso.size() - 1) * spaziatura + 115.f)) / 2.f;
            
            for(int i = 0; i < manoBasso.size(); i++) {
                if (gioco->getGiocatori()[0].getIsBot()) {
                    sf::RectangleShape rect({60.f, 90.f}); 
                    rect.setPosition({startX + (i * spaziatura), 680.f}); 
                    rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White);
                    rect.setFillColor(getSFMLColor(manoBasso[i].getColore(!latoOscuro)));
                    window.draw(rect);
                } else {
                    sf::RectangleShape rect, ombra; sf::Text testo(font);
                    impostaGraficaCarta(rect, ombra, testo, manoBasso[i], latoOscuro, font, startX + (i * spaziatura), 600.f);
                    window.draw(ombra); window.draw(rect); window.draw(testo);
                }
            }

            if (numGiocatori > 1) {
                vector<Carta> mano1 = gioco->getGiocatori()[1].getMano();
                if (numGiocatori == 2) {
                    float maxSpazioAlto = 600.f;
                    float spaziaturaAlto = min(50.f, maxSpazioAlto / max(1.f, (float)mano1.size()));
                    float startXAlto = (1024.f - ((mano1.size() - 1) * spaziaturaAlto + 60.f)) / 2.f;
                    for(int i = 0; i < mano1.size(); i++) {
                        sf::RectangleShape rect({60.f, 90.f}); 
                        rect.setPosition({startXAlto + (i * spaziaturaAlto), -10.f}); 
                        rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White);
                        rect.setFillColor(getSFMLColor(mano1[i].getColore(!latoOscuro)));
                        window.draw(rect);
                    }
                } else {
                    float maxSpazioSx = 400.f;
                    float spaziaturaSx = min(40.f, maxSpazioSx / max(1.f, (float)mano1.size()));
                    float startYSx = (768.f - ((mano1.size() - 1) * spaziaturaSx + 60.f)) / 2.f;
                    for(int i = 0; i < mano1.size(); i++) {
                        sf::RectangleShape rect({60.f, 90.f}); 
                        rect.setOrigin({30.f, 45.f}); 
                        rect.setPosition({80.f, startYSx + (i * spaziaturaSx)}); 
                        rect.setRotation(sf::degrees(90.f)); 
                        rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White);
                        rect.setFillColor(getSFMLColor(mano1[i].getColore(!latoOscuro)));
                        window.draw(rect);
                    }
                }
            }

            if (numGiocatori > 2) {
                vector<Carta> manoAlto = gioco->getGiocatori()[2].getMano();
                float maxSpazioAlto = 600.f;
                float spaziaturaAlto = min(50.f, maxSpazioAlto / max(1.f, (float)manoAlto.size()));
                float startXAlto = (1024.f - ((manoAlto.size() - 1) * spaziaturaAlto + 60.f)) / 2.f;
                for(int i = 0; i < manoAlto.size(); i++) {
                    sf::RectangleShape rect({60.f, 90.f}); 
                    rect.setPosition({startXAlto + (i * spaziaturaAlto), -10.f}); 
                    rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White);
                    rect.setFillColor(getSFMLColor(manoAlto[i].getColore(!latoOscuro)));
                    window.draw(rect);
                }
            }

            if (numGiocatori > 3) {
                vector<Carta> manoDx = gioco->getGiocatori()[3].getMano();
                float maxSpazioDx = 400.f;
                float spaziaturaDx = min(40.f, maxSpazioDx / max(1.f, (float)manoDx.size()));
                float startYDx = (768.f - ((manoDx.size() - 1) * spaziaturaDx + 60.f)) / 2.f;
                for(int i = 0; i < manoDx.size(); i++) {
                    sf::RectangleShape rect({60.f, 90.f}); 
                    rect.setOrigin({30.f, 45.f});
                    rect.setPosition({1024.f - 80.f, startYDx + (i * spaziaturaDx)}); 
                    rect.setRotation(sf::degrees(-90.f)); 
                    rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White);
                    rect.setFillColor(getSFMLColor(manoDx[i].getColore(!latoOscuro)));
                    window.draw(rect);
                }
            }
            
            // ==========================================
            // INDICATORE DI TURNO E SENSO DI MARCIA
            // ==========================================
            
            sf::Text testoSenso(font, gioco->getSensoOrario() ? "GIRO: ORARIO >>" : "<< GIRO: ANTIORARIO", 16);
            testoSenso.setFillColor(gioco->getSensoOrario() ? sf::Color(100, 255, 100) : sf::Color(100, 255, 255));
            testoSenso.setOutlineColor(sf::Color::Black);
            testoSenso.setOutlineThickness(2.f);
            testoSenso.setPosition({440.f, 260.f});
            window.draw(testoSenso);

            sf::CircleShape markerTurno(15.f, 3); // Un triangolo!
            markerTurno.setFillColor(sf::Color::Yellow);
            markerTurno.setOutlineThickness(3.f);
            markerTurno.setOutlineColor(sf::Color::Red);
            markerTurno.setOrigin({15.f, 15.f});

            int turno = gioco->getTurnoCorrente();
            if (turno == 0) {
                markerTurno.setPosition({512.f, 570.f});
                markerTurno.setRotation(sf::degrees(180.f)); // Punta a te
            } else if (numGiocatori == 2 && turno == 1) {
                markerTurno.setPosition({512.f, 110.f});
                markerTurno.setRotation(sf::degrees(0.f));   // Punta al bot sopra
            } else if (numGiocatori == 4) {
                if (turno == 1) {
                    markerTurno.setPosition({150.f, 384.f});
                    markerTurno.setRotation(sf::degrees(-90.f)); // Punta al bot Sx
                } else if (turno == 2) {
                    markerTurno.setPosition({512.f, 110.f});
                    markerTurno.setRotation(sf::degrees(0.f));   // Punta al bot sopra
                } else if (turno == 3) {
                    markerTurno.setPosition({874.f, 384.f});
                    markerTurno.setRotation(sf::degrees(90.f));  // Punta al bot Dx
                }
            }
            window.draw(markerTurno);

            testoBotLog.setString(gioco->getUltimoLogBot());
            if (penalitaInflitta) testoFeedback.setString("Penalita' per non aver detto UNO: +2 Carte!");
            else testoFeedback.setString(gioco->getGiocatori()[gioco->getTurnoCorrente()].getNome() + " sta giocando...");
            
            window.draw(testoFeedback); window.draw(testoBotLog);

            if (!gioco->getGiocatori()[0].getIsBot() && manoBasso.size() == 2) { window.draw(btnUno); window.draw(testoUno); }

            if (modalitaSceltaColore) {
                if (!latoOscuro) {
                    btnColori[0].setFillColor(sf::Color(220, 20, 20)); btnColori[1].setFillColor(sf::Color(240, 190, 0)); 
                    btnColori[2].setFillColor(sf::Color(20, 160, 20)); btnColori[3].setFillColor(sf::Color(20, 50, 220)); 
                } else {
                    btnColori[0].setFillColor(sf::Color(255, 60, 150)); btnColori[1].setFillColor(sf::Color(0, 150, 136)); 
                    btnColori[2].setFillColor(sf::Color(255, 100, 0)); btnColori[3].setFillColor(sf::Color(100, 20, 150)); 
                }
                for(int i=0; i<4; i++) window.draw(btnColori[i]);
            }

            if (avvisoInCorso) {
                window.draw(overlayScurito);
                window.draw(testoPopUp);
            }
        }
        else if (statoAttuale == FINE) {
            window.draw(testoVincitore);
            window.draw(testoClassifica);
            window.draw(btnTornaMenu); window.draw(txtTornaMenu);
        }

        window.display();             
    }

    if (gioco != nullptr) delete gioco;
    return 0;
}