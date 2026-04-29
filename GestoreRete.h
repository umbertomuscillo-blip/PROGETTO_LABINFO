/**
 * @file GestoreRete.h
 * @brief Classi per gestire il Multiplayer tramite SFML Network
 */

#ifndef GESTORERETE_H
#define GESTORERETE_H

#include <SFML/Network.hpp>
#include <string>

class ServerRete {
private:
    sf::TcpListener listener;
    sf::TcpSocket socketClient;
    bool connesso;

public:
    ServerRete();
    bool preparaServer(unsigned short porta = 53000);
    bool attendiClient(); // Questa ora sarà NON bloccante!
    bool inviaMessaggio(std::string msg);
    std::string riceviMessaggio();
    bool isConnesso();
    void scollega();
};

class ClientRete {
private:
    sf::TcpSocket socket;
    bool connesso;

public:
    ClientRete();
    bool connetti(std::string ip, unsigned short porta = 53000);
    bool inviaMessaggio(std::string msg);
    std::string riceviMessaggio();
    bool isConnesso();
    void scollega();
};

#endif