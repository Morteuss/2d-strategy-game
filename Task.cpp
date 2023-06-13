#include "Task.h"

#include "ComponentLiftable.h"
#include "Constants.h"
#include "GameEntity.h"
#include "GameLibrary.h"
#include "ComponentMovement.h"
#include "ComponentTaskExecute.h"

#include <iostream>
#include <limits>

using namespace std;

Task::Task(int tileIndex) {
    this->tileIndex = tileIndex;
}

TaskCutTree::TaskCutTree(int tileIndex) : Task(tileIndex) {
}

void TaskCutTree::acceptTaskMessage(MessageType messageType) {
    if (messageType == MessageType::destinationReached) {
        destinationReached = true;
        cout << "Wiadomosc o dotarciu do celu dotarla do TaskCutTree" << endl;
    }
}

void TaskCutTree::update(GameEntity &gameEntity) {
    if (!pathSet) {
        auto ptr = gl::getComponentPtr<ComponentMovement>(gameEntity);
        ptr->setDestinationTile(tileIndex);
        pathSet = true;
        cout << "Sciezka dla TaskCutTree ustawiona" << endl;
    }
        // wykonywanie zadania zakonczone, usuwamy obiekt zarzadzany przez currentTask (czyli ten obiekt)
    else if (taskFinished) {
        // temp
        gl::getComponentPtr<ComponentTaskExecute>(gameEntity)->finished = false;
        gl::getComponentPtr<ComponentTaskExecute>(gameEntity)->setCurrentTask();
        cout << "task finished" << endl;
    } else if (taskBeingPerformed) {
        // do poprawienia
        gl::getComponentPtr<ComponentTaskExecute>(gameEntity)->finished = true;
        taskFinished = true;
        cout << "task being performed" << endl;
    } else if (destinationReached) {
        taskBeingPerformed = true;
        cout << "destination Reached" << endl;
    }
}

TaskBuildWall::TaskBuildWall(int tileIndex) : Task(tileIndex) {
}

void TaskBuildWall::acceptTaskMessage(MessageType messageType) {
    if (messageType == MessageType::destinationReached) {
        switch (phaseCounter) {
            case 0 : {
                blueprintReached = true;
                break;
            }
            case 1 : {
                resourceWoodReached = true;
                break;
            }
            case 2 : {
                wallReached = true;
                break;
            }
            default:
                break;
        }

        cout << "Wiadomosc o dotarciu do celu dotarla do TaskBuildWall" << endl;
    }
}

void TaskBuildWall::update(GameEntity &gameEntity) {
}

