#ifndef TASK_EXECUTE_SYSTEM_H
#define TASK_EXECUTE_SYSTEM_H

#include <memory>
#include <vector>

#include "Task.h"

class GameEntity;

class Map;

class TaskExecuteSystem {
public:
    TaskExecuteSystem();

    void addTask(Task *task);

    void cancelTask(int tileIndex);

    void executeTasks(std::vector<GameEntity> &objects, Map &gameMap);

    void update(std::vector<GameEntity> &objects, Map &gameMap);

private:
    std::vector<std::unique_ptr<Task>> pendingTasksContainer;

};

#endif // TASK_EXECUTE_SYSTEM_H
