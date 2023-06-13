#include "ComponentMovement.h"

#include <chrono>
#include <iostream>
#include <limits>

#include "GameEntity.h"
#include "Constants.h"
#include "GameLibrary.h"
#include "RandomNumberGenerator.h"

using namespace std::chrono;

ComponentMovement::ComponentMovement(Map &gameMap) : gameMap(gameMap) {
    this->movementSpeed = RandomNumberGenerator::getIntNumber(3, 8);

    destinationTile = RandomNumberGenerator::getIntNumber(0, Constants::MAP_WIDTH * Constants::MAP_HEIGHT - 1);
}

ComponentMovement *ComponentMovement::clone() const {
    return new ComponentMovement(*this);
}

void ComponentMovement::update(GameEntity &gameEntity) {
    if (!isPathCalculated) {
        int startTileIndex = gl::calculateTileIndex(gameEntity.getPosX(), gameEntity.getPosY());
        if (gameMap.getAreaNumber(startTileIndex) == gameMap.getAreaNumber(destinationTile))
            findNewPath(gameEntity);
        else {
            destinationTile = startTileIndex;
            isPathCalculated = true;
        }
    }

    if (path.size() > 0) {
        // dla danej sciezki nie zostala jeszcze wyslana wiadomosc o jej ukonczeniu
        messageSent = false;

        // flagi oznaczajace czy polozenie X i Y nowej plytki zostalo osiagniete przez komponent ruchu
        bool posXAchieved = false, posYAchieved = false;

        float currentPosX = gameEntity.getPosX();
        float currentPosY = gameEntity.getPosY();
        float destinationPosX = path.back() % Constants::MAP_WIDTH * Constants::TILE_WIDTH;
        float destinationPosY = path.back() / Constants::MAP_WIDTH * Constants::TILE_HEIGHT;

        if (currentPosX + movementSpeed < destinationPosX) {
            gameEntity.setPosX(currentPosX + movementSpeed);
            gameEntity.setCurrentChangeInPosX(movementSpeed);
        } else if (currentPosX - movementSpeed > destinationPosX) {
            gameEntity.setPosX(currentPosX - movementSpeed);
            gameEntity.setCurrentChangeInPosX(-movementSpeed);
        } else {
            posXAchieved = true;

            if ((path.size() > 1 &&
                 (*(path.rbegin() + 1) % Constants::MAP_WIDTH) == (path.back() % Constants::MAP_WIDTH)) ||
                path.size() == 1) {
                gameEntity.setCurrentChangeInPosX(0);
                gameEntity.setPosX(destinationPosX);
            } else {
                gameEntity.setPosX(currentPosX + gameEntity.getCurrentChangeInPosX());
            }
        }

        if (currentPosY + movementSpeed < destinationPosY) {
            gameEntity.setPosY(currentPosY + movementSpeed);
            gameEntity.setCurrentChangeInPosY(movementSpeed);
        } else if (currentPosY - movementSpeed > destinationPosY) {
            gameEntity.setPosY(currentPosY - movementSpeed);
            gameEntity.setCurrentChangeInPosY(-movementSpeed);
        } else {
            posYAchieved = true;

            if ((path.size() > 1 &&
                 (*(path.rbegin() + 1) / Constants::MAP_WIDTH) == (path.back() / Constants::MAP_WIDTH)) ||
                path.size() == 1) {
                gameEntity.setCurrentChangeInPosY(0);
                gameEntity.setPosY(destinationPosY);
            } else {
                gameEntity.setPosY(currentPosY + gameEntity.getCurrentChangeInPosY());
            }
        }

        if (posXAchieved && posYAchieved) {
            if (path.front() != destinationTile) {
                isPathCalculated = false;
                std::cout << "PosX and PosY achieved " << " front to : " << path.front() << " dest to : "
                          << destinationTile << " Request to find new path sent!\n";
                path.clear();
            } else
                path.pop_back();
        }
    } else {
        if (isPathCalculated == true &&
            destinationTile == gl::calculateTileIndex(gameEntity.getPosX(), gameEntity.getPosY())) {
            if (messageSent == false) {
                gameEntity.sendMessage(MessageType::destinationReached);
                messageSent = true;
            }
        }
    }
}

