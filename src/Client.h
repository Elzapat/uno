#ifndef CLIENT_H
#define CLIENT_H

#include "../SFML/include/SFML/System.hpp"
#include "../SFML/include/SFML/Window.hpp"
#include "../SFML/include/SFML/Graphics.hpp"
#include "../SFML/include/SFML/Audio.hpp"
#include "Player.h"
#include "Server.h"
#include "Card.h"
#include "Deck.h"
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <map>
#include <vector>
#include <list>

class Client {

    public:
        struct Sprite {
            sf::Texture texture;
            sf::Sprite sprite;
        };
        struct Sound {
            sf::SoundBuffer buffer;
            sf::Sound sound;
        };
        struct Animation {
            sf::Vector2f start, finish, direction;
            float speed, totalDistance, distanceMoved;
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
        std::vector<sf::Text> playerList;
        Player player;
        std::map<std::string, Sound> sounds;
        Sprite background;
        Card topCard;
        Card::Color topColor;
        bool canPlay;
        bool alreadyDrew;
        bool showUnoButton;
        bool chooseColor;
        bool movingCard;
        int movingCardIndex;
        sf::Vector2f mousePos;
        Card* cardPicked;
        sf::Clock unoClock;
        Sprite drawButton,
               drawButtonH,
               unoButton,
               unoButtonH;
        void initPlayerList(sf::Font& font);
        void initButton(Sprite &button, sf::Texture texture, int x, int y, sf::IntRect textureRect);
        void initAnimations();
        int processPacket(sf::Packet &packet);
        void processEvent(sf::Event event, int& hovered, sf::FloatRect red, sf::FloatRect yellow, sf::FloatRect green, sf::FloatRect blue);
        void drawEverything(int hovered = 0); 
        std::map<std::string, sf::RectangleShape> colorRects;
        void drawAnimation();
        void rearrangeHandAnimation();
        void cardPlayedAnimation(Card& card);
        void putCardBackInHandAnimation(Card& card); 
};

#endif
