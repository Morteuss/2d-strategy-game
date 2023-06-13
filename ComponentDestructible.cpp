#include "ComponentDestructible.h"

ComponentDestructible *ComponentDestructible::clone() const {
    return new ComponentDestructible(*this);
}

void ComponentDestructible::acceptMessage(MessageType messageType) {
}