void TaskBuildWall::execute(GameEntity &gameEntity, std::vector<GameEntity> &objects, Map &gameMap) {
    if (!pathToBlueprintSet) {
        auto ptr = gl::getComponentPtr<ComponentMovement>(gameEntity);
        ptr->setDestinationTile(tileIndex);
        pathToBlueprintSet = true;
    } else if (blueprintReached) {
        objects.emplace_back(gl::createWallBlueprint(gameEntity.getPosX() / Constants::TILE_WIDTH,
                                                     gameEntity.getPosY() / Constants::TILE_HEIGHT));

        // do przemyslenia, moga byc problemy, gdy w trakcie wykonywania tego zadania zostanie
        // cos usuniete z vectora obiektow i ten pointer przestanie wskazywac w dobre miejsce
        blueprintPtr = &objects.back();

        blueprintReached = false;
        ++phaseCounter;
    } else if (!pathToResourceWoodSet && phaseCounter == 1) {
        auto ptr = gl::getComponentPtr<ComponentMovement>(gameEntity);

        ComponentLiftable *componentLiftablePtr = nullptr;
        GameEntity *chosenObject = nullptr;
        int minimalDistance = std::numeric_limits<int>::max();

        for (auto &object: objects) {
            componentLiftablePtr = gl::getComponentPtr<ComponentLiftable>(object);
            if (componentLiftablePtr != nullptr && componentLiftablePtr->isLifted() == false &&
                componentLiftablePtr->isReserved() == false) {
                int distance = gl::calculateHeuristicDistance(tileIndex, gl::calculateTileIndex(object.getPosX(),
                                                                                                object.getPosY()));
                if (distance < minimalDistance) {
                    minimalDistance = distance;
                    chosenObject = &object;
                }
            }
        }

        if (chosenObject != nullptr) {
            ptr->setDestinationTile(gl::calculateTileIndex(chosenObject->getPosX(), chosenObject->getPosY()));
            resourceWoodPtr = chosenObject;
            gl::getComponentPtr<ComponentLiftable>(*resourceWoodPtr)->setReserved(true);
        } else {
            // wykonywanie zadania zakonczone, usuwamy obiekt zarzadzany przez currentTask (czyli ten obiekt)
            gl::getComponentPtr<ComponentTaskExecute>(gameEntity)->setCurrentTask();
            blueprintPtr->setExists(false);

            return;
        }

        pathToResourceWoodSet = true;
    } else if (resourceWoodReached) {
        // ustawiamy drewno jako obiekt niesiony przez nasze gameEntity
        gl::getComponentPtr<ComponentLiftable>(*resourceWoodPtr)->setPickedBy(&gameEntity);
        gl::getComponentPtr<ComponentLiftable>(*resourceWoodPtr)->setLifted(true);

        resourceWoodReached = false;
        ++phaseCounter;
    } else if (!pathToWallSet && phaseCounter == 2) {
        auto ptr = gl::getComponentPtr<ComponentMovement>(gameEntity);
        ptr->setDestinationTile(tileIndex);
        pathToWallSet = true;
    } else if (wallReached) {
        // usuwamy niesone przez gameEntity drewno
        resourceWoodPtr->setExists(false);

        // usuwamy blueprint, ktory zastapimy prawdziwa sciana
        blueprintPtr->setExists(false);

        // tworzymy sciane w danym miejscu
        objects.emplace_back(gl::createWall(gameEntity.getPosX() / Constants::TILE_WIDTH,
                                            gameEntity.getPosY() / Constants::TILE_HEIGHT, gameMap));

        // wykonywanie zadania zakonczone, usuwamy obiekt zarzadzany przez currentTask (czyli ten obiekt)
        gl::getComponentPtr<ComponentTaskExecute>(gameEntity)->setCurrentTask();

        // umieszamy ludzika kolo zbudowanej sciany
        int tileIndex = gl::calculateTileIndex(gameEntity.getPosX(), gameEntity.getPosY());
        if (tileIndex + 1 >= 0 && tileIndex + 1 < Constants::MAP_WIDTH * Constants::MAP_HEIGHT &&
            gameMap.isWalkable(tileIndex + 1)) {
            gl::getComponentPtr<ComponentMovement>(gameEntity)->setDestinationTile(tileIndex + 1);
        } else if (tileIndex - 1 >= 0 && tileIndex - 1 < Constants::MAP_WIDTH * Constants::MAP_HEIGHT &&
                   gameMap.isWalkable(tileIndex - 1)) {
            gl::getComponentPtr<ComponentMovement>(gameEntity)->setDestinationTile(tileIndex - 1);
        } else if (tileIndex - Constants::MAP_WIDTH >= 0 &&
                   tileIndex - Constants::MAP_WIDTH < Constants::MAP_WIDTH * Constants::MAP_HEIGHT &&
                   gameMap.isWalkable(tileIndex - Constants::MAP_WIDTH)) {
            gl::getComponentPtr<ComponentMovement>(gameEntity)->setDestinationTile(tileIndex - Constants::MAP_WIDTH);
        } else if (tileIndex + Constants::MAP_WIDTH >= 0 &&
                   tileIndex + Constants::MAP_WIDTH < Constants::MAP_WIDTH * Constants::MAP_HEIGHT &&
                   gameMap.isWalkable(tileIndex + Constants::MAP_WIDTH)) {
            gl::getComponentPtr<ComponentMovement>(gameEntity)->setDestinationTile(tileIndex + Constants::MAP_WIDTH);
        }
    }
}
