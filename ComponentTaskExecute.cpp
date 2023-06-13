#include "ComponentTaskExecute.h"

#include "GameEntity.h"

#include <iostream>

using namespace std;

ComponentTaskExecute::ComponentTaskExecute() {
    // currentTask to smart pointer ktory w tym momencie "przechowuje" nullptr
    if (currentTask.get() == nullptr && !currentTask)
        cout << "ComponentTaskExecute jest nullptr" << endl;
}

ComponentTaskExecute::ComponentTaskExecute(const ComponentTaskExecute &taskExecuteComponent) {
    if (taskExecuteComponent.currentTask)
        currentTask = std::unique_ptr<Task>(taskExecuteComponent.currentTask->clone());
}

ComponentTaskExecute *ComponentTaskExecute::clone() const {
    return new ComponentTaskExecute(*this);
}

Task *ComponentTaskExecute::getCurrentTask() {
    return currentTask.get();
}

void ComponentTaskExecute::setCurrentTask(Task *task) {
    // zwalniamy pamiec uzywana przez stary pointer i dajemy currentTask nowy pointer do przechowywania
    currentTask.reset(task);
}

void ComponentTaskExecute::acceptMessage(MessageType messageType) {
    if (messageType == MessageType::destinationReached) {
        if (currentTask)
            currentTask->acceptTaskMessage(messageType);
    }
}

void ComponentTaskExecute::update(GameEntity &gameEntity) {
    if (currentTask) {
        currentTask->update(gameEntity);
    }
}

void ComponentTaskExecute::execute(GameEntity &gameEntity, std::vector<GameEntity> &objects, Map &gameMap) {
    if (currentTask) {
        currentTask->execute(gameEntity, objects, gameMap);
    }
}
