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
                                it->setAddress(it->getSocket()->getRemoteAddress().toString());
                                for (std::vector<Player>::iterator it2 = players.begin(); it2 != players.end(); ++it2) {
                                    if (it != it2) {
                                        sf::Packet sendNamePacket;
                                        sf::Packet sendNamePacket2;
                                        PacketPrefix prefix2 = PLAYER_INFO;
                                        sendNamePacket << static_cast<int>(prefix2) << it->getName() << it->getUniqueID() << it->getAddress();
                                        it2->getSocket()->send(sendNamePacket);
                                        sendNamePacket2 << static_cast<int>(prefix2) << it2->getName() << it2->getUniqueID() << it2->getAddress();
                                        it->getSocket()->send(sendNamePacket2);
                                    } else {
                                        sf::Packet sendIDPacket; 
                                        PacketPrefix prefix = PLAYER_ID;
                                        sendIDPacket << static_cast<int>(prefix) << it->getUniqueID();
                                        it->getSocket()->send(sendIDPacket);
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

    Deck drawDeck(false);
    Deck playDeck(true); 
    drawDeck.shuffle();
    dealSevenCards(drawDeck);
    drawFirstCard(drawDeck, playDeck);
    std::vector<Player>::iterator turnIt = players.begin();
    sendTurnInfo(turnIt);

    bool running = true;
    while (running) {
        if (players.size() == 0) running = false;
        if (selector.wait()) {
            for (auto it = players.begin(); it != players.end(); ++it) {
                if (it == turnIt) it->isPlaying = true;
                else it->isPlaying = false;
                if (selector.isReady(*(it->getSocket()))) {
                    sf::Packet packet;
                    sf::Socket::Status status = it->getSocket()->receive(packet);
                    if (status == sf::Socket::Done) processPacket(packet, it, turnIt, playDeck);
                    else if (status = sf::Socket::Disconnected) playerDisconnected(*it);
                }
            }
        }
    }
}

void Server::dealSevenCards(Deck &deck) {

    for (auto &player : players) {
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

    Card drawnCard;
    do {
        drawnCard = drawDeck.draw();
        playDeck.addCardOnTop(drawnCard);
    } while (drawnCard.getColor() == Card::Color::BLACK ||
             drawnCard.getValue() == Card::Value::SKIP ||
             drawnCard.getValue() == Card::Value::REVERSE ||
             drawnCard.getValue() == Card::Value::DRAW_TWO);

    for (auto p : players) {
        sf::Packet packet;
        PacketPrefix prefix = CARD_PLAYED;
        packet << static_cast<int>(prefix) << drawnCard;
        p.getSocket()->send(packet);
    }
}

void Server::processPacket(sf::Packet& packet, std::vector<Player>::iterator player, std::vector<Player>::iterator turnIt, Deck& playDeck) {

    int prefix;
    packet >> prefix;

    switch (static_cast<PacketPrefix>(prefix)) {
        case CARD_PLAYED:
            if (player != turnIt) break;
            Card card;
            Card topCard = playDeck.getCards().back();
            packet >> card;
            if (card.getValue() == topCard.getValue() || card.getColor() == topCard.getColor() || card.getColor() == Card::Color::BLACK) {
                if (card.getColor() == Card::Color::BLACK) {
                    if (card.getValue() == Card::Value::WILD) {
                        sf::Packet packet;
                        PacketPrefix prefix = CHOOSE_COLOR;
                        player->getSocket()->send(packet);
                        break;
                    } else if (card.getValue() == Card::Value::WILD_FOUR) {

                    } 
                }
            } else if (topCard.getColor() == Card::Color::BLACK && card.getColor() == topColor)
            break;
        case COLOR_CHOSEN:
            int colorChosen;
            topColor = static_cast<Card::Color>(colorChosen);
            packet >> colorChosen;
            sf::Packet packet;
            PacketPrefix prefix = TOP_COLOR_CHANGE;
            packet << static_cast<int>(prefix) << colorChosen;
            for (auto p : players) p.getSocket()->send(packet);
    }
}

void Server::playerDisconnected(Player player) {

    PacketPrefix prefix = PLAYER_DISCONNECTED;
    sf::Packet packet;
    packet << static_cast<int>(prefix) << player.getUniqueID();
    for (auto p : players) {
        if (p.getUniqueID() != player.getUniqueID()) p.getSocket()->send(packet);
    }
}

void Server::sendTurnInfo(std::vector<Player>::iterator turnIt) {

    sf::Packet packet;
    PacketPrefix prefix = TURN_INFO;
    packet << static_cast<int>(prefix) << turnIt->getUniqueID();
    for (auto p : players) p.getSocket()->send(packet);
}
