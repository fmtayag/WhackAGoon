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

#pragma region HoleCollisionManager
HoleCollisionManager::HoleCollisionManager(std::vector<std::shared_ptr<Hole>> &holes, GameMouse *gMouse, int *score)
{
    m_holesRef = holes;
    m_pMouse = gMouse;
    m_pScore = score;
    m_flagWhackedTownie = false;
}
HoleCollisionManager::~HoleCollisionManager()
{
}
void HoleCollisionManager::update()
{
    GameRules gRules;
    m_flagWhackedTownie = false;

    for (std::shared_ptr<Hole> hole : m_holesRef)
    {
        //printf("drawing particles\n");
        hole->update(*m_pMouse);

        Hole::HoleHitStatus hitStatus = hole->fetchHitStatus();
        if (hitStatus == Hole::HoleHitStatus::HIT)
        {
            Hole::HoleType holeType = hole->fetchHoleType();
            switch (holeType)
            {
            case Hole::HoleType::HT_GOON:
                (*m_pScore)++;
                break;
            case Hole::HoleType::HT_TOWNIE:
                (*m_pScore) -= gRules.SCORE_PENALTY;
                m_flagWhackedTownie = true;
                break;
            case Hole::HoleType::HT_MAYOR:
                printf("You hit the mayor, idiot!\n");
                break;
            default:
                printf("Warning: reached default case in switch(holeType).\n");
                break;
            }

            // Negative score check
            if ((*m_pScore) < 0)
                (*m_pScore) = 0;
        }
    }
}
#pragma endregion HoleCollisionManager