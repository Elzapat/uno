#ifndef SERVER_H
#define SERVER_H

#include "../SFML/include/SFML/Network.hpp"
#include "Player.h"
#include "Card.h"
#include "Deck.h"
#include <iostream>
#include <string>

class Server {

    public:
                                Server              (sf::TcpListener *listenerInit);
        enum PacketPrefix {
            PLAYER_INFO,
            PLAYER_DISCONNECTED,
            GAME_START,
            CARD_DRAWN,
            CARD_PLAYED
        };
        void                    lobby               ();
        void                    game                ();
        void                    waitForConnections  ();
        void                    waitForPackets      ();
        void                    processPacket       (sf::Packet packet);
        void                    addPlayer           (sf::TcpSocket *socket);
        void                    addPlayer           (Player player);
        std::vector<Player>     getPlayers          ();
        static const unsigned short PORT = 2905;

    private:
        std::vector<Player>     players;
        sf::TcpListener         &listener;
        sf::SocketSelector      selector;
        void                    dealSevenCards(Deck &deck);
        void                    drawFirstCard(Deck &drawDeck, Deck &playDeck);
};

#endif
