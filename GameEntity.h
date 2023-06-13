#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <memory>
#include <vector>

#include "Component.h"

class Component;

class ComponentGraphics;
namespace sf {
    class RenderWindow;
}

class GameEntity {
public:
    GameEntity(unsigned int posXIndex, unsigned int posYIndex, ComponentGraphics *graphics);

    GameEntity(const GameEntity &gameEntity);

    ~GameEntity();

    GameEntity &operator=(const GameEntity &gameEntity);

    float getCurrentChangeInPosX() const;

    float getCurrentChangeInPosY() const;

    float getPosX() const;

    float getPosY() const;

    bool getExists() const;

    void setCurrentChangeInPosX(float currentChangeInPosX);

    void setCurrentChangeInPosY(float currentChangeInPosY);

    void setPosX(float posX);

    void setPosY(float posY);

    void setExists(bool exists);

    void addComponent(Component *newComponent);

    std::string getInformationString() const;

    void render(sf::RenderWindow &window, double timeProgressValue);

    void sendMessage(MessageType messageType);

    void update();

    float posX;
    float posY;
    float currentChangeInPosX;
    float currentChangeInPosY;
    bool exists;
    std::unique_ptr<ComponentGraphics> graphicsComponent;
    static int entitesNumber;

    std::vector<std::unique_ptr<Component>> components;
};

#endif // GAME_ENTITY_H
