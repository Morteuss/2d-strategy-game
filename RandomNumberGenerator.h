#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

#include <random>

class RandomNumberGenerator {
private:
    static unsigned seed;
    static std::default_random_engine generator;
public:
    static int getIntNumber(int leftRange, int rightRange);
};

#endif // RANDOM_NUMBER_GENERATOR_H
