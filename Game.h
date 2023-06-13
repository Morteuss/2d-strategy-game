#ifndef GAME_H
#define GAME_H

#include <atomic>

#include "Console.h"
#include "ContextMenuOrders.h"
#include "DebugDisplaySystem.h"
#include "GameClock.h"
#include "GameEntity.h"
#include "InputManager.h"
#include "Map.h"
#include "TaskExecuteSystem.h"
#include "TreeGenerationSystem.h"

using sf::RenderWindow;
using sf::View;

class Game {
public:
    Game(sf::RenderWindow &window, std::atomic<bool> &isLoaded);

    void setGameSpeed(unsigned newGameSpeed);

    Map &getMap();

    void addObject(GameEntity gameEntity);

    //void removeDeletedObjects();
    void render(double timeProgressValue);

    int run();

    void update();

    bool gamePause = true;
    int numberOfObjects;
    std::vector<GameEntity> objects;

    void foo(int start, int end);

    RenderWindow &window;
    InputManager inputManager;
    Map gameMap;
    View gameCamera;
    GameClock gameClock;
    ContextMenuOrders contextMenuOrders;
    TaskExecuteSystem taskExecuteSystem;
    TreeGenerationSystem treeGenerationSystem;
    Console gameConsole;
    DebugDisplaySystem debugDisplaySystem;
    unsigned int currentGameSpeed;
};

#endif // GAME_H
