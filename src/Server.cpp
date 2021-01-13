#include "Server.h"

Server::Server(sf::TcpListener *listenerInit) : listener(*listenerInit), nbDrawCard(0) {}
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
    std::cout << "server ok" << std::endl;
    if (!listenerBound) {
        if (listener.listen(PORT) != sf::Socket::Done) {
            std::cout << "erruer au listen du listener" << std::endl;
        }
        selector.add(listener);
        listenerBound = true;
    }

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
                                reverseTurn = false;
                                game();
                                std::cout << "server: game finished" << std::endl;
                                for (auto p : players) std::cout << "server: player: " << p.getName() << std::endl;
                                for (auto& p : players) p.getHand().clear();
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

    std::cout << "server: game started" << std::endl;
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
                    if (status == sf::Socket::Done) {
                        int action;
                        action = processPacket(packet, it, turnIt, playDeck, drawDeck);
                        if (action == 2) {
                            std::cout << "server: action 2 choosen" << std::endl;
                            running = false;
                        } else continue;
                    } else if (status = sf::Socket::Disconnected) playerDisconnected(*it);
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
            initialDrawPacket << static_cast<int>(prefix) << drawnCard << player.getUniqueID();
            for (auto p : players) p.getSocket()->send(initialDrawPacket);
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

    topColor = drawnCard.getColor();

    for (auto p : players) {
        sf::Packet packet;
        PacketPrefix prefix = CARD_PLAYED;
        packet << static_cast<int>(prefix) << drawnCard;
        p.getSocket()->send(packet);
    }
}

int Server::processPacket(sf::Packet& packet, std::vector<Player>::iterator& player, std::vector<Player>::iterator& turnIt, Deck& playDeck, Deck& drawDeck) {

    int prefix;
    packet >> prefix;
    Card card;
    int color, value;
    Card topCard = playDeck.getCards().back();

    switch (static_cast<PacketPrefix>(prefix)) {
        case CARD_PLAYED:
            if (player->getUniqueID() != turnIt->getUniqueID()) break;
            packet >> color >> value;
            card = Card(static_cast<Card::Color>(color), static_cast<Card::Value>(value));
            if (card.getValue() == topCard.getValue() || card.getColor() == topCard.getColor() || card.getColor() == Card::Color::BLACK || (topCard.getColor() == Card::Color::BLACK && card.getColor() == topColor)) {
                if (card.getColor() == Card::Color::BLACK) {
                    sf::Packet packet;
                    PacketPrefix prefix = CHOOSE_COLOR;
                    packet << static_cast<int>(prefix); 
                    player->getSocket()->send(packet);
                    choosingColor = true;
                    if (card.getValue() == Card::Value::WILD_FOUR) {
                        passExtraTurn = true;
                        wildFourChosen = true;
                    }
                } else if (card.getValue() == Card::Value::SKIP) {
                    passExtraTurn = true;
                } else if (card.getValue() == Card::Value::REVERSE) {
                    if (reverseTurn) reverseTurn = false;
                    else reverseTurn = true;
                    if (players.size() == 2) passExtraTurn = true;
                } else if (card.getValue() == Card::Value::DRAW_TWO) {
                    passExtraTurn = true;
                    nbDrawCard += 2;
                }
                sf::Packet packet;
                PacketPrefix prefix = CARD_PLAYED;
                packet << static_cast<int>(prefix) << card << player->getUniqueID();
                for (auto p : players) p.getSocket()->send(packet);
                turnIt->removeCardFromHand(card);
                playDeck.addCardOnTop(card);
                topColor = card.getColor();
            
                if (turnIt->getHand().size() == 1) {
                    goingThroughUno = true;
                    sf::Packet unoPacket;
                    PacketPrefix unoPrefix = UNO;
                    unoPacket << static_cast<int>(unoPrefix);
                    turnIt->getSocket()->send(unoPacket);
                } else if (turnIt->getHand().size() == 0) {
                    computeScores();

                    sf::Packet winPacket;
                    PacketPrefix winPrefix = PLAYER_WON;
                    winPacket << static_cast<int>(winPrefix) << turnIt->getUniqueID();
                    for (auto p : players) p.getSocket()->send(winPacket);

                    return 2;
                }

                //std::cout << "server: 1pass extra turn in card played: " << passExtraTurn << std::endl;
                if (passExtraTurn && !goingThroughUno && !choosingColor) {
                    passTurn(turnIt);
                    for (int i = 0; i < nbDrawCard; ++i) drawACard(turnIt, drawDeck, playDeck);
                    nbDrawCard = 0;
                    passExtraTurn = false;
                }

                if (!goingThroughUno && !choosingColor) passTurn(turnIt);
               
                //std::cout << "server: 2pass extra turn in card played: " << passExtraTurn << std::endl;
                break;
            } 
            break;
        case DRAW_CARD:
            canPlay = drawACard(turnIt, drawDeck, playDeck);
            if (!canPlay) passTurn(turnIt);
            break;
        case UNO:
            packet >> pressedUno; 
            if (!pressedUno) {
                drawACard(turnIt, drawDeck, playDeck);
                drawACard(turnIt, drawDeck, playDeck);
            }
            if (passExtraTurn && !choosingColor) {
                passTurn(turnIt);
                for (int i = 0; i < nbDrawCard; ++i) drawACard(turnIt, drawDeck, playDeck);
                passExtraTurn = false;
            }
            if (wildFourChosen && !choosingColor) {
                for (int i = 0; i < 4; ++i) drawACard(turnIt, drawDeck, playDeck);
                wildFourChosen = false;
            }
            if (!choosingColor) passTurn(turnIt);
            nbDrawCard = 0;
            goingThroughUno = false;
            break;
        case COLOR_CHOSEN:
            int colorChosen;
            packet >> colorChosen;
            topColor = static_cast<Card::Color>(colorChosen);
            sf::Packet packet;
            PacketPrefix prefix = TOP_COLOR_CHANGE;
            packet << static_cast<int>(prefix) << colorChosen;
            for (auto p : players) p.getSocket()->send(packet);

            if (!goingThroughUno) {
                if (passExtraTurn) {
                    passTurn(turnIt);
                    for (int i = 0; i < nbDrawCard; ++i) drawACard(turnIt, drawDeck, playDeck);
                    passExtraTurn = false;
                }
                if (wildFourChosen) {
                    for (int i = 0; i < 4; ++i) drawACard(turnIt, drawDeck, playDeck);
                    wildFourChosen = false;
                }
                passTurn(turnIt);
            } else if (!goingThroughUno) passTurn(turnIt);
            nbDrawCard = 0;
            choosingColor = false;
            break;
    }
    return 0;
}

