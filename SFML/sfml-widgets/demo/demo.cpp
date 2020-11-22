#include "Gui/Menu.hpp"
#include "Gui/Theme.hpp"
#include "Gui/Gui.hpp"
#include <SFML/Graphics.hpp>

sf::Color hex2color(const std::string& hexcolor)
{
    sf::Color color = sf::Color::Black;
    if (hexcolor.size() == 7) // #ffffff
    {
        color.r = strtoul(hexcolor.substr(1, 2).c_str(), NULL, 16);
        color.g = strtoul(hexcolor.substr(3, 2).c_str(), NULL, 16);
        color.b = strtoul(hexcolor.substr(5, 2).c_str(), NULL, 16);
    }
    else if (hexcolor.size() == 4) // #fff
    {
        color.r = strtoul(hexcolor.substr(1, 1).c_str(), NULL, 16) * 17;
        color.g = strtoul(hexcolor.substr(2, 1).c_str(), NULL, 16) * 17;
        color.b = strtoul(hexcolor.substr(3, 1).c_str(), NULL, 16) * 17;
    }
    return color;
}

struct Theme
{
    sf::Color backgroundColor;
    std::string texturePath;
};

int main()
{
    Theme defaultTheme = {
        hex2color("#dddbde"),
        "demo/texture-default.png"
    };

    Theme win98Theme = {
        hex2color("#d4d0c8"),
        "demo/texture-win98.png"
    };

    // Create the main window
    sf::RenderWindow app(sf::VideoMode(640, 480), "SFML Widgets", sf::Style::Close);

    gui::Menu menu(app);
    menu.setPosition(10, 10);

    gui::Theme::loadFont("demo/tahoma.ttf");
    gui::Theme::loadTexture(defaultTheme.texturePath);
    gui::Theme::textSize = 11;
    gui::Theme::click.textColor      = hex2color("#191B18");
    gui::Theme::click.textColorHover = hex2color("#191B18");
    gui::Theme::click.textColorFocus = hex2color("#000");
    gui::Theme::input.textColor = hex2color("#000");
    gui::Theme::input.textColorHover = hex2color("#000");
    gui::Theme::input.textColorFocus = hex2color("#000");
    gui::Theme::PADDING = 2.f;
    gui::Theme::windowBgColor = defaultTheme.backgroundColor;

    gui::HBoxLayout* hbox = menu.addHBoxLayout();
    gui::FormLayout* form = hbox->addFormLayout();

    sf::Text text("Hello world!", gui::Theme::getFont());
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
    text.setPosition(320, 360);

    // Textbox
    gui::TextBox* textbox = new gui::TextBox();
    textbox->setText("Hello world!");
    textbox->setCallback([&]() {
        text.setString(textbox->getText());
        text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
    });
    form->addRow("Text", textbox);
    // Start the application loop
    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Send events to menu
            menu.onEvent(event);
            if (event.type == sf::Event::Closed)
                app.close();
        }

        // Clear screen
        app.clear(gui::Theme::windowBgColor);
        app.draw(menu);
        app.draw(text);
        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}
