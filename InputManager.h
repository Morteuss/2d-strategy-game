#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

class Game;
namespace sf {
    class RenderWindow;

    class View;
}

class InputManager {
private:
    sf::RenderWindow &window;
    sf::View &gameCamera;
public:
    InputManager(sf::RenderWindow &window, sf::View &gameCamera);

    void handleInput(Game &game);
};

#endif // INPUT_MANAGER_H
