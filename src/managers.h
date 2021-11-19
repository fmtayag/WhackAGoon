#ifndef MANAGERS_H
#define MANAGERS_H
#pragma once

#include <memory>
#include <vector>
#include "widgets.h"
#include "sprites.h"

class HoleManager
{
private:
    enum HoleChances
    {
        HC_GOON = 70,
        HC_TOWNIE = 20,
        HC_MAYOR = 10
    };

    std::vector<std::shared_ptr<Hole>> m_holes;
    GTimer m_tmrBeforeSpawn;
    Uint32 m_delayBeforeSpawn;

    Hole::HoleType chooseHoleType();
    bool checkForMayor();

public:
    HoleManager(std::vector<std::shared_ptr<Hole>> &holes);
    ~HoleManager();

    void update();
};

#endif // MANAGERS_H