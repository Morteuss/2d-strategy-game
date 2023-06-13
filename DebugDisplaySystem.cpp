#include "DebugDisplaySystem.h"

#include <iostream>
#include <typeinfo>

#include "ComponentDestructible.h"
#include "Constants.h"
#include "GameEntity.h"
#include "GameLibrary.h"
#include "ComponentMovement.h"
#include "RandomNumberGenerator.h"

using namespace std;

DebugDisplaySystem::DebugDisplaySystem(int alphaValue) : alphaValue(alphaValue) {
    mapVertices.setPrimitiveType(sf::Quads);
}

void DebugDisplaySystem::removeTrees(std::vector<GameEntity> &objects) {
    for (auto &object: objects) {
        ComponentDestructible *componentDestructiblePtr = gl::getComponentPtr<ComponentDestructible>(object);

        if (componentDestructiblePtr != nullptr) {
            object.setExists(false);
        }
    }
}

void DebugDisplaySystem::showAreas(Map &gameMap) {
    mapVertices.clear();

    int tileCount = Constants::MAP_WIDTH * Constants::MAP_HEIGHT;
    mapVertices.resize(tileCount * 4);

    std::map<int, sf::Color> areasMap;

    for (int i = 0; i < tileCount; ++i) {
        positionQuad(i);

        sf::Vertex *currentQuad = &mapVertices[i * 4];

        if (areasMap.count(gameMap.getAreaNumber(i)) == 0) {
            areasMap[gameMap.getAreaNumber(i)] = sf::Color(
                    RandomNumberGenerator::getIntNumber(0, 255),
                    RandomNumberGenerator::getIntNumber(0, 255),
                    RandomNumberGenerator::getIntNumber(0, 255)
            );
        }

        currentQuad[0].color = areasMap[gameMap.getAreaNumber(i)];
        currentQuad[1].color = areasMap[gameMap.getAreaNumber(i)];
        currentQuad[2].color = areasMap[gameMap.getAreaNumber(i)];
        currentQuad[3].color = areasMap[gameMap.getAreaNumber(i)];
    }
}

void DebugDisplaySystem::showPaths(std::vector<GameEntity> &objects) {
    mapVertices.clear();

    int tileCount = Constants::MAP_WIDTH * Constants::MAP_HEIGHT;
    mapVertices.resize(tileCount * 4);

    for (int i = 0; i < tileCount; ++i) {
        positionQuad(i);

        sf::Vertex *currentQuad = &mapVertices[i * 4];
        currentQuad[0].color.a = 0;
        currentQuad[1].color.a = 0;
        currentQuad[2].color.a = 0;
        currentQuad[3].color.a = 0;
    }

    for (auto &object: objects) {
        ComponentMovement *movementComponentPtr = gl::getComponentPtr<ComponentMovement>(object);

        if (movementComponentPtr != nullptr) {
            sf::Color pathColor = sf::Color(
                    RandomNumberGenerator::getIntNumber(0, 255),
                    RandomNumberGenerator::getIntNumber(0, 255),
                    RandomNumberGenerator::getIntNumber(0, 255)
            );

            const vector<int> &currentPath = movementComponentPtr->getPath();

            for (auto &i: currentPath) {
                sf::Vertex *currentQuad = &mapVertices[i * 4];

                currentQuad[0].color = pathColor;
                currentQuad[1].color = pathColor;
                currentQuad[2].color = pathColor;
                currentQuad[3].color = pathColor;

                currentQuad[0].color.a = alphaValue;
                currentQuad[1].color.a = alphaValue;
                currentQuad[2].color.a = alphaValue;
                currentQuad[3].color.a = alphaValue;
            }
        }
    }
}

void DebugDisplaySystem::showWalkable(Map &gameMap) {
    mapVertices.clear();

    int tileCount = Constants::MAP_WIDTH * Constants::MAP_HEIGHT;
    mapVertices.resize(tileCount * 4);

    for (int i = 0; i < tileCount; ++i) {
        positionQuad(i);

        sf::Vertex *currentQuad = &mapVertices[i * 4];

        if (gameMap.isWalkable(i)) {
            currentQuad[0].color = sf::Color::Green;
            currentQuad[1].color = sf::Color::Green;
            currentQuad[2].color = sf::Color::Green;
            currentQuad[3].color = sf::Color::Green;

            currentQuad[0].color.a = alphaValue;
            currentQuad[1].color.a = alphaValue;
            currentQuad[2].color.a = alphaValue;
            currentQuad[3].color.a = alphaValue;
        } else {
            currentQuad[0].color = sf::Color::Red;
            currentQuad[1].color = sf::Color::Red;
            currentQuad[2].color = sf::Color::Red;
            currentQuad[3].color = sf::Color::Red;

            currentQuad[0].color.a = alphaValue;
            currentQuad[1].color.a = alphaValue;
            currentQuad[2].color.a = alphaValue;
            currentQuad[3].color.a = alphaValue;
        }
    }
}

void DebugDisplaySystem::cancel() {
    mapVertices.clear();
}

void DebugDisplaySystem::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(mapVertices, states);
}

// ustawia czworki wierzcholkow w odpowiednich pozycjach
void DebugDisplaySystem::positionQuad(int quadIndex) {
    int tileIndexX = quadIndex % Constants::MAP_WIDTH;
    int tileIndexY = quadIndex / Constants::MAP_HEIGHT;
    sf::Vertex *currentQuad = &mapVertices[quadIndex * 4];

    currentQuad[0].position = sf::Vector2f(tileIndexX * Constants::TILE_WIDTH, tileIndexY * Constants::TILE_HEIGHT);
    currentQuad[1].position = sf::Vector2f((tileIndexX + 1) * Constants::TILE_WIDTH,
                                           tileIndexY * Constants::TILE_HEIGHT);
    currentQuad[2].position = sf::Vector2f((tileIndexX + 1) * Constants::TILE_WIDTH,
                                           (tileIndexY + 1) * Constants::TILE_HEIGHT);
    currentQuad[3].position = sf::Vector2f(tileIndexX * Constants::TILE_WIDTH,
                                           (tileIndexY + 1) * Constants::TILE_HEIGHT);
}
