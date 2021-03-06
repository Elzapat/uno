#include "Player.h"

Player::Player(std::string name_i) : name(name_i) { score = 0; }
Player::Player() { score = 0; }

int Player::getUniqueID() { return this->uniqueID; }
void Player::setUniqueID(int uniqueID) { this->uniqueID = uniqueID; }
std::string Player::getName() { return this->name; }
void Player::setName(std::string name) { this->name = name; }
int Player::getScore() { return this->score; }
void Player::setScore(int score) { this->score = score; }
std::string Player::getAddress() { return this->address; }
void Player::setAddress(std::string address) { this->address = address; }
std::string Player::getHostAddress() { return this->hostAddress; }
void Player::setHostAddress(std::string hostAddress) { this->hostAddress = hostAddress; }
void Player::setSocket(sf::TcpSocket *socket) { this->socket = socket; }
sf::TcpSocket* Player::getSocket() { return this->socket; }
std::vector<Card>& Player::getHand() { return this->hand; }
void Player::addCardToHand(Card card) { this->hand.push_back(card); }
void Player::removeCardFromHand(Card card) {

    int i = 0;
    for (auto c : hand) {
        if (c.getValue() == card.getValue() && c.getColor() == card.getColor()) {
            hand.erase(hand.begin() + i);
            break;
        }
        i++;
    }
}

