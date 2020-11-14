#include "uno.h"

int main() {
    //définition variables
    const int MAX_ERROR_DESC_LENGTH = 100;

    char description[MAX_ERROR_DESC_LENGTH] = ":)";

    sf::TcpListener *listener = new sf::TcpListener;
    sf::RenderWindow window(
            sf::VideoMode(sf::VideoMode::getDesktopMode().width > 2560 ? sf::VideoMode::getDesktopMode().width / 2 : sf::VideoMode::getDesktopMode().width,
            sf::VideoMode::getDesktopMode().height), "Uno", sf::Style::Titlebar | sf::Style::Close);

    Menu menu(window, listener);
backToMenu:
    Player player = menu.initMenu();

    if (player.isHost && window.isOpen()) {
        Server *server = new Server(listener);
        std::thread serverThread(&Server::lobby, server);
        serverThread.detach();
        std::cout << "Serveur lancé" << std::endl;
    }

    //window.clear(sf::Color(100, 100, 100));
    if (window.isOpen()) {
        Client *client = new Client(window, player);
        int action = client->lobby();
        if (action == 1) {
            goto backToMenu;
        }
    }

    /*Server *server = new Server;
    char who;
    std::cout << "server (s) or client (c) ?" << std::endl;
    std::cin >> who;

    if (who == 's') {
        std::cout << "server about to start yo" << std::endl;
        std::thread serverThread(&Server::lobby, server);
        serverThread.join();
    } else if (who == 'c') {
        sf::TcpSocket *socket = new sf::TcpSocket;
        if (socket->connect("192.168.1.8", 55001) == sf::Socket::Done) {
            //throw std::runtime_error("sf::TcpSocket::connect()");
            int prefix = 0;
            while (1) {
                sf::Packet packet;
                packet << prefix;
                std::string info;
                cin >> info;
                packet << info;
                socket->send(packet);
            }
        }
     }*/



    //Initialisation de tout
    /*if (0!startInitialize("var1, var2, var3, etc...")) {
        cout << "Erreur : " << description << endl;
        return 1;
    }*/

    /*while(1) { //Remplacer "1" par "SIGTERMN'T"   Ceci est la boucle principale du jeu, c'est à partir de la que la bibliothèque graphique rentre en heu et attend les inputs.
        switch(buttonClicked) {
            case 1:
                //game(); //TODO
                break;
            case 2:
                //settings(); //TODO
                break;
            case 3:
                //Send SIGTERM
                break;
            default :
                cout << "{insérer message marrant soulignant le fait que le programme ne soit pas censé exécuter cette ligne}" << endl;
        }
    }*/

    //quitFreeEverything("var1, var2, var3, etc...");
    return 0;
}

    //Écran titre (JOUER | OPTIONS | QUITTER) (boucle d'affichage + attente d'évènements (bon, après ça ce sera tout le long...))
    /*
    Si JOUER on arrive sur un nouvel écran (après une transition de génie dans le menu trop kawaii que Morgan va coder).
    On a un bouton créer et un bouton rejoindre (serveur / client).

    En cliquant sur créer on précise les paramètres du serveur sur un écran (Interface(s), port(s)) (/!\ penser à gérer les permissions du pare feu /!\)
    En validant on est mis en attente de joueurs (qui apparaîtront en liste) sur un écran de réglages de la partie (durée max tour, règles alternatives, etc).
    Ensuite, on passera sur la boucle du jeu lui-même avec tous les affichages et toutes les gestions de messages que ça implique.

    En cliquant sur rejoindre on donne l'ip et le port auquels se connecter (+gestion dns?) (/!\ gaffe au pare feu encore une fois /!\)
    On arrive dans la salle de l'hôte et on apparaît dans la liste des joueurs (les options en train d'être réglées par l'hôte sont visibles).
    Idem que l'hôte à partir du lancement.
    */

    /*
    Si OPTIONS lol on verra. (skins pour les cartes?, changer son pseudo?, volume du son du jeu? (si on met du son))
    */

    /*
    Si QUITTER mdr on quitte. (attention à gérer l'arrêt de la bibliothèque graphique et à démalloquer tout le monde)
    */
