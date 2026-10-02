#ifndef FLOWER_H
#define FLOWER_H

class Flower
{
public:
    Flower(int lastMowed, int type, int height, int xCoord, int yCoord, bool reducedMowing, bool isPlantMaterial):

        plantType(type),
        lastMowingTime(lastMowed),
        plantHeight(height),
        x(xCoord),
        y(yCoord),
        lessMowing(reducedMowing),
        isPlant(isPlantMaterial){}


    // GETTER AND SETTER FUNCTIONS

    // Time that cell was last mowed
    int getLastMowingTime();
    void setLastMowingTime(int newLastMowingTime);

    // Plant type per cell, see plantType int object number-plant assignments
    int getPlantType();
    void setPlantType(int newPlantType);

    // Height of plant in each cell
    int getPlantHeight();
    void setPlantHeight(int newPlantHeight);

    // X-coordinate of each cell
    int getX();
    void setX(int newX);

    // Y-coordinate of each cell
    int getY();
    void setY(int newY);

    // Boolean value used for distinguishing between lawn and meadow patches; if true, it is a meadow and will only be mowed twice in whole run; if false,
    // it is a lawn cell and will be mowed according to the mowing frequency parameter set at beginning of model run
    bool getLessMowing();
    void setLessMowing(bool newLessMowing);

    // Boolean value to distinguish between plant cells and non-plant cells (buildings, streets, paths, trees also included as their growth is not relevant)
    bool getIsPlant();
    void setIsPlant(bool newIsPlant);

private:

    // OBJECTS USED IN GETTER/SETTER FUNCTIONS

    int lastMowingTime;

    int plantType; // 0 = ground, 1 = daisy, 2 = dandelion, 3 = clover, 4 = turfgrass

    int plantHeight;

    int x;

    int y;

    bool lessMowing;

    bool isPlant;
};



#endif // FLOWER_H
