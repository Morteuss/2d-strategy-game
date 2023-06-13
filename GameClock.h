#ifndef GAME_CLOCK_H
#define GAME_CLOCK_H

#include <string>

using std::string;

class GameClock {
private:
    int currentYear = 1;
    int currentMonth = 1;
    int currentDay = 1;

    int currentHour = 12;
    int currentMinute = 0;
    int currentSecond = 0;

    void correctInternalValues();

public:
    GameClock();

    int getSecond();

    int getMinute();

    int getHour();

    int getDay();

    int getMonth();

    int getYear();

    string getFullDate();

    void updateWithOneGameTick();
};

#endif // GAME_CLOCK_H
