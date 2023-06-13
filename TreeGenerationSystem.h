#ifndef TREE_GENERATION_SYSTEM_H
#define TREE_GENERATION_SYSTEM_H

#include <vector>

class GameEntity;

class Map;

class Game;

class TreeGenerationSystem {
public:
    TreeGenerationSystem();

    void generateTreesOnStart(const Map &gameMap, Game &game);

    void update(int timeChange, const Map &gameMap, Game &game);

private:
    std::vector<int> treesIndexes;
};

#endif // TREE_GENERATION_SYSTEM_H
