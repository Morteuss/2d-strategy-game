#include <iostream>
#include <thread>

#include "SFML\OpenGL.hpp"

#include "Constants.h"
#include "Component.h"
#include "Game.h"
#include "Map.h"

using namespace std;


#include "Loader.h"

int main() {
    while (true) {
        try {
            sf::ContextSettings settings;
            // Nie uzywamy antyaliasingu, gdyz nawet mala wartosc powyzej 0 powoduje powstawanie lini(artefaktow) miedzy plytkami planszy, kiedy uzywany jest zoom
            settings.antialiasingLevel = 0;

            sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width / 2,
                                                  sf::VideoMode::getDesktopMode().height / 2), "The Game xD",
                                    sf::Style::Close, settings);

            window.setFramerateLimit(Constants::FRAME_RATE_LIMIT);
            window.setVerticalSyncEnabled(Constants::VERTICAL_SYNC_ENABLED);

            // Ladowanie gry
            atomic<bool> isLoaded(false);
            window.setActive(false);
            std::thread loadingThread(&Loader::start, &window, std::ref(isLoaded));

            unique_ptr<Game> game(new Game(window, isLoaded));
            loadingThread.join();

            // Gra zostala juz zaladowana i mozemy ja odpalic
            game->run();
        }
        catch (std::exception &e) {
            cout << e.what() << endl;
        }

        cout << "Wpisz 0 aby wylaczyc gre, inna cyfre aby stworzyc nowa gre: \n";

        int a;
        cin >> a;
        if (a == 0)
            break;
    }

    return 0;
}
