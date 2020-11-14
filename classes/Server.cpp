#include "Server.h"

Server::Server(sf::TcpListener *listenerInit) : listener(*listenerInit) {}
void Server::addPlayer(sf::TcpSocket *socket) {
    Player player;
    player.setSocket(socket);
    players.push_back(player);
}

void Server::addPlayer(Player player) {
    players.push_back(player);
}

std::vector<Player> Server::getPlayers() { return players; }

void Server::lobby() {

    int ID = 0;
    if (listener.listen(PORT) != sf::Socket::Done) {
        std::cout << "erruer au listen du listener" << std::endl;
    }
    selector.add(listener);

    bool running = true;

    while (running) {
        if (selector.wait()) {
            if (selector.isReady(listener)) {
                sf::TcpSocket *client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done) {
                    std::cout << "new connection" << std::endl;
                    Player player;
                    player.setSocket(client);
                    players.push_back(player);
                    selector.add(*(player.getSocket()));
                } else delete client;
            } else {
                for (std::vector<Player>::iterator it = players.begin(); it != players.end(); ++it) {
                    if (selector.isReady(*(it->getSocket()))) {
                        sf::Packet packet;
                        sf::Socket::Status status = it->getSocket()->receive(packet);
                        if (status == sf::Socket::Done) {
                            int prefix;
                            packet >> prefix;
                            if (static_cast<PacketPrefix>(prefix) == PLAYER_INFO) {
                                std::string name;
                                packet >> name;
                                it->setName(name);
                                it->setUniqueID(ID++);
                                for (std::vector<Player>::iterator it2 = players.begin(); it2 != players.end(); ++it2) {
                                    std::cout << "it2:" << it2->getName() << " it:" << it->getName() << std::endl;
                                    if (it != it2) {
                                        sf::Packet sendNamePacket;
                                        sf::Packet sendNamePacket2;
                                        PacketPrefix prefix2 = PLAYER_INFO;
                                        sendNamePacket << static_cast<int>(prefix2) << it->getName() << it->getUniqueID();
                                        it2->getSocket()->send(sendNamePacket);
                                        sendNamePacket2 << static_cast<int>(prefix2) << it2->getName() << it->getUniqueID();
                                        it->getSocket()->send(sendNamePacket2);
                                    }
                                }
                            } else if (static_cast<PacketPrefix>(prefix) == GAME_START) {
                                sf::Packet startGamePacket;
                                PacketPrefix prefix = GAME_START;
                                startGamePacket << static_cast<int>(prefix);
                                for (std::vector<Player>::iterator it2 = players.begin(); it2 != players.end(); ++it2) {
                                    if (it != it2) it2->getSocket()->send(startGamePacket);
                                }
                                game();
                                return;
                            }
                        } else if (status == sf::Socket::Disconnected) {
                            sf::Packet packet;
                            PacketPrefix prefix = PLAYER_DISCONNECTED;
                            packet << static_cast<int>(prefix) << it->getUniqueID();
                            for (std::vector<Player>::iterator it2 = players.begin(); it2 != players.end(); ++it2) {
                               if (it != it2) it2->getSocket()->send(packet);
                            }
                            std::cout << "server: disconnected: " << it->getName() << std::endl;
                            players.erase(it--);
                        }
                    }
                }
            }
        }
    }
}

void Server::game() {

    std::cout << "server: game is about to start!" << std::endl;
    Deck drawDeck(false);
    Deck playDeck(true); 
    drawDeck.shuffle();
    dealSevenCards(drawDeck);
    drawFirstCard(drawDeck, playDeck);

    bool running = true;
    while (running) {
        if (players.size() == 0) running = false;
        if (selector.wait()) {
            for (auto &player : players) {

            }
        }
    }
}

void Server::dealSevenCards(Deck &deck) {

    for (auto &player : players) {
        std::cout << "server: sending 7 cards to: " << player.getName() << std::endl;
        for (int i = 0; i < 7; ++i) {
            Card drawnCard = deck.draw();
            PacketPrefix prefix = CARD_DRAWN;
            sf::Packet initialDrawPacket;
            initialDrawPacket << static_cast<int>(prefix) << drawnCard;
            player.getSocket()->send(initialDrawPacket);
            player.addCardToHand(drawnCard);
        }
    }
}

void Server::drawFirstCard(Deck &drawDeck, Deck &playDeck) {

    Card drawnCard = drawDeck.draw();
    playDeck.addCardOnTop(drawnCard);

    for (auto p : players) {
        sf::Packet packet;
        PacketPrefix prefix = CARD_PLAYED;
        packet << static_cast<int>(prefix) << drawnCard;
        p.getSocket()->send(packet);
    }
}
