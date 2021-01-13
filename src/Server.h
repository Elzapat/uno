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
            PLAYER_ID,
            GAME_START,
            DRAW_CARD,
            CARD_DRAWN,
            CARD_PLAYED,
            TURN_INFO,
            CHOOSE_COLOR,
            COLOR_CHOSEN,
            TOP_COLOR_CHANGE,
            UNO,
            PLAYER_WON,
            PLAYER_SCORE
        };
        void                    lobby               ();
        void                    game                ();
        void                    waitForConnections  ();
        void                    addPlayer           (sf::TcpSocket *socket);
        void                    addPlayer           (Player player);
        std::vector<Player>     getPlayers          ();
        static const unsigned short PORT = 2905;

    private:
        std::vector<Player>     players;
        sf::TcpListener         &listener;
        sf::SocketSelector      selector;
        Card::Color             topColor;
        int                     nbDrawCard;
        bool                    listenerBound = false;
        bool                    passExtraTurn = false;
        bool                    reverseTurn = false;
        bool                    canPlay = false;
        bool                    choosingColor = false;
        bool                    goingThroughUno = false;
        bool                    pressedUno = false;
        bool                    wildFourChosen = false;
        void                    dealSevenCards(Deck &deck);
        void                    drawFirstCard(Deck &drawDeck, Deck &playDeck);
        int                     processPacket(sf::Packet& packet, std::vector<Player>::iterator& player, std::vector<Player>::iterator& turnIt, Deck& playDeck, Deck& drawDeck);
        void                    playerDisconnected(Player player);
        void                    sendTurnInfo(std::vector<Player>::iterator& turnIt);
        void                    passTurn(std::vector<Player>::iterator& turnIt);
        bool                    drawACard(std::vector<Player>::iterator& player, Deck& drawDeck, Deck& playDeck);
        void                    computeScores();
};

#endif
