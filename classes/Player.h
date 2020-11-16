#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Card.h"
#include "Deck.h"
#include "../SFML/include/SFML/Network.hpp"

class Player {

    public:
                        Player          ();
                        Player          (std::string name);
        int             getUniqueID     ();
        void            setUniqueID     (int uniqueID);
        std::string     getName         ();
        void            setName         (std::string name);
        int             getScore        ();
        void            setScore        (int score);
        std::string     getAddress      ();
        void            setAddress      (std::string address);
        std::string     getHostAddress  ();
        void            setHostAddress  (std::string address);
        void            addCardToHand   (Card card);
        void            removeCardFromHand (Card card);
        void            setSocket       (sf::TcpSocket *socket);
        sf::TcpSocket   *getSocket      ();
        std::vector<Card>& getHand       ();
        bool            isHost;
        bool            isPlaying;

    private:
        int             uniqueID;
        std::string     name;
        int             score;
        std::string     address;
        std::string     hostAddress;
        std::vector<Card> hand;
        sf::TcpSocket   *socket;
};

#endif
