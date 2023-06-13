#include "TaskExecuteSystem.h"

#include <typeinfo>

#include "ComponentDestructible.h"
#include "Constants.h"
#include "GameEntity.h"
#include "GameLibrary.h"
#include "Map.h"
#include "ComponentTaskExecute.h"

#include <iostream>
#include <limits>

using namespace std;

TaskExecuteSystem::TaskExecuteSystem() {

}

void TaskExecuteSystem::addTask(Task *task) {
    for (auto &i: pendingTasksContainer) {
        if (i->tileIndex == task->tileIndex) {
            if (typeid(*i) == typeid(*task)) {
                return;
            }
        }
    }

    pendingTasksContainer.emplace_back(task);
}

void TaskExecuteSystem::cancelTask(int tileIndex) {
    for (auto &i: pendingTasksContainer) {
        if (i->tileIndex == tileIndex) {
            // usuwam dany Task z kontenera podmieniajac go z ostatnim i usuwajac ostatni element kontenera
            i = std::move(pendingTasksContainer.back());
            pendingTasksContainer.pop_back();
            break;
        }
    }
}

void TaskExecuteSystem::executeTasks(std::vector<GameEntity> &objects, Map &gameMap) {
    ComponentTaskExecute *taskExecuteComponentPtr = nullptr;
    for (auto &gameEntityReference: objects) {
        if (gameEntityReference.getExists()) {
            taskExecuteComponentPtr = gl::getComponentPtr<ComponentTaskExecute>(gameEntityReference);
            if (taskExecuteComponentPtr != nullptr) {
                taskExecuteComponentPtr->execute(gameEntityReference, objects, gameMap);
            }
        }
    }
}

void TaskExecuteSystem::update(std::vector<GameEntity> &objects, Map &gameMap) {
    if (pendingTasksContainer.size() > 0) {
        for (auto taskIterator = pendingTasksContainer.begin();
             taskIterator != pendingTasksContainer.end(); ++taskIterator) {
            int minimalDistance = std::numeric_limits<int>::max();
            ComponentTaskExecute *bestTaskExecuteComponentPtr = nullptr;
            ComponentTaskExecute *taskExecuteComponentPtr = nullptr;

            for (auto &gameEntityReference: objects) {
                for (auto &componentPtr: gameEntityReference.components) {
                    if (typeid(*componentPtr) == typeid(ComponentTaskExecute)) {
                        taskExecuteComponentPtr = static_cast<ComponentTaskExecute *>(componentPtr.get());

                        if (taskExecuteComponentPtr->getCurrentTask() == nullptr) {
                            int startTileIndex = gl::calculateTileIndex(gameEntityReference.getPosX(),
                                                                        gameEntityReference.getPosY());
                            int endTileIndex = (*taskIterator)->tileIndex;

                            if (gameMap.getAreaNumber(startTileIndex) == gameMap.getAreaNumber(endTileIndex)) {
                                int distance = gl::calculateHeuristicDistance(startTileIndex, endTileIndex);
                                cout << "distance : " << distance;

                                if (distance < minimalDistance) {
                                    bestTaskExecuteComponentPtr = taskExecuteComponentPtr;
                                    minimalDistance = distance;
                                }
                            }
                        }
                        break;
                    }
                }
            }
            if (bestTaskExecuteComponentPtr != nullptr) {
                bestTaskExecuteComponentPtr->setCurrentTask((*taskIterator).release());
                pendingTasksContainer.erase(taskIterator);
                // po uzyciu erase iterator taskIterator jest niewlasciwy i manualnie go przestawiam, aby wskazywal w odpowiednie miejsce
                taskIterator--;
            }
        }
    }

    int index = -1;

    for (auto &i: objects) {
        if ((gl::getComponentPtr<ComponentTaskExecute>(i) != nullptr) &&
            gl::getComponentPtr<ComponentTaskExecute>(i)->finished == true) {
            index = gl::calculateTileIndex(i.getPosX(), i.getPosY());
        }
        if (gl::getComponentPtr<ComponentDestructible>(i) != nullptr &&
            gl::calculateTileIndex(i.getPosX(), i.getPosY()) == index) {
            i = gl::createResourceWood(i.getPosX() / Constants::TILE_WIDTH, i.getPosY() / Constants::TILE_HEIGHT);
        }
    }
}
