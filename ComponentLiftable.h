#ifndef COMPONENT_LIFTABLE_H
#define COMPONENT_LIFTABLE_H

#include "Component.h"

class ComponentLiftable : public Component {
public:
    ComponentLiftable *clone() const override;

    bool isLifted() const;

    void setLifted(bool lifted);

    bool isReserved() const;

    void setReserved(bool reserved);

    void setPickedBy(GameEntity *gameEntity);

    void acceptMessage(MessageType messageType) override;

    virtual void update(GameEntity &gameEntity) override;

private:
    bool lifted = false;
    bool reserved = false;
    GameEntity *pickedyBy = nullptr;
};

#endif // COMPONENT_LIFTABLE_H