void ComponentMovement::acceptMessage(MessageType messageType) {
}

void ComponentMovement::setDestinationTile(int tileIndex) {
    this->destinationTile = tileIndex;
    if (path.size() == 0) {
        isPathCalculated = false;
    }
}

unsigned int ComponentMovement::getHeuristicDistance(int startTileIndex, int endTileIndex) {
    int startPosX = startTileIndex % Constants::MAP_WIDTH;
    int startPosY = startTileIndex / Constants::MAP_HEIGHT;
    int endPosX = endTileIndex % Constants::MAP_WIDTH;
    int endPosY = endTileIndex / Constants::MAP_WIDTH;

    return std::abs(endPosX - startPosX) + std::abs(endPosY - startPosY);
}

const std::vector<int> &ComponentMovement::getPath() const {
    return path;
}

void ComponentMovement::findNewPath(GameEntity &gameEntity) {
    auto timer1 = high_resolution_clock::now();
    std::cout << "findNewPath() with ";
    std::cout << gameEntity.getPosX() << " " << gameEntity.getPosY() << " -> ";

    std::vector<PathStep> closedList;
    closedList.reserve(Constants::MAP_WIDTH * Constants::MAP_HEIGHT);
    std::vector<PathStep> openList;

    int endTileIndex = destinationTile;
    int startPosX = gameEntity.getPosX() / Constants::TILE_WIDTH;
    int startPosY = gameEntity.getPosY() / Constants::TILE_HEIGHT;
    // indeks w tablicy 1-wymiarowej z ktorego zaczynamy szukac drogi do indeksu endTileIndex
    int startTileIndex = startPosX + startPosY * Constants::MAP_WIDTH;

    openList.emplace_back(startTileIndex, 0, 0);

    PathStep *currentStep = nullptr;

    bool pathFound = false;

    while (openList.size() > 0) {
        unsigned int minimalValue = std::numeric_limits<unsigned int>::max();
        for (auto &it: openList) {
            if (it.getScoreF() < minimalValue) {
                minimalValue = it.getScoreF();
                currentStep = &it;
            }
        }

        closedList.emplace_back(currentStep->tileIndex, currentStep->pathScoreG, currentStep->pathScoreH,
                                currentStep->parent);

        // wydajne usuniecie elementu z wektora, poprzez zamiane z ostatnim i pop ostatniego
        std::swap(*currentStep, openList.back());
        openList.pop_back();

        currentStep = &closedList.back();

        if (currentStep->tileIndex == endTileIndex) {
            pathFound = true;
            break;
        }

        // dodajemy wszystkie poprawne, sasiadujace z naszym obecnym kafelkiem kafelki do kontenera adjacentTilesIndexes
        std::vector<int> adjacentTilesIndexes;
        adjacentTilesIndexes.reserve(4);
        int currentTileIndex = currentStep->tileIndex;

        // plytka gorna
        if (currentTileIndex - Constants::MAP_WIDTH > 0 && gameMap.isWalkable(currentTileIndex - Constants::MAP_WIDTH))
            adjacentTilesIndexes.emplace_back(currentTileIndex - Constants::MAP_WIDTH);
        // plytka prawa
        if (((currentTileIndex + 1) % Constants::MAP_WIDTH) != 0 && gameMap.isWalkable(currentTileIndex + 1))
            adjacentTilesIndexes.emplace_back(currentTileIndex + 1);
        // plytka dolna
        if (currentTileIndex + Constants::MAP_WIDTH < Constants::MAP_WIDTH * Constants::MAP_HEIGHT &&
            gameMap.isWalkable(currentTileIndex + Constants::MAP_WIDTH))
            adjacentTilesIndexes.emplace_back(currentTileIndex + Constants::MAP_WIDTH);
        // plytka lewa
        if ((currentTileIndex % Constants::MAP_WIDTH) != 0 && gameMap.isWalkable(currentTileIndex - 1))
            adjacentTilesIndexes.emplace_back(currentTileIndex - 1);

        // wykonujemy dla kazdego pola sasiadujacego (przechodzimy przez caly kontener adjacentTilesIndexes)
        for (unsigned int i = 0; i < adjacentTilesIndexes.size(); ++i) {
            // sprawdzamy czy dany kafelek z tych sasiadujacych byl juz w zamknietej liscie
            bool isInClosedList = false;
            for (auto it = closedList.begin(); it != closedList.end(); ++it) {
                if ((*it).tileIndex == adjacentTilesIndexes[i]) {
                    isInClosedList = true;
                    break;
                }
            }
            if (isInClosedList)
                continue;

            // sprawdzamy czy dany kafelek z tych sasiadujacych byl juz w otwartej liscie
            bool isInOpenList = false;
            PathStep *adjacentTile = nullptr;

            for (auto it = openList.begin(); it != openList.end(); ++it) {
                if ((*it).tileIndex == adjacentTilesIndexes[i]) {
                    isInOpenList = true;
                    adjacentTile = &(*it);
                    break;
                }
            }
            if (!isInOpenList) {
                openList.emplace_back(adjacentTilesIndexes[i], currentStep->pathScoreG + 1,
                                      getHeuristicDistance(adjacentTilesIndexes[i], endTileIndex), currentStep);
            } else {
                if (currentStep->pathScoreG + 1 <= adjacentTile->pathScoreG) {
                    adjacentTile->pathScoreG = currentStep->pathScoreG + 1;
                    adjacentTile->parent = currentStep;
                }
            }
        }
    }

    path.clear();

    if (pathFound) {
        while (currentStep->parent != nullptr) {
            path.emplace_back(currentStep->tileIndex);
            currentStep = currentStep->parent;
        }
    } else
        this->destinationTile = startTileIndex;

    isPathCalculated = true;

    std::cout << " findNewPath() finished\n";
    auto timer2 = high_resolution_clock::now();
    auto wynikPomiaru = duration_cast<milliseconds>(timer2 - timer1).count();
    std::cout << "Pomiar wynik to : " << wynikPomiaru << "\n";
}

