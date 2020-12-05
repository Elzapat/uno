cmake_minimum_required(VERSION 3.10)

project(Uno VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SFML 2.5 COMPONENTS system window graphics network REQUIRED)
add_executable(Uno main.cpp classes/Player.cpp classes/Player.h
    classes/Card.cpp classes/Card.h classes/Deck.cpp classes/Deck.h
    classes/Menu.cpp classes/Menu.h classes/Server.cpp classes/Server.h
    classes/Client.cpp classes/Client.h uno.h classes/functions.cpp)
target_link_libraries(Uno /home/morgan/Programmation/Projects/Uno/SFML/sfml-widgets/lib/libsfml-widgets.a)
target_link_libraries(Uno sfml-graphics sfml-window sfml-system sfml-network
    sfml-audio pthread GL)

#configure_file(UnoConfig.h.in UnoConfig.h)

target_include_directories(Uno PUBLIC "${PROJECT_BINARY_DIR}")
