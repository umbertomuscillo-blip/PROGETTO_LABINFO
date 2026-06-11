// GestoreRete.cpp
// Implementazione dei socket NON bloccanti.

#include "GestoreRete.h"
#include <iostream>
#include <optional>

using namespace std;

ServerRete::ServerRete() { connesso = false; }

bool ServerRete::preparaServer(unsigned short porta) {
    if (listener.listen(porta) != sf::Socket::Status::Done) {
        cout << "[SERVER-ERRORE] Impossibile aprire la porta " << porta << endl;
        return false;
    }
    
    // LA MAGIA: Il listener non bloccherà più il gioco!
    listener.setBlocking(false); 
    
    cout << "[SERVER] In ascolto sulla porta " << porta << "... In attesa del Client..." << endl;
    return true;
}

bool ServerRete::attendiClient() {
    // Prova ad accettare. Se non c'è nessuno, restituisce NotReady e il gioco continua a girare
    if (listener.accept(socketClient) == sf::Socket::Status::Done) {
        socketClient.setBlocking(false); // Rende non bloccanti anche i messaggi
        connesso = true;
        auto remoteAddress = socketClient.getRemoteAddress();
        if (remoteAddress.has_value()) {
            cout << "[SERVER] CLIENT CONNESSO! IP: " << remoteAddress.value() << endl;
        }
        return true;
    }
    return false; // Nessuno si è ancora collegato
}

bool ServerRete::inviaMessaggio(std::string msg) {
    if (!connesso) return false;
    sf::Packet pacchetto; pacchetto << msg;
    return (socketClient.send(pacchetto) == sf::Socket::Status::Done);
}

std::string ServerRete::riceviMessaggio() {
    if (!connesso) return "";
    sf::Packet pacchetto;
    if (socketClient.receive(pacchetto) == sf::Socket::Status::Done) {
        string msg; if (pacchetto >> msg) return msg;
    }
    return "";
}

bool ServerRete::isConnesso() { return connesso; }
void ServerRete::scollega() { socketClient.disconnect(); connesso = false; }


ClientRete::ClientRete() { connesso = false; }

bool ClientRete::connetti(std::string ip, unsigned short porta) {
    std::optional<sf::IpAddress> address = sf::IpAddress::resolve(ip);
    if (!address.has_value()) return false;

    // Se non trova il server, si arrende dopo 5 secondi, evitando freeze lunghi ma dando il tempo a Whisky di risolvere la rete
    if (socket.connect(address.value(), porta, sf::seconds(5.f)) != sf::Socket::Status::Done) {
        cout << "[CLIENT-ERRORE] Impossibile connettersi al Server!" << endl;
        return false;
    }
    
    socket.setBlocking(false); // Rende non bloccanti i messaggi
    connesso = true;
    cout << "[CLIENT] CONNESSO AL SERVER!" << endl;
    return true;
}

bool ClientRete::inviaMessaggio(std::string msg) {
    if (!connesso) return false;
    sf::Packet pacchetto; pacchetto << msg;
    return (socket.send(pacchetto) == sf::Socket::Status::Done);
}

std::string ClientRete::riceviMessaggio() {
    if (!connesso) return "";
    sf::Packet pacchetto;
    if (socket.receive(pacchetto) == sf::Socket::Status::Done) {
        string msg; if (pacchetto >> msg) return msg;
    }
    return "";
}

bool ClientRete::isConnesso() { return connesso; }
void ClientRete::scollega() { socket.disconnect(); connesso = false; }