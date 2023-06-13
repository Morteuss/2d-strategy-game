#include "Map.h"

#include <fstream>
#include <iostream>
#include <vector>

#include "Constants.h"
#include "RandomNumberGenerator.h"

// public methods
Map::Map(int widthTilesNumber, int heightTilesNumber) : widthTilesNumber(widthTilesNumber),
                                                        heightTilesNumber(heightTilesNumber) {
    // Przydzielamy pamiec dla tablicy przechowujacej plytki mapy
    mapTable = new Tile[widthTilesNumber * heightTilesNumber];

    generateMap();

    // Ladujemy tileset mapy
    if (!mapTileset.loadFromFile("Graphics/Map/map.png")) {

    }

    // Zmieniamy rozmiar dynamicznej tablicy wierzcholkow
    mapVertices.setPrimitiveType(sf::Quads);
    mapVertices.resize(widthTilesNumber * heightTilesNumber * 4);

    for (int i = 0; i < heightTilesNumber; ++i) {
        for (int j = 0; j < widthTilesNumber; ++j) {
            int tileIndex = static_cast<int>(mapTable[i * widthTilesNumber + j].getType());

            int textureCoordinateX = tileIndex % (mapTileset.getSize().x / Constants::TILE_WIDTH);
            int textureCoordinateY = tileIndex / (mapTileset.getSize().x / Constants::TILE_WIDTH);

            sf::Vertex *currentQuad = &mapVertices[(i * widthTilesNumber + j) * 4];

            currentQuad[0].position = sf::Vector2f(j * Constants::TILE_WIDTH, i * Constants::TILE_HEIGHT);
            currentQuad[1].position = sf::Vector2f((j + 1) * Constants::TILE_WIDTH, i * Constants::TILE_HEIGHT);
            currentQuad[2].position = sf::Vector2f((j + 1) * Constants::TILE_WIDTH, (i + 1) * Constants::TILE_HEIGHT);
            currentQuad[3].position = sf::Vector2f(j * Constants::TILE_WIDTH, (i + 1) * Constants::TILE_HEIGHT);

            currentQuad[0].texCoords = sf::Vector2f(Constants::MAP_TILESET_GRAPHICS_INITIAL_POSITION +
                                                    textureCoordinateX *
                                                    (Constants::TILE_WIDTH + Constants::MAP_TILESET_GRAPHICS_OFFSET),
                                                    Constants::MAP_TILESET_GRAPHICS_INITIAL_POSITION +
                                                    textureCoordinateY *
                                                    (Constants::TILE_HEIGHT + Constants::MAP_TILESET_GRAPHICS_OFFSET));

            currentQuad[1].texCoords = sf::Vector2f(Constants::MAP_TILESET_GRAPHICS_INITIAL_POSITION +
                                                    (textureCoordinateX + 1) * Constants::TILE_WIDTH +
                                                    textureCoordinateX * Constants::MAP_TILESET_GRAPHICS_OFFSET,
                                                    Constants::MAP_TILESET_GRAPHICS_INITIAL_POSITION +
                                                    textureCoordinateY *
                                                    (Constants::TILE_HEIGHT + Constants::MAP_TILESET_GRAPHICS_OFFSET));

            currentQuad[2].texCoords = sf::Vector2f(Constants::MAP_TILESET_GRAPHICS_INITIAL_POSITION +
                                                    (textureCoordinateX + 1) * Constants::TILE_WIDTH +
                                                    textureCoordinateX * Constants::MAP_TILESET_GRAPHICS_OFFSET,
                                                    Constants::MAP_TILESET_GRAPHICS_INITIAL_POSITION +
                                                    (textureCoordinateY + 1) * Constants::TILE_HEIGHT +
                                                    textureCoordinateY * Constants::MAP_TILESET_GRAPHICS_OFFSET);

            currentQuad[3].texCoords = sf::Vector2f(Constants::MAP_TILESET_GRAPHICS_INITIAL_POSITION +
                                                    textureCoordinateX *
                                                    (Constants::TILE_WIDTH + Constants::MAP_TILESET_GRAPHICS_OFFSET),
                                                    Constants::MAP_TILESET_GRAPHICS_INITIAL_POSITION +
                                                    (textureCoordinateY + 1) * Constants::TILE_HEIGHT +
                                                    textureCoordinateY * Constants::MAP_TILESET_GRAPHICS_OFFSET);
        }
    }
}

Map::~Map() {
    delete[] mapTable;
}

// private methods

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &mapTileset;
    target.draw(mapVertices, states);
}

