/**
 * @file main_grafico.cpp
 * @brief Punto di ingresso (Entry Point) dell'applicazione grafica UNO Flip.
 */

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp> 
#include <optional>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib> 
#include <ctime>   
#include "Partita.h"
#include "Database.h" 
#include "GestoreRete.h" 

using namespace std;

enum StatoGioco { LOGIN, MENU, LOBBY, ATTESA_CONNESSIONE, IN_CORSO_RETE, IN_CORSO, FINE };

// --- HELPER PER STRINGHE ---
vector<string> splittaStringa(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token; vector<string> res;
    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}

void creaBottone(sf::RectangleShape& rect, sf::Text& testo, sf::Font& font, string etichetta, float x, float y, sf::Color colore) {
    rect.setSize({350.f, 50.f}); rect.setPosition({x, y}); rect.setFillColor(colore); rect.setOutlineThickness(3.f); rect.setOutlineColor(sf::Color::White);
    testo.setFont(font); testo.setString(etichetta); testo.setCharacterSize(18); testo.setFillColor(sf::Color::White); testo.setPosition({x + 20.f, y + 13.f}); 
}

string ottieniSimboloBreve(Carta c, bool latoOscuro) {
    Valore v = c.getValore(latoOscuro);
    if (v == UNO) return "1"; if (v == DUE) return "2"; if (v == TRE) return "3"; if (v == QUATTRO) return "4"; if (v == CINQUE) return "5";
    if (v == SEI) return "6"; if (v == SETTE) return "7"; if (v == OTTO) return "8"; if (v == NOVE) return "9"; 
    if (v == PESCA_UNO) return "+1"; if (v == PESCA_CINQUE) return "+5";
    if (v == INVERTI) return "INV"; if (v == SALTA || v == SALTA_TUTTI) return "ALT"; if (v == FLIP) return "FLIP";
    if (v == JOLLY) return "J"; if (v == JOLLY_PESCA_DUE) return "J+2"; if (v == JOLLY_PESCA_COLORE) return "J+C";
    return "?";
}

void disegnaCartaFronte(sf::RenderWindow& window, Carta cartaDati, bool latoOscuro, sf::Font& font, float x, float y) {
    string desc = latoOscuro ? cartaDati.getDescrizioneOscura() : cartaDati.getDescrizioneChiara();
    sf::Color bgColor = sf::Color(30, 30, 30); 
    if (desc.find("Rosso") != string::npos) bgColor = sf::Color(220, 20, 20); else if (desc.find("Blu") != string::npos) bgColor = sf::Color(20, 50, 220);
    else if (desc.find("Verde Acqua") != string::npos) bgColor = sf::Color(0, 150, 136); else if (desc.find("Verde") != string::npos) bgColor = sf::Color(20, 160, 20);
    else if (desc.find("Giallo") != string::npos) bgColor = sf::Color(240, 190, 0); else if (desc.find("Rosa") != string::npos) bgColor = sf::Color(255, 60, 150);
    else if (desc.find("Arancione") != string::npos) bgColor = sf::Color(255, 100, 0); else if (desc.find("Viola") != string::npos) bgColor = sf::Color(100, 20, 150);
    
    sf::RectangleShape ombra({115.f, 150.f}); ombra.setPosition({x + 4.f, y + 4.f}); ombra.setFillColor(sf::Color(0, 0, 0, 120)); 
    sf::RectangleShape rect({115.f, 150.f}); rect.setPosition({x, y}); rect.setFillColor(bgColor); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White);
    sf::RectangleShape bordino({105.f, 140.f}); bordino.setPosition({x + 5.f, y + 5.f}); bordino.setFillColor(sf::Color::Transparent); bordino.setOutlineThickness(1.5f); bordino.setOutlineColor(sf::Color(255, 255, 255, 120));
    sf::CircleShape ovale(40.f); ovale.setScale({0.85f, 1.2f}); ovale.setOrigin({40.f, 40.f}); ovale.setPosition({x + 115.f/2.f, y + 150.f/2.f}); ovale.setRotation(sf::degrees(-20.f)); ovale.setFillColor(sf::Color::White);

    string simbolo = ottieniSimboloBreve(cartaDati, latoOscuro);
    int sizeCentro = 42; int sizeAngoli = 18; 
    if (simbolo.length() >= 4) { sizeCentro = 22; sizeAngoli = 12; } else if (simbolo.length() == 3) { sizeCentro = 26; sizeAngoli = 14; } else if (simbolo.length() == 2) { sizeCentro = 34; sizeAngoli = 16; }

    sf::Text txtCentro(font, simbolo, sizeCentro); txtCentro.setFillColor(bgColor == sf::Color(30, 30, 30) ? sf::Color::Black : bgColor);
    sf::FloatRect bounds = txtCentro.getLocalBounds(); txtCentro.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
    txtCentro.setPosition({x + 115.f/2.f, y + 150.f/2.f}); txtCentro.setRotation(sf::degrees(-20.f));
    
    sf::Text txtTL(font, simbolo, sizeAngoli); txtTL.setFillColor(sf::Color::White); txtTL.setOutlineColor(sf::Color::Black); txtTL.setOutlineThickness(1.2f);
    sf::FloatRect boundsTL = txtTL.getLocalBounds(); txtTL.setOrigin({boundsTL.position.x + boundsTL.size.x / 2.f, boundsTL.position.y + boundsTL.size.y / 2.f}); txtTL.setPosition({x + 16.f, y + 16.f}); 

    sf::Text txtBR(font, simbolo, sizeAngoli); txtBR.setFillColor(sf::Color::White); txtBR.setOutlineColor(sf::Color::Black); txtBR.setOutlineThickness(1.2f);
    sf::FloatRect boundsBR = txtBR.getLocalBounds(); txtBR.setOrigin({boundsBR.position.x + boundsBR.size.x / 2.f, boundsBR.position.y + boundsBR.size.y / 2.f});
    txtBR.setPosition({x + 115.f - 16.f, y + 150.f - 16.f}); txtBR.setRotation(sf::degrees(180.f));

    window.draw(ombra); window.draw(rect); window.draw(bordino); window.draw(ovale); window.draw(txtCentro); window.draw(txtTL); window.draw(txtBR);
}

