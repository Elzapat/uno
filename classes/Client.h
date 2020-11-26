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
#include <unistd.h>
#include <math.h>

class Client {

    struct Sprite {
        sf::Texture texture;
        sf::Sprite sprite;
    };

    public:
        Client(sf::RenderWindow &initWindow, Player initPlayer);
        int lobby();
        int game();

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
        bool canPlay;
        bool alreadyDrew;
        bool showUnoButton;
        bool chooseColor;
        bool movingCard;
        sf::Vector2f mousePos;
        Card* cardPicked;
        sf::Clock unoClock;
        Sprite drawButton,
               drawButtonH,
               unoButton,
               unoButtonH;
        void initPlayerList(sf::Text* playerList, sf::Font& font);
        void initButton(Sprite &button, sf::Texture texture, int x, int y, sf::IntRect textureRect);
        int processPacket(sf::Packet &packet);
        void processEvent(sf::Event event, sf::FloatRect red, sf::FloatRect yellow, sf::FloatRect green, sf::FloatRect blue);
        void processEvent(sf::Event event, int& hovered);
        void drawEverything(int hovered = 0); 
        std::map<std::string, sf::RectangleShape> colorRects;
        void drawAnimation();
        void rearrangeHandAnimation();
        void cardPlayedAnimation();
};

#endif
