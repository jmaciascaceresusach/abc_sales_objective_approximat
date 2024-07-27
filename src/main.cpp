#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <string>
#include <limits>
#include <utility>
#include <map>
#include <iomanip>
#include "../include/Parameter.h"
#include "../include/SimulationEngine.h"
#include "../include/DataLoader.h"

int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now();

    int numberOfIterations = 0, tolerance = 0, daysToSimulate = 0;

    std::string currentDate = getCurrentDate();

    loadSimulationConfig("../data/simulation_config_initial.txt", numberOfIterations, tolerance, daysToSimulate);

    if (numberOfIterations == 0 || tolerance == 0 || daysToSimulate == 0) {
        std::cerr << "Failed to load simulation configuration correctly." << std::endl;
        return 1;
    }

    SimulationEngine simulationEngine;

    SKUData skuData = loadSKUData("../data/matriz_intervals_df_Z285320_" + currentDate + ".csv");
    
    std::map<std::string, double> normalizedFeatures = loadNormalizedFeatures("../data/df_features_Z285320_sku_norm_" + currentDate + ".txt");

    std::map<std::string, double> noNormalizedFeatures = loadNoNormalizedFeatures("../data/df_features_Z285320_sku_" + currentDate + ".txt");
    
    simulationEngine.setProductData(skuData);
    simulationEngine.setNormalizedFeatures(normalizedFeatures);
    simulationEngine.setNoNormalizedFeatures(noNormalizedFeatures);
    
    simulationEngine.runSimulations(numberOfIterations, daysToSimulate, tolerance);

    std::cout << "\n";
    std::cout << "*** Configurations ***" << std::endl;
    std::cout << "numberOfIterations: " << numberOfIterations << std::endl;
    std::cout << "tolerance: " << tolerance << std::endl;
    std::cout << "daysToSimulate: " << daysToSimulate << std::endl;
    std::cout << "\n";

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "*** Duration ***" << std::endl;
    std::cout << "Time: " << duration.count() << " milliseconds" << std::endl;
    std::cout << "\n";

    return 0;
}