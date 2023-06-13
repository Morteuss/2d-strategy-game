#ifndef COMPONENT_GRAPHICS_H
#define COMPONENT_GRAPHICS_H

#include "SFML/Graphics.hpp"
#include "Component.h"

class GameEntity;

class ComponentGraphics : public Component {
private:
    std::string fileName;
    sf::Texture texture;
    sf::Sprite sprite;
    unsigned int alphaValue;

public:
    ComponentGraphics(const std::string& fileName, unsigned int alphaValue = 255);

    ComponentGraphics *clone() const override;

    std::string getFileName() const;

    unsigned int getAlphaValue() const;

    void acceptMessage(MessageType messageType);

    void render(GameEntity &gameEntity, sf::RenderWindow &window, double timeProgressValue);
};

#endif // COMPONENT_GRAPHICS_H
