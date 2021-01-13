#ifndef UNO_H
#define UNO_H

#include <iostream>
#include <thread>

#include "src/Player.h"
#include "src/Card.h"
#include "src/Menu.h"
#include "src/Deck.h"
#include "src/Server.h"
#include "src/Client.h"

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
