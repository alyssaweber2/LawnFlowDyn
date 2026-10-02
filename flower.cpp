#include "flower.h"



// GETTER  AND SETTER FUNCTIONS


// Time that cell was last mowed
int Flower::getLastMowingTime()
{
    return lastMowingTime;
}

void Flower::setLastMowingTime(int newLastMowingTime)
{
    lastMowingTime = newLastMowingTime;
}

 // Plant type per cell, see plantType int object number-plant assignments
int Flower::getPlantType()
{
    return plantType;
}

void Flower::setPlantType(int newPlantType)
{
    plantType = newPlantType;
}

 // Height of plant in each cell
int Flower::getPlantHeight()
{
    return plantHeight;
}

void Flower::setPlantHeight(int newPlantHeight)
{
    plantHeight = newPlantHeight;
}

// X-coordinate of each cell
int Flower::getX()
{
    return x;
}

void Flower::setX(int newX)
{
    x = newX;
}

// Y-coordinate of each cell
int Flower::getY()
{
    return y;
}

void Flower::setY(int newY)
{
    y = newY;
}

// Boolean value used for distinguishing between lawn and meadow patches; if true, it is a meadow and will only be mowed twice in whole run; if false,
// it is a lawn cell and will be mowed according to the mowing frequency parameter set at beginning of model run
bool Flower::getLessMowing()
{
    return lessMowing;
}

void Flower::setLessMowing(bool newLessMowing)
{
    lessMowing = newLessMowing;
}

// Boolean value to distinguish between plant cells and non-plant cells (buildings, streets, paths, trees also included as their growth is not relevant)
bool Flower::getIsPlant()
{
    return isPlant;
}

void Flower::setIsPlant(bool newIsPlant)
{
    isPlant = newIsPlant;
}
