#include "ComponentLiftable.h"

#include "GameEntity.h"

ComponentLiftable *ComponentLiftable::clone() const {
    return new ComponentLiftable(*this);
}

bool ComponentLiftable::isLifted() const {
    return lifted;
}

void ComponentLiftable::setLifted(bool lifted) {
    this->lifted = lifted;
}

bool ComponentLiftable::isReserved() const {
    return reserved;
}

void ComponentLiftable::setReserved(bool reserved) {
    this->reserved = reserved;
}

void ComponentLiftable::setPickedBy(GameEntity *gameEntity) {
    pickedyBy = gameEntity;
}

void ComponentLiftable::acceptMessage(MessageType messageType) {

}

void ComponentLiftable::update(GameEntity &gameEntity) {
    if (lifted == true) {
        gameEntity.setPosX(pickedyBy->getPosX());
        gameEntity.setPosY(pickedyBy->getPosY());
    }
}