void Server::playerDisconnected(Player player) {

    PacketPrefix prefix = PLAYER_DISCONNECTED;
    sf::Packet packet;
    packet << static_cast<int>(prefix) << player.getUniqueID();
    for (auto p : players) {
        if (p.getUniqueID() != player.getUniqueID()) p.getSocket()->send(packet);
    }
}

void Server::sendTurnInfo(std::vector<Player>::iterator& turnIt) {

    sf::Packet packet;
    PacketPrefix prefix = TURN_INFO;
    packet << static_cast<int>(prefix) << turnIt->getUniqueID();
    for (auto p : players) p.getSocket()->send(packet);
}

void Server::passTurn(std::vector<Player>::iterator& turnIt) {

    if (reverseTurn && turnIt == players.begin()) turnIt = std::prev(players.end());
    else if (reverseTurn) turnIt--;
    else if (!reverseTurn && turnIt == std::prev(players.end())) turnIt = players.begin();
    else turnIt++;

    sf::Packet packet;
    PacketPrefix prefix = TURN_INFO;
    packet << static_cast<int>(prefix) << turnIt->getUniqueID();
    for (auto p : players) p.getSocket()->send(packet);
}

bool Server::drawACard(std::vector<Player>::iterator& player, Deck& drawDeck, Deck& playDeck) {

    bool canPlay = false;
    Card topCard = playDeck.getCards().back();
    Card drawnCard = drawDeck.draw();
    player->addCardToHand(drawnCard);
    if (drawnCard.getValue() == topCard.getValue() || drawnCard.getColor() == topCard.getColor() || topColor == drawnCard.getColor() || drawnCard.getColor() == Card::Color::BLACK) canPlay = true;
     else canPlay = false;
    sf::Packet packet;
    PacketPrefix prefix = CARD_DRAWN;
    packet << static_cast<int>(prefix) << drawnCard << player->getUniqueID();
    for (auto p : players) p.getSocket()->send(packet);

    if (drawDeck.getCards().size() == 0) {
        int playDeckSize = playDeck.getCards().size();
        for (int i = 0; i < playDeckSize - 1; ++i) {
            std::cout << i << std::endl;
            Card card = playDeck.removeFirstCard();
            drawDeck.addCardOnTop(card);
        }
        drawDeck.shuffle();
    }

    return canPlay;
}

void Server::computeScores() {

    for (Player& p : players) {
        int score = 0;    
        for (auto c : p.getHand()) {
            if (c.getValue() == Card::Value::SKIP || c.getValue() == Card::Value::REVERSE || c.getValue() == Card::Value::DRAW_TWO)
                score += 20;
            else if (c.getColor() == Card::Color::BLACK)
                score += 50;
            else score += static_cast<int>(c.getValue());
        }
        p.setScore(p.getScore() + score);

        sf::Packet scorePacket;
        PacketPrefix prefix = PLAYER_SCORE;
        scorePacket << static_cast<int>(prefix) << p.getUniqueID() << p.getScore();
        for (auto player : players) player.getSocket()->send(scorePacket);
    }
}
