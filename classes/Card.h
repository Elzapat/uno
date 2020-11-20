#ifndef CARD_H
#define CARD_H

#include "../SFML/include/SFML/System.hpp"
#include "../SFML/include/SFML/Window.hpp"
#include "../SFML/include/SFML/Graphics.hpp"
#include "../SFML/include/SFML/Network.hpp"
#include <string>
#include <iostream>


class Card {

    public:
        enum Color { RED, YELLOW, GREEN, BLUE, BLACK };
        enum Value { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, SKIP, REVERSE, DRAW_TWO, WILD, WILD_FOUR, BACK };
                    Card        ();
                    Card        (Color color, Value value, float cardSize = 0, int SCREEN_WIDTH = 0, int SCREEN_HEIGHT = 0);
        Color       getColor    ();
        Value       getValue    ();
        static void loadAssets  ();
        sf::Sprite  sprite;

    private:
        std::string name; // Yup, they'll have a name, all  of 'em. ID never is enough.
        Color       color; // will take values 'r' 'y' 'b' and 'g'
        Value       value; // <-- Will also identify black cards with values such as 10 and 11
        static sf::Texture texture;
        static sf::Texture backTexture;
};

sf::Packet& operator << (sf::Packet &packet, Card card);
sf::Packet& operator >> (sf::Packet& packet, Card card);

#endif
