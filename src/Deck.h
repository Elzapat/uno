#ifndef DECK_H
#define DECK_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>

#include "Card.h"

class Deck {

    public:
                Deck            (bool empty);
                ~Deck           ();
        void    shuffle         ();
        void    swap            (Card &card1, Card &card2);
        Card    draw            ();
        void    addCardOnTop    (Card card);
        std::vector<Card>& getCards();
        Card    removeFirstCard ();

    private:
        const short NB_CARDS = 108;
        std::vector<Card> cards;

};

#endif
