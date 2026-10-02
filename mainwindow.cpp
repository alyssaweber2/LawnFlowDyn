#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <unistd.h>
#include <QTimer>
#include <QValueAxis>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this); // this is autodeleted with delete ui in ~Mainwindow

    // Setting the scene in the graphics view
    ui->lawnGraphicsView->setScene(scene);

    // Configuring the daisy growth chance spinbox in the UI
    ui->daisy_GrowthChance->setValue(daisySpawnChance);
    ui->daisy_GrowthChance->setRange(0.0, 1.0);
    ui->daisy_GrowthChance->setSingleStep(0.05);

    // Configuring the dandelion growth chance spinbox in the UI
    ui->dandelion_GrowthChance->setValue(dandelionSpawnChance);
    ui->dandelion_GrowthChance->setRange(0.0, 1.0);
    ui->dandelion_GrowthChance->setSingleStep(0.05);

    // Configuring the clover growth chance spinbox in the UI
    ui->clover_GrowthChance->setValue(cloverSpawnChance);
    ui->clover_GrowthChance->setRange(0.0, 1.0);
    ui->clover_GrowthChance->setSingleStep(0.05);

    // Configuring the turfgrass growth chance spinbox in the UI
    ui->turfgrass_GrowthChance->setValue(turfgrassSpawnChance);
    ui->turfgrass_GrowthChance->setRange(0.0, 1.0);
    ui->turfgrass_GrowthChance->setSingleStep(0.05);

    // Configuring the mowing frequency spinbox in the UI
    ui->mowingFreq->setValue(mowingFrequency);
    ui->mowingFreq->setRange(1, 26);

    // Configuring the daisy mowing chance spinbox in the UI
    ui->daisy_MowChance->setValue(daisyMowChance);
    ui->daisy_MowChance->setRange(0.0, 1.0);
    ui->daisy_MowChance->setSingleStep(0.05);

    // Configuring the dandelion mowing chance spinbox in the UI
    ui->dandelion_MowChance->setValue(dandelionMowChance);
    ui->dandelion_MowChance->setRange(0.0, 1.0);
    ui->dandelion_MowChance->setSingleStep(0.05);

    // Configuring the clover mowing chance spinbox in the UI
    ui->clover_MowChance->setValue(cloverMowChance);
    ui->clover_MowChance->setRange(0.0, 1.0);
    ui->clover_MowChance->setSingleStep(0.05);

    // Configuring the turfgrass mowing chance spinbox in the UI
    ui->turfgrass_MowChance->setValue(turfgrassMowChance);
    ui->turfgrass_MowChance->setRange(0.0, 1.0);
    ui->turfgrass_MowChance->setSingleStep(0.05);

    // Configuring the mortality chance spinbox in the UI
    ui->mortalityChance->setValue(mortalityChance);
    ui->mortalityChance->setRange(0.0, 1.0);
    ui->mortalityChance->setSingleStep(0.05);


    // Setting the image and scene
    QImage scenarioLayout = map.getImage();
    scene->addPixmap(QPixmap::fromImage(scenarioLayout));

    // Scaling the graphics view for optimal user viewing - change if scaling is not optimal
    ui->lawnGraphicsView->scale(0.6, 0.6);


    // Configuring the chart objects and the series objects for each plant
    chart = new QChart();

    // Daisy flower counts configuration
    daisySeries = new QLineSeries();
    daisySeries->setName("Daisy");
    daisySeries->setColor(Qt::blue);

    // Dandelion flower counts configuration
    dandelionSeries = new QLineSeries();
    dandelionSeries->setName("Dandelion");
    dandelionSeries->setColor(qRgb(247, 193, 41));

    // Clover flower counts configuration
    cloverSeries = new QLineSeries();
    cloverSeries->setName("White Clover");
    cloverSeries->setColor(Qt::green);

    // Turfgrass flower counts configuration
    turfgrassSeries = new QLineSeries();
    turfgrassSeries->setName("Red Fescue");
    turfgrassSeries->setColor(Qt::magenta);

    // Adding all series objects to the chart
    chart->addSeries(daisySeries);
    chart->addSeries(dandelionSeries);
    chart->addSeries(cloverSeries);
    chart->addSeries(turfgrassSeries);

    // Configuring the axes of the chart
    chart->createDefaultAxes();

    // Labeling the X and Y axes and titling the graph
    chart->axisX()->setTitleText("Time [weeks]");
    chart->axisY()->setTitleText("# Individuals");

    chart->setTitle("<H2>Flower counts per week</H2>");

    // Configuring X axis to count in integers in the correct bins
    chart->axisX()->setRange(0, 26);
    QValueAxis *xAxis = qobject_cast<QValueAxis*>(chart->axisX());
    xAxis->setLabelFormat("%d");
    xAxis->setTickCount(runTime/2 + 1);

    // Assigning starting Y-axis value (this does dynamically change as model runs)
    chart->axisY()->setRange(0, 10000);

    // Adding the chart object to the graph in the UI
    ui->diversityGraph->setChart(chart);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::reproduction(int x, int y)
{

    // Random number object for the spawning event of the plant types
    float spawnRNG = randomFloat_0_1(mt);


    //Spawn Chance Param values from UI
    float daisySpawn = ui->daisy_GrowthChance->value();
    float dandelionSpawn = ui->dandelion_GrowthChance->value();
    float cloverSpawn = ui->clover_GrowthChance->value();
    float turfgrassSpawn = ui->turfgrass_GrowthChance->value();

    // Initializing and clearing set object for flower neighbors, only contains unique values of plant types (0-4)
    std::set<int> flowerNeighbors;
    flowerNeighbors.clear();


    // This only applied to Flower agents with plant type = 0, which signifies an empty cell
    if (lawnPlants[y][x].getPlantType() == 0) {
        // Checks all neighborhood coordinates for validity, attains all flower types in neighborhood and puts
        // them into flowerNeighbors vector
        for (int c  = 0; c < neighborhood.size(); c++) {
            if (x + neighborhood[c].x >= 0 && y + neighborhood[c].y >= 0) {
                flowerNeighbors.insert(lawnPlants[y][x].getPlantType());
            }
        }

        // Takes all flower types from flowerNeighbors vector and inserts the unique values into the setVector
        std::vector<int> setVector(flowerNeighbors.begin(), flowerNeighbors.end());


        // If the only value in the setVector is 0 (meaning simply ground, no plant assigned), a random flower will be spawned there.
        // If there are any other flower types in the setVector, there is a stochastic chance that that flower type will grow in the empty cell.
        // All spawn chances of the plant types must add up to 1 in order for this process to work properly.
        // When new plant sprouts, the plant height will always start at 1 cm.
        for (int f = 0; f < setVector.size(); f++) {
            switch (setVector[f]) {
            case 0: // For ground cells
                if (setVector.size() == 1) { // Random flower will grow there, since there is only ground surrounding the cell
                    newSprout(cloverSpawn, dandelionSpawn, daisySpawn, x, y);
                }
                break;
            case 1: // For daisy cells - if rng is within daisy spawnchance
                if (spawnRNG < daisySpawn) { //
                    lawnPlants[y][x].setPlantType(1);
                    lawnPlants[y][x].setPlantHeight(1);
                } else { // If respective neighbor doesn't sprout new flower in target cell, then sprout random plant
                    newSprout(cloverSpawn, dandelionSpawn, daisySpawn, x, y);
                }
                break;
            case 2: // For dandelion cells - if rng is within dandelion spawn chance (must cancel out daisy)
                if (spawnRNG < dandelionSpawn + daisySpawn) {
                    lawnPlants[y][x].setPlantType(2);
                    lawnPlants[y][x].setPlantHeight(1);
                } else { // If respective neighbor doesn't sprout new flower in target cell, then sprout random plant
                    newSprout(cloverSpawn, dandelionSpawn, daisySpawn, x, y);
                }
                break;
            case 3: // For clover cells - if rng is within clover spawn chance (must cancel our daisy and dandelion)
                if (spawnRNG < cloverSpawn + dandelionSpawn + daisySpawn) {
                    lawnPlants[y][x].setPlantType(3);
                    lawnPlants[y][x].setPlantHeight(1);
                }else {  // If respective neighbor doesn't sprout new flower in target cell, then sprout random plant
                    newSprout(cloverSpawn, dandelionSpawn, daisySpawn, x, y);
                }
                break;
            case 4: // For turfgrass cells - the leftover percentage (1 - daisy - dandelion - clover) would be turfgrass spawn chance
                if (spawnRNG >= cloverSpawn + dandelionSpawn + daisySpawn){
                    lawnPlants[y][x].setPlantType(4);
                    lawnPlants[y][x].setPlantHeight(1);
                } else { // If respective neighbor doesn't sprout new flower in target cell, then sprout random plant
                    newSprout(cloverSpawn, dandelionSpawn, daisySpawn, x, y);
                }
                break;
            }
            setVector.clear(); // Clearing the vector for the next cell
        }
    }
}

