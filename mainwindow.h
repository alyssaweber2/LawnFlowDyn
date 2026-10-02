#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QGraphicsScene>
#include <random>
#include "flower.h"
#include "map.h"
#include <vector>
#include <QtCharts>





QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

// Point structure used for the organisation of x and y coordinates in the neighborhood vector
struct Point { // pulled from Qwen 3 30b coder instruct
    int x;
    int y;

    // Constructor
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};


class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Start_clicked();

    void on_scenarioChooser_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene = nullptr; // QGraphicsScene for the lawn map

    // Growing percentages/spawn percentages for each plant type
    float daisySpawnChance = 0.2;
    float cloverSpawnChance = 0.2;
    float dandelionSpawnChance = 0.2;
    float turfgrassSpawnChance = 0.4;

    // Initial mowing frequency value
    int mowingFrequency = 2;

    // Chance each plant will be mowed
    float daisyMowChance = 0.6;
    float cloverMowChance = 0.5;
    float dandelionMowChance = 0.8;
    float turfgrassMowChance = 0.9;

    // Minimum flowering heights for each flower
    float minDaisyFlower = 4; // Literature originally estimated for 2 cm - 10 cm, but 2 cm seems not too common and want to use a more average value for daisy flowering height
    float minDandelionFlower = 8; // Research suggests 8 cm min flowering height in Dandelions
    float minCloverFlower = 8; // Research suggests smallest flowers are at 3 - 4 in, so ~8 cm Clover flowering height
    float minTurfgrassFlower = 10; // Research suggests min flowering height of 10 cm for Red Fescue


    int runTime = 26; // 1 week temporal grain over 6 months is 26 weeks (52/2 weeks in a year)

    float mortalityChance = 0.15; // Chance of each plant dying from mowing event, applies to all plant types


    // Size of the scenario layout in pixels, matches map image resolution
    int xSize = 825;
    int ySize = 825;


    // Colors
    QRgb white = qRgb(255, 255, 255); // Daisy
    QRgb darkBlue = qRgb(57, 93, 208); // Clover
    QRgb yellow = qRgb(230, 226, 0); // Dandelion
    QRgb purple = qRgb(171, 138, 189); // Red fescue (turfgrass species)
    QRgb lawnGround = qRgb(207, 238, 144); // For identifying regular lawn that gets mowed according to set mowing frequency
    QRgb meadowGround = qRgb(89, 163, 97); // For identifying the less mowed meadow areas as well as flower gardens. These patches are mowed twice per run.


    // Rng object
    std::mt19937 mt;
    std::uniform_real_distribution<float> randomFloat_0_1;


    Map map; // map object for all map proccesses/data


    // VECTORS

    std::vector< std::vector<Flower> > lawnPlants; // Keeps track of plants in each cell of the map image
    std::vector<float> effectiveSpeciesVector; // Stores the effective species number for each week of the model run. Min, max, and avg values reported at end of run.
    std::vector<Point> neighborhood = {{0,1}, {-1, 0}, {0, -1}, {1,0}}; // Stores coordinates for neighborhood of each cell


    // LAWN GROWTH DYNAMICS AND PORTRAYAL FUNCTIONS

    void setupLawn(QImage &image); // Initial setup of the lawn

    void sprout(int x, int y, QImage &image); // Simulates growth of the plants

    void reproduction(int x, int y); // Simulates simple interspecies competition for reproduction in empty cells (due to mortality, for example)

    void mow(int x, int y, QImage &image, int week); // Simulates a mowing event

    void paintLawn(QImage &image); // Used to change pixel colors of cells based on the status of the plant/plant type

    void newSprout(float cloverSpawn, float dandelionSpawn, float daisySpawn, int x, int y); // Creates the 2D vector of the map pixels that changes throughout the model run


    // Calculates the Effective Species Number with Shannon Entropy and Hill Numbers (q = 1)
    void calcShannonH_q1(int daisyCount, int cloverCount, int dandelionCount, int turfgrassCount, float &effectiveSpecies);

    // Loads the correct map image based on the selected scenario
    void loadScenarioMap();


    // QCHART-RELATED OBJECTS

    QLineSeries *daisySeries; // Pointer for the data points of daisy flower counts
    QLineSeries *dandelionSeries; // Pointer for the data points of dandelion flower counts
    QLineSeries *cloverSeries; // Pointer for the data points of clover flower counts
    QLineSeries *turfgrassSeries; // Pointer for the data points of turfgrass flower counts

    QChart *chart; // Pointer to chart
    
    
    

};
#endif // MAINWINDOW_H
