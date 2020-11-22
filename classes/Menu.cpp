#include "Menu.h"

Menu::Menu(sf::RenderWindow &initWindow, sf::TcpListener *initListener) : window(initWindow), listener(initListener) {};

Player Menu::initMenu() {

    SCREEN_WIDTH = window.getSize().x;
    SCREEN_HEIGHT = window.getSize().y;

    const int BUTTON_WIDTH = 404;
    const int BUTTON_HEIGHT = 107;
    const int SCREEN_CENTER_X = SCREEN_WIDTH / 2;
    const int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;

    sf::Image icon = sf::Image{};
    icon.loadFromFile("assets/sprites/icon.png");
    
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    window.setFramerateLimit(30);
    window.clear();

    background.texture.loadFromFile("assets/sprites/background.jpg");
    background.sprite.setTexture(background.texture);
    background.sprite.setPosition(0,0);
    window.draw(background.sprite);

    logo.texture.loadFromFile("assets/sprites/uno.png");
    logo.sprite.setTexture(logo.texture);
    logo.sprite.scale((float)SCREEN_WIDTH / 1920, (float)SCREEN_HEIGHT / 1080);
    logo.sprite.setPosition((SCREEN_WIDTH / 2) - (logo.sprite.getGlobalBounds().width / 2), (SCREEN_HEIGHT / 3.7) - (logo.sprite.getGlobalBounds().height / 2));
    window.draw(logo.sprite);

    sf::Texture buttonsTexture;
    sf::Texture buttonsTextureH;
    sf::Texture buttons2Texture;
    sf::Texture buttons2TextureH;
    sf::Texture buttons3Texture;
    sf::Texture buttons3TextureH;
    buttonsTexture.loadFromFile("assets/sprites/buttons.png");
    buttonsTextureH.loadFromFile("assets/sprites/buttons_hovered.png");
    buttons2Texture.loadFromFile("assets/sprites/buttons2.png");
    buttons2TextureH.loadFromFile("assets/sprites/buttons2_hovered.png");
    buttons3Texture.loadFromFile("assets/sprites/buttons3.png");
    buttons3TextureH.loadFromFile("assets/sprites/buttons3_hovered.png");

    initButton(playButton, buttonsTexture, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.1, sf::IntRect(98, 95, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(playButtonH, buttonsTextureH, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.1, sf::IntRect(98, 95, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(settingsButton, buttonsTexture, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.4, sf::IntRect(98, 245, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(settingsButtonH, buttonsTextureH, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.4, sf::IntRect(98, 245, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(quitButton, buttonsTexture, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.7, sf::IntRect(98, 397, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(quitButtonH, buttonsTextureH, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.7, sf::IntRect(98, 397, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(backButton, buttons2Texture, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.7, sf::IntRect(98, 397, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(backButtonH, buttons2TextureH, SCREEN_CENTER_X, SCREEN_CENTER_Y * 1.7, sf::IntRect(98, 397, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(joinButton, buttons2Texture, SCREEN_CENTER_X - 400, SCREEN_CENTER_Y * 0.8, sf::IntRect(98, 95, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(joinButtonH, buttons2TextureH, SCREEN_CENTER_X - 400, SCREEN_CENTER_Y * 0.8, sf::IntRect(98, 95, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(hostButton, buttons2Texture, SCREEN_CENTER_X + 400, SCREEN_CENTER_Y * 0.8, sf::IntRect(98, 245, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(hostButtonH, buttons2TextureH, SCREEN_CENTER_X + 400, SCREEN_CENTER_Y * 0.8, sf::IntRect(98, 245, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(connectButton, buttons3Texture, SCREEN_CENTER_X, SCREEN_CENTER_Y * 0.88, sf::IntRect(98, 95, BUTTON_WIDTH, BUTTON_HEIGHT));
    initButton(connectButtonH, buttons3TextureH, SCREEN_CENTER_X, SCREEN_CENTER_Y * 0.88, sf::IntRect(98, 95, BUTTON_WIDTH, BUTTON_HEIGHT));

    gui::Theme::loadFont("assets/fonts/ArialCE.ttf");
    gui::Theme::loadTexture("assets/sprites/texture-default.png");
    gui::Theme::click.textColor      = hex2color("#000");
    gui::Theme::click.textColorHover = hex2color("#191b18");
    gui::Theme::click.textColorFocus = hex2color("#000");
    gui::Theme::input.textColor      = hex2color("#000");
    gui::Theme::input.textColorHover = hex2color("#000");
    gui::Theme::input.textColorFocus = hex2color("#000");
    gui::Theme::PADDING = 2.f;
    gui::Theme::MARGIN = 20.f;
    gui::Theme::minWidgetWidth = 50;

    listenForInputs();

    Player player(playerName);
    player.setHostAddress(hostIpAddress);
    player.isHost = isHost;
    player.setSocket(socket);

    return player;
}

void Menu::listenForInputs() {

    sf::Event event;
    sf::Clock clock;
    sf::Time previousTime = clock.getElapsedTime();
    sf::Time currentTime;
    float fps;
    int buttonHovered = 0;


    while (window.isOpen()) {

        window.clear();
        window.draw(background.sprite);
        window.draw(logo.sprite);

        switch(buttonHovered) {
            case 1:
                window.draw(playButtonH.sprite);
                window.draw(settingsButton.sprite);
                window.draw(quitButton.sprite);
                break;
            case 2:
                window.draw(playButton.sprite);
                window.draw(settingsButtonH.sprite);
                window.draw(quitButton.sprite);
                break;
            case 3:
                window.draw(playButton.sprite);
                window.draw(settingsButton.sprite);
                window.draw(quitButtonH.sprite);
                break;
            default:
                window.draw(playButton.sprite);
                window.draw(settingsButton.sprite);
                window.draw(quitButton.sprite);
        }

        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseMoved:
                    if (playButton.sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                        buttonHovered = 1;
                    else if (settingsButton.sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                        buttonHovered = 2;
                    else if (quitButton.sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                        buttonHovered = 3;
                    else buttonHovered = 0;
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (playButton.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            if (hostOrJoin()) return;
                        }
                        else if (settingsButton.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            if (settings()) return;
                        }
                        else if (quitButton.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            window.close();
                        }
                    }
                    break;
            }
        }

        /*currentTime = clock.getElapsedTime();
        fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
        std::cout << "fps=" << fps << std::endl;
        previousTime = currentTime;*/
        window.display();
    }
}

void Menu::initButton(Sprite &button, sf::Texture texture, int x, int y, sf::IntRect textureRect) {

    button.texture = texture;
    button.sprite.setTexture(button.texture);
    button.sprite.setTextureRect(textureRect);
    button.sprite.scale((float)SCREEN_WIDTH / 1920, (float)SCREEN_HEIGHT / 1080);
    button.sprite.setPosition(x - button.sprite.getGlobalBounds().width / 2, y - button.sprite.getGlobalBounds().height / 2);
}

int Menu::settings() {

    sf::RectangleShape backRect(sf::Vector2f(1000 * ((float)SCREEN_WIDTH / 1920), 600 * ((float)SCREEN_HEIGHT / 1080)));
    backRect.setFillColor(sf::Color(15, 57, 84));
    backRect.setPosition(SCREEN_WIDTH / 2 - backRect.getGlobalBounds().width / 2, SCREEN_HEIGHT / 2.7 - backRect.getGlobalBounds().height / 2);
    gui::Theme::textSize = 30;
    
    gui::Menu settingsMenu(window);
    settingsMenu.setPosition(backRect.getPosition().x + 50, backRect.getPosition().y + 50);

    gui::HBoxLayout *hbox = settingsMenu.addHBoxLayout();
    gui::FormLayout *form = hbox->addFormLayout();

    sf::Text nameText("Votre pseudo est : " + playerName, gui::Theme::getFont(), 30);
    nameText.setFillColor(sf::Color(0, 0, 0));
    nameText.setPosition(backRect.getPosition().x + 50, backRect.getPosition().y + 100);

    gui::TextBox *nameTextbox = new gui::TextBox(300);
    nameTextbox->setText(playerName);
    nameTextbox->setPosition(0, 0);
    nameTextbox->setMaxLength(15);
    nameTextbox->setCallback([&]() {
        playerName = nameTextbox->getText().toAnsiString();
        nameText.setString("Votre pseudo est : " + playerName);
    });
    form->addRow("Pseudo ", nameTextbox);

    sf::Event event;
    int buttonHovered = 0;

    while (window.isOpen()) {
        window.clear();
        window.draw(background.sprite);
        window.draw(backRect);
        window.draw(settingsMenu);
        window.draw(nameText);
        switch (buttonHovered) {
            case 1:
                window.draw(backButtonH.sprite);
                break;
            default:
                window.draw(backButton.sprite);
        }
        while (window.pollEvent(event)) {
            settingsMenu.onEvent(event);
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseMoved:
                    if (backButton.sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                        buttonHovered = 1;
                    else buttonHovered = 0;
                    break;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (backButton.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            playerName = nameTextbox->getText().toAnsiString();
                            return 0;
                        }
                    }
                    break;
            }
        }
        window.display();
    }
    return 0;
}

int Menu::hostOrJoin() {

    sf::Event event;
    bool emptyName = false;
    bool serverFail = false;

    sf::Text emptyNameWarning(L"Vous n'avez pas choisi de pseudo, allez dans les options pour en choisir un.", gui::Theme::getFont(), 30);
    emptyNameWarning.setFillColor(sf::Color(255, 0, 0));
    emptyNameWarning.setPosition(SCREEN_WIDTH / 2 - emptyNameWarning.getGlobalBounds().width / 2, SCREEN_HEIGHT / 1.9);

    sf::Text serverFailWarning(L"Le lancement du serveur a échoué, veuillez réessayer.", gui::Theme::getFont(), 30);
    serverFailWarning.setFillColor(sf::Color(255, 0, 0));
    serverFailWarning.setPosition(SCREEN_WIDTH / 2 - serverFailWarning.getGlobalBounds().width / 2, SCREEN_HEIGHT / 1.9);

    int buttonHovered = 0;
    while (window.isOpen()) {
        window.clear();
        window.draw(background.sprite);
        if (emptyName) window.draw(emptyNameWarning);
        if (serverFail) window.draw(serverFailWarning);
        switch (buttonHovered) {
            case 1:
                window.draw(joinButtonH.sprite);
                window.draw(hostButton.sprite);
                window.draw(backButton.sprite);
                break;
            case 2:
                window.draw(joinButton.sprite);
                window.draw(hostButtonH.sprite);
                window.draw(backButton.sprite);
                break;
            case 3:
                window.draw(joinButton.sprite);
                window.draw(hostButton.sprite);
                window.draw(backButtonH.sprite);
                break;
            default:
                window.draw(backButton.sprite);
                window.draw(joinButton.sprite);
                window.draw(hostButton.sprite);
        }
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseMoved:
                        if (joinButton.sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                            buttonHovered = 1;
                        else if (hostButton.sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                            buttonHovered = 2;
                        else if (backButton.sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                            buttonHovered = 3;
                        else buttonHovered = 0;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (joinButton.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            if (joinGame(emptyName)) return 1;
                        }
                        else if (hostButton.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            if (hostGame(emptyName, serverFail)) return 1;
                        }
                        else if (backButton.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                            return 0;
                    }
                    break;
            }
        }
        window.display();
    }
    return 0;
}

int Menu::joinGame(bool &emptyName) {

    if (playerName == "") {
        emptyName = true;
        return 0;
    }

    sf::Event event;
    int buttonHovered = 0;
    bool invalidAddress = false;
    bool connectFail = false;

    sf::RectangleShape backRect(sf::Vector2f(1000 * ((float)SCREEN_WIDTH / 1920), 300 * ((float)SCREEN_HEIGHT / 1080)));
    backRect.setPosition(SCREEN_WIDTH / 2 - backRect.getGlobalBounds().width / 2, SCREEN_HEIGHT / 2.7 - backRect.getGlobalBounds().height / 2);
    backRect.setFillColor(sf::Color(15, 57, 84));

    gui::Theme::textSize = 50 * ((float)SCREEN_WIDTH / 1920);

    gui::Menu ipTextboxMenu(window);

    gui::HBoxLayout *hbox = ipTextboxMenu.addHBoxLayout();
    gui::FormLayout *form = hbox->addFormLayout();

    gui::TextBox *ipTextbox = new gui::TextBox(700);
    ipTextbox->setText(hostIpAddress);
    ipTextbox->setPosition(0, 0);
    ipTextbox->setMaxLength(30);
    ipTextbox->setCallback([&]() {
        hostIpAddress = ipTextbox->getText().toAnsiString();
    });
    form->addRow("", ipTextbox);

    ipTextboxMenu.setPosition(SCREEN_WIDTH / 2 - ipTextboxMenu.getSize().x / 2, SCREEN_HEIGHT / 2.22 - backRect.getGlobalBounds().height / 2);


    sf::Text info(L"Rentrez l'adresse IP de l'hébergeur :", gui::Theme::getFont(), 40 * ((float)SCREEN_WIDTH / 1920));
    info.setFillColor(sf::Color(0, 0, 0));
    info.setPosition(SCREEN_WIDTH / 2 - info.getGlobalBounds().width / 2, backRect.getGlobalBounds().height - 20);

    sf::Text invalidAddressWarning(L"L'adresse IP que vous avez rentrée n'est pas valide.", gui::Theme::getFont(), 30);
    invalidAddressWarning.setFillColor(sf::Color(255, 0, 0));
    invalidAddressWarning.setPosition(SCREEN_WIDTH / 2 - invalidAddressWarning.getGlobalBounds().width / 2, SCREEN_HEIGHT / 1.9);

    sf::Text connectFailWarning(L"La connection a échoué, réessayez ou vérifiez l'adresse IP.", gui::Theme::getFont(), 30);
    connectFailWarning.setFillColor(sf::Color(255, 0, 0));
    connectFailWarning.setPosition(SCREEN_WIDTH / 2 - connectFailWarning.getGlobalBounds().width / 2, SCREEN_HEIGHT / 1.9);

    while (window.isOpen()) {
        window.clear();
        window.draw(background.sprite);
        window.draw(backRect);
        window.draw(info);
        window.draw(ipTextboxMenu);
        if (invalidAddress) window.draw(invalidAddressWarning);
        if (connectFail) window.draw(connectFailWarning);
        switch (buttonHovered) {
            case 1:
                window.draw(connectButtonH.sprite);
                window.draw(backButton.sprite);
                break;
            case 2:
                window.draw(connectButton.sprite);
                window.draw(backButtonH.sprite);
                break;
            default:
                window.draw(connectButton.sprite);
                window.draw(backButton.sprite);
        }
        while (window.pollEvent(event)) {
            ipTextboxMenu.onEvent(event);
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseMoved:
                        if (connectButton.sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                            buttonHovered = 1;
                        else if (backButton.sprite.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y))
                            buttonHovered = 2;
                        else buttonHovered = 0;
                case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (backButton.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
                            return 0;
                        else if (connectButton.sprite.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                            hostIpAddress = ipTextbox->getText().toAnsiString();
                            if (connect(hostIpAddress, playerName, invalidAddress, connectFail)) return 1;
                        }
                    }
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Return) {
                        hostIpAddress = ipTextbox->getText().toAnsiString();
                        if (connect(hostIpAddress, playerName, invalidAddress, connectFail)) return 1;
                    }
                    break;
            }
        }
        window.display();
    }
    return 0;
}

int Menu::connect(std::string ipAddress, std::string name, bool &invalidAddress, bool &connectFail) {

    if (!validateIP(ipAddress)) {
        invalidAddress = true;
        return 0;
    } else invalidAddress = false;

    socket = new sf::TcpSocket;
    socket->setBlocking(true);
    if (socket->connect(ipAddress, Server::PORT, sf::seconds(5)) != sf::Socket::Done) {
        socket->setBlocking(false);
        connectFail = true;
        return 0;
    } else connectFail = false;
    socket->setBlocking(false);

    return 1;
}

int Menu::hostGame(bool &emptyName, bool &serverFail) {

    if (playerName == "") {
        emptyName = true;
        return 0;
    }

    if (listener->listen(Server::PORT) != sf::Socket::Done) {
        serverFail = true;
        return 0;
    } else serverFail = false;

    isHost = true;
    return 1;
}
