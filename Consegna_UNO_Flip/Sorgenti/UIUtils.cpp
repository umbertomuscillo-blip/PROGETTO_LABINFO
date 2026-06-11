#include "UIUtils.h"
#include "SoundManager.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>

using namespace std;

RoundedRectangleShape::RoundedRectangleShape(const sf::Vector2f& size, float radius, unsigned int cornerPointCount)
    : m_size(size), m_radius(radius), m_cornerPointCount(cornerPointCount)
{
    update();
}

void RoundedRectangleShape::setSize(const sf::Vector2f& size) {
    m_size = size;
    update();
}

void RoundedRectangleShape::setCornerRadius(float radius) {
    m_radius = radius;
    update();
}

std::size_t RoundedRectangleShape::getPointCount() const {
    return m_cornerPointCount * 4;
}

sf::Vector2f RoundedRectangleShape::getPoint(std::size_t index) const {
    if (index >= m_cornerPointCount * 4) return {0, 0};
    
    float deltaAngle = 90.0f / (m_cornerPointCount - 1);
    sf::Vector2f center;
    unsigned int centerIndex = index / m_cornerPointCount;
    static const float pi = 3.141592654f;

    switch (centerIndex) {
        case 0: center = sf::Vector2f(m_size.x - m_radius, m_radius); break;
        case 1: center = sf::Vector2f(m_radius, m_radius); break;
        case 2: center = sf::Vector2f(m_radius, m_size.y - m_radius); break;
        case 3: center = sf::Vector2f(m_size.x - m_radius, m_size.y - m_radius); break;
    }

    float angle = deltaAngle * (index - centerIndex * m_cornerPointCount) + centerIndex * 90.0f;
    return {
        center.x + m_radius * std::cos(angle * pi / 180.f),
        center.y - m_radius * std::sin(angle * pi / 180.f)
    };
}

Bottone::Bottone(const sf::Font& font) : testo(font), scaleAttuale(1.0f), scaleTarget(1.0f) {}

void Bottone::setup(string etichetta, float x, float y, float width, float height, sf::Color base, sf::Color hover) {
    coloreBase = base; coloreHover = hover;
    rect = RoundedRectangleShape({width, height}, 15.f, 20); 
    rect.setOrigin({width / 2.f, height / 2.f});
    rect.setPosition({x + width / 2.f, y + height / 2.f}); 
    rect.setFillColor(coloreBase);
    rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color(255, 255, 255, 150)); 
    testo.setString(etichetta); testo.setCharacterSize(20); testo.setFillColor(sf::Color::White);
    sf::FloatRect bounds = testo.getLocalBounds();
    testo.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
    testo.setPosition({x + width / 2.f, y + height / 2.f});
}

void Bottone::aggiorna(sf::Vector2f mousePos, float deltaTime) {
    if (rect.getGlobalBounds().contains(mousePos)) { 
        rect.setFillColor(coloreHover); rect.setOutlineColor(sf::Color::White); rect.setOutlineThickness(3.f); 
        scaleTarget = 1.05f;
    } 
    else { 
        rect.setFillColor(coloreBase); rect.setOutlineColor(sf::Color(255, 255, 255, 100)); rect.setOutlineThickness(2.f); 
        scaleTarget = 1.0f;
    }
    
    // Smooth scaling
    scaleAttuale += (scaleTarget - scaleAttuale) * 15.0f * deltaTime;
    rect.setScale({scaleAttuale, scaleAttuale});
    testo.setScale({scaleAttuale, scaleAttuale});
}

void Bottone::disegna(sf::RenderWindow& window) { window.draw(rect); window.draw(testo); }

bool Bottone::cliccato(sf::Vector2f mousePos) { 
    bool clicked = rect.getGlobalBounds().contains(mousePos); 
    if(clicked) SoundManager::getInstance().playSound("click");
    return clicked;
}

void ParticleSystem::init(int count, sf::Vector2f area, bool isFireworks) {
    particelle.clear();
    for (int i = 0; i < count; i++) {
        Particella p;
        p.pos = { (float)(rand() % (int)area.x), (float)(rand() % (int)area.y) };
        if (isFireworks) {
            p.pos = { area.x / 2.f, area.y };
            p.vel = { (float)((rand() % 400) - 200), (float)(-(rand() % 400 + 300)) };
        } else {
            p.vel = { (float)((rand() % 50) - 25), (float)((rand() % 50) - 25) };
        }
        p.size = (float)(rand() % 6 + 2);
        p.alpha = 255.f;
        p.lifetime = isFireworks ? (float)(rand() % 200 + 100) / 100.f : 9999.f;
        
        int colorType = rand() % 4;
        if (colorType == 0) p.color = sf::Color(255, 60, 150, 150); // Pink
        else if (colorType == 1) p.color = sf::Color(0, 150, 136, 150); // Teal
        else if (colorType == 2) p.color = sf::Color(255, 215, 0, 150); // Gold
        else p.color = sf::Color(100, 20, 150, 150); // Purple

        particelle.push_back(p);
    }
}

