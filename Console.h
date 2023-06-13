#ifndef CONSOLE_H
#define CONSOLE_H

#include <SFML/Graphics.hpp>

class DebugDisplaySystem;

class GameEntity;

class Map;

class Console : public sf::Drawable {
public:
    Console(Map &gameMap, std::vector<GameEntity> &gameEntities);

    void executeCommand(DebugDisplaySystem &debugDisplaySystem);

    void processInput(sf::Event &event, DebugDisplaySystem &debugDisplaySystem);

    void changeVisible();

    bool isVisible() const;

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void reset();

    Map &gameMap;
    std::vector<GameEntity> &gameEntities;

    sf::Font textFont;
    sf::Text textDisplayed;
    std::string textString;
    bool visible;

    std::string removeTreesString = "removeTrees";
    std::string showAreasString = "showAreas";
    std::string showPathsString = "showPaths";
    std::string showWalkableString = "showWalkable";
    std::string cancelString = "cancel";
};

#endif // CONSOLE_H