/*
Alternatywna metoda do szukania drogi, u�ywj�ca listy zamiast vectora.
*/

// void ComponentMovement::findNewPath(GameEntity& gameEntity)
//{
//    auto timer1 = high_resolution_clock::now();
//    std::cout << "findNewPath() with ";
//    std::cout << gameEntity.getPosX() << " " << gameEntity.getPosY() << " -> ";
//
//    std::vector<PathStep> closedList;
//    closedList.reserve(Constants::MAP_WIDTH * Constants::MAP_HEIGHT);
//    std::list<PathStep> openList;
////    std::vector<PathStep> openList;
//
//    int endTileIndex = destinationTile;
//    int startPosX = gameEntity.getPosX() / Constants::TILE_WIDTH;
//    int startPosY = gameEntity.getPosY() / Constants::TILE_HEIGHT;
//    // indeks w tablicy 1-wymiarowej z ktorego zaczynamy szukac drogi do indeksu endTileIndex
//    int startTileIndex = startPosX + startPosY * Constants::MAP_WIDTH;
//
//    openList.emplace_front(startTileIndex, 0, 0);
//
//    PathStep* currentStep = nullptr;
//
//    bool pathFound = false;
//
//    while(openList.size() > 0)
//    {
//        currentStep = &openList.front();
//
//        closedList.emplace_back(currentStep->tileIndex, currentStep->pathScoreG, currentStep->pathScoreH, currentStep->parent);
//        openList.pop_front();
//
//        currentStep = &closedList.back();
//
//        if(currentStep->tileIndex == endTileIndex)
//        {
//            pathFound = true;
//            break;
//        }
//
//        // dodajemy wszystkie poprawne, sasiadujace z naszym obecnym kafelkiem kafelki do kontenera adjacentTilesIndexes
//        std::vector<int> adjacentTilesIndexes;
//        adjacentTilesIndexes.reserve(4);
//        int currentTileIndex = currentStep->tileIndex;
//
//        // plytka gorna
//        if(currentTileIndex - Constants::MAP_WIDTH > 0 && gameMap.isWalkable(currentTileIndex - Constants::MAP_WIDTH))
//            adjacentTilesIndexes.emplace_back(currentTileIndex - Constants::MAP_WIDTH);
//        // plytka prawa
//        if(((currentTileIndex + 1) % Constants::MAP_WIDTH) != 0 && gameMap.isWalkable(currentTileIndex + 1))
//            adjacentTilesIndexes.emplace_back(currentTileIndex + 1);
//        // plytka dolna
//        if(currentTileIndex + Constants::MAP_WIDTH < Constants::MAP_WIDTH * Constants::MAP_HEIGHT && gameMap.isWalkable(currentTileIndex + Constants::MAP_WIDTH))
//            adjacentTilesIndexes.emplace_back(currentTileIndex + Constants::MAP_WIDTH);
//        // plytka lewa
//        if((currentTileIndex % Constants::MAP_WIDTH) != 0 && gameMap.isWalkable(currentTileIndex - 1))
//            adjacentTilesIndexes.emplace_back(currentTileIndex - 1);
//
//        // wykonujemy dla kazdego pola sasiadujacego (przechodzimy przez caly kontener adjacentTilesIndexes)
//        for(unsigned int i = 0; i < adjacentTilesIndexes.size(); ++i)
//        {
//            // sprawdzamy czy dany kafelek z tych sasiadujacych byl juz w zamknietej liscie
//            bool isInClosedList = false;
//            for(auto it = closedList.begin(); it != closedList.end(); ++it)
//            {
//                if((*it).tileIndex == adjacentTilesIndexes[i])
//                {
//                    isInClosedList = true;
//                    break;
//                }
//            }
//            if(isInClosedList)
//                continue;
//
//            // sprawdzamy czy dany kafelek z tych sasiadujacych byl juz w otwartej liscie
//            bool isInOpenList = false;
//            PathStep* adjacentTile = nullptr;
//
//            for(auto it = openList.begin(); it != openList.end(); ++it)
//            {
//                if((*it).tileIndex == adjacentTilesIndexes[i])
//                {
//                    isInOpenList = true;
//                    adjacentTile = &(*it);
//                    break;
//                }
//            }
//            if(!isInOpenList)
//            {
//                if(openList.size() == 0)
//                {
//                    openList.emplace_front(adjacentTilesIndexes[i], currentStep->pathScoreG + 1, getHeuristicDistance(adjacentTilesIndexes[i], endTileIndex), currentStep);
//                }
//                else if(openList.back().getScoreF() < getHeuristicDistance(adjacentTilesIndexes[i], endTileIndex) + currentStep->pathScoreG + 1)
//                {
//                    openList.emplace_back(adjacentTilesIndexes[i], currentStep->pathScoreG + 1, getHeuristicDistance(adjacentTilesIndexes[i], endTileIndex), currentStep);
//                }
//                else
//                {
//                    for(auto it = openList.begin(); it != openList.end(); ++it)
//                    {
//                        if((*it).getScoreF() >= getHeuristicDistance(adjacentTilesIndexes[i], endTileIndex) + currentStep->pathScoreG + 1)
//                        {
//                            openList.emplace(it, adjacentTilesIndexes[i], currentStep->pathScoreG + 1,
//                                             getHeuristicDistance(adjacentTilesIndexes[i], endTileIndex), currentStep);
//                            break;
//                        }
//                    }
//                }
//            }
//            else
//            {
//                if(currentStep->pathScoreG + 1 <= adjacentTile->pathScoreG)
//                {
//                    adjacentTile->pathScoreG = currentStep->pathScoreG + 1;
//                    adjacentTile->parent = currentStep;
//                }
//            }
//        }
//    }
//
//    path.clear();
//
//    if(pathFound)
//    {
//        while(currentStep->parent != nullptr)
//        {
//            path.emplace_back(currentStep->tileIndex);
//            currentStep = currentStep->parent;
//        }
//    }
//    else
//        this->destinationTile = startTileIndex;
//
//    isPathCalculated = true;
//
//    std::cout << " findNewPath() finished\n";
//    auto timer2 = high_resolution_clock::now();
//    auto wynikPomiaru = duration_cast<milliseconds>(timer2 - timer1).count();
//    std::cout << "Pomiar wynik to : " << wynikPomiaru << "\n";
//}