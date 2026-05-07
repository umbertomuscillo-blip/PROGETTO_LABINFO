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

enum StatoGioco { LOGIN, MENU, LOBBY, ATTESA_CONNESSIONE, IN_CORSO_RETE, IN_CORSO, FINE, MENU_CLASSIFICA };

struct Bottone {
    sf::RectangleShape rect; sf::Text testo; sf::Color coloreBase; sf::Color coloreHover;
    Bottone(const sf::Font& font) : testo(font) {}
    void setup(string etichetta, float x, float y, float width, float height, sf::Color base, sf::Color hover) {
        coloreBase = base; coloreHover = hover;
        rect.setSize({width, height}); rect.setPosition({x, y}); rect.setFillColor(coloreBase);
        rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color(255, 255, 255, 150)); 
        testo.setString(etichetta); testo.setCharacterSize(20); testo.setFillColor(sf::Color::White);
        sf::FloatRect bounds = testo.getLocalBounds();
        testo.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
        testo.setPosition({x + width / 2.f, y + height / 2.f});
    }
    void aggiorna(sf::Vector2f mousePos) {
        if (rect.getGlobalBounds().contains(mousePos)) { rect.setFillColor(coloreHover); rect.setOutlineColor(sf::Color::White); rect.setOutlineThickness(3.f); } 
        else { rect.setFillColor(coloreBase); rect.setOutlineColor(sf::Color(255, 255, 255, 100)); rect.setOutlineThickness(2.f); }
    }
    void disegna(sf::RenderWindow& window) { window.draw(rect); window.draw(testo); }
    bool cliccato(sf::Vector2f mousePos) { return rect.getGlobalBounds().contains(mousePos); }
};

vector<string> splittaStringa(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token; vector<string> res;
    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) { token = s.substr(pos_start, pos_end - pos_start); pos_start = pos_end + delim_len; res.push_back(token); } res.push_back(s.substr(pos_start)); return res;
}

struct RigaClassifica { string nome; int vittorie; int partite; };
bool ordinaClassifica(const RigaClassifica& a, const RigaClassifica& b) { if (a.vittorie != b.vittorie) return a.vittorie > b.vittorie; return a.partite < b.partite; }

vector<RigaClassifica> leggiClassificaDati() {
    vector<RigaClassifica> dati; ifstream file("classifica.csv");
    if (file.is_open()) { string riga;
        while(getline(file, riga)) { vector<string> colonne = splittaStringa(riga, ",");
            if (colonne.size() >= 2 && colonne[0] != "Nome") { 
                RigaClassifica rec; rec.nome = colonne[0]; rec.vittorie = 0; rec.partite = 0;
                if (colonne.size() >= 2) rec.vittorie = stoi(colonne[1]);
                if (colonne.size() >= 3) rec.partite = stoi(colonne[2]);
                dati.push_back(rec);
            }
        } file.close();
    } sort(dati.begin(), dati.end(), ordinaClassifica); return dati;
}

void disegnaTabellaClassifica(sf::RenderWindow& window, sf::Font& font, vector<RigaClassifica>& dati) {
    float startX = 250.f; float startY = 280.f;
    sf::Text th1(font, "POS", 18); th1.setPosition({startX + 20.f, startY}); th1.setFillColor(sf::Color::Cyan); window.draw(th1);
    sf::Text th2(font, "GIOCATORE", 18); th2.setPosition({startX + 100.f, startY}); th2.setFillColor(sf::Color::Cyan); window.draw(th2);
    sf::Text th3(font, "VITTORIE", 18); th3.setPosition({startX + 350.f, startY}); th3.setFillColor(sf::Color::Cyan); window.draw(th3);
    sf::Text th4(font, "PARTITE", 18); th4.setPosition({startX + 470.f, startY}); th4.setFillColor(sf::Color::Cyan); window.draw(th4);
    sf::RectangleShape lineaIntestazione({540.f, 2.f}); lineaIntestazione.setPosition({startX, startY + 30.f}); lineaIntestazione.setFillColor(sf::Color::White); window.draw(lineaIntestazione);

    if (dati.empty()) { sf::Text vuota(font, "NESSUNA PARTITA REGISTRATA.", 20); vuota.setPosition({startX + 100.f, startY + 60.f}); vuota.setFillColor(sf::Color(150, 150, 150)); window.draw(vuota); return; }

    for(int i = 0; i < min(8, (int)dati.size()); i++) {
        float rowY = startY + 45.f + (i * 30.f); sf::Color coloreRiga = sf::Color::White;
        if (i == 0) coloreRiga = sf::Color(255, 215, 0); else if (i == 1) coloreRiga = sf::Color(192, 192, 192); else if (i == 2) coloreRiga = sf::Color(205, 127, 50); 
        sf::Text tdPos(font, to_string(i+1) + ".", 20); tdPos.setPosition({startX + 20.f, rowY}); tdPos.setFillColor(coloreRiga); window.draw(tdPos);
        sf::Text tdNome(font, dati[i].nome, 20); tdNome.setPosition({startX + 100.f, rowY}); tdNome.setFillColor(coloreRiga); window.draw(tdNome);
        sf::Text tdVit(font, to_string(dati[i].vittorie), 20); tdVit.setPosition({startX + 370.f, rowY}); tdVit.setFillColor(coloreRiga); window.draw(tdVit);
        sf::Text tdPart(font, to_string(dati[i].partite), 20); tdPart.setPosition({startX + 490.f, rowY}); tdPart.setFillColor(coloreRiga); window.draw(tdPart);
    }
}

