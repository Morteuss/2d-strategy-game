#include "ContextMenuOrders.h"

#include <iostream>

#include "GameLibrary.h"
#include "TaskExecuteSystem.h"

ContextMenuOrders::ContextMenuOrders() {
    addOrder(std::make_unique<OrderBuildWall>());
    addOrder(std::make_unique<OrderCutTree>());
    addOrder(std::make_unique<OrderCancel>());

    height = listOfOrders.size() * orderTextSize;

    textFont.loadFromFile("font.ttf");
}

void ContextMenuOrders::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (visible) {
        sf::RectangleShape menuGraphics;

        menuGraphics.setPosition(positionX, positionY);
        menuGraphics.setSize(sf::Vector2f(width, height));
        menuGraphics.setFillColor(sf::Color(150, 150, 150));
        menuGraphics.setOutlineColor(sf::Color::Black);
        menuGraphics.setOutlineThickness(5);

        target.draw(menuGraphics);


        sf::Text text;
        text.setFont(textFont);
        text.setCharacterSize(orderTextSize);

        for (unsigned int i = 0; i < listOfOrders.size(); ++i) {
            text.setString(listOfOrders[i]->getOrderText());
            text.setPosition(positionX, positionY + (i * orderTextSize));

            target.draw(text);
        }
    }
}

void ContextMenuOrders::addOrder(std::unique_ptr<Order> order) {
    listOfOrders.emplace_back(std::move(order));
}

bool ContextMenuOrders::processMousePosition(int mousePosX, int mousePosY, TaskExecuteSystem &taskExecuteSystem,
                                             sf::RenderWindow &window) {
    if (visible) {
        // pozycja myszy znajduje sie w zakresie ContextMenuOrders
        if ((mousePosX >= positionX && mousePosX <= positionX + width) &&
            (mousePosY >= positionY && mousePosY <= positionY + height)) {
            int orderIndex = (mousePosY - positionY) / orderTextSize;

            std::cout << listOfOrders[orderIndex]->getOrderText() << std::endl;

            listOfOrders[orderIndex]->executeOrder(taskExecuteSystem, selectedTileIndex);

            visible = false;
            return true;
        }

        // pozycja myszy nie znajduje sie w zakresie ContexMenuOrders
        return false;
    }
    return false;
}