// Simulates growth of the plants per cell
void MainWindow::sprout(int x, int y, QImage &image)
{

    //WEEKLY GROWTH (~3 cm for each plant)
    lawnPlants[y][x].setLastMowingTime(lawnPlants[y][x].getLastMowingTime() + 1); // Not useful in current model, may be useful for future model advancement, number describes when (in weeks) was cell last mowed
    lawnPlants[y][x].setPlantHeight(lawnPlants[y][x].getPlantHeight() + 3); // Each plant grows 3 cm per week

}


// Simulates a mowing event
void MainWindow::mow(int x, int y, QImage &image, int week)
{
    // Random-number objects for mowing chance of each plant and the mortality of each plant during a mowing event
    float mowRNG = randomFloat_0_1(mt);
    float mortalityRNG = randomFloat_0_1(mt);

    // Checks if the week matches with that of the mowing frequency as well as if the plants are REGULARLY-MOWED LAWN plants and not meadow patches
    if (week % ui->mowingFreq->value() == 0 && lawnPlants[y][x].getLessMowing() == false) {
        // Daisy mowing - if plant is a daisy and the daisy mowing chance is less than the random number object, then plant will get mowed
        if (lawnPlants[y][x].getPlantType() == 1 && mowRNG < daisyMowChance) {
            lawnPlants[y][x].setLastMowingTime(0);
            lawnPlants[y][x].setPlantHeight(3);
        }
        // Dandelion mowing - if plant is a dandelion and the dandelion mowing chance is less than the random number object, then plant will get mowed
        if (lawnPlants[y][x].getPlantType() == 2 && mowRNG < dandelionMowChance) {
            lawnPlants[y][x].setLastMowingTime(0);
            lawnPlants[y][x].setPlantHeight(3);
        }
        // Clover mowing - if plant is a clover and the clover mowing chance is less than the random number object, then plant will get mowed
        if (lawnPlants[y][x].getPlantType() == 3 && mowRNG < cloverMowChance) {
            lawnPlants[y][x].setLastMowingTime(0);
            lawnPlants[y][x].setPlantHeight(3);
        }
        // Turfgrass mowing - if plant is turfgrass and the turfgrass mowing chance is less than the random number object, then plant will get mowed
        if (lawnPlants[y][x].getPlantType() == 4 && mowRNG < turfgrassMowChance) {
            lawnPlants[y][x].setLastMowingTime(0);
            lawnPlants[y][x].setPlantHeight(3);
        }
        // General mortality mechanics applied to all plants that are mowed
        if (mortalityRNG < ui->mortalityChance->value()) {
            lawnPlants[y][x].setPlantHeight(0);
            lawnPlants[y][x].setPlantType(0);
        }
    // Mowing carried out for the less mowed plants only 2 times in the run (meadow patches)
    } else if (lawnPlants[y][x].getLessMowing() == true && week % 13 == 0) {
        // Daisy mowing - if plant is a daisy and the daisy mowing chance is less than the random number object, then plant will get mowed
        if (lawnPlants[y][x].getPlantType() == 1 && mowRNG < daisyMowChance) {
            lawnPlants[y][x].setLastMowingTime(0);
            lawnPlants[y][x].setPlantHeight(2);
        }
        // Dandelion mowing - if plant is a dandelion and the dandelion mowing chance is less than the random number object, then plant will get mowed
        if (lawnPlants[y][x].getPlantType() == 2 && mowRNG < dandelionMowChance) {
            lawnPlants[y][x].setLastMowingTime(0);
            lawnPlants[y][x].setPlantHeight(2);
        }
        // Clover mowing - if plant is a clover and the clover mowing chance is less than the random number object, then plant will get mowed
        if (lawnPlants[y][x].getPlantType() == 3 && mowRNG < cloverMowChance) {
            lawnPlants[y][x].setLastMowingTime(0);
            lawnPlants[y][x].setPlantHeight(2);
        }
        // Turfgrass mowing - if plant is turfgrass and the turfgrass mowing chance is less than the random number object, then plant will get mowed
        if (lawnPlants[y][x].getPlantType() == 4 && mowRNG < turfgrassMowChance) {
            lawnPlants[y][x].setLastMowingTime(0);
            lawnPlants[y][x].setPlantHeight(2);
        }
        // General mortality mechanics applied to all plants that are mowed
        if (mortalityRNG < ui->mortalityChance->value()) {
            lawnPlants[y][x].setPlantHeight(0);
            lawnPlants[y][x].setPlantType(0);
        }
    }
}


