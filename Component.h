#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <typeinfo>

class GameEntity;

enum class MessageType {
    destinationReached
};

class Component {
public:
    virtual ~Component() {}

    virtual Component *clone() const = 0;

    virtual void acceptMessage(MessageType messageType) = 0;

    virtual std::string getInformationString() const {
        std::string informationString(typeid(*this).name());
        informationString += " - No information";

        return informationString;
    };

    virtual void update(GameEntity &gameEntity) {};
};

#endif // COMPONENT_H
