#ifndef MENU_H
#define MENU_H

#include "../SFML/include/SFML/System.hpp"
#include "../SFML/include/SFML/Window.hpp"
#include "../SFML/include/SFML/Graphics.hpp"
#include "../SFML/include/SFML/Network.hpp"
#include "../SFML/sfml-widgets/src/Gui/Menu.hpp"
#include "../SFML/sfml-widgets/src/Gui/Theme.hpp"
#include "../SFML/sfml-widgets/src/Gui/Gui.hpp"
#include "functions.h"
#include "Server.h"
#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include "Player.h"

class Menu {

    struct Sprite {
        sf::Texture texture;
        sf::Sprite sprite;
    };

    public:
                         Menu            (sf::RenderWindow &initWindow, sf::TcpListener *initListener);
        Player           initMenu        ();

    private:
        void             initButton      (Sprite &button, sf::Texture texture, int x, int y, sf::IntRect textureRect);
        void             listenForInputs ();
        int              settings        ();
        int              hostOrJoin      ();
        int              joinGame        (bool &emptyName);
        int              hostGame        (bool &emptyName, bool &serverFail);
        int              connect         (std::string ipAddress, std::string name, bool &invalidAddress, bool &emptyName);
        sf::RenderWindow &window;
        std::string      playerName = "";
        std::string      hostIpAddress = "";
        bool             isHost = false;
        bool             enableAnimation;
        int              SCREEN_WIDTH;
        int              SCREEN_HEIGHT;
        int              OG_SCREEN_WIDTH;
        int              OG_SCREEN_HEIGHT;
        sf::TcpSocket    *socket;
        sf::TcpListener  *listener;
        Sprite           playButton,
                         playButtonH,
                         settingsButton,
                         settingsButtonH,
                         quitButton,
                         quitButtonH,
                         backButton,
                         backButtonH,
                         hostButton,
                         hostButtonH,
                         joinButton,
                         joinButtonH,
                         connectButton,
                         connectButtonH,
                         background,
                         logo;
};

#endif
