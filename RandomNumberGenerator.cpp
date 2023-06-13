#include "RandomNumberGenerator.h"
#include <chrono>

unsigned RandomNumberGenerator::seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine RandomNumberGenerator::generator(seed);

int RandomNumberGenerator::getIntNumber(int leftRange, int rightRange) {
    std::uniform_int_distribution<int> distribution(leftRange, rightRange);
    return distribution(generator);
}
