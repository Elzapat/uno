#include "Deck.h"

Deck::Deck(bool empty) {

    if (!empty) {
        for (int colorInt = Card::Color::RED; colorInt != Card::Color::BLACK; ++colorInt) {
            for (int valueInt = Card::Value::ZERO; valueInt != Card::Value::WILD; ++valueInt) {
                for (int i = 0; i < 2; ++i) {
                    Card::Color color = static_cast<Card::Color>(colorInt);
                    Card::Value value = static_cast<Card::Value>(valueInt);
                    if (!(i == 0 && value == Card::Value::ZERO))
                        cards.push_back(Card(color, value));
                }
            }
        }
        for (int valueInt = Card::Value::WILD; valueInt != Card::Value::WILD_FOUR + 1; ++valueInt) {
            for (int i = 0; i < 4; ++i) {
                Card::Color color = Card::Color::BLACK;
                Card::Value value = static_cast<Card::Value>(valueInt);
                cards.push_back(Card(color, value));
            }
        }
    }
}

Deck::~Deck() {

    //for (auto e : cards) delete e;
}

std::vector<Card>& Deck::getCards() { return this->cards; }

void Deck::shuffle() {

    
    srand(time(NULL));

    for (int i = 0; i < NB_CARDS; ++i) {
        int r = i + (rand() % (NB_CARDS - i));

        swap(cards[i], cards[r]);
    }
}

void Deck::swap(Card &card1, Card &card2) {

    Card temp;
    temp = card1;
    card1 = card2;
    card2 = temp;
}

Card Deck::draw() {

    Card drawnCard = static_cast<Card>(cards.back());
    cards.pop_back();
    return drawnCard;
}

void Deck::addCardOnTop(Card card) { this->cards.push_back(card); }

Card Deck::removeFirstCard() {

    Card removedCard = cards.front();
    cards.erase(cards.begin());

    return removedCard;
}