void ParticleSystem::update(float deltaTime, bool isFireworks) {
    for (auto& p : particelle) {
        p.pos += p.vel * deltaTime;
        if (isFireworks) {
            p.vel.y += 300.f * deltaTime; // Gravity
            p.lifetime -= deltaTime;
            if (p.lifetime <= 0) {
                p.pos = { 512.f, 768.f };
                p.vel = { (float)((rand() % 600) - 300), (float)(-(rand() % 400 + 400)) };
                p.lifetime = (float)(rand() % 200 + 100) / 100.f;
            }
        } else {
            // Bounce off walls
            if (p.pos.x < 0 || p.pos.x > 1024) p.vel.x = -p.vel.x;
            if (p.pos.y < 0 || p.pos.y > 768) p.vel.y = -p.vel.y;
        }
    }
}

void ParticleSystem::draw(sf::RenderWindow& window) {
    for (const auto& p : particelle) {
        rectShape.setPosition(p.pos);
        rectShape.setSize({p.size, p.size});
        rectShape.setOrigin({p.size / 2.f, p.size / 2.f});
        rectShape.setFillColor(p.color);
        window.draw(rectShape);
    }
}

vector<string> splittaStringa(string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token; vector<string> res;
    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) { token = s.substr(pos_start, pos_end - pos_start); pos_start = pos_end + delim_len; res.push_back(token); } res.push_back(s.substr(pos_start)); return res;
}

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

void disegnaCartaFronte(sf::RenderWindow& window, Carta cartaDati, bool latoOscuro, sf::Font& font, float x, float y, float scale) {
    string desc = latoOscuro ? cartaDati.getDescrizioneOscura() : cartaDati.getDescrizioneChiara(); sf::Color bgColor = sf::Color(30, 30, 30); 
    if (desc.find("Rosso") != string::npos) bgColor = sf::Color(220, 20, 20); else if (desc.find("Blu") != string::npos) bgColor = sf::Color(20, 50, 220); else if (desc.find("Verde Acqua") != string::npos) bgColor = sf::Color(0, 150, 136); else if (desc.find("Verde") != string::npos) bgColor = sf::Color(20, 160, 20); else if (desc.find("Giallo") != string::npos) bgColor = sf::Color(240, 190, 0); else if (desc.find("Rosa") != string::npos) bgColor = sf::Color(255, 60, 150); else if (desc.find("Arancione") != string::npos) bgColor = sf::Color(255, 100, 0); else if (desc.find("Viola") != string::npos) bgColor = sf::Color(100, 20, 150);
    
    sf::Transform t; t.translate(sf::Vector2f(x, y)); t.scale(sf::Vector2f(scale, scale));
    
    sf::RectangleShape ombra({115.f, 150.f}); ombra.setPosition({4.f, 4.f}); ombra.setFillColor(sf::Color(0, 0, 0, 120)); 
    sf::RectangleShape rect({115.f, 150.f}); rect.setFillColor(bgColor); rect.setOutlineThickness(2.f); rect.setOutlineColor(sf::Color::White);
    sf::RectangleShape bordino({105.f, 140.f}); bordino.setPosition({5.f, 5.f}); bordino.setFillColor(sf::Color::Transparent); bordino.setOutlineThickness(1.5f); bordino.setOutlineColor(sf::Color(255, 255, 255, 120));
    sf::CircleShape ovale(40.f); ovale.setScale({0.85f, 1.2f}); ovale.setOrigin({40.f, 40.f}); ovale.setPosition({115.f/2.f, 150.f/2.f}); ovale.setRotation(sf::degrees(-20.f)); ovale.setFillColor(sf::Color::White);

    string simbolo = ottieniSimboloBreve(cartaDati, latoOscuro); int sizeCentro = 42; int sizeAngoli = 18; if (simbolo.length() >= 4) { sizeCentro = 22; sizeAngoli = 12; } else if (simbolo.length() == 3) { sizeCentro = 26; sizeAngoli = 14; } else if (simbolo.length() == 2) { sizeCentro = 34; sizeAngoli = 16; }

    sf::Text txtCentro(font, simbolo, sizeCentro); txtCentro.setFillColor(bgColor == sf::Color(30, 30, 30) ? sf::Color::Black : bgColor); sf::FloatRect bounds = txtCentro.getLocalBounds(); txtCentro.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f}); txtCentro.setPosition({115.f/2.f, 150.f/2.f}); txtCentro.setRotation(sf::degrees(-20.f));
    sf::Text txtTL(font, simbolo, sizeAngoli); txtTL.setFillColor(sf::Color::White); txtTL.setOutlineColor(sf::Color::Black); txtTL.setOutlineThickness(1.2f); sf::FloatRect boundsTL = txtTL.getLocalBounds(); txtTL.setOrigin({boundsTL.position.x + boundsTL.size.x / 2.f, boundsTL.position.y + boundsTL.size.y / 2.f}); txtTL.setPosition({16.f, 16.f}); 
    sf::Text txtBR(font, simbolo, sizeAngoli); txtBR.setFillColor(sf::Color::White); txtBR.setOutlineColor(sf::Color::Black); txtBR.setOutlineThickness(1.2f); sf::FloatRect boundsBR = txtBR.getLocalBounds(); txtBR.setOrigin({boundsBR.position.x + boundsBR.size.x / 2.f, boundsBR.position.y + boundsBR.size.y / 2.f}); txtBR.setPosition({115.f - 16.f, 150.f - 16.f}); txtBR.setRotation(sf::degrees(180.f));

    window.draw(ombra, t); window.draw(rect, t); window.draw(bordino, t); window.draw(ovale, t); window.draw(txtCentro, t); window.draw(txtTL, t); window.draw(txtBR, t);
}

