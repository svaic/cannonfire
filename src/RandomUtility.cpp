//
// Created by ljovanov on 05.11.2022.
//

#include "RandomUtility.hpp"

float RandomUtility::getRandomFloat(float min, float max) {
    std::uniform_real_distribution<float> unif(min, max);
    return unif(randomEngine);
}

int RandomUtility::getRandomInt(int min, int max) {
    std::uniform_int_distribution<int> unii(min, max);
    return unii(randomEngine);
}
