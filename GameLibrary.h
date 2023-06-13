#ifndef GAME_LIBRARY_H
#define GAME_LIBRARY_H

#include "GameEntity.h"
#include "ComponentGraphics.h"

class Map;

namespace gl {
    // factory functions
    GameEntity createCharacter(int tileIndexX, int tileIndexY, Map &gameMap);

    GameEntity createTreeSapling(int tileIndexX, int tileIndexY);

    GameEntity createTreeYoung(int tileIndexX, int tileIndexY);

    GameEntity createTreeOld(int tileIndexX, int tileIndexY);

    GameEntity createResourceWood(int tileIndexX, int tileIndexY);

    GameEntity createWall(int tileIndexX, int tileIndexY, Map &gameMap);

    GameEntity createWallBlueprint(int tileIndexX, int tileIndexY);

    // helpful functions
    int calculateTileIndex(int positionX, int positionY);

    int calculateHeuristicDistance(int startTileIndex, int endTileIndex);

    void changePositionToWorldPosition(int &mouseX, int &mouseY, sf::RenderWindow &window);

    int convertIndexesToIndex(int tileIndexX, int tileIndexY);

    // template functions
    template<typename T>
    T *getComponentPtr(GameEntity &gameEntity) {
        for (auto &componentReference: gameEntity.components) {
            if (typeid(T) == typeid(*componentReference)) {
                return static_cast<T *>(componentReference.get());
            }
        }
        return nullptr;
    }
}

#endif // GAME_LIBRARY_H