// Used to change pixel colors of cells based on the status of the plant/plant type
void MainWindow::paintLawn(QImage &image)
{

    // Integer objects relating to the plant type for easier readability. These coded plant types are already used throughout the model.
    const int daisy = 1;
    const int dandelion = 2;
    const int clover = 3;
    const int turfgrass = 4;
    const int nonflowers = 0;

    // Iterates through every pixel in the map, checks the if the pixel is a plant, then attains the plant type and height, and assigns the
    // acccording pixel color based on these factors
    for (int y = 0; y < ySize; y++) {
        for(int x= 0; x < xSize; x++) {

            if(lawnPlants[y][x].getIsPlant() == true) { // Checking if pixel is a plant
                switch (lawnPlants[y][x].getPlantType()) { // Returning the plant type (in integer form) of the pixel
                case daisy:
                     // Pixel will only be colored (to simulate a flower) when the min. Daisy flowering height is met
                    if (lawnPlants[y][x].getPlantHeight() >= minDaisyFlower) {
                    }  else { // If plant is not tall enough, pixel remains a ground-colored pixel
                        image.setPixelColor(x, y, meadowGround);
                    }
                    break;
                case dandelion:
                    // Pixel will only be colored (to simulate a flower) when the min. Dandelion flowering height is met
                    if (lawnPlants[y][x].getPlantHeight() >= minDandelionFlower) {
                        image.setPixelColor(x, y, yellow);
                    } else { // If plant is not tall enough, pixel remains a ground-colored pixel
                        image.setPixelColor(x, y, meadowGround);
                    }
                    break;
                case clover:
                    // Pixel will only be colored (to simulate a flower) when the min. Clover flowering height is met
                    if (lawnPlants[y][x].getPlantHeight() >= minCloverFlower) {
                        image.setPixelColor(x, y, darkBlue);
                    } else { // If plant is not tall enough, pixel remains a ground-colored pixel
                        image.setPixelColor(x, y, meadowGround);
                    }
                    break;
                case turfgrass:
                    // Pixel will only be colored (to simulate a flower) when the min. turfgrass flowering height is met
                    if (lawnPlants[y][x].getPlantHeight() >= minTurfgrassFlower) {
                        image.setPixelColor(x, y, purple);
                    } else { // If plant is not tall enough, pixel remains a ground-colored pixel
                        image.setPixelColor(x, y, meadowGround);
                    }
                    break;
                case nonflowers: // Any plant pixel that is empty (plant died in previous step) will be returned to the ground color
                    image.setPixelColor(x, y, meadowGround);
                    break;
                }
            }
        }
    }
}


