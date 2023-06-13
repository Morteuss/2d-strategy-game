#include "InputManager.h"

#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Constants.h"
#include "Game.h"
#include "GameEntity.h"
#include "GameLibrary.h"
#include "ComponentMovement.h"

using namespace std;

InputManager::InputManager(sf::RenderWindow &window, sf::View &gameCamera) : window(window), gameCamera(gameCamera) {

}

void InputManager::handleInput(Game &game) {
    double moveValue = 40;
    sf::Event event;

    if (!game.gameConsole.isVisible()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            gameCamera.move(-moveValue, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            gameCamera.move(moveValue, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            gameCamera.move(0, -moveValue);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            gameCamera.move(0, moveValue);
        }
    }

    while (window.pollEvent(event)) {
        game.gameConsole.processInput(event, game.debugDisplaySystem);

        if (event.type == sf::Event::Resized) {
            std::cout << "Nowa szerokosc: " << event.size.width << "\n";
            std::cout << "Nowa wysokosc: " << event.size.height << "\n";
        }
        if (event.type == sf::Event::Closed) {
            window.close();
            return;
        }
        if (event.type == sf::Event::MouseButtonReleased) {
            int mouseX = event.mouseButton.x;
            int mouseY = event.mouseButton.y;

            sf::Vector2i pixelPos(mouseX, mouseY);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            if (!(worldPos.x < 0 || worldPos.x > Constants::MAP_WIDTH * Constants::TILE_WIDTH || worldPos.y < 0 ||
                  worldPos.y > Constants::MAP_HEIGHT * Constants::TILE_HEIGHT)) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // kliknelismy myszka na menu wydawania rozkazow
                    if (game.contextMenuOrders.processMousePosition(mouseX, mouseY, game.taskExecuteSystem,
                                                                    game.window)) {

                    }
                        // kliknelismy myszka na mape swiata gry
                    else {
                        gl::changePositionToWorldPosition(mouseX, mouseY, window);
                        for (auto &object: game.objects) {
                            if (object.getExists()) {
                                sf::FloatRect objectRect(
                                        object.getPosX(), object.getPosY(),
                                        Constants::TILE_WIDTH, Constants::TILE_HEIGHT
                                );

                                if (objectRect.contains(mouseX, mouseY)) {
                                    std::cout << object.getInformationString() << std::endl;
                                    break;
                                }
                            }
                        }
                    }
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    game.contextMenuOrders.setPosition(mouseX, mouseY);
                    game.contextMenuOrders.setVisible(true);

                    gl::changePositionToWorldPosition(mouseX, mouseY, window);
                    int tileIndex = gl::calculateTileIndex(mouseX, mouseY);
                    game.contextMenuOrders.setSelectedTileIndex(tileIndex);
                }
            } else {
                std::cout << "Klikniecie poza obszarem gry!\n";
            }
        }
        if (event.type == sf::Event::MouseWheelScrolled) {
            gameCamera.zoom(1 + (-event.mouseWheelScroll.delta) * 0.1);
            gameCamera.setSize(std::floor(gameCamera.getSize().x), std::floor(gameCamera.getSize().y));
        }
        if (event.type == sf::Event::KeyReleased) {
            switch (event.key.code) {
                case sf::Keyboard::F1 : {
                    game.gameConsole.changeVisible();
                    break;
                }
                case sf::Keyboard::Escape : {
                    if (game.contextMenuOrders.isVisible()) {
                        game.contextMenuOrders.setVisible(false);
                        break;
                    }
                    window.close();
                    return;
                }
                case sf::Keyboard::Space : {
                    game.gamePause = !game.gamePause;
                    break;
                }
                case sf::Keyboard::Q : {
                    window.create(sf::VideoMode(sf::VideoMode::getDesktopMode().width,
                                                sf::VideoMode::getDesktopMode().height), "Tytul okna",
                                  sf::Style::Fullscreen);
                    window.setVerticalSyncEnabled(true);
                    break;
                }

                case sf::Keyboard::M : {
                    game.gameMap.saveMap();
                    break;
                }

                case sf::Keyboard::Num1 : {
                    game.setGameSpeed(Constants::GAME_SPEED);
                    break;
                }
                case sf::Keyboard::Num2 : {
                    game.setGameSpeed(Constants::GAME_SPEED / 2);
                    break;
                }
                case sf::Keyboard::Num3 : {
                    game.setGameSpeed(Constants::GAME_SPEED / 3);
                    break;
                }
                default : {
                    break;
                }
            }
        }
    }
}
