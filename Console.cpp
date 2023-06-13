#include "Console.h"

#include "Constants.h"
#include "DebugDisplaySystem.h"
#include "GameEntity.h"
#include "Map.h"


#include <iostream>

Console::Console(Map &gameMap, std::vector<GameEntity> &gameEntities) :
        gameMap(gameMap),
        gameEntities(gameEntities),
        visible(false) {
    reset();
    textFont.loadFromFile(Constants::fontPathToOpenSans);

    textDisplayed.setFont(textFont);
    textDisplayed.setFillColor(sf::Color::White);
    textDisplayed.setString(textString);
    textDisplayed.setCharacterSize(20);
}

void Console::executeCommand(DebugDisplaySystem &debugDisplaySystem) {
    if (textString == showAreasString) {
        debugDisplaySystem.showAreas(gameMap);
    } else if (textString == showPathsString) {
        debugDisplaySystem.showPaths(gameEntities);
    } else if (textString == showWalkableString) {
        debugDisplaySystem.showWalkable(gameMap);
    } else if (textString == cancelString) {
        debugDisplaySystem.cancel();
    } else if (textString == removeTreesString) {
        debugDisplaySystem.removeTrees(gameEntities);
    }
}

void Console::processInput(sf::Event &event, DebugDisplaySystem &debugDisplaySystem) {
    if (visible) {
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Return : {
                    visible = false;
                    executeCommand(debugDisplaySystem);
                    reset();
                    break;
                }
                default : {
                    break;
                }
            }
        } else if (event.type == sf::Event::TextEntered) {
            // sprawdzamy czy znak to 'backspace'
            if (event.text.unicode == '\b') {
                if (textString.size() > 0) {
                    textString.pop_back();
                }
                textDisplayed.setString(textString);
            }
                // sprawdzamy czy wpisany znak jest w zakresie znakow ASCII
            else if (event.text.unicode < 128) {
                textString += static_cast<char>(event.text.unicode);
                textDisplayed.setString(textString);
            }
        }
    }
}

void Console::changeVisible() {
    visible = !visible;
}

bool Console::isVisible() const {
    return visible;
}

void Console::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (visible) {
        sf::RectangleShape consoleRectangle(sf::Vector2f(target.getSize().x, textDisplayed.getCharacterSize() + 10));
        sf::Text tempText = textDisplayed;
        consoleRectangle.setPosition(sf::Vector2f(0, target.getSize().y - consoleRectangle.getSize().y));
        consoleRectangle.setFillColor(sf::Color::Blue);

        tempText.setPosition(consoleRectangle.getPosition());

        target.draw(consoleRectangle, states);
        target.draw(tempText, states);
    }
}

void Console::reset() {
    textString = "";
    textDisplayed.setString(textString);
}