string ottieniSimboloBreve(Carta c, bool latoOscuro) {
    Valore v = c.getValore(latoOscuro);
    if (v == UNO) return "1"; if (v == DUE) return "2"; if (v == TRE) return "3"; if (v == QUATTRO) return "4"; if (v == CINQUE) return "5"; if (v == SEI) return "6"; if (v == SETTE) return "7"; if (v == OTTO) return "8"; if (v == NOVE) return "9"; 
    if (v == PESCA_UNO) return "+1"; if (v == PESCA_CINQUE) return "+5"; if (v == INVERTI) return "INV"; if (v == SALTA || v == SALTA_TUTTI) return "ALT"; if (v == FLIP) return "FLIP";
    if (v == JOLLY) return "J"; if (v == JOLLY_PESCA_DUE) return "J+2"; if (v == JOLLY_PESCA_COLORE) return "J+C"; return "?";
}

void disegnaCartaFronte(sf::RenderWindow& window, Carta cartaDati, bool latoOscuro, sf::Font& font, float x, float y) {
    string desc = latoOscuro ? cartaDati.getDescrizioneOscura() : cartaDati.getDescrizioneChiara(); sf::Color bgColor = sf::Color(30, 30, 30); 
    if (desc.find("Rosso") != string::npos) bgColor = sf::Color(220, 20, 20); else if (desc.find("Blu") != string::npos) bgColor = sf::Color(20, 50, 220); else if (desc.find("Verde Acqua") != string::npos) bgColor = sf::Color(0, 150, 136); else if (desc.find("Verde") != string::npos) bgColor = sf::Color(20, 160, 20); else if (desc.find("Giallo") != string::npos) bgColor = sf::Color(240, 190, 0); else if (desc.find("Rosa") != string::npos) bgColor = sf::Color(255, 60, 150); else if (desc.find("Arancione") != string::npos) bgColor = sf::Color(255, 100, 0); else if (desc.find("Viola") != string::npos) bgColor = sf::Color(100, 20, 150);
    
    sf::RectangleShape ombra({115.f, 150.f}); ombra.setPosition({x + 4.f, y + 4.f}); ombra.setFillColor(sf::Color(0, 0, 0, 120)); 
    sf::RectangleShape rect({115.f, 150.f}); rect.setPosition({x, y}); rect.setFillColor(bgColor); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White);
    sf::RectangleShape bordino({105.f, 140.f}); bordino.setPosition({x + 5.f, y + 5.f}); bordino.setFillColor(sf::Color::Transparent); bordino.setOutlineThickness(1.5f); bordino.setOutlineColor(sf::Color(255, 255, 255, 120));
    sf::CircleShape ovale(40.f); ovale.setScale({0.85f, 1.2f}); ovale.setOrigin({40.f, 40.f}); ovale.setPosition({x + 115.f/2.f, y + 150.f/2.f}); ovale.setRotation(sf::degrees(-20.f)); ovale.setFillColor(sf::Color::White);

    string simbolo = ottieniSimboloBreve(cartaDati, latoOscuro); int sizeCentro = 42; int sizeAngoli = 18; if (simbolo.length() >= 4) { sizeCentro = 22; sizeAngoli = 12; } else if (simbolo.length() == 3) { sizeCentro = 26; sizeAngoli = 14; } else if (simbolo.length() == 2) { sizeCentro = 34; sizeAngoli = 16; }

    sf::Text txtCentro(font, simbolo, sizeCentro); txtCentro.setFillColor(bgColor == sf::Color(30, 30, 30) ? sf::Color::Black : bgColor); sf::FloatRect bounds = txtCentro.getLocalBounds(); txtCentro.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f}); txtCentro.setPosition({x + 115.f/2.f, y + 150.f/2.f}); txtCentro.setRotation(sf::degrees(-20.f));
    sf::Text txtTL(font, simbolo, sizeAngoli); txtTL.setFillColor(sf::Color::White); txtTL.setOutlineColor(sf::Color::Black); txtTL.setOutlineThickness(1.2f); sf::FloatRect boundsTL = txtTL.getLocalBounds(); txtTL.setOrigin({boundsTL.position.x + boundsTL.size.x / 2.f, boundsTL.position.y + boundsTL.size.y / 2.f}); txtTL.setPosition({x + 16.f, y + 16.f}); 
    sf::Text txtBR(font, simbolo, sizeAngoli); txtBR.setFillColor(sf::Color::White); txtBR.setOutlineColor(sf::Color::Black); txtBR.setOutlineThickness(1.2f); sf::FloatRect boundsBR = txtBR.getLocalBounds(); txtBR.setOrigin({boundsBR.position.x + boundsBR.size.x / 2.f, boundsBR.position.y + boundsBR.size.y / 2.f}); txtBR.setPosition({x + 115.f - 16.f, y + 150.f - 16.f}); txtBR.setRotation(sf::degrees(180.f));

    window.draw(ombra); window.draw(rect); window.draw(bordino); window.draw(ovale); window.draw(txtCentro); window.draw(txtTL); window.draw(txtBR);
}

