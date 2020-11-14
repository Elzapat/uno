#include "./SFML/System.hpp"
#include "./SFML/Window.hpp"
#include "./SFML/Graphics.hpp"
#include "Player.h"

class Game {

    public:
        Game();

    private:
        void dealCards (Player *players);
};
