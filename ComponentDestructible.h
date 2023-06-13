#ifndef COMPONENT_DESTRUCTIBLE_H
#define COMPONENT_DESTRUCTIBLE_H

#include "Component.h"

class ComponentDestructible : public Component {
public:
    ComponentDestructible *clone() const override;

    void acceptMessage(MessageType messageType);

};

#endif // COMPONENT_DESTRUCTIBLE_H
