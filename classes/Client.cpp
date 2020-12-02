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

    // Initialize all sounds
    Sound sound;
    if(!sound.buffer.loadFromFile("assets/sounds/deal_card_1.wav")) std::cerr << "problem loading sounds buffer" << std::endl;
    sound.sound.setBuffer(sound.buffer);
    sounds["deal_card_1"] = sound;

    Sound sound2;
    sound2.buffer.loadFromFile("assets/sounds/card_played_1.wav");
    sound2.sound.setBuffer(sound2.buffer);
    sounds["card_played_1"] = sound2;

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

    window.setFramerateLimit(120);
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
                    if (action = 2) {
                        for (auto& p : players) p.getHand().clear();
                        player.getHand().clear();
                        return 2;
                    }
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
    movingCard = false;
    int hovered = 0;
    playerList = new sf::Text[players.size()];
    initPlayerList(playerList, font);

    sf::Text playerWonTest("", font, 50);

    topCard = Card(Card::Color::BLACK, Card::Value::BACK, 0.8f, SCREEN_WIDTH, SCREEN_HEIGHT);
    topCard.sprite.setPosition(SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT / 2.4 - topCard.sprite.getGlobalBounds().height / 2);

    int rectangleWidth = SCREEN_WIDTH / 10;
    int rectangleHeight = SCREEN_HEIGHT / 10;
    int colorsArea = SCREEN_WIDTH / 2 + topCard.sprite.getGlobalBounds().width + 20;
    colorsArea = colorsArea + (SCREEN_WIDTH - colorsArea) / 2;
    sf::RectangleShape red(sf::Vector2f(rectangleWidth, rectangleHeight)); 
    red.setPosition(colorsArea - rectangleWidth - 10, SCREEN_HEIGHT / 2.4 - rectangleHeight - 10);
    red.setFillColor(sf::Color(237, 28, 36));
    sf::RectangleShape yellow(sf::Vector2f(rectangleWidth, rectangleHeight)); 
    yellow.setPosition(colorsArea + 10, SCREEN_HEIGHT / 2.4 - rectangleHeight - 10);
    yellow.setFillColor(sf::Color(255, 194, 0));
    sf::RectangleShape green(sf::Vector2f(rectangleWidth, rectangleHeight)); 
    green.setPosition(colorsArea - rectangleWidth - 10, SCREEN_HEIGHT / 2.4 + 10);
    green.setFillColor(sf::Color(80, 170, 68));
    sf::RectangleShape blue(sf::Vector2f(rectangleWidth, rectangleHeight)); 
    blue.setPosition(colorsArea + 10, SCREEN_HEIGHT / 2.4 + 10);
    blue.setFillColor(sf::Color(85, 85, 255));

    colorRects.insert(std::pair<std::string, sf::RectangleShape>("red", red));
    colorRects.insert(std::pair<std::string, sf::RectangleShape>("yellow", yellow));
    colorRects.insert(std::pair<std::string, sf::RectangleShape>("green", green));
    colorRects.insert(std::pair<std::string, sf::RectangleShape>("blue", blue));

    for (auto c : player.getHand()) player.removeCardFromHand(c);

    while (window.isOpen()) {

        drawEverything(hovered);

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
            if (player.getUniqueID() == ID) {
                sounds["deal_card_1"].sound.play();
                player.addCardToHand(Card(card.getColor(), card.getValue(), 0.5f, SCREEN_WIDTH, SCREEN_HEIGHT));
                if (player.usesAnimations) drawAnimation();
                else {
                    std::vector<Card>& hand = player.getHand();
                    int spacing = SCREEN_WIDTH / (hand.size() + 1);
                    for (int i = 0; i < hand.size(); ++i)
                        hand[i].sprite.setPosition(spacing * (i + 1) - hand[i].sprite.getGlobalBounds().width / 2, SCREEN_HEIGHT * 0.75);
                }
            }

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
            card = Card(static_cast<Card::Color>(color), static_cast<Card::Value>(value), 0.8f, SCREEN_WIDTH, SCREEN_HEIGHT);
            for (Player& p : players) {
                if (p.getUniqueID() == ID) 
                    p.removeCardFromHand(card);
            }

            if (!player.isPlaying && player.usesAnimations) cardPlayedAnimation(card);

            sounds["card_played_1"].sound.play();

            topCard = Card(card.getColor(), card.getValue(), 0.8f, SCREEN_WIDTH, SCREEN_HEIGHT);
            topCard.sprite.setPosition(SCREEN_WIDTH / 2 + 20, SCREEN_HEIGHT / 2.4 - topCard.sprite.getGlobalBounds().height / 2);
            topColor = topCard.getColor();

            if (ID == player.getUniqueID() && player.usesAnimations) rearrangeHandAnimation();
            else {
                std::vector<Card>& hand = player.getHand();
                int spacing = SCREEN_WIDTH / (hand.size() + 1);
                for (int i = 0; i < hand.size(); ++i)
                    hand[i].sprite.setPosition(spacing * (i + 1) - hand[i].sprite.getGlobalBounds().width / 2, SCREEN_HEIGHT * 0.75);
            }
            break;
        case Server::PacketPrefix::TURN_INFO:
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
            if (event.mouseButton.button == sf::Mouse::Left) {
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
                    }
                }

                if (player.usesAnimations) {
                    for (auto& card : player.getHand()) {
                        if (card.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            cardPicked = &card;
                            movingCard = true;
                            mousePos.x = event.mouseButton.x;
                            mousePos.y = event.mouseButton.y;
                        }
                    }
                } else if (player.isPlaying) {
                    for (auto card : player.getHand()) {
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
        case sf::Event::MouseButtonReleased:
            if (movingCard && player.usesAnimations) {
                movingCard = false;
                if (cardPicked->sprite.getPosition().x >= topCard.sprite.getPosition().x - 100 &&
                    cardPicked->sprite.getPosition().x <= topCard.sprite.getPosition().x + topCard.sprite.getGlobalBounds().width + 100 &&
                    cardPicked->sprite.getPosition().y >= topCard.sprite.getPosition().y - 100 &&
                    cardPicked->sprite.getPosition().y <= topCard.sprite.getPosition().y + topCard.sprite.getGlobalBounds().width + 100 &&
                    player.isPlaying) {
                    
                    Card card = *cardPicked;
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
                putCardBackInHandAnimation(*cardPicked);
                    /*std::vector<Card>& hand = player.getHand();
                    int spacing = SCREEN_WIDTH / (hand.size() + 1);
                    for (int i = 0; i < hand.size(); ++i)
                        hand[i].sprite.setPosition(spacing * (i + 1) - hand[i].sprite.getGlobalBounds().width / 2, SCREEN_HEIGHT * 0.75);*/
            }
            break;
        case sf::Event::MouseMoved:
            if (drawButton.sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                hovered = 1;
            else if (unoButton.sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                hovered = 2;
            else hovered = 0;
            if (movingCard) {
                if (event.mouseMove.x < 0 || event.mouseMove.x > SCREEN_WIDTH ||
                    event.mouseMove.y < 0 || event.mouseMove.y > SCREEN_HEIGHT) break;
                cardPicked->sprite.move(event.mouseMove.x - mousePos.x, event.mouseMove.y - mousePos.y);
                mousePos.x = event.mouseMove.x;
                mousePos.y = event.mouseMove.y;
            }
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
        playerList[i].setFillColor(sf::Color(0, 0, 0));
        i++;
    }
}

void Client::drawEverything(int hovered) {

    Card cardBack(Card::Color::BLACK, Card::Value::BACK, 0.8f, SCREEN_WIDTH, SCREEN_HEIGHT);
    cardBack.sprite.setPosition(SCREEN_WIDTH / 2 - cardBack.sprite.getGlobalBounds().width - 20, SCREEN_HEIGHT / 2.4 - cardBack.sprite.getGlobalBounds().height / 2);

    window.clear();
    window.draw(background.sprite);
    window.draw(cardBack.sprite);
    window.draw(topCard.sprite);
    for (int i = 0; i < players.size(); ++i) window.draw(playerList[i]);

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

    if (chooseColor) {
        window.draw(colorRects.at("red"));
        window.draw(colorRects.at("yellow"));
        window.draw(colorRects.at("green"));
        window.draw(colorRects.at("blue"));
    } else if (topColor == Card::Color::RED) window.draw(colorRects.at("red"));
    else if (topColor == Card::Color::YELLOW) window.draw(colorRects.at("yellow"));
    else if (topColor == Card::Color::GREEN) window.draw(colorRects.at("green"));
    else if (topColor == Card::Color::BLUE) window.draw(colorRects.at("blue"));

    for (auto card : player.getHand()) window.draw(card.sprite);
    if (movingCard) window.draw(cardPicked->sprite);
}

void Client::drawAnimation() {

    std::vector<Card>& hand = player.getHand();
    int spacing = SCREEN_WIDTH / (hand.size() + 1);

    sf::Vector2f oldPos[hand.size() - 1];
    sf::Vector2f newPos[hand.size() - 1];

    for (int i = 0; i < hand.size() - 1; ++i)
        oldPos[i] = hand[i].sprite.getPosition();

    for (int i = 0; i < hand.size(); ++i)
        hand[i].sprite.setPosition(spacing * (i + 1) - hand[i].sprite.getGlobalBounds().width / 2, SCREEN_HEIGHT * 0.75);

    for (int i = 0; i < hand.size() - 1; ++i) {
        newPos[i] = hand[i].sprite.getPosition();
        hand[i].sprite.setPosition(oldPos[i]);
    }

    sf::Vector2f targetPos(hand[hand.size() - 1].sprite.getPosition().x, hand[hand.size() - 1].sprite.getPosition().y);
    sf::Vector2f startingPos(SCREEN_WIDTH / 2 - 20 - topCard.sprite.getGlobalBounds().width / 2 - hand[hand.size() - 1].sprite.getGlobalBounds().width / 2,
                             topCard.sprite.getPosition().y + topCard.sprite.getGlobalBounds().height);
                            
    sf::Vector2f diff = targetPos - startingPos;
    float diffNorm = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
    sf::Vector2f diffUnit(diff.x / diffNorm, diff.y / diffNorm);

    float speed = 1500.f;
    hand[hand.size() - 1].sprite.setPosition(startingPos);

    float dt = 1 / 120.0;

    bool continueAnimation = true;
    float speed2 = -1500.f;
    sf::Clock clock;
    while (continueAnimation) {
        continueAnimation = false;
        float frameTime = clock.restart().asSeconds();
        if (hand.size() > 1) {
            for (int i = 0; i < hand.size() - 1; ++i) {
                if (hand[i].sprite.getPosition().x > newPos[i].x) {
                    hand[i].sprite.move(speed2 * dt, 0);
                    continueAnimation = true;
                }
            }
        }
        while (frameTime > 0.0) {
            if (hand[hand.size() - 1].sprite.getPosition().x < targetPos.x && hand[hand.size() - 1].sprite.getPosition().y < targetPos.y) {
                float deltaTime = std::min(frameTime, dt);
                sf::Vector2f v = speed * diffUnit;
                hand[hand.size() - 1].sprite.move(v * deltaTime);
                frameTime -= deltaTime;
                continueAnimation = true;
            } else frameTime = 0;
        }
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        drawEverything();
        window.display();
    }
    hand[hand.size() - 1].sprite.setPosition(targetPos);
}

void Client::rearrangeHandAnimation() {

    std::vector<Card>& hand = player.getHand();
    int spacing = SCREEN_WIDTH / (hand.size() + 1);
    
    sf::Vector2f oldPos[hand.size()];
    sf::Vector2f newPos[hand.size()];

    for (int i = 0; i < hand.size(); ++i)
        oldPos[i] = hand[i].sprite.getPosition();

    for (int i = 0; i < hand.size(); ++i)
        newPos[i] = sf::Vector2f(spacing * (i + 1) - hand[i].sprite.getGlobalBounds().width / 2, SCREEN_HEIGHT * 0.75);

    float speed = 1200.f;
    bool continueAnimation = true;
    sf::Clock clock;

    while (continueAnimation) {
        continueAnimation = false;
        float deltaTime = clock.restart().asSeconds();
        for (int i = 0; i < hand.size(); ++i) {
            if (newPos[i].x <= oldPos[i].x) {
                if (hand[i].sprite.getPosition().x  >= newPos[i].x) {
                    hand[i].sprite.move(-speed * deltaTime, 0);
                    continueAnimation = true;
                }
            } else {
                if (hand[i].sprite.getPosition().x <= newPos[i].x) {
                    hand[i].sprite.move(speed * deltaTime, 0);
                    continueAnimation = true;
                }
            }
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
            }
        }
        drawEverything();
        window.display();
    }
}

void Client::cardPlayedAnimation(Card& card) {

    sf::Clock clock;
    float speed = -1200;
    card.sprite.setPosition(SCREEN_WIDTH, SCREEN_HEIGHT / 2.4 - card.sprite.getGlobalBounds().height / 2);
    while (card.sprite.getPosition().x > topCard.sprite.getPosition().x) {
        float dt = clock.restart().asSeconds();
        drawEverything();
        card.sprite.move(speed * dt, 0.f);
        window.draw(card.sprite);
        window.display();
    }
}

void Client::putCardBackInHandAnimation(Card& card) {

    int cardPosInHand;
    std::vector<Card>& hand = player.getHand();
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i].getColor() == card.getColor() &&
            hand[i].getValue() == card.getValue() &&
            hand[i].getUniqueID() == card.getUniqueID()) {

            cardPosInHand = i;
            break;
        }
    }

    sf::Vector2f startingPos(card.sprite.getPosition().x, card.sprite.getPosition().y);
    sf::Vector2f targetPos((SCREEN_WIDTH / (hand.size() + 1)) * (cardPosInHand + 1) - card.sprite.getGlobalBounds().width / 2, SCREEN_HEIGHT * 0.75);

    sf::Vector2f diff = targetPos - startingPos;
    float normDiff = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
    sf::Vector2f diffUnit(diff.x / normDiff, diff.y / normDiff);

    float speed = 3000.f;
    float distanceTraveled = 0.f;
    float dt = 1 / 120.0;
    card.sprite.setPosition(startingPos);

    sf::Clock clock;

    while (distanceTraveled < normDiff) {
        float frameTime = clock.restart().asSeconds();
        while (frameTime > 0.0) {
            float deltaTime = std::min(frameTime, dt);
            sf::Vector2f v = speed * diffUnit;
            sf::Vector2f travelVector = v * deltaTime;
            distanceTraveled += (sqrt(pow(travelVector.x, 2) + pow(travelVector.y, 2)));
            if (distanceTraveled < normDiff) card.sprite.move(travelVector);

            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
            }

            frameTime -= deltaTime;
        }
        drawEverything();
        window.display(); 
    }
    card.sprite.setPosition(targetPos);
}
