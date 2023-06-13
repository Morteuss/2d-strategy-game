#ifndef CONTEXT_MENU_ORDERS_H
#define CONTEXT_MENU_ORDERS_H

#include <memory>
#include <vector>

#include "SFML/Graphics.hpp"

#include "Order.h"

class TaskExecuteSystem;

class ContextMenuOrders : public sf::Drawable {
public:
    ContextMenuOrders();

    bool isVisible() { return visible; }

    void setVisible(bool visible) { this->visible = visible; }

    void setPosition(float positionX, float positionY) {
        this->positionX = positionX;
        this->positionY = positionY;
    }

    void setSelectedTileIndex(int selectedTileIndex) { this->selectedTileIndex = selectedTileIndex; }

    void addOrder(std::unique_ptr<Order> order);

    bool
    processMousePosition(int mousePosX, int mousePosY, TaskExecuteSystem &taskExecuteSystem, sf::RenderWindow &window);

private:
    // data members
    int width = 300;
    int height = 0;
    bool visible = false;
    float positionX = 0;
    float positionY = 0;
    sf::Font textFont;

    int orderTextSize = 30;
    std::vector<std::unique_ptr<Order>> listOfOrders;
    int selectedTileIndex = 0;

    // methods
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

#endif // CONTEXT_MENU_ORDERS_H
