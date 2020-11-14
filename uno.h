#ifndef UNO_H
#define UNO_H

#include <iostream>
#include <thread>

#include "classes/Game.h"
#include "classes/Player.h"
#include "classes/Card.h"
#include "classes/Menu.h"
#include "classes/Deck.h"
#include "classes/Server.h"
#include "classes/Client.h"

#define MAX_SIZE 30

#define REVERSE_CARD 10
#define PLUS_2 11
#define PLUS_4 12
#define CHANGE_COLOR 13
#define SKIP_TURN 14
#define REVERSE_TURNS 15

struct Options {
    int resolution;
    int musicVolume;
    int effectsVolume;
    int masterVolume;
};



int startInitialize(char **vars);
int game(char **vars);
int settings(char **vars);
int quitFreeEverything(char **vars);

#endif
