#include "Client.h"

Client::Client(sf::RenderWindow &initWindow, Player initPlayer) 
    : window(initWindow), player(initPlayer) {

    showUnoButton = false;

    std::cout << "client start" << std::endl;

    if (player.isHost) {
        sf::TcpSocket *socket = new sf::TcpSocket();
        socket->connect("127.0.0.1", Server::PORT);
        player.setSocket(socket);
    }
    player.setAddress(sf::IpAddress::getPublicAddress(sf::seconds(10)).toString());

    sf::Packet namePacket;
    namePacket << Server::PacketPrefix::PLAYER_INFO;
    namePacket << player.getName();
    player.getSocket()->send(namePacket);
}

int Client::lobby() {
    
    SCREEN_WIDTH = window.getSize().x;
    SCREEN_HEIGHT = window.getSize().y;

    Card::loadAssets();

    Card exempleCard(Card::Color::RED, Card::Value::ZERO, 0.8f, SCREEN_WIDTH, SCREEN_HEIGHT);
    const int BUTTON_WIDTH = 404;
    const int BUTTON_HEIGHT = 107;
    const int SCREEN_CENTER_X = SCREEN_WIDTH / 2;
    const int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;
    int unoButtonArea = SCREEN_WIDTH / 2 - exempleCard.sprite.getGlobalBounds().width - 20;
    unoButtonArea = unoButtonArea / 2;

    Sprite background;
    Sprite quitButton;
    Sprite quitButtonH;
    Sprite startButton;
    Sprite startButtonH;
    sf::Texture buttonsTexture;
    sf::Texture buttonsTextureH;
    sf::Texture buttons4Texture;
    sf::Texture buttons4TextureH;

    buttonsTexture.loadFromFile("assets/sprites/buttons3.png");
    buttonsTextureH.loadFromFile("assets/sprites/buttons3_hovered.png");
    buttons4Texture.loadFromFile("assets/sprites/buttons4.png");
    buttons4TextureH.loadFromFile("assets/sprites/buttons4_hovered.png");

    background.texture.loadFromFile("assets/sprites/game_background.jpg");
    background.sprite.setTexture(background.texture);
    background.sprite.setPosition(0,0);

    initButton(startButton, buttonsTexture, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.5, sf::IntRect(98, 245, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(startButtonH, buttonsTextureH, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.5, sf::IntRect(98, 245, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(quitButton, buttonsTexture, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.8, sf::IntRect(98, 397, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(quitButtonH, buttonsTextureH, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.8, sf::IntRect(98, 397, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(drawButton, buttons4Texture, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.28, sf::IntRect(98, 247, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(drawButtonH, buttons4TextureH, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.28, sf::IntRect(98, 247, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(unoButton, buttons4Texture, unoButtonArea, SCREEN_HEIGHT / 2.4 - quitButton.sprite.getGlobalBounds().height / 2, sf::IntRect(98, 95, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(unoButtonH, buttons4TextureH, unoButtonArea, SCREEN_HEIGHT / 2.4 - quitButton.sprite.getGlobalBounds().height / 2, sf::IntRect(98, 95, BUTTON_WIDTH, BUTTON_HEIGHT));
    unoButton.sprite.setPosition(unoButtonArea / 2, SCREEN_HEIGHT / 2.4 - unoButton.sprite.getGlobalBounds().height / 2);
    unoButtonH.sprite.setPosition(unoButtonArea / 2, SCREEN_HEIGHT / 2.4 - unoButton.sprite.getGlobalBounds().height / 2);

    sf::Event event;
    sf::Packet packet;
    sf::String playerList = "";
    sf::Font font;
    int fontSize = 45 * ((float)SCREEN_WIDTH / 1920);
    int buttonHovered = 0;
    font.loadFromFile("assets/fonts/ArialCE.ttf");

    sf::Text playerListT(playerList, font, fontSize);
    playerListT.setFillColor(sf::Color(0, 0, 0));
    playerListT.setPosition(100, 100);

    sf::String m1(L"Vous êtes l'hôte, votre adresse IP est ");
    sf::String m2(sf::IpAddress::getPublicAddress(sf::seconds(2)).toString() + ".\nN'oubliez pas d'ouvrir le port 2905 sur votre box.");
    sf::Text ipAddressHelp(m1 + m2, font, fontSize);
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
            playerList += "- " + p.getName() + " (" + std::to_string(p.getScore()) + ')' + '\n';
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
                                int action = 1;
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
            int prefix;
            int ID;
            std::string address;
            Player newPlayer;
            std::string playerName;
            packet >> prefix;
            switch (static_cast<Server::PacketPrefix>(prefix)) {
                case Server::PacketPrefix::PLAYER_INFO:
                    packet >> playerName;
                    packet >> ID;
                    packet >> address;
                    newPlayer.setName(playerName);
                    newPlayer.setUniqueID(ID);
                    newPlayer.setAddress(address);
                    players.push_back(newPlayer);
                    break;
                case Server::PacketPrefix::PLAYER_ID:
                    packet >> ID;
                    player.setUniqueID(ID);
                    players.push_back(player);
                    break;
                case Server::PacketPrefix::PLAYER_DISCONNECTED:
                    packet >> ID;
                    for (int i = 0; i < players.size(); ++i) {
                        if (players[i].getUniqueID() == ID) players.erase(players.begin() + i);
                    }
                    break;
                case Server::PacketPrefix::GAME_START:
                    int action;
                    action = game();
                    if (action = 2) return 2;
                    break;

            }
        }
        window.display();
    };
    return 0;
}

void Client::initButton(Sprite &button, sf::Texture texture, int x, int y, sf::IntRect textureRect) {

    button.texture = texture;
    button.texture.setSmooth(true);
    button.sprite.setTexture(button.texture);
    button.sprite.setTextureRect(textureRect);
    button.sprite.scale((float)SCREEN_WIDTH / 1920, (float)SCREEN_HEIGHT / 1080);
    button.sprite.setPosition(x - button.sprite.getGlobalBounds().width / 2, y - button.sprite.getGlobalBounds().height / 2);
}

int Client::game() {

    background.texture.loadFromFile("assets/sprites/game_background.jpg");
    background.sprite.setTexture(background.texture);
    background.sprite.setPosition(0,0);

    sf::Font font;
    font.loadFromFile("assets/fonts/ArialCE.ttf");

    sf::Event event;
    sf::Packet packet;
    chooseColor = false;
    canPlay = true;
    alreadyDrew = false;
    int hovered = 0;
    playerList = new sf::Text[players.size()];
    initPlayerList(playerList, font);

    sf::Text playerWonTest("", font, 50);

    topCard = Card(Card::Color::BLACK, Card::Value::BACK, 0.8f, SCREEN_WIDTH, SCREEN_HEIGHT);
    Card cardBack(Card::Color::BLACK, Card::Value::BACK, 0.8f, SCREEN_WIDTH, SCREEN_HEIGHT);
    topCard.sprite.setPosition(SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT / 2.4 - topCard.sprite.getGlobalBounds().height / 2);
    cardBack.sprite.setPosition(SCREEN_WIDTH / 2 - cardBack.sprite.getGlobalBounds().width - 20, SCREEN_HEIGHT / 2.4 - cardBack.sprite.getGlobalBounds().height / 2);

    int rectangleWidth = SCREEN_WIDTH / 10;
    int rectangleHeight = SCREEN_HEIGHT / 10;
    int colorsArea = SCREEN_WIDTH / 2 + topCard.sprite.getGlobalBounds().width + 20;
    colorsArea = colorsArea + (SCREEN_WIDTH - colorsArea) / 2;
    sf::RectangleShape red(sf::Vector2f(rectangleWidth, rectangleHeight)); 
    red.setPosition(colorsArea - rectangleWidth - 10, SCREEN_HEIGHT / 2.4 - rectangleHeight - 10);
    red.setFillColor(sf::Color(255, 0, 0));
    sf::RectangleShape yellow(sf::Vector2f(rectangleWidth, rectangleHeight)); 
    yellow.setPosition(colorsArea + 10, SCREEN_HEIGHT / 2.4 - rectangleHeight - 10);
    yellow.setFillColor(sf::Color(255, 255, 0));
    sf::RectangleShape green(sf::Vector2f(rectangleWidth, rectangleHeight)); 
    green.setPosition(colorsArea - rectangleWidth - 10, SCREEN_HEIGHT / 2.4 + 10);
    green.setFillColor(sf::Color(0, 255, 0));
    sf::RectangleShape blue(sf::Vector2f(rectangleWidth, rectangleHeight)); 
    blue.setPosition(colorsArea + 10, SCREEN_HEIGHT / 2.4 + 10);
    blue.setFillColor(sf::Color(0, 0, 255));

    while (window.isOpen()) {
        window.clear();
        window.draw(background.sprite);
        if (!player.getHand().empty()) {
            std::vector<Card>& hand = player.getHand();
            int spacing = SCREEN_WIDTH / (hand.size() + 1);
            int cardWidth = hand[0].sprite.getGlobalBounds().width;
            for (int i = 0; i < hand.size(); ++i) {
                hand[i].sprite.setPosition(spacing * (i + 1) - hand[i].sprite.getGlobalBounds().width / 2, SCREEN_HEIGHT * 0.75);
                window.draw(hand[i].sprite);
            }
        }
        window.draw(cardBack.sprite);
        window.draw(topCard.sprite);
        for (int i = 0; i < players.size(); ++i) window.draw(playerList[i]);
        if (chooseColor) {
            window.draw(red);
            window.draw(yellow);
            window.draw(green);
            window.draw(blue);
        }
        if (hovered == 1 && player.isPlaying && !chooseColor && !canPlay && !alreadyDrew) window.draw(drawButtonH.sprite);
        else if (player.isPlaying && !chooseColor && !canPlay && !alreadyDrew) window.draw(drawButton.sprite);

        if (hovered == 2 && showUnoButton) window.draw(unoButtonH.sprite);
        else if (showUnoButton) window.draw(unoButton.sprite);
        if (showUnoButton && unoClock.getElapsedTime().asMilliseconds() > 2000) {
            showUnoButton = false;
            sf::Packet unoPacket;
            Server::PacketPrefix unoPrefix = Server::PacketPrefix::UNO;
            bool pressedUno = false;
            unoPacket << static_cast<int>(unoPrefix) << pressedUno;
            player.getSocket()->send(unoPacket);
        }

        int action;
        if (player.getSocket()->receive(packet) == sf::Socket::Done) action = processPacket(packet);
        if (action == 2) {
            return 2;
        }
        while (window.pollEvent(event)) {
            if (chooseColor) processEvent(event, red.getGlobalBounds(), yellow.getGlobalBounds(), green.getGlobalBounds(), blue.getGlobalBounds());
            else processEvent(event, hovered);
        }

        window.display();
    }
    delete[] playerList;
    return 0;
}

int Client::processPacket(sf::Packet &packet) {

    int prefix;
    Card card;
    int ID;
    int i = 0;
    int color, value;
    int score;

    packet >> prefix;
    switch (static_cast<Server::PacketPrefix>(prefix)) {
        case Server::PacketPrefix::CARD_DRAWN:
            packet >> color >> value >> ID;
            card = Card(static_cast<Card::Color>(color), static_cast<Card::Value>(value));
            if (player.getUniqueID() == ID) player.addCardToHand(Card(card.getColor(), card.getValue(), 0.5f, SCREEN_WIDTH, SCREEN_HEIGHT));
            for (Player &p : players)
                if (p.getUniqueID() == ID) p.addCardToHand(card);
            for (auto card : player.getHand()) {
                if (card.getValue() == topCard.getValue() || card.getColor() == topCard.getColor() || card.getColor() == Card::Color::BLACK || (topCard.getColor() == Card::Color::BLACK && card.getColor() == topColor)) {
                    canPlay = true;
                    break;
                } else canPlay = false;
            }
            break;
        case Server::PacketPrefix::CARD_PLAYED:
            packet >> color >> value >> ID;
            card = Card(static_cast<Card::Color>(color), static_cast<Card::Value>(value));
            for (Player& p : players)
                if (p.getUniqueID() == ID) p.removeCardFromHand(card);
            topCard = Card(card.getColor(), card.getValue(), 0.8f, SCREEN_WIDTH, SCREEN_HEIGHT);
            topCard.sprite.setPosition(SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT / 2.4 - topCard.sprite.getGlobalBounds().height / 2);
            topColor = topCard.getColor();
            break;
        case Server::PacketPrefix::TURN_INFO:
            std::cout << "client: received turn info" << std::endl;
            packet >> ID;
            i = 0;
            canPlay = true;
            for (Player& p : players) {
                if (ID == p.getUniqueID()) {
                    p.isPlaying = true;
                    playerList[i].setFillColor(sf::Color(255, 0, 0));
                    playerList[i].setString(p.getName() + " (" + std::to_string(p.getHand().size()) + ")");
                } else {
                    p.isPlaying = false;
                    playerList[i].setFillColor(sf::Color(0, 0, 0));
                    playerList[i].setString(p.getName() + " (" + std::to_string(p.getHand().size()) + ")");
                }
                if (ID == player.getUniqueID()) {
                    player.isPlaying = true;
                    for (auto card : player.getHand())  {
                        if (card.getValue() == topCard.getValue() || card.getColor() == topCard.getColor() || card.getColor() == Card::Color::BLACK || (topCard.getColor() == Card::Color::BLACK && card.getColor() == topColor)) {
                            canPlay = true;
                            alreadyDrew = true;
                            break;
                        } else {
                            canPlay = false;
                            alreadyDrew = false;
                        }
                    }
                }
                else player.isPlaying = false;
                i++;
            }
            break;
        case Server::PacketPrefix::CHOOSE_COLOR:
            chooseColor = true; 
            break;
        case Server::PacketPrefix::TOP_COLOR_CHANGE:
            int newTopColor;
            packet >> newTopColor;
            topColor = static_cast<Card::Color>(newTopColor);
            chooseColor = false;
            break;
        case Server::PacketPrefix::PLAYER_DISCONNECTED:
            packet >> ID; 
            i = 0;
            for (auto p : players) {
                if (p.getUniqueID() == ID) {
                    players.erase(players.begin() + i);
                    break;
                }
            }
            break;
        case Server::PacketPrefix::UNO:
            std::cout << "client: uno intiated" << std::endl;
            showUnoButton = true;
            unoClock.restart();
            player.isPlaying = false;
            break;
        case Server::PacketPrefix::PLAYER_SCORE:
            packet >> ID;
            packet >> score;
            for (Player &p : players) {
                if (ID == p.getUniqueID()) p.setScore(score);
            }
            break;
        case Server::PacketPrefix::PLAYER_WON:
            packet >> ID;
            std::cout << "client: player won: " << ID << std::endl;

            std::string winner;

            for (auto p : players) {
                if (ID == p.getUniqueID()) {
                    winner = p.getName();
                    break;
                }
            }

            sf::Font font;
            font.loadFromFile("assets/fonts/ArialCE.ttf");

            sf::Text winnerText(winner + " gagne !", font, 40 * ((float)SCREEN_WIDTH / 1920));
            winnerText.setFillColor(sf::Color(0, 0, 0));
            winnerText.setPosition(SCREEN_WIDTH / 2 - winnerText.getGlobalBounds().width / 2, SCREEN_HEIGHT * 0.2);
            window.draw(winnerText);
            window.display();
            sleep(5);
            return 2;
            break;
    }
    return 0;
}

void Client::processEvent(sf::Event event, int& hovered) {

    switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::MouseButtonPressed:
            if (showUnoButton && unoButton.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                std::cout << "clicked on uno button" << std::endl;
                showUnoButton = false;
                sf::Packet unoPacket;
                Server::PacketPrefix unoPrefix = Server::PacketPrefix::UNO;
                bool pressedUno = true;
                unoPacket << static_cast<int>(unoPrefix) << pressedUno;
                player.getSocket()->send(unoPacket);
            } else if (player.isPlaying) {
                if (!alreadyDrew && drawButton.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    sf::Packet packet;
                    Server::PacketPrefix prefix = Server::PacketPrefix::DRAW_CARD;
                    packet << static_cast<int>(prefix);
                    player.getSocket()->send(packet);
                    alreadyDrew = true;
                    break;
                } else {
                    for (Card& card : player.getHand()) {
                        if (card.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            if (card.getValue() == topCard.getValue() || card.getColor() == topCard.getColor() || card.getColor() == Card::Color::BLACK || (topCard.getColor() == Card::Color::BLACK && card.getColor() == topColor)) {
                                sf::Packet packet;
                                Server::PacketPrefix prefix = Server::PacketPrefix::CARD_PLAYED;
                                packet << static_cast<int>(prefix) << card;
                                player.getSocket()->send(packet);
                                player.removeCardFromHand(card);
                                break;
                            }
                        }
                    }
                }
            }
            break;
        case sf::Event::MouseMoved:
            if (drawButton.sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                hovered = 1;
            else if (unoButton.sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                hovered = 2;
            else hovered = 0;
            break;
    }
}

void Client::processEvent(sf::Event event, sf::FloatRect red, sf::FloatRect yellow, sf::FloatRect green, sf::FloatRect blue) {

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Packet packet;
        Server::PacketPrefix prefix = Server::PacketPrefix::COLOR_CHOSEN;
        Card::Color chosenColor;
        if (red.contains(event.mouseButton.x, event.mouseButton.y)) {
            chosenColor = Card::Color::RED;
            packet << static_cast<int>(prefix) << static_cast<int>(chosenColor);
            player.getSocket()->send(packet); 
        } else if (yellow.contains(event.mouseButton.x, event.mouseButton.y)) {
            chosenColor = Card::Color::YELLOW;
            packet << static_cast<int>(prefix) << static_cast<int>(chosenColor);
            player.getSocket()->send(packet); 
        } else if (green.contains(event.mouseButton.x, event.mouseButton.y)) {
            chosenColor = Card::Color::GREEN;
            packet << static_cast<int>(prefix) << static_cast<int>(chosenColor);
            player.getSocket()->send(packet); 
        } else if (blue.contains(event.mouseButton.x, event.mouseButton.y)) {
            chosenColor = Card::Color::BLUE;
            packet << static_cast<int>(prefix) << static_cast<int>(chosenColor);
            player.getSocket()->send(packet); 
        }
    } else if (event.type == sf::Event::Closed) window.close();
}

void Client::initPlayerList(sf::Text* playerList, sf::Font &font) {

    int i = 0;
    int spacing = SCREEN_WIDTH / (players.size() + 1);
    for (auto p : players) {
        playerList[i] = sf::Text(p.getName(), font, 30);
        playerList[i].setPosition(spacing * (i + 1) - playerList[i].getGlobalBounds().width / 2, SCREEN_HEIGHT * 0.1);
        i++;
    }
}