sf::Color getSFMLColor(Colore c) {
    switch(c) {
        case ROSSO: return sf::Color(220, 20, 20); case BLU: return sf::Color(20, 50, 220); case VERDE: return sf::Color(20, 160, 20); case GIALLO: return sf::Color(240, 190, 0);
        case ROSA: return sf::Color(255, 60, 150); case VERDE_ACQUA: return sf::Color(0, 150, 136); case ARANCIONE: return sf::Color(255, 100, 0); case VIOLA: return sf::Color(100, 20, 150); default: return sf::Color(30, 30, 30); 
    }
}

void disegnaAvatarAvversario(sf::RenderWindow& window, sf::Font& font, string nome, int numCarte, float x, float y, bool turnoSuo) {
    sf::CircleShape avatar(35.f);
    avatar.setPosition({x, y});
    avatar.setOrigin({35.f, 35.f});
    avatar.setFillColor(sf::Color(30, 30, 50, 220));
    avatar.setOutlineThickness(4.f);
    if (turnoSuo) {
        avatar.setOutlineColor(sf::Color(255, 215, 0)); // Glow dorato se è il suo turno
        // Un alone luminoso "fake" usando un cerchio leggermente più grande
        sf::CircleShape glow(45.f);
        glow.setPosition({x, y});
        glow.setOrigin({45.f, 45.f});
        glow.setFillColor(sf::Color(255, 215, 0, 80));
        window.draw(glow);
    } else {
        avatar.setOutlineColor(sf::Color(100, 100, 150));
    }
    
    // Iniziale del nome
    string iniziale = nome.empty() ? "?" : nome.substr(0, 1);
    if (nome.find("Bot ") == 0 && nome.length() > 4) iniziale = nome.substr(4, 1); // Prende la lettera del bot
    
    sf::Text txtNome(font, iniziale, 32);
    txtNome.setFillColor(sf::Color::White);
    txtNome.setOrigin({txtNome.getLocalBounds().size.x / 2.f, txtNome.getLocalBounds().size.y / 2.f});
    txtNome.setPosition({x - 2.f, y - 8.f});
    
    // Badge rosso con numero di carte
    sf::CircleShape badge(16.f);
    badge.setFillColor(sf::Color(220, 20, 20));
    badge.setOutlineThickness(2.f);
    badge.setOutlineColor(sf::Color::White);
    badge.setOrigin({16.f, 16.f});
    badge.setPosition({x + 25.f, y + 25.f});
    
    sf::Text txtNum(font, to_string(numCarte), 16);
    txtNum.setFillColor(sf::Color::White);
    txtNum.setOrigin({txtNum.getLocalBounds().size.x / 2.f, txtNum.getLocalBounds().size.y / 2.f});
    txtNum.setPosition({x + 25.f, y + 22.f});
    
    // Etichetta del nome in basso (opzionale)
    sf::Text txtFullName(font, nome, 14);
    txtFullName.setFillColor(sf::Color(200, 200, 200));
    txtFullName.setOrigin({txtFullName.getLocalBounds().size.x / 2.f, 0.f});
    txtFullName.setPosition({x, y + 40.f});
    
    window.draw(avatar);
    window.draw(txtNome);
    window.draw(badge);
    window.draw(txtNum);
    window.draw(txtFullName);
}
