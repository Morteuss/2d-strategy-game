#include "TreeGenerationSystem.h"

#include <set>

#include "ComponentDestructible.h"
#include "Constants.h"
#include "Game.h"
#include "GameEntity.h"
#include "GameLibrary.h"
#include "Map.h"
#include "RandomNumberGenerator.h"

#include <iostream>

using namespace std;

TreeGenerationSystem::TreeGenerationSystem() {
}

void TreeGenerationSystem::generateTreesOnStart(const Map &gameMap, Game &game) {
    int tileIndexX = 0;
    int tileIndexY = 0;

    std::set<int> createdTrees;

    for (unsigned int i = 0; i < Constants::INITIAL_TREES_CREATED_MAXIMUM; ++i) {
        tileIndexX = RandomNumberGenerator::getIntNumber(0, Constants::MAP_WIDTH - 1);
        tileIndexY = RandomNumberGenerator::getIntNumber(0, Constants::MAP_HEIGHT - 1);

        if (gameMap.getType(tileIndexY * Constants::MAP_WIDTH + tileIndexX) == TileType::grass0 ||
            gameMap.getType(tileIndexY * Constants::MAP_WIDTH + tileIndexX) == TileType::grass1) {
            if (createdTrees.count(tileIndexY * Constants::MAP_WIDTH + tileIndexX) == 0) {
                switch (RandomNumberGenerator::getIntNumber(0, 2)) {
                    case 0 :
                        game.addObject(gl::createTreeSapling(tileIndexX, tileIndexY));
                        break;
                    case 1 :
                        game.addObject(gl::createTreeYoung(tileIndexX, tileIndexY));
                        break;
                    case 2 :
                        game.addObject(gl::createTreeOld(tileIndexX, tileIndexY));
                        break;
                }
                createdTrees.insert(tileIndexY * Constants::MAP_WIDTH + tileIndexX);
            }
        }
    }
}

void TreeGenerationSystem::update(int timeChange, const Map &gameMap, Game &game) {
    if (timeChange % 13 == 0) {
        int tileIndexX = RandomNumberGenerator::getIntNumber(0, Constants::MAP_WIDTH - 1);
        int tileIndexY = RandomNumberGenerator::getIntNumber(0, Constants::MAP_HEIGHT - 1);

        if ((gameMap.getType(tileIndexY * Constants::MAP_WIDTH + tileIndexX) == TileType::grass0 ||
             gameMap.getType(tileIndexY * Constants::MAP_WIDTH + tileIndexX) == TileType::grass1) &&
            (gameMap.isWalkable(tileIndexY * Constants::MAP_WIDTH + tileIndexX))) {
            bool occupiedByAnotherTree = false;


            for (auto &objectReference: game.objects) {
                auto ptr = gl::getComponentPtr<ComponentDestructible>(objectReference);
                if (ptr != nullptr) {
                    if (gl::calculateTileIndex(objectReference.getPosX(), objectReference.getPosY()) ==
                        gl::convertIndexesToIndex(tileIndexX, tileIndexY)) {
                        occupiedByAnotherTree = true;
                        return;
                    }
                }
            }

            if (occupiedByAnotherTree == false) {
                game.addObject(gl::createTreeSapling(tileIndexX, tileIndexY));
            }
        }
    }
}

//treeGeneratorComponent->update(gameClock.getMinute() * 100 + gameClock.getSecond());

//void TreeGeneratorComponent::update(const int changeTime)
//{
//	if (changeTime % 7 == 0)
//	{
//		int posX = RandomNumberGenerator::getIntNumber(0, Constants::MAP_WIDTH - 1);
//		int posY = RandomNumberGenerator::getIntNumber(0, Constants::MAP_HEIGHT - 1);
//
//		if (gameMap.getType(posY * Constants::MAP_WIDTH + posX) == 0 ||
//			gameMap.getType(posY * Constants::MAP_WIDTH + posX) == 1)
//		{
//			trees.emplace_back(new SingleTree(posX, posY, 0));
//		}
//	}
//
//	for (auto &it : trees)
//		it->update(changeTime);
//
//}

//void TreeGeneratorComponent::SingleTree::update(int timeChange)
//{
//
//	if ((*this).type == 0 && timeChange == 300)
//	{
//		(*this).type = 1;
//	}
//	else if ((*this).type == 1 && timeChange == 5000)
//	{
//		(*this).type = 2;
//	}
//
//}
