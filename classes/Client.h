#ifndef CLIENT_H
#define CLIENT_H

#include "../SFML/include/SFML/System.hpp"
#include "../SFML/include/SFML/Window.hpp"
#include "../SFML/include/SFML/Graphics.hpp"
#include "Player.h"
#include "Server.h"
#include "Card.h"
#include "Deck.h"
#include <iostream>

class Client {

    struct Sprite {
        sf::Texture texture;
        sf::Sprite sprite;
    };

    public:
        Client(sf::RenderWindow &initWindow, Player initPlayer);
        int lobby();
        void game();

    private:
        sf::RenderWindow &window;
        int SCREEN_WIDTH;
        int SCREEN_HEIGHT;
        std::vector<Player> players;
        sf::Text *playerList;
        Player player;
        Sprite background;
        Card topCard;
        Card::Color topColor;
        bool chooseColor;
        void initPlayerList(sf::Text* playerList, sf::Font& font);
        void initButton(Sprite &button, sf::Texture texture, int x, int y, sf::IntRect textureRect);
        void processPacket(sf::Packet &packet);
        void processEvent(sf::Event event, sf::IntRect red, sf::IntRect yellow, sf::IntRect green, sf::IntRect blue);
        void processEvent(sf::Event event);
};

#endif