sf::Color getSFMLColor(Colore c) {
    switch(c) {
        case ROSSO: return sf::Color(220, 20, 20); case BLU: return sf::Color(20, 50, 220); case VERDE: return sf::Color(20, 160, 20); case GIALLO: return sf::Color(240, 190, 0);
        case ROSA: return sf::Color(255, 60, 150); case VERDE_ACQUA: return sf::Color(0, 150, 136); case ARANCIONE: return sf::Color(255, 100, 0); case VIOLA: return sf::Color(100, 20, 150); default: return sf::Color(30, 30, 30); 
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({1024, 768}), "UNO Flip! - Premium Multiplayer Edition");
    sf::View view(sf::FloatRect({0.f, 0.f}, {1024.f, 768.f})); window.setView(view); window.setFramerateLimit(60);
    sf::Font font; if (!font.openFromFile("font.ttf")) return -1;

    StatoGioco statoAttuale = LOGIN; Partita* gioco = nullptr; 
    ServerRete mioServer; ClientRete mioClient; bool isServer = false; 

    sf::VertexArray tramaSfondo(sf::PrimitiveType::Lines);
    for(int i = -1024; i < 2048; i += 40) {
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i, 0.f), sf::Color(0, 0, 0, 40)}); tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i + 768.f, 768.f), sf::Color(0, 0, 0, 40)});
        tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i, 768.f), sf::Color(0, 0, 0, 40)}); tramaSfondo.append(sf::Vertex{sf::Vector2f((float)i + 768.f, 0.f), sf::Color(0, 0, 0, 40)});
    }

    string nomeInput = "";
    sf::Text testoTitoloLogin(font, "CREA O SELEZIONA PROFILO", 50); testoTitoloLogin.setPosition({150.f, 150.f}); testoTitoloLogin.setFillColor(sf::Color::Yellow);
    sf::Text testoIstruzioniLogin(font, "Scrivi il tuo nome e premi INVIO:", 24); testoIstruzioniLogin.setPosition({300.f, 300.f});
    sf::Text testoNomeInput(font, "_", 40); testoNomeInput.setPosition({350.f, 380.f}); testoNomeInput.setFillColor(sf::Color::Green);

    sf::Text titoloOmbra(font, "UNO FLIP!", 80); titoloOmbra.setPosition({325.f, 55.f}); titoloOmbra.setFillColor(sf::Color(0,0,0,180));
    sf::Text titolo(font, "UNO FLIP!", 80); titolo.setPosition({320.f, 50.f}); titolo.setFillColor(sf::Color::Yellow); titolo.setOutlineThickness(4.f); titolo.setOutlineColor(sf::Color::Red);

    Bottone btnGioca2(font), btnGioca4(font), btnSimul(font), btnMulti(font), btnClassificaMenu(font), btnIndietro(font);
    btnGioca2.setup("GIOCA LOCALE (1 VS 1)", 312.f, 180.f, 400.f, 55.f, sf::Color(20, 120, 40, 200), sf::Color(40, 180, 60));
    btnGioca4.setup("GIOCA LOCALE (4 GIOCATORI)", 312.f, 255.f, 400.f, 55.f, sf::Color(20, 80, 160, 200), sf::Color(40, 120, 220));
    btnSimul.setup("SIMULAZIONE (4 BOT)", 312.f, 330.f, 400.f, 55.f, sf::Color(160, 80, 20, 200), sf::Color(220, 120, 40));
    btnMulti.setup("MULTIPLAYER ONLINE", 312.f, 405.f, 400.f, 55.f, sf::Color(160, 30, 30, 200), sf::Color(220, 50, 50));
    btnClassificaMenu.setup("CONSULTA CLASSIFICA GLOBALE", 312.f, 480.f, 400.f, 55.f, sf::Color(100, 20, 160, 200), sf::Color(150, 40, 220));
    btnIndietro.setup("INDIETRO", 312.f, 650.f, 400.f, 55.f, sf::Color(80, 80, 80, 200), sf::Color(130, 130, 130));

    sf::Text titoloLobby(font, "SALA D'ATTESA MULTIPLAYER", 40); titoloLobby.setPosition({250.f, 100.f}); titoloLobby.setFillColor(sf::Color::Yellow);
    Bottone btnOspita(font), btnUnisciti(font); 
    btnOspita.setup("OSPITA PARTITA (Server)", 312.f, 250.f, 400.f, 55.f, sf::Color(20, 120, 40, 200), sf::Color(40, 180, 60));
    btnUnisciti.setup("UNISCITI A PARTITA (Client)", 312.f, 325.f, 400.f, 55.f, sf::Color(20, 80, 160, 200), sf::Color(40, 120, 220));
    string ipInput = "127.0.0.1"; sf::Text testoIstruzioniIP(font, "IP a cui connettersi (se Client):", 20); testoIstruzioniIP.setPosition({312.f, 410.f});
    sf::Text testoIP(font, ipInput, 30); testoIP.setPosition({312.f, 440.f}); testoIP.setFillColor(sf::Color::Cyan); bool inputIpAttivo = false; 

    Bottone btnAbbandona(font); btnAbbandona.setup("ABBANDONA", 20.f, 700.f, 150.f, 45.f, sf::Color(100, 50, 50, 200), sf::Color(180, 50, 50));
    Bottone btnContesta(font); btnContesta.setup("CONTESTA UNO!", 700.f, 400.f, 250.f, 55.f, sf::Color(200, 20, 20, 220), sf::Color(255, 50, 50));
    int idDimenticone = -1; 

    sf::RectangleShape pannelloClassifica({600.f, 420.f}); pannelloClassifica.setPosition({212.f, 180.f}); pannelloClassifica.setFillColor(sf::Color(20, 20, 30, 220)); pannelloClassifica.setOutlineThickness(3.f); pannelloClassifica.setOutlineColor(sf::Color(100, 200, 255));
    sf::Text testoTitoloClassifica(font, "--- HALL OF FAME ---", 34); testoTitoloClassifica.setFillColor(sf::Color::Yellow); testoTitoloClassifica.setPosition({(1024.f - testoTitoloClassifica.getLocalBounds().size.x)/2.f, 210.f});
    vector<RigaClassifica> datiClassificaRecenti;
    sf::Text testoVincitore(font, "", 55); testoVincitore.setFillColor(sf::Color::Yellow); testoVincitore.setOutlineThickness(4.f); testoVincitore.setOutlineColor(sf::Color::Red);
    Bottone btnTornaMenu(font); btnTornaMenu.setup("TORNA AL MENU", 312.f, 650.f, 400.f, 55.f, sf::Color(160, 30, 30, 200), sf::Color(220, 50, 50));

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

    sf::Clock timerBot; sf::Clock timerAvviso; sf::Clock timerAntiSpam; // Timer per il debounce
    bool avvisoInCorso = false; bool botPensando = false, modalitaSceltaColore = false, haDettoUno = false, penalitaInflitta = false; int indiceJollyInSospeso = -1; float tempoAttesaCalcolato = 1.0f; 
    sf::RectangleShape overlayScurito({1024.f, 768.f}); overlayScurito.setFillColor(sf::Color(0, 0, 0, 180));
    sf::Text testoPopUp(font, "", 34); testoPopUp.setFillColor(sf::Color::Yellow); testoPopUp.setOutlineColor(sf::Color::Red); testoPopUp.setOutlineThickness(3.f);

    bool animazioneUnoAttiva = false; sf::Clock timerAnimazioneUno; sf::Text testoAnimazioneUno(font, "UNO!", 150); testoAnimazioneUno.setFillColor(sf::Color::Yellow); testoAnimazioneUno.setOutlineColor(sf::Color::Red); testoAnimazioneUno.setOutlineThickness(8.f);
    sf::FloatRect boundsUnoAnim = testoAnimazioneUno.getLocalBounds(); testoAnimazioneUno.setOrigin({boundsUnoAnim.position.x + boundsUnoAnim.size.x / 2.f, boundsUnoAnim.position.y + boundsUnoAnim.size.y / 2.f}); testoAnimazioneUno.setPosition({512.f, 384.f});

    while (window.isOpen())
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window); sf::Vector2f mousePos = window.mapPixelToCoords(pixelPos);
        bool isSimulazione = (gioco != nullptr && gioco->getGiocatori()[0].getIsBot() && statoAttuale == IN_CORSO); 
        bool isMultiplayer = (statoAttuale == IN_CORSO_RETE); int idLocale = 0; if (isMultiplayer) idLocale = isServer ? 0 : 1;

        if (statoAttuale == MENU) { btnGioca2.aggiorna(mousePos); btnGioca4.aggiorna(mousePos); btnSimul.aggiorna(mousePos); btnMulti.aggiorna(mousePos); btnClassificaMenu.aggiorna(mousePos); } 
        else if (statoAttuale == MENU_CLASSIFICA || statoAttuale == LOBBY || statoAttuale == ATTESA_CONNESSIONE) { btnIndietro.aggiorna(mousePos); if (statoAttuale == LOBBY) { btnOspita.aggiorna(mousePos); btnUnisciti.aggiorna(mousePos); } } 
        else if ((statoAttuale == IN_CORSO || statoAttuale == IN_CORSO_RETE) && gioco != nullptr) { btnAbbandona.aggiorna(mousePos); if(idDimenticone != -1 && idDimenticone != idLocale && !isSimulazione) btnContesta.aggiorna(mousePos); }
        else if (statoAttuale == FINE) { btnTornaMenu.aggiorna(mousePos); }

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
                    if (unicode == '\b' && !nomeInput.empty()) nomeInput.pop_back(); else if (unicode == '\r' || unicode == '\n') { if (!nomeInput.empty()) statoAttuale = MENU; } else if (unicode >= 32 && unicode < 128 && nomeInput.size() < 12) nomeInput += static_cast<char>(unicode);
                    testoNomeInput.setString(nomeInput + "_");
                } 
                else if (statoAttuale == LOBBY && inputIpAttivo) {
                    if (unicode == '\b' && !ipInput.empty()) ipInput.pop_back(); else if ((unicode >= '0' && unicode <= '9') || unicode == '.') { if (ipInput.size() < 15) ipInput += static_cast<char>(unicode); }
                    testoIP.setString(ipInput + "_");
                }
            }

            if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
            {
                // DEBOUNCE CLICK: Se il click avviene a meno di 0.25s dal precedente, lo ignora!
                if (timerAntiSpam.getElapsedTime().asSeconds() < 0.25f) continue;
                timerAntiSpam.restart();

                if (statoAttuale == MENU) {
                    if (btnMulti.cliccato(mousePos)) { statoAttuale = LOBBY; inputIpAttivo = false; testoIP.setString(ipInput); }
                    else if (btnClassificaMenu.cliccato(mousePos)) { datiClassificaRecenti = leggiClassificaDati(); statoAttuale = MENU_CLASSIFICA; }
                    else if (btnGioca2.cliccato(mousePos) || btnGioca4.cliccato(mousePos) || btnSimul.cliccato(mousePos)) {
                        srand(time(NULL)); vector<Giocatore> giocatori;
                        if (btnGioca2.cliccato(mousePos)) { giocatori.push_back(Giocatore(nomeInput, false)); giocatori.push_back(Giocatore("Prof. Vessio", true)); } 
                        else if (btnGioca4.cliccato(mousePos)) { giocatori.push_back(Giocatore(nomeInput, false)); giocatori.push_back(Giocatore("Bot Luigi", true)); giocatori.push_back(Giocatore("Prof. Vessio", true)); giocatori.push_back(Giocatore("Bot Mario", true)); }
                        else { giocatori.push_back(Giocatore("Bot Alpha", true)); giocatori.push_back(Giocatore("Bot Beta", true)); giocatori.push_back(Giocatore("Bot Gamma", true)); giocatori.push_back(Giocatore("Bot Delta", true)); }
                        gioco = new Partita(giocatori); gioco->setupIniziale(); modalitaSceltaColore = false; haDettoUno = false; idDimenticone = -1; botPensando = false; avvisoInCorso = false; speedMultiplier = 1.0f; statoAttuale = IN_CORSO;
                    }
                }
                else if (statoAttuale == MENU_CLASSIFICA || statoAttuale == ATTESA_CONNESSIONE) {
                    if (btnIndietro.cliccato(mousePos)) { if (statoAttuale == ATTESA_CONNESSIONE) mioServer.scollega(); statoAttuale = MENU; }
                }
                else if (statoAttuale == LOBBY) {
                    if (testoIP.getGlobalBounds().contains(mousePos) || testoIstruzioniIP.getGlobalBounds().contains(mousePos)) { inputIpAttivo = true; testoIP.setString(ipInput + "_"); } else { inputIpAttivo = false; testoIP.setString(ipInput); }
                    if (btnIndietro.cliccato(mousePos)) statoAttuale = MENU;
                    if (btnOspita.cliccato(mousePos)) { if (mioServer.preparaServer()) { isServer = true; statoAttuale = ATTESA_CONNESSIONE; } }
                    if (btnUnisciti.cliccato(mousePos)) {
                        if (mioClient.connetti(ipInput)) {
                            isServer = false; mioClient.inviaMessaggio("NOME|" + nomeInput); bool partitaRicevuta = false;
                            while (!partitaRicevuta && window.isOpen()) {
                                string msg = mioClient.riceviMessaggio();
                                if (msg != "") { vector<string> dati = splittaStringa(msg, "|");
                                    if (dati[0] == "INIZIO") { srand(stoi(dati[2])); vector<Giocatore> giocatori = {Giocatore(dati[1], false), Giocatore(nomeInput, false)};
                                        gioco = new Partita(giocatori); gioco->setupIniziale(); idDimenticone = -1; partitaRicevuta = true; statoAttuale = IN_CORSO_RETE;
                                    }
                                }
                            }
                        }
                    }
                }
                else if ((statoAttuale == IN_CORSO || statoAttuale == IN_CORSO_RETE) && gioco != nullptr) {
                    if (isSimulazione) { if (btnVelocita[0].getGlobalBounds().contains(mousePos)) speedMultiplier = 1.0f; else if (btnVelocita[1].getGlobalBounds().contains(mousePos)) speedMultiplier = 2.0f; else if (btnVelocita[2].getGlobalBounds().contains(mousePos)) speedMultiplier = 4.0f; else if (btnVelocita[3].getGlobalBounds().contains(mousePos)) speedMultiplier = 100.0f; }

                    if (btnAbbandona.cliccato(mousePos)) {
                        if (isMultiplayer) { isServer ? mioServer.inviaMessaggio("QUIT") : mioClient.inviaMessaggio("QUIT"); }
                        statoAttuale = MENU; delete gioco; gioco = nullptr;
                        continue; 
                    }
                    if (idDimenticone != -1 && idDimenticone != idLocale && btnContesta.cliccato(mousePos) && !isSimulazione) {
                        gioco->assegnaPenalitaUno(idDimenticone);
                        if (isMultiplayer) { string m = "CONTESTA|" + to_string(idDimenticone); isServer ? mioServer.inviaMessaggio(m) : mioClient.inviaMessaggio(m); }
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
                                        if (isMultiplayer) { string m = "GIOCA|" + to_string(indiceJollyInSospeso) + "|" + to_string(c) + "|" + to_string(haDettoUno); isServer ? mioServer.inviaMessaggio(m) : mioClient.inviaMessaggio(m); }
                                        modalitaSceltaColore = false; indiceJollyInSospeso = -1; haDettoUno = false; btnUno.setFillColor(sf::Color::Red); 
                                    } 
                                }
                            } else {
                                if (miaMano.size() == 2 && btnUno.getGlobalBounds().contains(mousePos) && !haDettoUno) { haDettoUno = true; btnUno.setFillColor(sf::Color::Green); penalitaInflitta = false; animazioneUnoAttiva = true; timerAnimazioneUno.restart(); }
                                
                                if (mazzoGrafico.getGlobalBounds().contains(mousePos)) { 
                                    gioco->mossaUmano(-1, 0, haDettoUno); idDimenticone = -1; 
                                    if (isMultiplayer) isServer ? mioServer.inviaMessaggio("PESCA") : mioClient.inviaMessaggio("PESCA");
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
                                                if (avevaDue && !haDettoUno) idDimenticone = turno; else idDimenticone = -1;
                                                if (isMultiplayer) { string m = "GIOCA|" + to_string(i) + "|0|" + to_string(haDettoUno); isServer ? mioServer.inviaMessaggio(m) : mioClient.inviaMessaggio(m); }
                                                haDettoUno = false; btnUno.setFillColor(sf::Color::Red); 
                                            }
                                        } break; 
                                    }
                                }
                            }
                        }
                    }
                }
                else if (statoAttuale == FINE) { if (btnTornaMenu.cliccato(mousePos)) { delete gioco; gioco = nullptr; statoAttuale = MENU; } }
            }
        }

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
                gioco = new Partita(giocatori); gioco->setupIniziale(); idDimenticone = -1; statoAttuale = IN_CORSO_RETE;
            }
        }
        
        if (statoAttuale == IN_CORSO_RETE && gioco != nullptr) {
            if (gioco->getTurnoCorrente() != idLocale) {
                string msgRicevuto = isServer ? mioServer.riceviMessaggio() : mioClient.riceviMessaggio();
                if (msgRicevuto != "") {
                    vector<string> dati = splittaStringa(msgRicevuto, "|");
                    
                    if (dati[0] == "GIOCA") { 
                        int tAvv = gioco->getTurnoCorrente(); bool avevaDue = (gioco->getGiocatori()[tAvv].getMano().size() == 2); bool haDettoU = stoi(dati[3]);
                        gioco->mossaRete(stoi(dati[1]), stoi(dati[2]), haDettoU); 
                        if (avevaDue && !haDettoU) idDimenticone = tAvv; else idDimenticone = -1;
                    } 
                    else if (dati[0] == "PESCA") { gioco->mossaRete(-1, 0, false); idDimenticone = -1; }
                    else if (dati[0] == "QUIT") {
                        gioco->forzaFinePartita(gioco->getGiocatori()[idLocale].getNome());
                        avvisoInCorso = true; timerAvviso.restart(); testoPopUp.setString("L'AVVERSARIO E' SCAPPATO!");
                    }
                    else if (dati[0] == "CONTESTA") { gioco->assegnaPenalitaUno(stoi(dati[1])); idDimenticone = -1; }
                }
            }
        }

        if ((statoAttuale == IN_CORSO || statoAttuale == IN_CORSO_RETE) && gioco != nullptr && !animazioneUnoAttiva) {
            if (gioco->partitaTerminata()) {
                statoAttuale = FINE; 
                testoVincitore.setString("VITTORIA DI: " + gioco->getVincitore() + "!"); 
                testoVincitore.setPosition({(1024.f - testoVincitore.getLocalBounds().size.x) / 2.f, 90.f}); 
                Database db; db.caricaDati(); vector<string> nomiPartecipanti; string nomeVincitoreUmano = ""; 
                for (Giocatore g : gioco->getGiocatori()) { if (!g.getIsBot()) { nomiPartecipanti.push_back(g.getNome()); if (g.getNome() == gioco->getVincitore()) nomeVincitoreUmano = g.getNome(); } }
                db.aggiornaStatistiche(nomiPartecipanti, nomeVincitoreUmano); db.salvaDati(); datiClassificaRecenti = leggiClassificaDati();
            }
            else if (gioco->getMostraAvviso()) {
                if (!isMultiplayer && speedMultiplier >= 100.f) { gioco->resetAvviso(); } else {
                    if (!avvisoInCorso) { avvisoInCorso = true; timerAvviso.restart(); testoPopUp.setString(gioco->getMessaggioAvviso()); testoPopUp.setPosition({(1024.f - testoPopUp.getLocalBounds().size.x) / 2.f, 350.f}); }
                    else if (timerAvviso.getElapsedTime().asSeconds() > 2.5f) { avvisoInCorso = false; gioco->resetAvviso(); }
                }
            }
            else if (!isMultiplayer && gioco->getGiocatori()[gioco->getTurnoCorrente()].getIsBot() && !modalitaSceltaColore && !avvisoInCorso) {
                if (!botPensando) { 
                    timerBot.restart(); botPensando = true; int carteRimaste = gioco->getGiocatori()[gioco->getTurnoCorrente()].getMano().size();
                    if (carteRimaste <= 2) tempoAttesaCalcolato = 2.5f + (rand() % 15) / 10.0f; else if (carteRimaste <= 4) tempoAttesaCalcolato = 1.5f + (rand() % 10) / 10.0f; else tempoAttesaCalcolato = 0.6f + (rand() % 6) / 10.0f; tempoAttesaCalcolato = tempoAttesaCalcolato / speedMultiplier;
                } else if (timerBot.getElapsedTime().asSeconds() > tempoAttesaCalcolato) { gioco->mossaBot(); botPensando = false; }
            }
        }

        window.clear(sf::Color(10, 10, 15));

        if (statoAttuale == LOGIN) { window.draw(testoTitoloLogin); window.draw(testoIstruzioniLogin); window.draw(testoNomeInput); }
        else if (statoAttuale == MENU) { window.draw(tramaSfondo); window.draw(titoloOmbra); window.draw(titolo); sf::Text benvenuto(font, "Bentornato, " + nomeInput + "!", 24); benvenuto.setFillColor(sf::Color::Cyan); benvenuto.setPosition({(1024.f - benvenuto.getLocalBounds().size.x)/2.f, 130.f}); window.draw(benvenuto); btnGioca2.disegna(window); btnGioca4.disegna(window); btnSimul.disegna(window); btnMulti.disegna(window); btnClassificaMenu.disegna(window); }
        else if (statoAttuale == MENU_CLASSIFICA) { window.draw(tramaSfondo); window.draw(pannelloClassifica); window.draw(testoTitoloClassifica); disegnaTabellaClassifica(window, font, datiClassificaRecenti); btnIndietro.disegna(window); }
        else if (statoAttuale == LOBBY) { window.draw(tramaSfondo); window.draw(titoloLobby); btnOspita.disegna(window); btnUnisciti.disegna(window); window.draw(testoIstruzioniIP); window.draw(testoIP); btnIndietro.disegna(window); }
        else if (statoAttuale == ATTESA_CONNESSIONE) { window.draw(tramaSfondo); sf::Text testoAttesa(font, "IN ATTESA DELL'AVVERSARIO SULLA PORTA 53000...", 30); testoAttesa.setFillColor(sf::Color::Yellow); testoAttesa.setOutlineThickness(2.f); testoAttesa.setOutlineColor(sf::Color::Black); testoAttesa.setPosition({150.f, 300.f}); int puntini = (int)(timerAvviso.getElapsedTime().asSeconds() * 2) % 4; string strPuntini = ""; for(int p=0; p<puntini; p++) strPuntini += "."; testoAttesa.setString("IN ATTESA DELL'AVVERSARIO" + strPuntini); window.draw(testoAttesa); btnIndietro.disegna(window); }
        else if ((statoAttuale == IN_CORSO || statoAttuale == IN_CORSO_RETE) && gioco != nullptr) {
            bool latoOscuro = gioco->getLatoOscuroAttivo(); int numGiocatori = gioco->getGiocatori().size();
            sfondoPrincipale.setFillColor(latoOscuro ? sf::Color(40, 20, 40) : sf::Color(20, 60, 80)); window.draw(sfondoPrincipale); window.draw(tramaSfondo); tavoloCentrale.setFillColor(latoOscuro ? sf::Color(25, 10, 25) : sf::Color(20, 100, 40)); window.draw(tavoloCentrale); mazzoGrafico.setFillColor(latoOscuro ? sf::Color(80, 0, 120) : sf::Color(10, 30, 150)); ovaleMazzo.setFillColor(latoOscuro ? sf::Color(40, 0, 60) : sf::Color(0, 15, 80)); indicatoreColore.setFillColor(getSFMLColor(gioco->getColoreAttivo())); window.draw(mazzoOmbra); window.draw(mazzoGrafico); window.draw(bordinoMazzo); window.draw(ovaleMazzo); window.draw(testoMazzo); disegnaCartaFronte(window, gioco->getCartaInCima(), latoOscuro, font, 480.f, 310.f); window.draw(indicatoreColore); window.draw(testoIndicatore);
            
            vector<Carta> manoBasso = gioco->getGiocatori()[idLocale].getMano(); float spaziaturaBasso = min(120.f, 800.f / max(1.f, (float)manoBasso.size())); float startXBasso = (1024.f - ((manoBasso.size() - 1) * spaziaturaBasso + 115.f)) / 2.f;
            for(int i = 0; i < manoBasso.size(); i++) { if (isSimulazione) { sf::RectangleShape rect({60.f, 90.f}); rect.setPosition({startXBasso + (i * spaziaturaBasso) + 27.5f, 650.f}); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); rect.setFillColor(getSFMLColor(manoBasso[i].getColore(!latoOscuro))); window.draw(rect); sf::Text valTxt(font, ottieniSimboloBreve(manoBasso[i], latoOscuro), 22); valTxt.setFillColor(sf::Color::White); valTxt.setOutlineColor(sf::Color::Black); valTxt.setOutlineThickness(2.f); valTxt.setOrigin({valTxt.getLocalBounds().position.x + valTxt.getLocalBounds().size.x / 2.f, valTxt.getLocalBounds().position.y + valTxt.getLocalBounds().size.y / 2.f}); valTxt.setPosition({rect.getPosition().x + 30.f, rect.getPosition().y + 40.f}); window.draw(valTxt); } else disegnaCartaFronte(window, manoBasso[i], latoOscuro, font, startXBasso + (i * spaziaturaBasso), 600.f); }
            if (numGiocatori > 1) { if (numGiocatori == 2) { int idAvversario = isMultiplayer ? (isServer ? 1 : 0) : 1; vector<Carta> manoAlto = gioco->getGiocatori()[idAvversario].getMano(); float spaziaturaAlto = min(50.f, 600.f / max(1.f, (float)manoAlto.size())); float startXAlto = (1024.f - ((manoAlto.size() - 1) * spaziaturaAlto + 115.f)) / 2.f; for(int i = 0; i < manoAlto.size(); i++) { if (isSimulazione) disegnaCartaFronte(window, manoAlto[i], latoOscuro, font, startXAlto + (i * spaziaturaAlto), -30.f); else { sf::RectangleShape rect({60.f, 90.f}); rect.setPosition({startXAlto + (i * spaziaturaAlto), -10.f}); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); rect.setFillColor(getSFMLColor(manoAlto[i].getColore(!latoOscuro))); window.draw(rect); } } } else { vector<Carta> manoSx = gioco->getGiocatori()[1].getMano(); float spaziaturaSx = min(40.f, 400.f / max(1.f, (float)manoSx.size())); float startYSx = (768.f - ((manoSx.size() - 1) * spaziaturaSx + 60.f)) / 2.f; for(int i = 0; i < manoSx.size(); i++) { if (isSimulazione) { sf::Transform t; t.translate(sf::Vector2f(80.f, startYSx + (i * spaziaturaSx))); t.rotate(sf::degrees(90.f)); t.translate(sf::Vector2f(-57.f, -75.f)); sf::RectangleShape rect({115.f, 150.f}); rect.setFillColor(getSFMLColor(manoSx[i].getColore(latoOscuro))); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); window.draw(rect, t); sf::Text valTxt(font, ottieniSimboloBreve(manoSx[i], latoOscuro), 22); valTxt.setFillColor(sf::Color::White); valTxt.setOutlineColor(sf::Color::Black); valTxt.setOutlineThickness(2.f); valTxt.setOrigin({valTxt.getLocalBounds().position.x + valTxt.getLocalBounds().size.x/2.f, valTxt.getLocalBounds().position.y + valTxt.getLocalBounds().size.y/2.f}); valTxt.setPosition({80.f, startYSx + (i * spaziaturaSx)}); valTxt.setRotation(sf::degrees(90.f)); window.draw(valTxt); } else { sf::RectangleShape rect({60.f, 90.f}); rect.setOrigin({30.f, 45.f}); rect.setPosition({80.f, startYSx + (i * spaziaturaSx)}); rect.setRotation(sf::degrees(90.f)); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); rect.setFillColor(getSFMLColor(manoSx[i].getColore(!latoOscuro))); window.draw(rect); } } } }
            if (numGiocatori > 2) { vector<Carta> manoAlto = gioco->getGiocatori()[2].getMano(); float spaziaturaAlto = min(50.f, 600.f / max(1.f, (float)manoAlto.size())); float startXAlto = (1024.f - ((manoAlto.size() - 1) * spaziaturaAlto + 115.f)) / 2.f; for(int i = 0; i < manoAlto.size(); i++) { if (isSimulazione) disegnaCartaFronte(window, manoAlto[i], latoOscuro, font, startXAlto + (i * spaziaturaAlto), -30.f); else { sf::RectangleShape rect({60.f, 90.f}); rect.setPosition({startXAlto + (i * spaziaturaAlto), -10.f}); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); rect.setFillColor(getSFMLColor(manoAlto[i].getColore(!latoOscuro))); window.draw(rect); } } }
            if (numGiocatori > 3) { vector<Carta> manoDx = gioco->getGiocatori()[3].getMano(); float spaziaturaDx = min(40.f, 400.f / max(1.f, (float)manoDx.size())); float startYDx = (768.f - ((manoDx.size() - 1) * spaziaturaDx + 60.f)) / 2.f; for(int i = 0; i < manoDx.size(); i++) { if (isSimulazione) { sf::Transform t; t.translate(sf::Vector2f(944.f, startYDx + (i * spaziaturaDx))); t.rotate(sf::degrees(-90.f)); t.translate(sf::Vector2f(-57.f, -75.f)); sf::RectangleShape rect({115.f, 150.f}); rect.setFillColor(getSFMLColor(manoDx[i].getColore(latoOscuro))); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); window.draw(rect, t); sf::Text valTxt(font, ottieniSimboloBreve(manoDx[i], latoOscuro), 22); valTxt.setFillColor(sf::Color::White); valTxt.setOutlineColor(sf::Color::Black); valTxt.setOutlineThickness(2.f); valTxt.setOrigin({valTxt.getLocalBounds().position.x + valTxt.getLocalBounds().size.x/2.f, valTxt.getLocalBounds().position.y + valTxt.getLocalBounds().size.y/2.f}); valTxt.setPosition({944.f, startYDx + (i * spaziaturaDx)}); valTxt.setRotation(sf::degrees(-90.f)); window.draw(valTxt); } else { sf::RectangleShape rect({60.f, 90.f}); rect.setOrigin({30.f, 45.f}); rect.setPosition({1024.f - 80.f, startYDx + (i * spaziaturaDx)}); rect.setRotation(sf::degrees(-90.f)); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White); rect.setFillColor(getSFMLColor(manoDx[i].getColore(!latoOscuro))); window.draw(rect); } } }
            
            if (isSimulazione) { sf::Text txtL(font, "VELOCITA':", 16); txtL.setPosition({680.f, 25.f}); window.draw(txtL); for(int i=0; i<4; i++) { if ((i==0 && speedMultiplier==1.f) || (i==1 && speedMultiplier==2.f) || (i==2 && speedMultiplier==4.f) || (i==3 && speedMultiplier>=100.f)) btnVelocita[i].setFillColor(sf::Color::Green); else btnVelocita[i].setFillColor(sf::Color(50, 50, 50)); window.draw(btnVelocita[i]); window.draw(txtVelocita[i]); } }
            sf::Text testoSenso(font, gioco->getSensoOrario() ? "GIRO: ORARIO >>" : "<< GIRO: ANTIORARIO", 18); testoSenso.setFillColor(gioco->getSensoOrario() ? sf::Color(100, 255, 100) : sf::Color(100, 255, 255)); testoSenso.setOutlineColor(sf::Color::Black); testoSenso.setOutlineThickness(2.f); testoSenso.setPosition({420.f, 240.f}); window.draw(testoSenso);
            sf::CircleShape markerTurno(15.f, 3); markerTurno.setFillColor(sf::Color::Yellow); markerTurno.setOutlineThickness(3.f); markerTurno.setOutlineColor(sf::Color::Red); markerTurno.setOrigin({15.f, 15.f});
            int turno = gioco->getTurnoCorrente(); if (turno == idLocale) { markerTurno.setPosition({512.f, 570.f}); markerTurno.setRotation(sf::degrees(180.f)); } else if (numGiocatori == 2 && turno != idLocale) { markerTurno.setPosition({512.f, 170.f}); markerTurno.setRotation(sf::degrees(0.f)); } else if (numGiocatori == 4) { if (turno == 1) { markerTurno.setPosition({180.f, 384.f}); markerTurno.setRotation(sf::degrees(-90.f)); } else if (turno == 2) { markerTurno.setPosition({512.f, 170.f}); markerTurno.setRotation(sf::degrees(0.f)); } else if (turno == 3) { markerTurno.setPosition({844.f, 384.f}); markerTurno.setRotation(sf::degrees(90.f)); } } window.draw(markerTurno);
            testoBotLog.setString(gioco->getUltimoLogBot()); if (penalitaInflitta) testoFeedback.setString("Penalita' per non aver detto UNO: +2 Carte!"); else { string nomeG = gioco->getGiocatori()[gioco->getTurnoCorrente()].getNome(); if (turno == idLocale) testoFeedback.setString("Tocca a te! Fai la tua mossa."); else { if (tempoAttesaCalcolato > 2.0f && botPensando) testoFeedback.setString(nomeG + " sta pensando intensamente..."); else testoFeedback.setString("In attesa di " + nomeG + "..."); } } window.draw(testoFeedback); window.draw(testoBotLog); 
            if (manoBasso.size() == 2 && turno == idLocale) { window.draw(btnUno); window.draw(testoUno); }
            if (modalitaSceltaColore) { if (!latoOscuro) { btnColori[0].setFillColor(sf::Color(220, 20, 20)); btnColori[1].setFillColor(sf::Color(240, 190, 0)); btnColori[2].setFillColor(sf::Color(20, 160, 20)); btnColori[3].setFillColor(sf::Color(20, 50, 220)); } else { btnColori[0].setFillColor(sf::Color(255, 60, 150)); btnColori[1].setFillColor(sf::Color(0, 150, 136)); btnColori[2].setFillColor(sf::Color(255, 100, 0)); btnColori[3].setFillColor(sf::Color(100, 20, 150)); } for(int i=0; i<4; i++) window.draw(btnColori[i]); }
            
            btnAbbandona.disegna(window);
            if(idDimenticone != -1 && idDimenticone != idLocale && !isSimulazione) btnContesta.disegna(window);

            if (avvisoInCorso) { window.draw(overlayScurito); window.draw(testoPopUp); }
            if (animazioneUnoAttiva) { float elapsed = timerAnimazioneUno.getElapsedTime().asSeconds(); if (elapsed < 1.0f) { window.draw(overlayScurito); float scala = 1.0f + (elapsed * 2.0f); int alpha = 255 - (int)(elapsed * 255.f); testoAnimazioneUno.setScale({scala, scala}); testoAnimazioneUno.setFillColor(sf::Color(255, 255, 0, alpha)); testoAnimazioneUno.setOutlineColor(sf::Color(255, 0, 0, alpha)); window.draw(testoAnimazioneUno); } else { animazioneUnoAttiva = false; } }
        }
        else if (statoAttuale == FINE) { window.draw(tramaSfondo); window.draw(testoVincitore); window.draw(pannelloClassifica); window.draw(testoTitoloClassifica); disegnaTabellaClassifica(window, font, datiClassificaRecenti); btnTornaMenu.disegna(window); }

        window.display();             
    }

    if (gioco != nullptr) delete gioco;
    return 0;
}