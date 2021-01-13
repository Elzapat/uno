#include "Card.h"

sf::Texture Card::texture;
sf::Texture Card::backTexture;
int Card::ids = 0;

void Card::loadAssets () {
    texture.loadFromFile("assets/sprites/cards.png");
    backTexture.loadFromFile("assets/sprites/card_back.png");
}

Card::Card() {}
Card::Card(Color colorI, Value valueI, float cardSize, int SCREEN_WIDTH, int SCREEN_HEIGHT) : color(colorI), value(valueI) {
    srand(time(NULL));

    if (cardSize != 0) {
        if (color == Color::BLACK && value == Value::BACK) {
            sprite.setTexture(backTexture);
            sf::Vector2f scale = sprite.getScale();
            sprite.setScale(scale.x * cardSize * ((float)SCREEN_WIDTH / 1920), scale.y * cardSize * ((float)SCREEN_HEIGHT / 1080));
            return;
        }
        int cardWidth = texture.getSize().x / 14;
        int cardHeight = texture.getSize().y / 8;
        texture.setSmooth(true);
        sprite.setTexture(texture);
        if (color == Color::BLACK) {
            if (value == Value::WILD) sprite.setTextureRect(sf::IntRect(13 * cardWidth, 0, cardWidth, cardHeight));
            else if (value == Value::WILD_FOUR) sprite.setTextureRect(sf::IntRect(13 * cardWidth, 4 * cardHeight, cardWidth, cardHeight));
        } else sprite.setTextureRect(sf::IntRect((static_cast<int>(value)) * cardWidth, static_cast<int>(color) * cardHeight, cardWidth, cardHeight));
        sf::Vector2f scale = sprite.getScale();
        sprite.setScale(scale.x * cardSize * ((float)SCREEN_WIDTH / 1920), scale.y * cardSize * ((float)SCREEN_HEIGHT / 1080));
    }
    this->uniqueID = ids++;
}

Card::Color Card::getColor() { return this->color; }
Card::Value Card::getValue() { return this->value; }
void Card::setUniqueID(int ID) { this->uniqueID = ID; }
int Card::getUniqueID() const { return this->uniqueID; }

sf::Packet& operator << (sf::Packet& packet, Card card) {
    return packet << static_cast<int>(card.getColor()) << static_cast<int>(card.getValue());
}
/*
void Card::update(float dt) {

    if (!this->moving) return;
    std::cout << "test2" << this->distanceMoved << std::endl;

    sf::Vector2f move = this->direction * this->speed * dt;
    this->distanceMoved += sqrt(pow(move.x, 2) + pow(move.y, 2));
    this->sprite.move(move);

    if (this->distanceMoved >= this->totalDistance) {
        this->sprite.setPosition(this->finish);
        this->moving = false;
    }
}

void Card::startAnimation(sf::Vector2f start, sf::Vector2f finish, float speed) {
    
    std::cout << "start anim" << std::endl;
    this->moving = true;
    this->sprite.setPosition(start);

    sf::Vector2f diff = finish - start;
    float normDiff = sqrt(pow(diff.x, 2) + pow(diff.y, 2));

    this->direction = sf::Vector2f(diff.x / normDiff, diff.y / normDiff);
    this->totalDistance = normDiff;
    this->start = start;
    this->finish = finish;
    this->speed = speed;
    this->distanceMoved = 0.f;
}*/
