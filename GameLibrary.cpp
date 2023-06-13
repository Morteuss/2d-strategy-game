#include "GameLibrary.h"

#include "ComponentDestructible.h"
#include "ComponentLiftable.h"
#include "Constants.h"
#include "Map.h"
#include "ComponentMovement.h"
#include "ComponentTaskExecute.h"

namespace gl {
    GameEntity createCharacter(int tileIndexX, int tileIndexY, Map &gameMap) {
        GameEntity character(tileIndexX, tileIndexY, new ComponentGraphics(Constants::graphicsPathToCharacter));
        character.addComponent(new ComponentTaskExecute());
        character.addComponent(new ComponentMovement(gameMap));

        return character;
    }

    GameEntity createTreeSapling(int tileIndexX, int tileIndexY) {
        GameEntity tree(tileIndexX, tileIndexY, new ComponentGraphics(Constants::graphicsPathToTreeSapling));
        tree.addComponent(new ComponentDestructible());

        return tree;
    }

    GameEntity createTreeYoung(int tileIndexX, int tileIndexY) {
        GameEntity tree(tileIndexX, tileIndexY, new ComponentGraphics(Constants::graphicsPathToTreeYoung));
        tree.addComponent(new ComponentDestructible());

        return tree;
    }

    GameEntity createTreeOld(int tileIndexX, int tileIndexY) {
        GameEntity tree(tileIndexX, tileIndexY, new ComponentGraphics(Constants::graphicsPathToTreeOld));
        tree.addComponent(new ComponentDestructible());

        return tree;
    }

    GameEntity createResourceWood(int tileIndexX, int tileIndexY) {
        GameEntity resourceWood(tileIndexX, tileIndexY, new ComponentGraphics(Constants::graphicsPathToResourceTree));
        resourceWood.addComponent(new ComponentLiftable());

        return resourceWood;
    }

    GameEntity createWall(int tileIndexX, int tileIndexY, Map &gameMap) {
        GameEntity wall(tileIndexX, tileIndexY, new ComponentGraphics(Constants::graphicsPathToWall));
        gameMap.setOccupied(gl::convertIndexesToIndex(tileIndexX, tileIndexY), true);

        return wall;
    }

    GameEntity createWallBlueprint(int tileIndexX, int tileIndexY) {
        GameEntity wallBlueprint(tileIndexX, tileIndexY, new ComponentGraphics(Constants::graphicsPathToWall,
                                                                               Constants::BLUEPRINT_ALPHA_VALUE));

        return wallBlueprint;
    }

    int calculateTileIndex(int positionX, int positionY) {
        int indexX = positionX / Constants::TILE_WIDTH;
        int indexY = positionY / Constants::TILE_HEIGHT;

        int index = indexX + indexY * Constants::MAP_WIDTH;

        return index;
    }

    int calculateHeuristicDistance(int startTileIndex, int endTileIndex) {
        int startPosX = startTileIndex % Constants::MAP_WIDTH;
        int startPosY = startTileIndex / Constants::MAP_HEIGHT;
        int endPosX = endTileIndex % Constants::MAP_WIDTH;
        int endPosY = endTileIndex / Constants::MAP_WIDTH;

        return std::abs(endPosX - startPosX) + std::abs(endPosY - startPosY);
    }

    void changePositionToWorldPosition(int &mouseX, int &mouseY, sf::RenderWindow &window) {
        sf::Vector2i pixelPos(mouseX, mouseY);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
        mouseX = worldPos.x;
        mouseY = worldPos.y;
    }

    int convertIndexesToIndex(int tileIndexX, int tileIndexY) {
        return tileIndexX + tileIndexY * Constants::MAP_WIDTH;
    }
}
