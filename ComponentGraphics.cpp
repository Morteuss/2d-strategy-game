#include "GameEntity.h"
#include "ComponentGraphics.h"

#include <iostream>

ComponentGraphics::ComponentGraphics(const std::string& fileName, unsigned int alphaValue) :
        fileName(fileName),
        alphaValue(alphaValue) {
    texture.loadFromFile(fileName);
    texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.setColor(sf::Color(255, 255, 255, this->alphaValue));
}

ComponentGraphics *ComponentGraphics::clone() const {
    return new ComponentGraphics(*this);
}

std::string ComponentGraphics::getFileName() const {
    return fileName;
}

unsigned int ComponentGraphics::getAlphaValue() const {
    return alphaValue;
}

void ComponentGraphics::acceptMessage(MessageType messageType) {
}

void ComponentGraphics::render(GameEntity &gameEntity, sf::RenderWindow &window, double timeProgressValue) {
    sprite.setPosition(gameEntity.getPosX(), gameEntity.getPosY());

    // timeProgressValue uzyte w states do interpolacji / ekstrapolacji pozycji do wyswietlenia miedzy updatami silnika gry
    sf::RenderStates states;
    states.transform.translate(gameEntity.getCurrentChangeInPosX() * timeProgressValue,
                               gameEntity.getCurrentChangeInPosY() * timeProgressValue);

    window.draw(sprite, states);
}
