#include "Client.h"

Client::Client(sf::RenderWindow &initWindow, Player initPlayer) 
    : window(initWindow), player(initPlayer) {

    std::cout << "client start" << std::endl;

    if (player.isHost) {
        sf::TcpSocket *socket = new sf::TcpSocket();
        socket->connect("127.0.0.1", Server::PORT);
        player.setSocket(socket);
    }

    sf::Packet namePacket;
    namePacket << Server::PacketPrefix::PLAYER_INFO;
    namePacket << player.getName();
    player.getSocket()->send(namePacket);
}

int Client::lobby() {
    
    SCREEN_WIDTH = window.getSize().x;
    SCREEN_HEIGHT = window.getSize().y;

    const int BUTTON_WIDTH = 404;
    const int BUTTON_HEIGHT = 107;
    const int SCREEN_CENTER_X = SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2;
    const int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2 - BUTTON_HEIGHT / 2;

    Sprite background;
    Sprite quitButton;
    Sprite quitButtonH;
    Sprite startButton;
    Sprite startButtonH;
    sf::Texture buttonsTexture;
    sf::Texture buttonsTextureH;

    buttonsTexture.loadFromFile("assets/sprites/buttons3.png");
    buttonsTextureH.loadFromFile("assets/sprites/buttons3_hovered.png");

    background.texture.loadFromFile("assets/sprites/game_background.jpg");
    background.sprite.setTexture(background.texture);
    background.sprite.setPosition(0,0);

    initButton(startButton, buttonsTexture, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.4, sf::IntRect(98, 245, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(startButtonH, buttonsTextureH, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.4, sf::IntRect(98, 245, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(quitButton, buttonsTexture, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.7, sf::IntRect(98, 397, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(quitButtonH, buttonsTextureH, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.7, sf::IntRect(98, 397, BUTTON_WIDTH, BUTTON_HEIGHT));

    sf::Event event;
    sf::Packet packet;
    sf::String playerList = "";
    sf::Font font;
    int buttonHovered = 0;
    font.loadFromFile("assets/fonts/ArialCE.ttf");
    players.push_back(player.getName());

    sf::Text playerListT(playerList, font, 50);
    playerListT.setFillColor(sf::Color(0, 0, 0));
    playerListT.setPosition(100, 100);

    sf::String m1(L"Vous êtes l'hôte, votre adresse IP est ");
    sf::String m2(sf::IpAddress::getPublicAddress(sf::seconds(2)).toString() + ".\nN'oubliez pas d'ouvrir le port 2905 sur votre box.");
    sf::Text ipAddressHelp(m1 + m2, font, 50);
    ipAddressHelp.setFillColor(sf::Color(0, 0, 0));
    ipAddressHelp.setPosition (window.getSize().x - ipAddressHelp.getGlobalBounds().width - 100, 100);

    window.setFramerateLimit(30);
    player.getSocket()->setBlocking(false);

    while (window.isOpen()) {
        window.clear();
        window.draw(background.sprite);

        if (player.isHost) window.draw(ipAddressHelp);
        if (buttonHovered == 1) {
            if (player.isHost) window.draw(startButtonH.sprite);
            window.draw(quitButton.sprite);
        } else if (buttonHovered == 2) {
            if (player.isHost) window.draw(startButton.sprite);
            window.draw(quitButtonH.sprite);
        } else {
            if (player.isHost) window.draw(startButton.sprite);
            window.draw(quitButton.sprite);
        }

        playerList = L"Joueurs présents :\n";
        for (auto p : players)
            playerList += "- " + p.getName() + '\n';
        playerListT.setString(playerList);
        window.draw(playerListT);

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseMoved:
                    if (startButton.sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                        buttonHovered = 1;
                    else if (quitButton.sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                        buttonHovered = 2;
                    else buttonHovered = 0;
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (startButton.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            if (player.isHost) {
                                sf::Packet gameStartPacket;
                                Server::PacketPrefix prefix = Server::PacketPrefix::GAME_START;
                                gameStartPacket << static_cast<int>(prefix);
                                player.getSocket()->send(gameStartPacket);
                                game();
                                return 2;
                            }
                        } else if (quitButton.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            player.getSocket()->disconnect();
                            return 1;
                        }
                    }
                    break;
            }
        }
        if (player.getSocket()->receive(packet) == sf::Socket::Done) {
            std::cout << "packet received" << std::endl;
            int prefix;
            int ID;
            Player newPlayer;
            std::string playerName;
            packet >> prefix;
            switch (static_cast<Server::PacketPrefix>(prefix)) {
                case Server::PacketPrefix::PLAYER_INFO:
                    packet >> playerName;
                    packet >> ID;
                    newPlayer.setName(playerName);
                    newPlayer.setUniqueID(ID);
                    players.push_back(newPlayer);
                    break;
                case Server::PacketPrefix::PLAYER_DISCONNECTED:
                    packet >> ID;
                    for (int i = 0; i < players.size(); ++i) {
                        if (players[i].getUniqueID() == ID) players.erase(players.begin() + i);
                    }
                    break;
                case Server::PacketPrefix::GAME_START:
                    game();
                    break;

            }
        }
        window.display();
    };
    return 0;
}

void Client::initButton(Sprite &button, sf::Texture texture, int x, int y, sf::IntRect textureRect) {

    button.texture = texture;
    button.sprite.setTexture(button.texture);
    button.sprite.setTextureRect(textureRect);
    button.sprite.setPosition(x, y);
}

void Client::game() {

    background.texture.loadFromFile("assets/sprites/game_background.jpg");
    background.sprite.setTexture(background.texture);
    background.sprite.setPosition(0,0);

    sf::Font font;
    font.loadFromFile("assets/fonts/ArialCE.ttf");

    sf::Event event;
    sf::Packet packet;
    sf::Text* playerList = new sf::Text[players.size()];
    initPlayerList(playerList, font);
    int turn = 0;
    Card::loadAssets();
    Card topCard(Card::Color::BLACK, Card::Value::BACK, 0.8f);
    Card cardBack(Card::Color::BLACK, Card::Value::BACK, 0.8f);
    topCard.sprite.setPosition(SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT / 2.4 - topCard.sprite.getGlobalBounds().height / 2);
    cardBack.sprite.setPosition(SCREEN_WIDTH / 2 - cardBack.sprite.getGlobalBounds().width - 20, SCREEN_HEIGHT / 2.4 - cardBack.sprite.getGlobalBounds().height / 2);

    while (window.isOpen()) {
        window.clear();
        window.draw(background.sprite);
        for (int i = 0; i < players.size(); ++i) {
            if (i == turn) playerList[i].setFillColor(sf::Color(255, 0, 0));
            else playerList[i].setFillColor(sf::Color(0, 0, 0));
            window.draw(sf::Text(playerList[i]));
        }
        if (!player.getHand().empty()) {
            std::vector<Card> hand = player.getHand();
            int spacing = SCREEN_WIDTH / (hand.size() + 1);
            int cardWidth = hand[0].sprite.getGlobalBounds().width;
            for (int i = 0; i < hand.size(); ++i) {
                hand[i].sprite.setPosition(spacing * (i + 1) - hand[i].sprite.getGlobalBounds().width / 2, SCREEN_HEIGHT * 0.7);
                window.draw(hand[i].sprite);
            }
        }
        window.draw(cardBack.sprite);
        window.draw(topCard.sprite);

        if (player.getSocket()->receive(packet) == sf::Socket::Done) processPacket(packet, topCard);
        while (window.pollEvent(event)) processEvent(event);

        window.display();
    }
    delete[] playerList;
}

void Client::processPacket(sf::Packet &packet, Card &topCard) {

    int prefix;
    int color, value;
    packet >> prefix;
    switch (static_cast<Server::PacketPrefix>(prefix)) {
        case Server::PacketPrefix::CARD_DRAWN:
            packet >> color;
            packet >> value;
            player.addCardToHand(Card(static_cast<Card::Color>(color), static_cast<Card::Value>(value), 0.5f));
            break;
        case Server::PacketPrefix::CARD_PLAYED:
            packet >> color;
            packet >> value;
            topCard = Card(static_cast<Card::Color>(color), static_cast<Card::Value>(value), 0.8f);
            topCard.sprite.setPosition(SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT / 2.4 - topCard.sprite.getGlobalBounds().height / 2);
            break;
    }
}

void Client::processEvent(sf::Event event) {

    switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
    }
}
void Client::initPlayerList(sf::Text* playerList, sf::Font &font) {

    int i = 0;
    int spacing = SCREEN_WIDTH / (players.size() + 1);
    for (auto p : players) {
        playerList[i] = sf::Text(p.getName(), font, 30);
        playerList[i].setPosition(spacing * (i + 1) - playerList[i].getGlobalBounds().width / 2, 100);
        i++;
    }
}
