#ifndef TASK_H
#define TASK_H

#include <vector>

#include "Component.h"

class GameEntity;

class Map;

/*
	Task
*/

class Task {
    friend class TaskExecuteSystem;

public:
    Task(int tileIndex);

    virtual ~Task() {};

    virtual Task *clone() const = 0;

    virtual void acceptTaskMessage(MessageType messageType) = 0;

    virtual void update(GameEntity &gameEntity) = 0;

    virtual void execute(GameEntity &gameEntity, std::vector<GameEntity> &objects, Map &gameMap) {};

protected:
    int tileIndex;
};

/*
	TaskCutTree
*/

class TaskCutTree : public Task {
public:
    TaskCutTree(int tileIndex);

    TaskCutTree *clone() const override { return new TaskCutTree(*this); }

    void acceptTaskMessage(MessageType messageType) override;

    void update(GameEntity &gameEntity);

private:
    bool pathSet = false;
    bool destinationReached = false;
    bool taskBeingPerformed = false;
    bool taskFinished = false;
};

/*
	TaskBuildWall
*/

class TaskBuildWall : public Task {
public:
    TaskBuildWall(int tileIndex);

    TaskBuildWall *clone() const override { return new TaskBuildWall(*this); }

    void acceptTaskMessage(MessageType messageType) override;

    void update(GameEntity &gameEntity);

    virtual void execute(GameEntity &gameEntity, std::vector<GameEntity> &objects, Map &gameMap) override;

private:
    bool pathToBlueprintSet = false;
    bool blueprintReached = false;
    bool pathToResourceWoodSet = false;
    bool resourceWoodReached = false;
    bool pathToWallSet = false;
    bool wallReached = false;
    int phaseCounter = 0;

    GameEntity *resourceWoodPtr = nullptr;
    GameEntity *blueprintPtr = nullptr;
};

#endif // TASK_H
