#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>

#include "Tile.h"

class Map : public sf::Drawable, public sf::Transformable
{
public:
    Map(int widthTilesNumber, int heightTilesNumber);
    ~Map();

    bool isWalkable(int tileIndex) const;
	void setOccupied(int tileIndex, bool occupied);
    int getAreaNumber(int tileIndex);
	TileType getType(int tileIndex) const;
	void saveMap() const;

private:
    // data members
    Tile *mapTable;
    int widthTilesNumber, heightTilesNumber;
    sf::Texture mapTileset;
    sf::VertexArray mapVertices;

    // methods
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void fillMapTableOneValue(Tile value);
    void createMountain(int positionX, int positionY, int width, int height, Tile value);
    void generateMap();

    // nowe metody
    void mountainGenerator();
    void lakeGenerator(int posX, int posY, int width, int height);
    void riverGenerator(int currentX, int currentY, int toX, int toY);
    // wywolywany wewnetrznie prez lakeGenerator(), nie uzywac metody jako samej
    void sandFillerGenerator(int posX, int posY, int width, int height);

    // metoda to tworzenia obszarow dla algorytmu znajdywania drogii (do zrobienia w pliku Map.cpp)
    void generateMapAreasForPathFinding();

};

#endif // MAP_H