// Siumulates new growth of plants in an empty cell
void MainWindow::newSprout(float cloverSpawn, float dandelionSpawn, float daisySpawn, int x, int y)
{
    // Random number object for the chance of each plant type to grow in cell
    float spawnRNG = randomFloat_0_1(mt);

    // If spawnRNG is less than the daisy spawn chance, then pixel will sprout a Daisy
    if (spawnRNG < daisySpawn) {
        lawnPlants[y][x].setPlantType(1);
    } else if (spawnRNG < dandelionSpawn + daisySpawn) { // If spawnRNG is less than the Dandelion spawn chance (must cancel out daisy), Dandelion will sprout
        lawnPlants[y][x].setPlantType(2);
    } else if (spawnRNG < cloverSpawn + dandelionSpawn + daisySpawn) { // If spawn RNG is less than the Clover spawn chance (must cancel our daisy and dandelion), Clover will sprout
        lawnPlants[y][x].setPlantType(3);
    } else { // Anything else will sprout turfgrass in the cell
        lawnPlants[y][x].setPlantType(4);
    }
}

// Creates the 2D vector of the map pixels that changes throughout the model run
void MainWindow::setupLawn(QImage &image)
{
    // Clearning and resizing 2D vector to size of the map image
    lawnPlants.clear();
    lawnPlants.resize(image.height());

    // Sample plant to fill the vector with - every plant starts off at 2 cm
    Flower meadowGroundPlant(0, 0, 3, 0, 0, false, true);


    // Spawn Chance Param values from UI
    float daisySpawn = ui->daisy_GrowthChance->value();
    float dandelionSpawn = ui->dandelion_GrowthChance->value();
    float cloverSpawn = ui->clover_GrowthChance->value();
    float turfgrassSpawn = ui->turfgrass_GrowthChance->value();

    // Attaining vector object from the map, which contains info on the map pixels based (non-plant, regular lawn, reduced mowing meadow patch, etc)
    std::vector<std::vector <int>> lawnVector = map.getLawnVector();

    // For-loop for assigning proper values to lawnPlants vector based on lawnVector information
    for (int y = 0; y < image.height(); y++){ // Image rows (y-axis)

        // Resizing the x-axis of each vector in lawnPlants and assigning all positions with the filler meadowGroundPlant
        lawnPlants[y].resize(image.width(), meadowGroundPlant);

        for(int x = 0; x < image.width(); x++) { // Image columns (x-axis)

            // setting the coords of the location as well as the lessMowing boolean (TRUE).
            //if the value is 0 in the lawnVector, make it nonplant, but leave everything else
            // if the value is 1 in the lawnVector, make it regular lawn, no reduced mowing
            // if the value is 2 in the lawnVector, make it meadow, and reduce mowing
            switch (lawnVector[y][x]) {
            case 0: // For Non-plant surfaces, assigning pixel as non-plant and setting the x and y coordinates
                lawnPlants[y][x].setX(x);
                lawnPlants[y][x].setY(y);
                lawnPlants[y][x].setIsPlant(false);
                break;
            case 1: // For regularly mowed lawn pixels, assigning it as a plant, it doesn't receive less mowing, and also setting the x and y coordinates
                lawnPlants[y][x].setX(x);
                lawnPlants[y][x].setY(y);
                lawnPlants[y][x].setIsPlant(true);
                lawnPlants[y][x].setLessMowing(false);
                break;
            case 2: // For meadow pixels - assigning it as a plant, assigning less-mowing, and setting the x and y coordinates
                lawnPlants[y][x].setX(x);
                lawnPlants[y][x].setY(y);
                lawnPlants[y][x].setIsPlant(true);
                lawnPlants[y][x].setLessMowing(true);
                break;
            }

            // Each plant pixel then gets random flower assignment through newSprout()
            newSprout(cloverSpawn, dandelionSpawn, daisySpawn, x, y);
        }
    }
}


 // Calculates the Effective Species Number with Shannon Entropy and Hill Numbers (q = 1) - attains value that balances evenness and richness for all species
