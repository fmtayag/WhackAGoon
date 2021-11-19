#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include "managers.h"
#include "sprites.h"

#pragma region HoleManager
HoleManager::HoleManager(std::vector<std::shared_ptr<Hole>> &holes)
{
    m_holes = holes;
    m_tmrBeforeSpawn.start();
    m_delayBeforeSpawn = 400;
}
HoleManager::~HoleManager()
{
}
void HoleManager::update()
{
    int chosenIndex = rand() % m_holes.size();
    std::shared_ptr<Hole> chosenHole = m_holes[chosenIndex];
    bool isTimerUp = m_tmrBeforeSpawn.getTicks() >= m_delayBeforeSpawn;
    bool isResting = chosenHole->fetchHoleType() == Hole::HoleType::HT_NONE;

    if (isTimerUp && isResting)
    {
        m_tmrBeforeSpawn.stop();
        m_tmrBeforeSpawn.start();

        Hole::HoleType chosenHT = chooseHoleType();

        chosenHole->awaken(chosenHT);
    }
}
Hole::HoleType HoleManager::chooseHoleType()
{
    bool isMayorPresent = checkForMayor();

    int chanceGoon = HoleChances::HC_GOON;
    int chanceTownie = HoleChances::HC_TOWNIE;
    int chanceMayor = HoleChances::HC_MAYOR;

    // -- Generate bag for picking --
    std::vector<Hole::HoleType> bag;

    if (!isMayorPresent)
    {
        for (int i = 0; i < chanceMayor; i++)
        {
            bag.push_back(Hole::HoleType::HT_MAYOR);
        }
    }
    else
    {
        chanceTownie = chanceTownie + chanceMayor;
    }

    for (int i = 0; i < chanceGoon; i++)
    {
        bag.push_back(Hole::HoleType::HT_GOON);
    }

    for (int i = 0; i < chanceTownie; i++)
    {
        bag.push_back(Hole::HoleType::HT_TOWNIE);
    }

    // Shuffle bag
    unsigned seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
    shuffle(bag.begin(), bag.end(), std::default_random_engine(seed));

    // Pick an index of the bag randomly
    int chosenIndex = rand() % 100;

    // Return HoleType based on chosen index
    return bag[chosenIndex];
}
bool HoleManager::checkForMayor()
{
    for (std::shared_ptr<Hole> hole : m_holes)
    {
        if (hole->fetchHoleType() == Hole::HoleType::HT_MAYOR)
            return true;
    }
    return false;
}
#pragma endregion HoleManager