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
                    Card        (Color color, Value value, float cardSize = 0);
        Color       getColor    ();
        Value       getValue    ();
        static void loadAssets  ();
        sf::Sprite  sprite;

    private:
        int         uniqueID; // You wouldn't want to take that one red "2" card for the other red "2" card, would you ?
        std::string name; // Yup, they'll have a name, all  of 'em. ID never is enough.
        int         score; // Yup, there's a scoring system in original uno rules.
        Color       color; // will take values 'r' 'y' 'b' and 'g'
        Value       value; // <-- Will also identify black cards with values such as 10 and 11
        static sf::Texture texture;
        static sf::Texture backTexture;
};

sf::Packet& operator << (sf::Packet &packet, Card card);
sf::Packet& operator >> (sf::Packet& packet, Card card);

#endif
