#include "uno.h"
#include "UnoConfig.h"

int main() {

    int SCREEN_WIDTH = sf::VideoMode::getDesktopMode().width > 2560 ? sf::VideoMode::getDesktopMode().width / 2 : sf::VideoMode::getDesktopMode().width;
    int SCREEN_HEIGHT = sf::VideoMode::getDesktopMode().height;
    SCREEN_WIDTH = SCREEN_WIDTH * 0.9;
    SCREEN_HEIGHT = SCREEN_HEIGHT * 0.9;

    sf::TcpListener *listener = new sf::TcpListener;
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Uno", sf::Style::Titlebar | sf::Style::Close);

    Menu menu(window, listener);

    while (window.isOpen()) {
        Player player = menu.initMenu();
        if (!window.isOpen()) break;

        if (player.isHost && window.isOpen()) {
            Server *server = new Server(listener);
            std::thread serverThread(&Server::lobby, server);
            serverThread.detach();
        }

        Client *client = new Client(window, player);
        while (window.isOpen()) {
            int action = client->lobby();
            if (action == 1) break;
            else if (action == 2) continue;
        }
        delete client;
    }
}