void MainWindow::calcShannonH_q1(int daisyCount, int cloverCount, int dandelionCount, int turfgrassCount, float &effectiveSpecies)
{
    // Adding up all flower counts at a specific time period
    float totalFlowers = daisyCount + dandelionCount + cloverCount + turfgrassCount;

    // Relative abundance calculation for each plant type
    float daisyRelAbun = static_cast<float>(daisyCount)/totalFlowers;
    float dandelionRelAbun = static_cast<float>(dandelionCount)/totalFlowers;
    float cloverRelAbun = static_cast<float>(cloverCount)/totalFlowers;
    float turfgrassRelAbun = static_cast<float>(turfgrassCount)/totalFlowers;

    // Calculating the Shannon Entropy for each plant type
    float shannonDaisy = daisyRelAbun * log(daisyRelAbun);
    float shannonDandelion = dandelionRelAbun * log(dandelionRelAbun);
    float shannonClover = cloverRelAbun * log(cloverRelAbun);
    float shannonTurfgrass = turfgrassRelAbun * log(turfgrassRelAbun);

    // Getting rid of Nans and replacing them with 0 - works because values at the end are simply added, otherwise would have done this differently
    if (std::isnan(shannonDaisy)) {shannonDaisy = 0.0;}
    if (std::isnan(shannonDandelion)) {shannonDandelion = 0;}
    if (std::isnan(shannonClover)) {shannonClover = 0;}
    if (std::isnan(shannonTurfgrass)) {shannonTurfgrass = 0;}

    // Calculating final Shannon Entropy value combining all species
    float ShannonH = -1 * (shannonDaisy + shannonDandelion + shannonClover + shannonTurfgrass);

    // Applying Hill Numbers ( q = 1 ) rule to attain Effective Species Number
    effectiveSpecies = exp(ShannonH);
}