void Map::fillMapTableOneValue(Tile value) {
    for (int i = 0; i < widthTilesNumber * heightTilesNumber; ++i) {
        mapTable[i] = value;
    }
}

void Map::createMountain(int positionX, int positionY, int width, int height, Tile value) {
    for (int i = positionY; i < positionY + height; ++i) {
        for (int j = positionX; j < positionX + width; ++j) {
            mapTable[i * widthTilesNumber + j] = value;
        }
    }
}

void Map::generateMap() {
    fillMapTableOneValue(Tile(TileType::grass1, "trawa_1"));
    mountainGenerator();
    std::cout << "The rocks has been generated!\n";
    lakeGenerator(30, 10, 30, 30);
    std::cout << "The lake has been generated!\n";
    lakeGenerator(20, 80, 15, 10);
    std::cout << "The lake has been generated!\n";
    riverGenerator(0, 50, RandomNumberGenerator::getIntNumber(Constants::MAP_WIDTH / 2, Constants::MAP_WIDTH - 1),
                   RandomNumberGenerator::getIntNumber(5, Constants::MAP_HEIGHT - 10));
    std::cout << "The river has been generated!\n";

    generateMapAreasForPathFinding();
}

bool Map::isWalkable(int tileIndex) const {
    return mapTable[tileIndex].isWalkable();
}

void Map::setOccupied(int tileIndex, bool occupied) {
    mapTable[tileIndex].setOccupied(occupied);
}

void Map::mountainGenerator() {
    // generowanie skalek

    int randomNumber = 0;

    for (int y = 1; y < heightTilesNumber - 1; ++y) {
        for (int x = 1; x < widthTilesNumber - 1; ++x) {
            randomNumber = RandomNumberGenerator::getIntNumber(0, 8);
            if (randomNumber == 3 || randomNumber == 4) {
                mapTable[y * widthTilesNumber + x] = Tile(TileType::rock0, "skala_0", false);
            }
        }
    }

    // usuwanie pojedynczych skalek
    for (int y = 1; y < heightTilesNumber - 1; ++y) {
        for (int x = 1; x < widthTilesNumber - 1; ++x) {
            if (mapTable[y * widthTilesNumber + x].getType() == TileType::rock0) {
                if ((mapTable[y * widthTilesNumber + x - 1].getType() != TileType::rock0) &&
                    (mapTable[y * widthTilesNumber + x + 1].getType() != TileType::rock0) &&
                    (mapTable[(y - 1) * widthTilesNumber + x].getType() != TileType::rock0) &&
                    (mapTable[(y + 1) * widthTilesNumber + x].getType() != TileType::rock0) &&
                    (mapTable[(y - 1) * widthTilesNumber + x - 1].getType() != TileType::rock0) &&
                    (mapTable[(y + 1) * widthTilesNumber + x + 1].getType() != TileType::rock0) &&
                    (mapTable[(y - 1) * widthTilesNumber + x + 1].getType() != TileType::rock0) &&
                    (mapTable[(y + 1) * widthTilesNumber + x - 1].getType() != TileType::rock0)) {
                    mapTable[y * widthTilesNumber + x] = Tile(TileType::grass1, "trawa_1");
                }
            }
        }
    }
}

void Map::lakeGenerator(int posX, int posY, int width, int height) {
    // generowanie jeziora
    for (int y = posY; y < height + posY - 1; ++y) {
        int number1 = RandomNumberGenerator::getIntNumber(0, 9) + 3;
        int number2 = RandomNumberGenerator::getIntNumber(0, 9) + 3;

        int left = RandomNumberGenerator::getIntNumber(0, width / number1);
        int right = RandomNumberGenerator::getIntNumber(0, width / number2);
        if (y < posY + 4 || y > height - 4) {
            left = RandomNumberGenerator::getIntNumber(0, width / 2 - 1);
            right = RandomNumberGenerator::getIntNumber(0, width / 2 - 1);
        }

        for (int x = posX + left + 1; x < width + posX - right - 1; ++x) {
            mapTable[y * widthTilesNumber + x] = Tile(TileType::water0, "woda_0", false);
        }
    }

    // lake modifier

    for (int y = posY; y < height + posY; ++y) {
        for (int x = posX; x < width + posX; ++x) {
            if (mapTable[y * widthTilesNumber + x].getType() == TileType::water0) {
                if (mapTable[(y + 1) * widthTilesNumber + x].getType() != TileType::water0 &&
                    mapTable[(y - 1) * widthTilesNumber + x].getType() != TileType::water0) {
                    mapTable[y * widthTilesNumber + x] = Tile(TileType::grass1, "trawa_1");
                }
            }
            if (mapTable[y * widthTilesNumber + x].getType() != TileType::water0) {
                if (mapTable[(y + 1) * widthTilesNumber + x].getType() == TileType::water0 &&
                    mapTable[(y - 1) * widthTilesNumber + x].getType() == TileType::water0) {
                    mapTable[y * widthTilesNumber + x] = Tile(TileType::water0, "water_0", false);
                }
            }
        }
    }

    // wypelnianie piaskiem
    sandFillerGenerator(posX, posY, width, height);
}

