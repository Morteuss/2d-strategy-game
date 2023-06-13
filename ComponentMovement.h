#ifndef MOVEMENT_COMPONENT_H
#define MOVEMENT_COMPONENT_H

#include <vector>
#include <list>

#include "Component.h"
#include "Map.h"


class ComponentMovement : public Component {
public:
    ComponentMovement(Map &gameMap);

    ComponentMovement *clone() const override;

    const std::vector<int> &getPath() const;

    void acceptMessage(MessageType messageType);

    void setDestinationTile(int tileIndex);

    virtual void update(GameEntity &gameEntity);

private:
    class PathStep {
    public:
        int tileIndex;
        unsigned int pathScoreG;
        unsigned int pathScoreH;
        PathStep *parent;

        PathStep(int tileIndex, unsigned int pathScoreG, unsigned int pathScoreH, PathStep *parent = nullptr)
                : tileIndex(tileIndex), pathScoreG(pathScoreG), pathScoreH(pathScoreH), parent(parent) {}

        unsigned int getScoreF() { return pathScoreG + pathScoreH; }
    };

    int destinationTile;
    Map &gameMap;
    bool isPathCalculated = false;
    float movementSpeed;
    std::vector<int> path;
    bool messageSent = false;

    void findNewPath(GameEntity &gameEntity);

    unsigned int getHeuristicDistance(int startTileIndex, int endTileIndex);
};

#endif // MOVEMENT_COMPONENT_H
