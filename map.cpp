#include "map.h"

Map::Map() {
    loadScenarioOne();
}

// Loads map without meadows or flower gardens
void Map::loadScenarioOne()
{
    image.load(":/resources/images/ModelMap.png");
    image.convertTo(QImage::Format_ARGB32);
}

// Loads map with flower gardens and without meadows
void Map::loadScenarioTwo()
{
    image.load(":/resources/images/ModelMapFlowerGardens.png");
    image.convertTo(QImage::Format_ARGB32);
}

// Loads map with flower gardens and meadows
void Map::loadScenarioThree()
{
    image.load(":/resources/images/ModelMapMeadow.png");
    image.convertTo(QImage::Format_ARGB32);
}

// Getter function for the map image
QImage Map::getImage()
{
    return image;
}

// Right after scenario is loaded into the map, color of meadow patches will be put into coordinates
// into this vector for the creation and configuration of the lawnPlants vector
void Map::createMowingVector(QImage &image)
{
    // Clearing and resizing lawnVector to match with map image
    lawnVector.clear();
    lawnVector.resize(image.height());

    // Checking each pixel of the image and assignin values the classify all pixels into specific categories:
    //    - dark green meadow patchs (less frequently mowed areas)
    //    - All pixels with light green patches (regularly mowed lawn pixels)
    //    - Non-plant patches
    // These values are then put into a 2D vector that matches the resolution of the map image
    for (int y = 0; y < image.height(); y++) {

        lawnVector[y].resize(image.width()); // Resizing all horizontal vectors to match that of the horizontal image resolution

        for (int x = 0; x < image.width(); x++) {

            QColor pixelRGB = image.pixelColor(x, y); // attaining RGB characteristics of pixel

            // If pixel matches color of meadow pixels, a 2 goes into the vector at index of the coordinates
            if (pixelRGB.red() == 89 &&
                pixelRGB.green() == 163 &&
                pixelRGB.blue() == 97) {
                lawnVector[y][x] = 2;
            // If pixel matches color of lawn pixels, a 1 goes into the vector at index of the coordinates
            } else if (pixelRGB.red() == 207 &&
                       pixelRGB.green() == 238 &&
                       pixelRGB.blue() == 144) {
                lawnVector[y][x] = 1;
            } else { // Any other pixel color is considered to be non-plant and is assigned a 0
                lawnVector[y][x] = 0;
            }
        }
    }
}

// Getter function for the lawnVector
std::vector<std::vector<int> > Map::getLawnVector() const
{
    return lawnVector;
}

// Setter function for the lawnVector
void Map::setLawnVector(const std::vector<std::vector<int> > &newLawnVector)
{
    lawnVector = newLawnVector;
}