void Map::riverGenerator(int currentX, int currentY, int toX, int toY) {
    int randomNumber = 0;
    int number = 5;

    // zmienilem tu tymczasowo AND na OR
    while (currentX != toX || currentY != toY) {
        randomNumber = RandomNumberGenerator::getIntNumber(0, number - 1) + 2;

        if (currentX < toX) {
            for (int i = 0; i < randomNumber; ++i)
                mapTable[currentY * widthTilesNumber + currentX++] = Tile(TileType::water0, "woda_0", false);
        } else {
            randomNumber = RandomNumberGenerator::getIntNumber(0, number - 1) + 2;
            for (int i = 0; i < randomNumber; ++i)
                mapTable[currentY * widthTilesNumber + currentX--] = Tile(TileType::water0, "woda_0", false);
        }

        if (currentY < toY) {
            randomNumber = RandomNumberGenerator::getIntNumber(0, number - 1) + 2;
            for (int i = 0; i < randomNumber; ++i)
                mapTable[currentY++ * widthTilesNumber + currentX] = Tile(TileType::water0, "woda_0", false);
        } else {
            randomNumber = RandomNumberGenerator::getIntNumber(0, number - 1) + 2;
            for (int i = 0; i < randomNumber; ++i)
                mapTable[currentY-- * widthTilesNumber + currentX] = Tile(TileType::water0, "woda_0", false);
        }
    }
    std::cout << currentX << std::endl;
    std::cout << toX << std::endl;
    std::cout << currentY << std::endl;
    std::cout << toY << std::endl;
}

void Map::sandFillerGenerator(int posX, int posY, int width, int height) {
    for (int y = posY; y < posY + height - 1; ++y) {
        for (int x = posX + 1; x < posX + width - 1; ++x) {
            if (mapTable[y * widthTilesNumber + x].getType() == TileType::water0) {
                if (mapTable[y * widthTilesNumber + x + 1].getType() != TileType::water0) {
                    mapTable[y * widthTilesNumber + x + 1] = Tile(TileType::sand0, "piasek_0");
                }
                if (mapTable[y * widthTilesNumber + x - 1].getType() != TileType::water0) {
                    mapTable[y * widthTilesNumber + x - 1] = Tile(TileType::sand0, "piasek_0");
                }
                if (mapTable[(y - 1) * widthTilesNumber + x].getType() != TileType::water0) {
                    mapTable[(y - 1) * widthTilesNumber + x] = Tile(TileType::sand0, "piasek_0");
                }
                if (mapTable[(y + 1) * widthTilesNumber + x].getType() != TileType::water0) {
                    mapTable[(y + 1) * widthTilesNumber + x] = Tile(TileType::sand0, "piasek_0");
                }

                // diagonalne
                if (mapTable[(y + 1) * widthTilesNumber + x + 1].getType() != TileType::water0) {
                    mapTable[(y + 1) * widthTilesNumber + x + 1] = Tile(TileType::sand0, "piasek_0");
                }
                if (mapTable[(y - 1) * widthTilesNumber + x - 1].getType() != TileType::water0) {
                    mapTable[(y - 1) * widthTilesNumber + x - 1] = Tile(TileType::sand0, "piasek_0");
                }
                if (mapTable[(y - 1) * widthTilesNumber + x + 1].getType() != TileType::water0) {
                    mapTable[(y - 1) * widthTilesNumber + x + 1] = Tile(TileType::sand0, "piasek_0");
                }
                if (mapTable[(y + 1) * widthTilesNumber + x - 1].getType() != TileType::water0) {
                    mapTable[(y + 1) * widthTilesNumber + x - 1] = Tile(TileType::sand0, "piasek_0");
                }
            }
        }
    }
}

int Map::getAreaNumber(int tileIndex) {
    return mapTable[tileIndex].getAreaNumber();
}

TileType Map::getType(int tileIndex) const {
    return mapTable[tileIndex].getType();
}

