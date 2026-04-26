#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "Partita.h"

using namespace std;

// --- STATI DEL GIOCO ---
enum StatoGioco { MENU, IN_CORSO, FINE };

// --- FUNZIONI HELPER GRAFICHE ---
void creaBottone(sf::RectangleShape& rect, sf::Text& testo, sf::Font& font, string etichetta, float x, float y, sf::Color colore) {
    rect.setSize({350.f, 80.f});
    rect.setPosition({x, y});
    rect.setFillColor(colore);
    rect.setOutlineThickness(3.f);
    rect.setOutlineColor(sf::Color::White);

    testo.setFont(font);
    testo.setString(etichetta);
    testo.setCharacterSize(22);
    testo.setFillColor(sf::Color::White);
    testo.setPosition({x + 20.f, y + 25.f}); 
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

    sf::Font font;
    if (!font.openFromFile("font.ttf")) {
        cout << "Errore caricamento font.ttf" << endl;
        return -1;
    }

    // --- VARIABILI GLOBALI ---
    StatoGioco statoAttuale = MENU;
    Partita* gioco = nullptr; 
    
    // --- SETUP MENU ---
    sf::Text titolo(font, "UNO FLIP!", 80);
    titolo.setPosition({320.f, 100.f});
    titolo.setFillColor(sf::Color::Yellow);
    titolo.setOutlineThickness(5.f);
    titolo.setOutlineColor(sf::Color::Red);

    sf::RectangleShape btnServer, btnClient;
    // SFML 3: Devono avere il font da subito!
    sf::Text txtServer(font), txtClient(font); 
    creaBottone(btnServer, txtServer, font, "CREA PARTITA (Server)", 337.f, 300.f, sf::Color(20, 100, 20));
    creaBottone(btnClient, txtClient, font, "UNISCITI (Client)", 337.f, 420.f, sf::Color(20, 50, 150));

    // --- SETUP FINE PARTITA ---
    sf::Text testoVincitore(font, "", 50);
    testoVincitore.setFillColor(sf::Color::Yellow);
    testoVincitore.setOutlineThickness(3.f);
    testoVincitore.setOutlineColor(sf::Color::Black);
    
    sf::Text testoClassifica(font, "CLASSIFICA GENERALE:\n", 20);
    testoClassifica.setFillColor(sf::Color::White);
    
    sf::RectangleShape btnTornaMenu;
    // SFML 3: Deve avere il font
    sf::Text txtTornaMenu(font);
    creaBottone(btnTornaMenu, txtTornaMenu, font, "TORNA AL MENU", 337.f, 600.f, sf::Color(200, 30, 30));

    // --- SETUP GIOCO ---
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
    
    sf::Clock timerBot;
    bool botPensando = false, modalitaSceltaColore = false, haDettoUno = false, penalitaInflitta = false;
    int indiceJollyInSospeso = -1;

    vector<sf::RectangleShape> btnColori(4);
    for(int i=0; i<4; i++) {
        btnColori[i].setSize({80.f, 80.f});
        btnColori[i].setPosition({332.f + (i*90.f), 450.f});
        btnColori[i].setOutlineThickness(2.f);
    }

    // --- GAME LOOP ---
    while (window.isOpen())
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);

        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();

            // GESTIONE RESIZE
            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                float windowRatio = (float)resized->size.x / (float)resized->size.y;
                float viewRatio = 1024.f / 768.f;
                float sizeX = 1.f, sizeY = 1.f, posX = 0.f, posY = 0.f;
                if (windowRatio > viewRatio) { sizeX = viewRatio / windowRatio; posX = (1.f - sizeX) / 2.f; }
                else { sizeY = windowRatio / viewRatio; posY = (1.f - sizeY) / 2.f; }
                view.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY}));
                window.setView(view);
            }

            if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
            {
                // 1. CLICK NEL MENU
                if (statoAttuale == MENU) {
                    if (btnServer.getGlobalBounds().contains(mousePos) || btnClient.getGlobalBounds().contains(mousePos)) {
                        vector<Giocatore> giocatori;
                        giocatori.push_back(Giocatore("Umberto", false));
                        giocatori.push_back(Giocatore("Prof. Vessio", true));
                        gioco = new Partita(giocatori);
                        gioco->setupIniziale();
                        
                        modalitaSceltaColore = false; haDettoUno = false; penalitaInflitta = false; botPensando = false;
                        statoAttuale = IN_CORSO;
                        cout << "[SISTEMA] Partita Iniziata!" << endl;
                    }
                }
                // 2. CLICK IN GIOCO
                else if (statoAttuale == IN_CORSO && gioco != nullptr) {
                    int turno = gioco->getTurnoCorrente();
                    bool latoOscuro = gioco->getLatoOscuroAttivo();
                    vector<Carta> miaMano = gioco->getGiocatori()[0].getMano();

                    if (modalitaSceltaColore) {
                        for(int c=0; c<4; c++) {
                            if (btnColori[c].getGlobalBounds().contains(mousePos)) {
                                gioco->mossaUmano(indiceJollyInSospeso, c, haDettoUno); 
                                modalitaSceltaColore = false; indiceJollyInSospeso = -1; haDettoUno = false; 
                                btnUno.setFillColor(sf::Color::Red);
                            }
                        }
                    } else if (turno == 0 && !gioco->partitaTerminata()) {
                        
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
                            // SFML 3: FloatRect usa {{x, y}, {width, height}}
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
                // 3. CLICK FINE PARTITA
                else if (statoAttuale == FINE) {
                    if (btnTornaMenu.getGlobalBounds().contains(mousePos)) {
                        delete gioco; gioco = nullptr;
                        statoAttuale = MENU;
                    }
                }
            }
        }

        // --- UPDATE LOGICA BOT & CHECK VITTORIA ---
        if (statoAttuale == IN_CORSO && gioco != nullptr) {
            if (gioco->partitaTerminata()) {
                statoAttuale = FINE;
                testoVincitore.setString("VINCITORE: " + gioco->getVincitore());
                // SFML 3: getGlobalBounds().size.x
                testoVincitore.setPosition({(1024.f - testoVincitore.getGlobalBounds().size.x) / 2.f, 150.f}); 
                
                string classificaStr = "CLASSIFICA (dal Database):\n";
                ifstream file("classifica.csv");
                if (file.is_open()) {
                    string riga;
                    while(getline(file, riga)) classificaStr += riga + "\n";
                    file.close();
                } else {
                    classificaStr += "Nessun dato (Gioca per riempire il Database!)";
                }
                testoClassifica.setString(classificaStr);
                testoClassifica.setPosition({337.f, 250.f});
            }
            else if (gioco->getTurnoCorrente() == 1 && !modalitaSceltaColore) {
                if (!botPensando) { timerBot.restart(); botPensando = true; } 
                else if (timerBot.getElapsedTime().asSeconds() > 1.5f) { 
                    gioco->mossaBot(); botPensando = false;
                }
            }
        }

        // --- DISEGNO ---
        window.clear(sf::Color(20, 20, 20));

        if (statoAttuale == MENU) {
            window.draw(titolo);
            window.draw(btnServer); window.draw(txtServer);
            window.draw(btnClient); window.draw(txtClient);
        }
        else if (statoAttuale == IN_CORSO && gioco != nullptr) {
            bool latoOscuro = gioco->getLatoOscuroAttivo();
            vector<Carta> miaMano = gioco->getGiocatori()[0].getMano();
            vector<Carta> manoBot = gioco->getGiocatori()[1].getMano(); 

            sfondoTavolo.setFillColor(latoOscuro ? sf::Color(30, 20, 30) : sf::Color(20, 90, 40));
            mazzoGrafico.setFillColor(latoOscuro ? sf::Color(80, 0, 120) : sf::Color(10, 30, 150));
            indicatoreColore.setFillColor(getSFMLColor(gioco->getColoreAttivo()));

            window.draw(sfondoTavolo);    
            window.draw(mazzoOmbra); window.draw(mazzoGrafico); window.draw(testoMazzo);
            
            impostaGraficaCarta(cartaCentroShape, cartaCentroOmbra, testoCartaCentro, gioco->getCartaInCima(), latoOscuro, font, 437.f, 310.f);
            window.draw(cartaCentroOmbra); window.draw(cartaCentroShape); window.draw(testoCartaCentro);
            window.draw(indicatoreColore); window.draw(testoIndicatore);
            
            float maxSpazioMano = 800.f;
            float spaziatura = min(120.f, maxSpazioMano / max(1.f, (float)miaMano.size())); 
            float startX = (1024.f - ((miaMano.size() - 1) * spaziatura + 115.f)) / 2.f;
            for(int i = 0; i < miaMano.size(); i++) {
                sf::RectangleShape rect, ombra; sf::Text testo(font);
                impostaGraficaCarta(rect, ombra, testo, miaMano[i], latoOscuro, font, startX + (i * spaziatura), 600.f);
                window.draw(ombra); window.draw(rect); window.draw(testo);
            }

            float maxSpazioBot = 600.f;
            float spaziaturaBot = min(50.f, maxSpazioBot / max(1.f, (float)manoBot.size()));
            float startXBot = (1024.f - ((manoBot.size() - 1) * spaziaturaBot + 60.f)) / 2.f;
            for(int i = 0; i < manoBot.size(); i++) {
                sf::RectangleShape rect({60.f, 90.f}); 
                rect.setPosition({startXBot + (i * spaziaturaBot), -10.f}); 
                rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White);
                rect.setFillColor(getSFMLColor(manoBot[i].getColore(!latoOscuro)));
                window.draw(rect);
            }
            
            testoBotLog.setString(gioco->getUltimoLogBot());
            if (penalitaInflitta) testoFeedback.setString("Penalita' per non aver detto UNO: +2 Carte!");
            else testoFeedback.setString(gioco->getTurnoCorrente() == 0 ? "E' IL TUO TURNO!" : "Il Bot sta pensando...");
            
            window.draw(testoFeedback); window.draw(testoBotLog);

            if (miaMano.size() == 2) { window.draw(btnUno); window.draw(testoUno); }

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