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

/*
Comentarios generales:
- Este archivo contiene la función principal del programa.

Comentario específicos:
- Manejo de la configuración de la simulación y ejecución de SimulationEngine.
- Generación de logs y estadísticas de la simulación.
*/

int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now();

    int numberOfIterations = 0, tolerance = 0, daysToSimulate = 0;

    //std::string currentDate = getCurrentDate();
    std::string currentDate = "2024-07-28";

    loadSimulationConfig("../data/simulation_config_initial.txt", numberOfIterations, tolerance, daysToSimulate);

    if (numberOfIterations == 0 || tolerance == 0 || daysToSimulate == 0) {
        std::cerr << "Failed to load simulation configuration correctly." << std::endl;
        return 1;
    }

    SimulationEngine simulationEngine;

    SKUData skuData = loadSKUData("../data/Z285320/Z285320_matriz_intervals_df_" + currentDate + ".csv");
    
    std::map<std::string, double> normalizedFeatures = loadNormalizedFeatures("../data/Z285320/Z285320_df_features_sku_norm_" + currentDate + ".txt");

    std::map<std::string, double> noNormalizedFeatures = loadNoNormalizedFeatures("../data/Z285320/Z285320_df_features_sku_" + currentDate + ".txt");

    simulationEngine.loadMeanAndStdValues("../data/Z285320/Z285320_mean_values_features_sku_2024-07-28.csv",
                                          "../data/Z285320/Z285320_std_values_features_sku_2024-07-28.csv");
    
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