void Map::generateMapAreasForPathFinding() {
    unsigned int tilesNumber = widthTilesNumber * heightTilesNumber;

    std::vector<int> idNumbers;
    idNumbers.reserve(tilesNumber);
    for (unsigned int i = 0; i < tilesNumber; ++i)
        idNumbers.push_back(0);

    int id = 1;
    int numberOld = 0;
    int numberNew = 0;

    for (int y = 0; y < heightTilesNumber; ++y) {
        for (int x = 0; x < widthTilesNumber; ++x) {
            if (y == 0) {
                if (mapTable[y * widthTilesNumber + x].isWalkable() == true) {
                    if (x == 0) {
                        mapTable[y * widthTilesNumber + x].setAreaNumber(id);
                        ++idNumbers[id];
                    } else {
                        if (mapTable[y * widthTilesNumber + x].isWalkable() == false) {
                            ++id;
                            mapTable[y * widthTilesNumber + x].setAreaNumber(id);
                            ++idNumbers[id];
                        } else {
                            mapTable[y * widthTilesNumber + x].setAreaNumber(
                                    mapTable[y * widthTilesNumber + x - 1].getAreaNumber());
                            ++idNumbers[mapTable[y * widthTilesNumber + x - 1].getAreaNumber()];
                        }
                    }
                }
            } else {
                if (x == 0) {
                    if (mapTable[y * widthTilesNumber + x].isWalkable() == true) {
                        if (mapTable[(y - 1) * widthTilesNumber + x].isWalkable() == true) {
                            mapTable[y * widthTilesNumber + x].setAreaNumber(
                                    mapTable[(y - 1) * widthTilesNumber + x].getAreaNumber());
                            ++idNumbers[mapTable[(y - 1) * widthTilesNumber + x].getAreaNumber()];
                        } else {
                            ++id;
                            mapTable[y * widthTilesNumber + x].setAreaNumber(id);
                            ++idNumbers[id];
                        }
                    }
                } else {
                    if (mapTable[y * widthTilesNumber + x].isWalkable() == true) {
                        if (mapTable[(y - 1) * widthTilesNumber + x].isWalkable() == true) {
                            mapTable[y * widthTilesNumber + x].setAreaNumber(
                                    mapTable[(y - 1) * widthTilesNumber + x].getAreaNumber());
                            ++idNumbers[mapTable[(y - 1) * widthTilesNumber + x].getAreaNumber()];


                            if (x > 0 && mapTable[y * widthTilesNumber + x - 1].isWalkable() == true &&
                                mapTable[y * widthTilesNumber + x].getAreaNumber() !=
                                mapTable[y * widthTilesNumber + x - 1].getAreaNumber()) {

                                numberNew = mapTable[y * widthTilesNumber + x].getAreaNumber();
                                numberOld = mapTable[y * widthTilesNumber + x - 1].getAreaNumber();

                                long int position = y * widthTilesNumber + x;

                                while (position > 0 && idNumbers[numberOld] > 0) {
                                    if (mapTable[position].getAreaNumber() == numberOld) {
                                        mapTable[position].setAreaNumber(numberNew);
                                        ++idNumbers[numberNew];
                                        --idNumbers[numberOld];
                                    }
                                    --position;
                                }
                            }
                        } else if (mapTable[y * widthTilesNumber + x - 1].isWalkable() == true) {
                            mapTable[y * widthTilesNumber + x].setAreaNumber(
                                    mapTable[y * widthTilesNumber + x - 1].getAreaNumber());
                            ++idNumbers[mapTable[y * widthTilesNumber + x - 1].getAreaNumber()];
                        } else {
                            ++id;
                            mapTable[y * widthTilesNumber + x].setAreaNumber(id);
                            ++idNumbers[id];
                        }
                    }
                }
            }
        }
    }
    // mozna sie zastanowic nad lepszym rozwiazaniem problemu niz ten "hack"
    mapTable[0].setAreaNumber(mapTable[1].getAreaNumber());
}

void Map::saveMap() const {
    std::ofstream areaNumbers;
    areaNumbers.open("SavedAreaNumbers.txt", std::fstream::app);

    areaNumbers << "\n\n";
    for (int y = 0; y < heightTilesNumber; ++y) {
        for (int x = 0; x < widthTilesNumber; ++x) {
            areaNumbers << static_cast<char>(mapTable[y * widthTilesNumber + x].getAreaNumber());
        }
        areaNumbers << "\n";
    }
    areaNumbers.close();
}

