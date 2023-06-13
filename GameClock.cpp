#include "GameClock.h"

#include <sstream>

using std::stringstream;

GameClock::GameClock() {

}

void GameClock::updateWithOneGameTick() {
    this->currentSecond++;

    if (currentSecond >= 60)
        correctInternalValues();
}

void GameClock::correctInternalValues() {
    int numberOfSecondsInMinute = 60;
    int numberOfMinutesInHour = 60;
    int numberOfHoursInDay = 24;
    int numberOfDaysInMonth = 30;
    int numberOfMonthsInYear = 12;

    if (currentSecond >= numberOfSecondsInMinute) {
        currentSecond -= numberOfSecondsInMinute;
        currentMinute++;
    } else
        return;
    if (currentMinute >= numberOfMinutesInHour) {
        currentMinute -= numberOfMinutesInHour;
        currentHour++;
    } else
        return;
    if (currentHour >= numberOfHoursInDay) {
        currentHour -= numberOfHoursInDay;
        currentDay++;
    } else
        return;
    if (currentDay >= numberOfDaysInMonth) {
        currentDay -= numberOfDaysInMonth;
        currentMonth++;
    } else
        return;
    if (currentMonth >= numberOfMonthsInYear) {
        currentMonth -= numberOfMonthsInYear;
        currentYear++;
    } else
        return;
}

int GameClock::getSecond() {
    return currentSecond;
}

int GameClock::getMinute() {
    return currentMinute;
}

int GameClock::getHour() {
    return currentHour;
}

int GameClock::getDay() {
    return currentDay;
}

int GameClock::getMonth() {
    return currentMonth;
}

int GameClock::getYear() {
    return currentYear;
}

string GameClock::getFullDate() {
    stringstream temp;
    temp << currentYear << "." << currentMonth << "." << currentDay << "   " << currentHour << " : " << currentMinute
         << " : " << currentSecond;

    return temp.str();
}
