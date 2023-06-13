#include "Tile.h"

Tile::Tile() {
    type = TileType::ground0;
    walkable = true;
    occupied = false;
}

Tile::Tile(TileType type, string name, bool walkable, int areaNumber) {
    this->type = type;
    this->tileName = name;
    this->walkable = walkable;
    this->areaNumber = areaNumber;
    this->occupied = false;
}

Tile::~Tile() {
}

TileType Tile::getType() {
    return type;
}

bool Tile::isWalkable() {
    if (occupied) {
        return false;
    } else {
        return walkable;
    }
}


int Tile::getAreaNumber() {
    return this->areaNumber;
}

void Tile::setAreaNumber(int areaNumber) {
    this->areaNumber = areaNumber;
}

void Tile::setOccupied(bool occupied) {
    this->occupied = occupied;
}

bool Tile::isOccupied() const {
    return occupied;
}
