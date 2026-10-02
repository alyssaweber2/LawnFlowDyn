#ifndef MAP_H
#define MAP_H
#include <QtCharts>


class Map
{
public:
    Map();

    // SCENARIO LOADING FUNCTIONS
    void loadScenarioOne();
    void loadScenarioTwo();
    void loadScenarioThree();

    // Getter function for the map image
    QImage getImage();

    // Creates vector to analyze loaded map image scenario and distinguish the meadow patches from the lawn patches and the non-plant patches,
    // which is then used to set correct booleans in the lawnPlants vector later on
    void createMowingVector(QImage &image);


    // Getter/Setter functions for the LawnVector, which is used in createMowingVectors()
    std::vector<std::vector<int> > getLawnVector() const;
    void setLawnVector(const std::vector<std::vector<int> > &newLawnVector);

private:

    //image of the sccenario layout
    QImage image;

    // lawnVector used in createMowingVector() function
    std::vector<std::vector <int>> lawnVector;
};

#endif // MAP_H
