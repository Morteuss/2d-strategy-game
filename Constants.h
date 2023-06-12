#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <iostream>

namespace Constants {
    // Size of map
    const int MAP_WIDTH = 100;
    const int MAP_HEIGHT = 100;

    // Number of updates per second
    const unsigned int FRAMES_PER_SECOND = 60;
    const unsigned int GAME_SPEED = 1000 / FRAMES_PER_SECOND;

    // Max framerate, 0 - no limit
    const unsigned int FRAME_RATE_LIMIT = 0;
    const bool VERTICAL_SYNC_ENABLED = false;

    // Constants to the map.png
    const unsigned int TILE_WIDTH = 64;
    const unsigned int TILE_HEIGHT = 64;
    const unsigned int MAP_TILESET_GRAPHICS_OFFSET = 4;
    const unsigned int MAP_TILESET_GRAPHICS_INITIAL_POSITION = 3;

    // Starting number of objects to generate
    const unsigned int INITIAL_OBJECTS_RESERVED = 10;
    const unsigned int INITIAL_TREES_CREATED_MAXIMUM = 500;

    // Alpha value for building 
    const unsigned int BLUEPRINT_ALPHA_VALUE = 50;

    // Graphics paths
    const std::string graphicsPathToCharacter = "Graphics/character.png";
    const std::string graphicsPathToTreeSapling = "Graphics/tree_sapling.png";
    const std::string graphicsPathToTreeYoung = "Graphics/tree_young.png";
    const std::string graphicsPathToTreeOld = "Graphics/tree_old.png";
    const std::string graphicsPathToResourceTree = "Graphics/resource_tree.png";
    const std::string graphicsPathToWall = "Graphics/wall.png";

    // Font paths
    const std::string fontPathToOpenSans = "OpenSans-Regular.ttf";
ó
    // Debug constants
    const int DEBUG_DISPLAY_ALPHA_VALUE = 170;
}

#endif // CONSTANTS_H
