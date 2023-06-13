#include "Game.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <thread>
#include <typeinfo>

#include "Constants.h"
#include "ContextMenuOrders.h"
#include "GameLibrary.h"
#include "ComponentGraphics.h"
#include "Map.h"
#include "ComponentMovement.h"
#include "DebugDisplaySystem.h"
#include "RandomNumberGenerator.h"
#include "ComponentTaskExecute.h"

using namespace std::chrono;
using namespace std;


Game::Game(sf::RenderWindow &window, atomic<bool> &isLoaded) :
        window(window),
        inputManager(window, gameCamera),
        gameMap(Constants::MAP_WIDTH, Constants::MAP_HEIGHT),
        contextMenuOrders(), taskExecuteSystem(),
        gameConsole(gameMap, objects),
        debugDisplaySystem(Constants::DEBUG_DISPLAY_ALPHA_VALUE),
        currentGameSpeed(Constants::GAME_SPEED) {
    this->numberOfObjects = Constants::INITIAL_OBJECTS_RESERVED;
    objects.reserve(Constants::INITIAL_OBJECTS_RESERVED + Constants::INITIAL_TREES_CREATED_MAXIMUM);

    // tworzenie postaci i umieszczanie ich w kontenerze (objects)
    for (int i = 0; i < numberOfObjects; ++i) {
        int tileIndexX;
        int tileIndexY;
        do {
            tileIndexX = RandomNumberGenerator::getIntNumber(0, Constants::MAP_WIDTH - 1);
            tileIndexY = RandomNumberGenerator::getIntNumber(0, Constants::MAP_HEIGHT - 1);
        } while (!gameMap.isWalkable(gl::convertIndexesToIndex(tileIndexX, tileIndexY)));

        addObject(gl::createCharacter(tileIndexX, tileIndexY, gameMap));
    }

    // generowanie drzew na planszy na polach o typie "trawa"
    treeGenerationSystem.generateTreesOnStart(gameMap, *this);

    this_thread::sleep_for(2000ms);
    isLoaded = true;
}

int Game::run() {
    ofstream fileHandle;
    fileHandle.open("time_logs.txt");
    gameCamera.reset(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    gameCamera.setCenter(3000, 3000);
    sf::RenderStates states;


    double timeDelay = 0;
    auto previousTimeMeasure = high_resolution_clock::now();
    while (window.isOpen()) {
        // Pomiar czasu wykonywania 1 klatki gry
        auto newTimeMeasure = high_resolution_clock::now();
        double timeDifference = duration_cast<microseconds>(newTimeMeasure - previousTimeMeasure).count();

        // dzielimy przez 1000 bo chcemy miec timeDelay w milisekunach, a timeDifference jest mierzone w mikrosekundach
        timeDelay += timeDifference / 1000;
        previousTimeMeasure = newTimeMeasure;

        while (timeDelay >= currentGameSpeed) {
            // Obsluga inputu
            inputManager.handleInput(*this);

            // silnik ai,physics...
            if (gamePause == false && gameConsole.isVisible() == false) {
                update();
                gameClock.updateWithOneGameTick();
            }
            timeDelay -= currentGameSpeed;
        }

        // Renderowanie grafiki
        render(timeDelay / currentGameSpeed);

        if (timeDifference / 1000 > 10)
            fileHandle << timeDifference / 1000 << "\n";
    }
    fileHandle.close();
    return 0;
}

void Game::render(double timeProgressValue) {
    window.setView(gameCamera);

    window.clear(sf::Color::Black);

    window.setView(window.getDefaultView());

    window.setView(gameCamera);

    window.draw(gameMap);

    if (gamePause == true)
        timeProgressValue = 0;

    for (auto &i: objects) {
        i.render(window, timeProgressValue);
    }

    // Rysowanie elementow niezaleznie od kamery
    window.setView(window.getDefaultView());

    //dzien i noc oswietlenie
    sf::RectangleShape dayNightCycler;

    dayNightCycler.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    dayNightCycler.setPosition(0, 0);
    sf::Uint8 alphaValue = 0;
    if (gameClock.getMinute() < 30) {
        alphaValue = (255.0 / 59) * gameClock.getMinute();
    } else if (gameClock.getMinute() <= 59) {
        alphaValue = (255.0 / 59) * (59 - gameClock.getMinute());
    }

    dayNightCycler.setFillColor(sf::Color(0, 0, 0, alphaValue));
    window.draw(dayNightCycler);

    // rysowanie menu wydawania rozkazow
    window.draw(contextMenuOrders);

    window.setView(gameCamera);

    // rysowanie do debugowania
    window.draw(debugDisplaySystem);

    window.setView(window.getDefaultView());

    // rysowanie konsoli
    window.draw(gameConsole);

    window.setView(gameCamera);

    window.display();
}

void Game::update() {
    taskExecuteSystem.update(objects, gameMap);
    taskExecuteSystem.executeTasks(objects, gameMap);

    for (unsigned int i = 0; i < objects.size(); ++i)
        objects[i].update();

    treeGenerationSystem.update(gameClock.getMinute() * 100 + gameClock.getSecond(), gameMap, *this);
}

void Game::foo(int start, int end) {
    for (int i = start; i < end; ++i)
        objects[i].update();
}

void Game::setGameSpeed(unsigned newGameSpeed) {
    this->currentGameSpeed = newGameSpeed;
}

Map &Game::getMap() {
    return this->gameMap;
}

void Game::addObject(GameEntity gameEntity) {
    for (auto &object: objects) {
        if (object.getExists() == false) {
            object = gameEntity;
            std::cout << "sukces dodania nowego obiektu na miejsce starego" << std::endl;
            return;
        }
    }

    // jesli nie bylo zadnych usunietych gameEntities w objects dodajemy nowy obiekt
    objects.emplace_back(gameEntity);
}

/*
Nie bedzie chyba jednak potrzebne po zmienieniu sposobu dodawania i ustawiania obiektow jako skasowane
*/

//void Game::removeDeletedObjects()
//{
//	// wydajne usuwanie obiektow, ktore juz nie istnieja w grze
//	for (auto it = objects.begin(); it != objects.end(); ++it)
//	{
//		if (it->getExists() == false)
//		{
//			*it = objects.back();
//			objects.pop_back();
//			// zmniejszamy it o 1, aby zachowac waznosc iteratora i nie pominac obiektu, ktory byl na koncu vectora
//			--it;
//		}
//	}
//}
