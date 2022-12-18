//
// Created by ljovanov on 05.11.2022.
//

#ifndef OPENGLPRJ_RANDOMUTILITY_HPP
#define OPENGLPRJ_RANDOMUTILITY_HPP


#include <random>
#include <ctime>

class RandomUtility {
public:
    static std::mt19937_64 randomEngine;

    static float getRandomFloat(float min, float max);

    static int getRandomInt(int min, int max);

    static bool getRandomBool();
};


#endif //OPENGLPRJ_RANDOMUTILITY_HPP