// Loads the correct map image based on the selected scenario in the UI
void MainWindow::loadScenarioMap()
{
    if (ui->scenarioChooser->currentText() == "No meadows, no flower patches") {
        map.loadScenarioOne();
    } else if (ui->scenarioChooser->currentText() == "No meadows, flower patches") {
        map.loadScenarioTwo();
    } else {
        map.loadScenarioThree();
    }
}


// The entire model run when the start button is pushed
void MainWindow::on_pushButton_Start_clicked()
{

    // Removes all items from lawnMap scene
    scene->clear();

    // Clearing values from all series and reporters
    daisySeries->clear();
    dandelionSeries->clear();
    cloverSeries->clear();
    turfgrassSeries->clear();

    ui->EffectiveSpecies->clear();
    ui->TimeReporter->clear();

    // Getting the image from the map class for the mowing vector creation
    QImage scenarioLayout = map.getImage();

    // Creates mowing vector containing info about pixels based on their color from the loaded scenario layout
    map.createMowingVector(scenarioLayout);

    // Setting up the lawn
    setupLawn(scenarioLayout);

    // For-loop for each timestep
    for (int t = 0; t < runTime + 1; t++) {

        // Setting all flower counts to 0
        int daisyCount = 0;
        int dandelionCount = 0;
        int cloverCount = 0;
        int turfgrassCount = 0;

        // for-loop going over every x and y pixel in the image
        for (int y = 0; y < ySize; y++){
            for (int x = 0; x < xSize; x++) {

                // Weekly Flower agent procedures
                mow(x, y, scenarioLayout, t);
                sprout(x, y, scenarioLayout);
                reproduction(x, y);

                // Counting up each flower per timestep for the series inputs
                switch (lawnPlants[y][x].getPlantType()) {
                case 1:
                    if (lawnPlants[y][x].getPlantHeight() >= minDaisyFlower) {
                        daisyCount++;
                    }
                    break;
                case 2:
                    if (lawnPlants[y][x].getPlantHeight() >= minDandelionFlower) {
                       dandelionCount++;
                    }
                    break;
                case 3:
                    if (lawnPlants[y][x].getPlantHeight() >= minCloverFlower) {
                        cloverCount++;
                    }
                    break;
                case 4:
                    if (lawnPlants[y][x].getPlantHeight() >= minTurfgrassFlower) {
                        turfgrassCount++;
                    }
                    break;
                }
            }
        }

        // Initializing and defining effective species number with 0 as filler before the calculation of the Effective Species Number
        float effectiveSpecies = 0;

        calcShannonH_q1(daisyCount, cloverCount, dandelionCount, turfgrassCount, effectiveSpecies);

        // Effective species number for each timestepm gets pushed back into the vector - min, max, and avg are taken from this vector later on
        effectiveSpeciesVector.push_back(effectiveSpecies);

        // Adding flower counts per timestep to each respective series
        daisySeries->append(t, daisyCount);
        dandelionSeries->append(t, dandelionCount);
        cloverSeries->append(t, cloverCount);
        turfgrassSeries->append(t, turfgrassCount);



        // Repainting the map to correspond to the condition of all plants in given timestep
        paintLawn(scenarioLayout);

        // Reporter for time in weeks
        ui->TimeReporter->append("Week: " + QString::number(t));

        // Reporting message if lawn was mowed in given timestep
        if (t % ui->mowingFreq->value() == 0) {
            ui->TimeReporter->append("<font color='darkgreen'>Lawn was mowed!</font>");
        }

        // Updating the map image in the UI
        scene->addPixmap(QPixmap::fromImage(scenarioLayout));

        // Configuring/adjusting the y-axis on the graph to match that of the highest value of any of the flower counts
        QValueAxis *yAxis = qobject_cast<QValueAxis*>(chart->axisY()); // pulled from AI Qwen 30B Instruct

        if (yAxis->max() < std::max({daisyCount, dandelionCount, cloverCount, turfgrassCount})) {
            chart->axisY()->setRange(0, std::max({daisyCount, dandelionCount, cloverCount, turfgrassCount}));
        }


        // Processing the GUI to be able to see daily changes with short delay between timesteps

        QEventLoop loop; // attained from AI Qwen  30B Instruct
        QTimer::singleShot(500, &loop, &QEventLoop::quit);
        loop.exec();
    }

    // When model run is over, minimum, maximum, and average Effective Species Numbers are calculated and reported
    ui->EffectiveSpecies->append("Min Effective Species Number: " + QString::number(*std::min_element(effectiveSpeciesVector.begin(), effectiveSpeciesVector.end())));
    ui->EffectiveSpecies->append("Max Effective Species Number: " + QString::number(*std::max_element(effectiveSpeciesVector.begin(), effectiveSpeciesVector.end())));

    // Avg Efective Species Number calculation and reporting
    float avgESN = 0;
    for(int i = 1; i < effectiveSpeciesVector.size(); i++) {
        avgESN = avgESN + effectiveSpeciesVector[i];
    }
    avgESN = avgESN/effectiveSpeciesVector.size();

    ui->EffectiveSpecies->append("Avg Effective Species Number: " + QString::number(avgESN));

}


// Updating the map whenever the selection is changed in the UI
void MainWindow::on_scenarioChooser_currentTextChanged(const QString &arg1)
{
    loadScenarioMap();
    QImage scenarioLayout = map.getImage();
    scene->addPixmap(QPixmap::fromImage(scenarioLayout));
}




