#include "Loader.h"

#include "SFML/Graphics.hpp"

void Loader::start(sf::RenderWindow *window, std::atomic<bool> &isLoaded) {
    sf::RectangleShape test;
    test.setSize(sf::Vector2f(window->getSize().x / 2, window->getSize().y / 2));
    test.setOrigin(sf::Vector2f(test.getSize().x / 2, test.getSize().y / 2));
    test.setPosition(window->getSize().x / 2, window->getSize().y / 2);
    test.setFillColor(sf::Color::Red);

    while (true) {
        window->clear(sf::Color::Black);
        test.rotate(0.01f);
        window->draw(test);
        window->display();

        // tego nie modyfikowac, mowi nam kiedy zakonczyc watek
        if (isLoaded == true)
            break;
    }
    window->setActive(false);
}
