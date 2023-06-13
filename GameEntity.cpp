#include "GameEntity.h"

#include <iostream>

#include "Constants.h"
#include "ComponentGraphics.h"

int GameEntity::entitesNumber = 0;

GameEntity::GameEntity(unsigned int posXIndex, unsigned int posYIndex, ComponentGraphics *graphics) : graphicsComponent(
        graphics) {
    this->posX = posXIndex * Constants::TILE_WIDTH;
    this->posY = posYIndex * Constants::TILE_HEIGHT;
    this->currentChangeInPosX = 0;
    this->currentChangeInPosY = 0;
    this->exists = true;

    entitesNumber++;
}

GameEntity::GameEntity(const GameEntity &gameEntity) {
    this->posX = gameEntity.posX;
    this->posY = gameEntity.posY;
    this->currentChangeInPosX = gameEntity.currentChangeInPosX;
    this->currentChangeInPosY = gameEntity.currentChangeInPosY;
    this->exists = gameEntity.exists;

    this->graphicsComponent = std::unique_ptr<ComponentGraphics>(
            new ComponentGraphics(gameEntity.graphicsComponent->getFileName(),
                                  gameEntity.graphicsComponent->getAlphaValue()));

    // unique_ptr nie da sie kopiowac, wiec vector trzymajacy unique_ptr tez nie moze byc skopiowany. Robimy to wiec "manualnie" uzywajac wzorzec Clone()
    components.reserve(gameEntity.components.size());
    for (auto &i: gameEntity.components) {
        components.emplace_back(i->clone());
    }

    entitesNumber++;
}

GameEntity::~GameEntity() {
    entitesNumber--;
}

GameEntity &GameEntity::operator=(const GameEntity &gameEntity) {
    if (this != &gameEntity) {
        this->posX = gameEntity.posX;
        this->posY = gameEntity.posY;
        this->currentChangeInPosX = gameEntity.currentChangeInPosX;
        this->currentChangeInPosY = gameEntity.currentChangeInPosY;
        this->exists = gameEntity.exists;
        this->graphicsComponent = std::unique_ptr<ComponentGraphics>(
                new ComponentGraphics(gameEntity.graphicsComponent->getFileName(),
                                      gameEntity.graphicsComponent->getAlphaValue()));

        // unique_ptr nie da sie kopiowac, wiec vector trzymajacy unique_ptr tez nie moze byc skopiowany. Robimy to wiec "manualnie" uzywajac wzorzec Clone()
        components.clear();
        components.reserve(gameEntity.components.size());
        for (auto &i: gameEntity.components) {
            components.emplace_back(i->clone());
        }
    }

    return *this;
}

void GameEntity::addComponent(Component *newComponent) {
    components.emplace_back(newComponent);
}

std::string GameEntity::getInformationString() const {
    if (exists) {
        std::string informationString;

        if (graphicsComponent) {
            informationString += graphicsComponent->getInformationString();
            informationString += '\n';
        }

        for (auto &componentReference: components) {
            informationString += componentReference->getInformationString();
            informationString += '\n';
        }
        return informationString;
    } else {
        return "";
    }
}

void GameEntity::render(sf::RenderWindow &window, double timeProgressValue) {
    if (exists) {
        graphicsComponent->render(*this, window, timeProgressValue);
    }
}

void GameEntity::sendMessage(MessageType messageType) {
    if (exists) {
        for (auto &componentReference: components) {
            componentReference->acceptMessage(messageType);
        }
    }
}

void GameEntity::update() {
    if (exists) {
        for (auto it = components.begin(); it != components.end(); ++it) {
            (*it)->update(*this);
        }
    }
}

float GameEntity::getCurrentChangeInPosX() const {
    return currentChangeInPosX;
}

float GameEntity::getCurrentChangeInPosY() const {
    return currentChangeInPosY;
}

float GameEntity::getPosX() const {
    return posX;
}

float GameEntity::getPosY() const {
    return posY;
}

bool GameEntity::getExists() const {
    return exists;
}

void GameEntity::setCurrentChangeInPosX(float currentChangeInPosX) {
    this->currentChangeInPosX = currentChangeInPosX;
}

void GameEntity::setCurrentChangeInPosY(float currentChangeInPosY) {
    this->currentChangeInPosY = currentChangeInPosY;
}

void GameEntity::setPosX(float posX) {
    this->posX = posX;
}

void GameEntity::setPosY(float posY) {
    this->posY = posY;
}

void GameEntity::setExists(bool exists) {
    this->exists = exists;
}
