#ifndef TASK_EXECUTE_COMPONENT_H
#define TASK_EXECUTE_COMPONENT_H

#include <memory>

#include "Component.h"
#include "Task.h"

class ComponentTaskExecute : public Component {
public:
    ComponentTaskExecute();

    ComponentTaskExecute(const ComponentTaskExecute &taskExecuteComponent);

    ComponentTaskExecute *clone() const override;

    Task *getCurrentTask();

    void setCurrentTask(Task *task = nullptr);

    void acceptMessage(MessageType messageType);

    virtual void update(GameEntity &gameEntity);

    void execute(GameEntity &gameEntity, std::vector<GameEntity> &objects, Map &gameMap);

private:
    std::unique_ptr<Task> currentTask;

// temp
public:
    bool finished = false;
};

#endif // TASK_EXECUTE_COMPONENT_H
