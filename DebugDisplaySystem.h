#ifndef DEBUG_DISPLAY_SYSTEM_H
#define DEBUG_DISPLAY_SYSTEM_H

#include <SFML/Graphics.hpp>

#include "Map.h"

class GameEntity;

class DebugDisplaySystem : public sf::Drawable {
public:
    // alphaValue w zakresie [0, 255] decyduje o poziomie przezroczystosci efektow graficznych debugowania
    DebugDisplaySystem(int alphaValue);

    void removeTrees(std::vector<GameEntity> &objects);

    void showAreas(Map &gameMap);

    void showPaths(std::vector<GameEntity> &objects);

    void showWalkable(Map &gameMap);

    void cancel();

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    void positionQuad(int quadIndex);

    sf::VertexArray mapVertices;
    int alphaValue;
};

#endif // DEBUG_DISPLAY_SYSTEM_H
