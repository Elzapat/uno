#include "Card.h"

sf::Texture Card::texture;
sf::Texture Card::backTexture;

void Card::loadAssets () {
    texture.loadFromFile("assets/sprites/cards.png");
    backTexture.loadFromFile("assets/sprites/card_back.png");
}

Card::Card() {}
Card::Card(Color colorI, Value valueI, float cardSize, int SCREEN_WIDTH, int SCREEN_HEIGHT) : color(colorI), value(valueI) {

    if (cardSize != 0) {
        if (color == Color::BLACK && value == Value::BACK) {
            sprite.setTexture(backTexture);
            sf::Vector2f scale = sprite.getScale();
            sprite.setScale(scale.x * cardSize * ((float)SCREEN_WIDTH / 1920), scale.y * cardSize * ((float)SCREEN_HEIGHT / 1080));
            return;
        }
        int cardWidth = texture.getSize().x / 14;
        int cardHeight = texture.getSize().y / 8;
        sprite.setTexture(texture);
        if (color == Color::BLACK) {
            if (value == Value::WILD) sprite.setTextureRect(sf::IntRect(13 * cardWidth, 0, cardWidth, cardHeight));
            else if (value == Value::WILD_FOUR) sprite.setTextureRect(sf::IntRect(13 * cardWidth, 4 * cardHeight, cardWidth, cardHeight));
        } else sprite.setTextureRect(sf::IntRect((static_cast<int>(value)) * cardWidth, static_cast<int>(color) * cardHeight, cardWidth, cardHeight));
        sf::Vector2f scale = sprite.getScale();
        sprite.setScale(scale.x * cardSize * ((float)SCREEN_WIDTH / 1920), scale.y * cardSize * ((float)SCREEN_HEIGHT / 1080));
    }
}

Card::Color Card::getColor() { return this->color; }
Card::Value Card::getValue() { return this->value; }

sf::Packet& operator << (sf::Packet& packet, Card card) {
    return packet << static_cast<int>(card.getColor()) << static_cast<int>(card.getValue());
}
