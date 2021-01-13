#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <vector>
#include <string>
#include "../SFML/include/SFML/System.hpp"
#include "../SFML/include/SFML/Window.hpp"
#include "../SFML/include/SFML/Graphics.hpp"

#define DELIM "."

sf::Color hex2color(const std::string& hexcolor);
bool validateIP(std::string ip);
std::vector<std::string> split(const std::string& str, char delim);
bool chkNumber(const std::string& str);

#endif