sf::Color getSFMLColor(Colore c) {
    switch(c) {
        case ROSSO: return sf::Color(220, 20, 20); case BLU: return sf::Color(20, 50, 220); case VERDE: return sf::Color(20, 160, 20); case GIALLO: return sf::Color(240, 190, 0);
        case ROSA: return sf::Color(255, 60, 150); case VERDE_ACQUA: return sf::Color(0, 150, 136); case ARANCIONE: return sf::Color(255, 100, 0); case VIOLA: return sf::Color(100, 20, 150);
        default: return sf::Color(30, 30, 30); 
    }
}

int main()
{
    // NO SRAND INIZIALE. Lo gestiremo quando si avvia la partita.

    sf::RenderWindow window(sf::VideoMode({1024, 768}), "UNO Flip! - Multiplayer Network");
    sf::View view(sf::FloatRect({0.f, 0.f}, {1024.f, 768.f})); window.setView(view); window.setFramerateLimit(60);

    sf::Font font; if (!font.openFromFile("font.ttf")) return -1;

    StatoGioco statoAttuale = LOGIN; 
    Partita* gioco = nullptr; 
    
    ServerRete mioServer; ClientRete mioClient;
    bool isServer = false; 

    sf::VertexArray tramaSfondo(sf::PrimitiveType::Lines);
    for(int i = -1024; i < 2048; i += 40) {
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i, 0.f), sf::Color(0, 0, 0, 40)});
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i + 768.f, 768.f), sf::Color(0, 0, 0, 40)});
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i, 768.f), sf::Color(0, 0, 0, 40)});
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i + 768.f, 0.f), sf::Color(0, 0, 0, 40)});
    }

    string nomeInput = "";
    sf::Text testoTitoloLogin(font, "CREA O SELEZIONA PROFILO", 50); testoTitoloLogin.setPosition({150.f, 150.f}); testoTitoloLogin.setFillColor(sf::Color::Yellow);
    sf::Text testoIstruzioniLogin(font, "Scrivi il tuo nome e premi INVIO:", 24); testoIstruzioniLogin.setPosition({300.f, 300.f});
    sf::Text testoNomeInput(font, "_", 40); testoNomeInput.setPosition({350.f, 380.f}); testoNomeInput.setFillColor(sf::Color::Green);

    sf::Text titolo(font, "UNO FLIP!", 80); titolo.setPosition({320.f, 80.f}); titolo.setFillColor(sf::Color::Yellow); titolo.setOutlineThickness(5.f); titolo.setOutlineColor(sf::Color::Red);

    sf::RectangleShape btnGioca2, btnGioca4, btnSimul, btnMulti; sf::Text txtGioca2(font), txtGioca4(font), txtSimul(font), txtMulti(font); 
    creaBottone(btnGioca2, txtGioca2, font, "GIOCA LOCALE (1 VS 1)", 337.f, 220.f, sf::Color(20, 100, 20));
    creaBottone(btnGioca4, txtGioca4, font, "GIOCA LOCALE (4 GIOCATORI)", 337.f, 290.f, sf::Color(20, 50, 150));
    creaBottone(btnSimul, txtSimul, font, "SIMULAZIONE (4 BOT)", 337.f, 360.f, sf::Color(150, 80, 20));
    creaBottone(btnMulti, txtMulti, font, "MULTIPLAYER ONLINE", 337.f, 430.f, sf::Color(150, 20, 20));

    sf::Text titoloLobby(font, "SALA D'ATTESA MULTIPLAYER", 40); titoloLobby.setPosition({250.f, 100.f}); titoloLobby.setFillColor(sf::Color::Yellow);
    sf::RectangleShape btnOspita, btnUnisciti; sf::Text txtOspita(font), txtUnisciti(font);
    creaBottone(btnOspita, txtOspita, font, "OSPITA PARTITA (Server)", 337.f, 250.f, sf::Color(20, 100, 20));
    creaBottone(btnUnisciti, txtUnisciti, font, "UNISCITI A PARTITA (Client)", 337.f, 320.f, sf::Color(20, 50, 150));
    string ipInput = "127.0.0.1"; sf::Text testoIstruzioniIP(font, "IP a cui connettersi (se Client):", 20); testoIstruzioniIP.setPosition({337.f, 400.f});
    sf::Text testoIP(font, ipInput, 30); testoIP.setPosition({337.f, 430.f}); testoIP.setFillColor(sf::Color::Cyan); bool inputIpAttivo = false; 
    sf::RectangleShape btnIndietro; sf::Text txtIndietro(font); creaBottone(btnIndietro, txtIndietro, font, "INDIETRO", 337.f, 600.f, sf::Color(100, 100, 100));

    sf::Text testoVincitore(font, "", 50); testoVincitore.setFillColor(sf::Color::Yellow); testoVincitore.setOutlineThickness(3.f); testoVincitore.setOutlineColor(sf::Color::Black);
    sf::Text testoClassifica(font, "CLASSIFICA GENERALE:\n", 20); testoClassifica.setFillColor(sf::Color::White);
    sf::RectangleShape btnTornaMenu; sf::Text txtTornaMenu(font); creaBottone(btnTornaMenu, txtTornaMenu, font, "TORNA AL MENU", 337.f, 600.f, sf::Color(200, 30, 30));

    sf::RectangleShape sfondoPrincipale({1024.f, 768.f}); sf::RectangleShape tavoloCentrale({600.f, 320.f}); tavoloCentrale.setPosition({212.f, 224.f}); tavoloCentrale.setOutlineThickness(5.f); tavoloCentrale.setOutlineColor(sf::Color(0, 0, 0, 150));
    sf::RectangleShape mazzoGrafico({115.f, 150.f}), mazzoOmbra({115.f, 150.f}); mazzoOmbra.setPosition({306.f, 316.f}); mazzoOmbra.setFillColor(sf::Color(0, 0, 0, 100)); mazzoGrafico.setPosition({300.f, 310.f}); mazzoGrafico.setOutlineThickness(2.f); mazzoGrafico.setOutlineColor(sf::Color::White);
    sf::RectangleShape bordinoMazzo({105.f, 140.f}); bordinoMazzo.setPosition({305.f, 315.f}); bordinoMazzo.setFillColor(sf::Color::Transparent); bordinoMazzo.setOutlineThickness(1.5f); bordinoMazzo.setOutlineColor(sf::Color(255, 255, 255, 120));
    sf::CircleShape ovaleMazzo(40.f); ovaleMazzo.setScale({0.85f, 1.2f}); ovaleMazzo.setOrigin({40.f, 40.f}); ovaleMazzo.setPosition({300.f + 115.f/2.f, 310.f + 150.f/2.f}); ovaleMazzo.setRotation(sf::degrees(-20.f)); ovaleMazzo.setOutlineThickness(2.f); ovaleMazzo.setOutlineColor(sf::Color::White);
    sf::Text testoMazzo(font, "UNO\nFLIP", 20); testoMazzo.setFillColor(sf::Color::Yellow); testoMazzo.setOutlineThickness(2.f); testoMazzo.setOutlineColor(sf::Color::Red);
    sf::FloatRect boundsM = testoMazzo.getLocalBounds(); testoMazzo.setOrigin({boundsM.position.x + boundsM.size.x / 2.f, boundsM.position.y + boundsM.size.y / 2.f}); testoMazzo.setPosition({300.f + 115.f/2.f, 310.f + 150.f/2.f}); testoMazzo.setRotation(sf::degrees(-20.f));
    
    sf::CircleShape indicatoreColore(20.f); indicatoreColore.setPosition({660.f, 320.f}); indicatoreColore.setOutlineThickness(3.f); indicatoreColore.setOutlineColor(sf::Color::White);
    sf::Text testoIndicatore(font, "Colore\nAttuale", 14); testoIndicatore.setFillColor(sf::Color::White); testoIndicatore.setPosition({652.f, 280.f});
    sf::Text testoFeedback(font, "", 24); testoFeedback.setFillColor(sf::Color::White); testoFeedback.setOutlineThickness(2.f); testoFeedback.setOutlineColor(sf::Color::Black); testoFeedback.setPosition({20.f, 20.f});
    sf::Text testoBotLog(font, "", 18); testoBotLog.setFillColor(sf::Color(255, 200, 50)); testoBotLog.setOutlineThickness(1.5f); testoBotLog.setOutlineColor(sf::Color::Black); testoBotLog.setPosition({20.f, 55.f});

    sf::RectangleShape btnUno({100.f, 50.f}); btnUno.setPosition({900.f, 700.f}); btnUno.setFillColor(sf::Color::Red); btnUno.setOutlineThickness(3.f);
    sf::Text testoUno(font, "UNO!", 20); testoUno.setPosition({920.f, 712.f});
    vector<sf::RectangleShape> btnColori(4); for(int i=0; i<4; i++) { btnColori[i].setSize({60.f, 60.f}); btnColori[i].setPosition({372.f + (i*70.f), 480.f}); btnColori[i].setOutlineThickness(2.f); }
    
    float speedMultiplier = 1.0f;
    vector<sf::RectangleShape> btnVelocita(4); vector<sf::Text> txtVelocita(4, sf::Text(font, "", 16)); string etichetteVelocita[] = {"1x", "2x", "4x", "MAX"};
    for(int i=0; i<4; i++) { btnVelocita[i].setSize({45.f, 30.f}); btnVelocita[i].setPosition({780.f + (i*55.f), 20.f}); btnVelocita[i].setOutlineThickness(2.f); txtVelocita[i].setString(etichetteVelocita[i]); txtVelocita[i].setPosition({788.f + (i*55.f), 25.f}); }

    sf::Clock timerBot; sf::Clock timerAvviso; bool avvisoInCorso = false; bool botPensando = false, modalitaSceltaColore = false, haDettoUno = false, penalitaInflitta = false; int indiceJollyInSospeso = -1; float tempoAttesaCalcolato = 1.0f; 
    sf::RectangleShape overlayScurito({1024.f, 768.f}); overlayScurito.setFillColor(sf::Color(0, 0, 0, 180));
    sf::Text testoPopUp(font, "", 34); testoPopUp.setFillColor(sf::Color::Yellow); testoPopUp.setOutlineColor(sf::Color::Red); testoPopUp.setOutlineThickness(3.f);

    bool animazioneUnoAttiva = false; sf::Clock timerAnimazioneUno; sf::Text testoAnimazioneUno(font, "UNO!", 150); testoAnimazioneUno.setFillColor(sf::Color::Yellow); testoAnimazioneUno.setOutlineColor(sf::Color::Red); testoAnimazioneUno.setOutlineThickness(8.f);
    sf::FloatRect boundsUnoAnim = testoAnimazioneUno.getLocalBounds(); testoAnimazioneUno.setOrigin({boundsUnoAnim.position.x + boundsUnoAnim.size.x / 2.f, boundsUnoAnim.position.y + boundsUnoAnim.size.y / 2.f}); testoAnimazioneUno.setPosition({512.f, 384.f});

    while (window.isOpen())
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
        
        bool isSimulazione = (gioco != nullptr && gioco->getGiocatori()[0].getIsBot() && statoAttuale == IN_CORSO); 
        bool isMultiplayer = (statoAttuale == IN_CORSO_RETE);
        int idLocale = 0; if (isMultiplayer) idLocale = isServer ? 0 : 1;

        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();
            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                float windowRatio = (float)resized->size.x / (float)resized->size.y; float viewRatio = 1024.f / 768.f; float sizeX = 1.f, sizeY = 1.f, posX = 0.f, posY = 0.f;
                if (windowRatio > viewRatio) { sizeX = viewRatio / windowRatio; posX = (1.f - sizeX) / 2.f; } else { sizeY = windowRatio / viewRatio; posY = (1.f - sizeY) / 2.f; }
                view.setViewport(sf::FloatRect({posX, posY}, {sizeX, sizeY})); window.setView(view);
            }

            if (event->is<sf::Event::TextEntered>()) {
                uint32_t unicode = event->getIf<sf::Event::TextEntered>()->unicode;
                if (statoAttuale == LOGIN) {
                    if (unicode == '\b' && !nomeInput.empty()) nomeInput.pop_back(); else if (unicode == '\r' || unicode == '\n') { if (!nomeInput.empty()) statoAttuale = MENU; }
                    else if (unicode >= 32 && unicode < 128 && nomeInput.size() < 12) nomeInput += static_cast<char>(unicode);
                    testoNomeInput.setString(nomeInput + "_");
                } 
                else if (statoAttuale == LOBBY && inputIpAttivo) {
                    if (unicode == '\b' && !ipInput.empty()) ipInput.pop_back(); else if ((unicode >= '0' && unicode <= '9') || unicode == '.') { if (ipInput.size() < 15) ipInput += static_cast<char>(unicode); }
                    testoIP.setString(ipInput + "_");
                }
            }

            if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
            {
                if (statoAttuale == MENU) {
                    if (btnMulti.getGlobalBounds().contains(mousePos)) { statoAttuale = LOBBY; inputIpAttivo = false; testoIP.setString(ipInput); }
                    else if (btnGioca2.getGlobalBounds().contains(mousePos) || btnGioca4.getGlobalBounds().contains(mousePos) || btnSimul.getGlobalBounds().contains(mousePos)) {
                        srand(time(NULL)); // MISTERO RISOLTO: Srand solo per il locale!
                        vector<Giocatore> giocatori;
                        if (btnGioca2.getGlobalBounds().contains(mousePos)) { giocatori.push_back(Giocatore(nomeInput, false)); giocatori.push_back(Giocatore("Prof. Vessio", true)); } 
                        else if (btnGioca4.getGlobalBounds().contains(mousePos)) { giocatori.push_back(Giocatore(nomeInput, false)); giocatori.push_back(Giocatore("Bot Luigi", true)); giocatori.push_back(Giocatore("Prof. Vessio", true)); giocatori.push_back(Giocatore("Bot Mario", true)); }
                        else { giocatori.push_back(Giocatore("Bot Alpha", true)); giocatori.push_back(Giocatore("Bot Beta", true)); giocatori.push_back(Giocatore("Bot Gamma", true)); giocatori.push_back(Giocatore("Bot Delta", true)); }
                        gioco = new Partita(giocatori); gioco->setupIniziale(); modalitaSceltaColore = false; haDettoUno = false; penalitaInflitta = false; botPensando = false; avvisoInCorso = false; speedMultiplier = 1.0f; statoAttuale = IN_CORSO;
                    }
                }
                else if (statoAttuale == LOBBY) {
                    if (testoIP.getGlobalBounds().contains(mousePos) || testoIstruzioniIP.getGlobalBounds().contains(mousePos)) { inputIpAttivo = true; testoIP.setString(ipInput + "_"); } else { inputIpAttivo = false; testoIP.setString(ipInput); }
                    if (btnIndietro.getGlobalBounds().contains(mousePos)) statoAttuale = MENU;
                    
                    if (btnOspita.getGlobalBounds().contains(mousePos)) {
                        if (mioServer.preparaServer()) { isServer = true; statoAttuale = ATTESA_CONNESSIONE; }
                    }
                    if (btnUnisciti.getGlobalBounds().contains(mousePos)) {
                        if (mioClient.connetti(ipInput)) {
                            isServer = false;
                            mioClient.inviaMessaggio("NOME|" + nomeInput);
                            cout << "[CLIENT] In attesa dei dati dal Server..." << endl;
                            bool partitaRicevuta = false;
                            while (!partitaRicevuta && window.isOpen()) {
                                string msg = mioClient.riceviMessaggio();
                                if (msg != "") {
                                    vector<string> dati = splittaStringa(msg, "|");
                                    if (dati[0] == "INIZIO") {
                                        string nomeAvversario = dati[1]; int randomSeed = stoi(dati[2]);
                                        
                                        // IMPORTANTE: Usa lo stesso identico seed del Server!
                                        srand(randomSeed); 
                                        
                                        // MISTERO RISOLTO 2: Ricorda, in 1v1 Client: io sono l'indice 1, ma la mia schermata disegna l'indice 0 in basso.
                                        vector<Giocatore> giocatori = {Giocatore(nomeAvversario, false), Giocatore(nomeInput, false)};
                                        gioco = new Partita(giocatori); 
                                        gioco->setupIniziale(); // Crea le stesse esatte carte del Server!
                                        
                                        partitaRicevuta = true; statoAttuale = IN_CORSO_RETE;
                                    }
                                }
                            }
                        }
                    }
                }
                else if ((statoAttuale == IN_CORSO || statoAttuale == IN_CORSO_RETE) && gioco != nullptr) {
                    if (isSimulazione) {
                        if (btnVelocita[0].getGlobalBounds().contains(mousePos)) speedMultiplier = 1.0f; else if (btnVelocita[1].getGlobalBounds().contains(mousePos)) speedMultiplier = 2.0f; else if (btnVelocita[2].getGlobalBounds().contains(mousePos)) speedMultiplier = 4.0f; else if (btnVelocita[3].getGlobalBounds().contains(mousePos)) speedMultiplier = 100.0f; 
                    }

                    if (!avvisoInCorso && !animazioneUnoAttiva) { 
                        int turno = gioco->getTurnoCorrente(); bool latoOscuro = gioco->getLatoOscuroAttivo();
                        
                        if ((isMultiplayer && turno == idLocale) || (!isMultiplayer && !gioco->getGiocatori()[turno].getIsBot())) {
                            
                            vector<Carta> miaMano = gioco->getGiocatori()[turno].getMano();
                            if (modalitaSceltaColore) {
                                for(int c=0; c<4; c++) { 
                                    if (btnColori[c].getGlobalBounds().contains(mousePos)) { 
                                        gioco->mossaUmano(indiceJollyInSospeso, c, haDettoUno); 
                                        if (isMultiplayer) { string m = "GIOCA|" + to_string(indiceJollyInSospeso) + "|" + to_string(c) + "|" + to_string(haDettoUno); isServer ? mioServer.inviaMessaggio(m) : mioClient.inviaMessaggio(m); }
                                        modalitaSceltaColore = false; indiceJollyInSospeso = -1; haDettoUno = false; btnUno.setFillColor(sf::Color::Red); 
                                    } 
                                }
                            } else {
                                if (miaMano.size() == 2 && btnUno.getGlobalBounds().contains(mousePos) && !haDettoUno) { haDettoUno = true; btnUno.setFillColor(sf::Color::Green); penalitaInflitta = false; animazioneUnoAttiva = true; timerAnimazioneUno.restart(); }
                                
                                if (mazzoGrafico.getGlobalBounds().contains(mousePos)) { 
                                    gioco->mossaUmano(-1, 0, haDettoUno); penalitaInflitta = false; 
                                    if (isMultiplayer) isServer ? mioServer.inviaMessaggio("PESCA") : mioClient.inviaMessaggio("PESCA");
                                }
                                
                                float spaziatura = min(120.f, 800.f / max(1.f, (float)miaMano.size())); float startX = (1024.f - ((miaMano.size() - 1) * spaziatura + 115.f)) / 2.f;
                                for(int i = miaMano.size() - 1; i >= 0; i--) {
                                    sf::FloatRect bounds({startX + (i * spaziatura), 600.f}, {115.f, 150.f});
                                    if (bounds.contains(mousePos)) {
                                        if (gioco->mossaValida(miaMano[i])) {
                                            if (miaMano.size() == 2 && !haDettoUno) penalitaInflitta = true; else penalitaInflitta = false;
                                            if (miaMano[i].getColore(latoOscuro) == NERO) { modalitaSceltaColore = true; indiceJollyInSospeso = i; } 
                                            else { 
                                                gioco->mossaUmano(i, 0, haDettoUno); 
                                                if (isMultiplayer) { string m = "GIOCA|" + to_string(i) + "|0|" + to_string(haDettoUno); isServer ? mioServer.inviaMessaggio(m) : mioClient.inviaMessaggio(m); }
                                                if (miaMano.size() > 2) haDettoUno = false; btnUno.setFillColor(sf::Color::Red); 
                                            }
                                        } break; 
                                    }
                                }
                            }
                        }
                    }
                }
                else if (statoAttuale == FINE) { if (btnTornaMenu.getGlobalBounds().contains(mousePos)) { delete gioco; gioco = nullptr; statoAttuale = MENU; } }
            }
        }

        // --- CREAZIONE DELLA PARTITA MULTIPLAYER (Lato Server) ---
        if (statoAttuale == ATTESA_CONNESSIONE) {
            if (mioServer.attendiClient()) {
                bool nomeRicevuto = false; string nomeAvversario = "Avversario";
                while (!nomeRicevuto && window.isOpen()) {
                    string msg = mioServer.riceviMessaggio();
                    if (msg != "") { vector<string> dati = splittaStringa(msg, "|"); if (dati[0] == "NOME") { nomeAvversario = dati[1]; nomeRicevuto = true; } }
                }
                
                int randomSeed = time(NULL); srand(randomSeed); 
                mioServer.inviaMessaggio("INIZIO|" + nomeInput + "|" + to_string(randomSeed));
                
                vector<Giocatore> giocatori = {Giocatore(nomeInput, false), Giocatore(nomeAvversario, false)};
                gioco = new Partita(giocatori); gioco->setupIniziale(); statoAttuale = IN_CORSO_RETE;
            }
        }
        
        // --- ASCOLTO RETE DURANTE IL GIOCO ---
        if (statoAttuale == IN_CORSO_RETE && gioco != nullptr) {
            // Se non è il mio turno, ascolto se la Rete mi manda una mossa
            if (gioco->getTurnoCorrente() != idLocale) {
                string msgRicevuto = isServer ? mioServer.riceviMessaggio() : mioClient.riceviMessaggio();
                if (msgRicevuto != "") {
                    vector<string> dati = splittaStringa(msgRicevuto, "|");
                    if (dati[0] == "GIOCA") { gioco->mossaRete(stoi(dati[1]), stoi(dati[2]), stoi(dati[3])); } 
                    else if (dati[0] == "PESCA") { gioco->mossaRete(-1, 0, false); }
                }
            }
        }

        if ((statoAttuale == IN_CORSO || statoAttuale == IN_CORSO_RETE) && gioco != nullptr && !animazioneUnoAttiva) {
            if (gioco->partitaTerminata()) {
                statoAttuale = FINE; testoVincitore.setString("VINCITORE: " + gioco->getVincitore()); testoVincitore.setPosition({(1024.f - testoVincitore.getGlobalBounds().size.x) / 2.f, 150.f}); 
                Database db; db.caricaDati(); vector<string> nomiPartecipanti; for (Giocatore g : gioco->getGiocatori()) nomiPartecipanti.push_back(g.getNome());
                db.aggiornaStatistiche(nomiPartecipanti, gioco->getVincitore()); db.salvaDati(); 
                string classificaStr = "CLASSIFICA (Aggiornata!):\n"; ifstream file("classifica.csv"); if (file.is_open()) { string riga; while(getline(file, riga)) classificaStr += riga + "\n"; file.close(); }
                testoClassifica.setString(classificaStr); testoClassifica.setPosition({337.f, 250.f});
            }
            else if (gioco->getMostraAvviso()) {
                if (!isMultiplayer && speedMultiplier >= 100.f) { gioco->resetAvviso(); } else {
                    if (!avvisoInCorso) { avvisoInCorso = true; timerAvviso.restart(); testoPopUp.setString(gioco->getMessaggioAvviso()); testoPopUp.setPosition({(1024.f - testoPopUp.getGlobalBounds().size.x) / 2.f, 350.f}); }
                    else if (timerAvviso.getElapsedTime().asSeconds() > 2.5f) { avvisoInCorso = false; gioco->resetAvviso(); }
                }
            }
            else if (!isMultiplayer && gioco->getGiocatori()[gioco->getTurnoCorrente()].getIsBot() && !modalitaSceltaColore && !avvisoInCorso) {
                if (!botPensando) { 
                    timerBot.restart(); botPensando = true; int carteRimaste = gioco->getGiocatori()[gioco->getTurnoCorrente()].getMano().size();
                    if (carteRimaste <= 2) tempoAttesaCalcolato = 2.5f + (rand() % 15) / 10.0f; else if (carteRimaste <= 4) tempoAttesaCalcolato = 1.5f + (rand() % 10) / 10.0f; else tempoAttesaCalcolato = 0.6f + (rand() % 6) / 10.0f;
                    tempoAttesaCalcolato = tempoAttesaCalcolato / speedMultiplier;
                } else if (timerBot.getElapsedTime().asSeconds() > tempoAttesaCalcolato) { gioco->mossaBot(); botPensando = false; }
            }
        }

        window.clear(sf::Color(10, 10, 15));

        if (statoAttuale == LOGIN) { window.draw(testoTitoloLogin); window.draw(testoIstruzioniLogin); window.draw(testoNomeInput); }
        else if (statoAttuale == MENU) {
            window.draw(titolo); sf::Text benvenuto(font, "Bentornato, " + nomeInput + "!", 24); benvenuto.setFillColor(sf::Color::Cyan); benvenuto.setPosition({337.f, 160.f}); window.draw(benvenuto); 
            window.draw(btnGioca2); window.draw(txtGioca2); window.draw(btnGioca4); window.draw(txtGioca4); window.draw(btnSimul); window.draw(txtSimul); window.draw(btnMulti); window.draw(txtMulti);
        }
        else if (statoAttuale == LOBBY) {
            window.draw(titoloLobby); window.draw(btnOspita); window.draw(txtOspita); window.draw(btnUnisciti); window.draw(txtUnisciti); window.draw(testoIstruzioniIP); window.draw(testoIP); window.draw(btnIndietro); window.draw(txtIndietro);
        }
        else if (statoAttuale == ATTESA_CONNESSIONE) {
            sf::Text testoAttesa(font, "IN ATTESA DELL'AVVERSARIO SULLA PORTA 53000...", 30); testoAttesa.setFillColor(sf::Color::Yellow); testoAttesa.setOutlineThickness(2.f); testoAttesa.setOutlineColor(sf::Color::Black); testoAttesa.setPosition({150.f, 350.f});
            int puntini = (int)(timerAvviso.getElapsedTime().asSeconds() * 2) % 4; string strPuntini = ""; for(int p=0; p<puntini; p++) strPuntini += "."; testoAttesa.setString("IN ATTESA DELL'AVVERSARIO" + strPuntini);
            window.draw(testoAttesa); window.draw(btnIndietro); window.draw(txtIndietro);
        }
        else if ((statoAttuale == IN_CORSO || statoAttuale == IN_CORSO_RETE) && gioco != nullptr) {
            bool latoOscuro = gioco->getLatoOscuroAttivo(); int numGiocatori = gioco->getGiocatori().size();
            sfondoPrincipale.setFillColor(latoOscuro ? sf::Color(40, 20, 40) : sf::Color(20, 60, 80)); window.draw(sfondoPrincipale); window.draw(tramaSfondo);
            tavoloCentrale.setFillColor(latoOscuro ? sf::Color(25, 10, 25) : sf::Color(20, 100, 40)); window.draw(tavoloCentrale);
            mazzoGrafico.setFillColor(latoOscuro ? sf::Color(80, 0, 120) : sf::Color(10, 30, 150)); ovaleMazzo.setFillColor(latoOscuro ? sf::Color(40, 0, 60) : sf::Color(0, 15, 80)); indicatoreColore.setFillColor(getSFMLColor(gioco->getColoreAttivo()));
            window.draw(mazzoOmbra); window.draw(mazzoGrafico); window.draw(bordinoMazzo); window.draw(ovaleMazzo); window.draw(testoMazzo);
            disegnaCartaFronte(window, gioco->getCartaInCima(), latoOscuro, font, 480.f, 310.f); window.draw(indicatoreColore); window.draw(testoIndicatore);
            
            // --- DISEGNO DINAMICO BASATO SU idLocale ---
            vector<Carta> manoBasso = gioco->getGiocatori()[idLocale].getMano();
            float spaziaturaBasso = min(120.f, 800.f / max(1.f, (float)manoBasso.size())); float startXBasso = (1024.f - ((manoBasso.size() - 1) * spaziaturaBasso + 115.f)) / 2.f;
            for(int i = 0; i < manoBasso.size(); i++) {
                if (isSimulazione) {
                    sf::RectangleShape rect({60.f, 90.f}); rect.setPosition({startXBasso + (i * spaziaturaBasso) + 27.5f, 650.f}); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); rect.setFillColor(getSFMLColor(manoBasso[i].getColore(!latoOscuro))); window.draw(rect);
                    sf::Text valTxt(font, ottieniSimboloBreve(manoBasso[i], latoOscuro), 22); valTxt.setFillColor(sf::Color::White); valTxt.setOutlineColor(sf::Color::Black); valTxt.setOutlineThickness(2.f); valTxt.setOrigin({valTxt.getLocalBounds().position.x + valTxt.getLocalBounds().size.x / 2.f, valTxt.getLocalBounds().position.y + valTxt.getLocalBounds().size.y / 2.f}); valTxt.setPosition({rect.getPosition().x + 30.f, rect.getPosition().y + 40.f}); window.draw(valTxt);
                } else disegnaCartaFronte(window, manoBasso[i], latoOscuro, font, startXBasso + (i * spaziaturaBasso), 600.f);
            }

            if (numGiocatori > 1) {
                if (numGiocatori == 2) {
                    int idAvversario = isMultiplayer ? (isServer ? 1 : 0) : 1;
                    vector<Carta> manoAlto = gioco->getGiocatori()[idAvversario].getMano();
                    float spaziaturaAlto = min(50.f, 600.f / max(1.f, (float)manoAlto.size())); float startXAlto = (1024.f - ((manoAlto.size() - 1) * spaziaturaAlto + 115.f)) / 2.f;
                    for(int i = 0; i < manoAlto.size(); i++) {
                        if (isSimulazione) disegnaCartaFronte(window, manoAlto[i], latoOscuro, font, startXAlto + (i * spaziaturaAlto), -30.f);
                        else { sf::RectangleShape rect({60.f, 90.f}); rect.setPosition({startXAlto + (i * spaziaturaAlto), -10.f}); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); rect.setFillColor(getSFMLColor(manoAlto[i].getColore(!latoOscuro))); window.draw(rect); }
                    }
                } else {
                    vector<Carta> manoSx = gioco->getGiocatori()[1].getMano();
                    float spaziaturaSx = min(40.f, 400.f / max(1.f, (float)manoSx.size())); float startYSx = (768.f - ((manoSx.size() - 1) * spaziaturaSx + 60.f)) / 2.f;
                    for(int i = 0; i < manoSx.size(); i++) {
                        if (isSimulazione) {
                            sf::Transform t; t.translate(sf::Vector2f(80.f, startYSx + (i * spaziaturaSx))); t.rotate(sf::degrees(90.f)); t.translate(sf::Vector2f(-57.f, -75.f));
                            sf::RectangleShape rect({115.f, 150.f}); rect.setFillColor(getSFMLColor(manoSx[i].getColore(latoOscuro))); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); window.draw(rect, t);
                            sf::Text valTxt(font, ottieniSimboloBreve(manoSx[i], latoOscuro), 22); valTxt.setFillColor(sf::Color::White); valTxt.setOutlineColor(sf::Color::Black); valTxt.setOutlineThickness(2.f); valTxt.setOrigin({valTxt.getLocalBounds().position.x + valTxt.getLocalBounds().size.x/2.f, valTxt.getLocalBounds().position.y + valTxt.getLocalBounds().size.y/2.f}); valTxt.setPosition({80.f, startYSx + (i * spaziaturaSx)}); valTxt.setRotation(sf::degrees(90.f)); window.draw(valTxt);
                        } else { sf::RectangleShape rect({60.f, 90.f}); rect.setOrigin({30.f, 45.f}); rect.setPosition({80.f, startYSx + (i * spaziaturaSx)}); rect.setRotation(sf::degrees(90.f)); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); rect.setFillColor(getSFMLColor(manoSx[i].getColore(!latoOscuro))); window.draw(rect); }
                    }
                }
            }

            if (numGiocatori > 2) {
                vector<Carta> manoAlto = gioco->getGiocatori()[2].getMano();
                float spaziaturaAlto = min(50.f, 600.f / max(1.f, (float)manoAlto.size())); float startXAlto = (1024.f - ((manoAlto.size() - 1) * spaziaturaAlto + 115.f)) / 2.f;
                for(int i = 0; i < manoAlto.size(); i++) {
                    if (isSimulazione) disegnaCartaFronte(window, manoAlto[i], latoOscuro, font, startXAlto + (i * spaziaturaAlto), -30.f);
                    else { sf::RectangleShape rect({60.f, 90.f}); rect.setPosition({startXAlto + (i * spaziaturaAlto), -10.f}); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); rect.setFillColor(getSFMLColor(manoAlto[i].getColore(!latoOscuro))); window.draw(rect); }
                }
            }

            if (numGiocatori > 3) {
                vector<Carta> manoDx = gioco->getGiocatori()[3].getMano();
                float spaziaturaDx = min(40.f, 400.f / max(1.f, (float)manoDx.size())); float startYDx = (768.f - ((manoDx.size() - 1) * spaziaturaDx + 60.f)) / 2.f;
                for(int i = 0; i < manoDx.size(); i++) {
                    if (isSimulazione) {
                        sf::Transform t; t.translate(sf::Vector2f(944.f, startYDx + (i * spaziaturaDx))); t.rotate(sf::degrees(-90.f)); t.translate(sf::Vector2f(-57.f, -75.f));
                        sf::RectangleShape rect({115.f, 150.f}); rect.setFillColor(getSFMLColor(manoDx[i].getColore(latoOscuro))); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); window.draw(rect, t);
                        sf::Text valTxt(font, ottieniSimboloBreve(manoDx[i], latoOscuro), 22); valTxt.setFillColor(sf::Color::White); valTxt.setOutlineColor(sf::Color::Black); valTxt.setOutlineThickness(2.f); valTxt.setOrigin({valTxt.getLocalBounds().position.x + valTxt.getLocalBounds().size.x/2.f, valTxt.getLocalBounds().position.y + valTxt.getLocalBounds().size.y/2.f}); valTxt.setPosition({944.f, startYDx + (i * spaziaturaDx)}); valTxt.setRotation(sf::degrees(-90.f)); window.draw(valTxt);
                    } else { sf::RectangleShape rect({60.f, 90.f}); rect.setOrigin({30.f, 45.f}); rect.setPosition({1024.f - 80.f, startYDx + (i * spaziaturaDx)}); rect.setRotation(sf::degrees(-90.f)); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); rect.setFillColor(getSFMLColor(manoDx[i].getColore(!latoOscuro))); window.draw(rect); }
                }
            }

            if (isSimulazione) {
                sf::Text txtL(font, "VELOCITA':", 16); txtL.setPosition({680.f, 25.f}); window.draw(txtL);
                for(int i=0; i<4; i++) { if ((i==0 && speedMultiplier==1.f) || (i==1 && speedMultiplier==2.f) || (i==2 && speedMultiplier==4.f) || (i==3 && speedMultiplier>=100.f)) btnVelocita[i].setFillColor(sf::Color::Green); else btnVelocita[i].setFillColor(sf::Color(50, 50, 50)); window.draw(btnVelocita[i]); window.draw(txtVelocita[i]); }
            }

            sf::Text testoSenso(font, gioco->getSensoOrario() ? "GIRO: ORARIO >>" : "<< GIRO: ANTIORARIO", 18); testoSenso.setFillColor(gioco->getSensoOrario() ? sf::Color(100, 255, 100) : sf::Color(100, 255, 255)); testoSenso.setOutlineColor(sf::Color::Black); testoSenso.setOutlineThickness(2.f); testoSenso.setPosition({420.f, 240.f}); window.draw(testoSenso);
            sf::CircleShape markerTurno(15.f, 3); markerTurno.setFillColor(sf::Color::Yellow); markerTurno.setOutlineThickness(3.f); markerTurno.setOutlineColor(sf::Color::Red); markerTurno.setOrigin({15.f, 15.f});
            
            int turno = gioco->getTurnoCorrente();
            if (turno == idLocale) { markerTurno.setPosition({512.f, 570.f}); markerTurno.setRotation(sf::degrees(180.f)); } 
            else if (numGiocatori == 2 && turno != idLocale) { markerTurno.setPosition({512.f, 170.f}); markerTurno.setRotation(sf::degrees(0.f)); } 
            else if (numGiocatori == 4) { if (turno == 1) { markerTurno.setPosition({180.f, 384.f}); markerTurno.setRotation(sf::degrees(-90.f)); } else if (turno == 2) { markerTurno.setPosition({512.f, 170.f}); markerTurno.setRotation(sf::degrees(0.f)); } else if (turno == 3) { markerTurno.setPosition({844.f, 384.f}); markerTurno.setRotation(sf::degrees(90.f)); } }
            window.draw(markerTurno);

            testoBotLog.setString(gioco->getUltimoLogBot());
            if (penalitaInflitta) testoFeedback.setString("Penalita' per non aver detto UNO: +2 Carte!"); else { string nomeG = gioco->getGiocatori()[gioco->getTurnoCorrente()].getNome(); if (turno == idLocale) testoFeedback.setString("Tocca a te! Fai la tua mossa."); else { if (tempoAttesaCalcolato > 2.0f && botPensando) testoFeedback.setString(nomeG + " sta pensando intensamente..."); else testoFeedback.setString("In attesa di " + nomeG + "..."); } }
            window.draw(testoFeedback); window.draw(testoBotLog); if (manoBasso.size() == 2 && turno == idLocale) { window.draw(btnUno); window.draw(testoUno); }

            if (modalitaSceltaColore) {
                if (!latoOscuro) { btnColori[0].setFillColor(sf::Color(220, 20, 20)); btnColori[1].setFillColor(sf::Color(240, 190, 0)); btnColori[2].setFillColor(sf::Color(20, 160, 20)); btnColori[3].setFillColor(sf::Color(20, 50, 220)); } 
                else { btnColori[0].setFillColor(sf::Color(255, 60, 150)); btnColori[1].setFillColor(sf::Color(0, 150, 136)); btnColori[2].setFillColor(sf::Color(255, 100, 0)); btnColori[3].setFillColor(sf::Color(100, 20, 150)); }
                for(int i=0; i<4; i++) window.draw(btnColori[i]);
            }
            if (avvisoInCorso) { window.draw(overlayScurito); window.draw(testoPopUp); }
            if (animazioneUnoAttiva) { float elapsed = timerAnimazioneUno.getElapsedTime().asSeconds(); if (elapsed < 1.0f) { window.draw(overlayScurito); float scala = 1.0f + (elapsed * 2.0f); int alpha = 255 - (int)(elapsed * 255.f); testoAnimazioneUno.setScale({scala, scala}); testoAnimazioneUno.setFillColor(sf::Color(255, 255, 0, alpha)); testoAnimazioneUno.setOutlineColor(sf::Color(255, 0, 0, alpha)); window.draw(testoAnimazioneUno); } else { animazioneUnoAttiva = false; } }
        }
        else if (statoAttuale == FINE) { window.draw(testoVincitore); window.draw(testoClassifica); window.draw(btnTornaMenu); window.draw(txtTornaMenu); }

        window.display();             
    }

    if (gioco != nullptr) delete gioco;
    return 0;
}