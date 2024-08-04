#include "Simulation.h"
#include "DataLoader.h"
#include <iostream> // 04-08-2024 1810

// 04-08-2024 1810
void runSimulationForSKU(SimulationEngine& simulationEngine, const std::string& sku, const std::string& dayForSimulate, int numberOfIterations, int daysToSimulate, double tolerance) {
    std::string basePath = "../data/input/sku_" + sku + "/" + dayForSimulate + "/";
    
    SKUData skuData = loadSKUData(basePath + sku + "_matriz_intervals_df_" + dayForSimulate + ".csv");
    std::map<std::string, double> normalizedFeatures = loadNormalizedFeatures(basePath + sku + "_df_features_sku_norm_" + dayForSimulate + ".txt");
    std::map<std::string, double> noNormalizedFeatures = loadNoNormalizedFeatures(basePath + sku + "_df_features_sku_" + dayForSimulate + ".txt");

    simulationEngine.loadMeanAndStdValues(basePath + sku + "_mean_values_features_sku_" + dayForSimulate + ".csv",
                                          basePath + sku + "_std_values_features_sku_" + dayForSimulate + ".csv");

    simulationEngine.setProductData(skuData);
    simulationEngine.setNormalizedFeatures(normalizedFeatures);
    simulationEngine.setNoNormalizedFeatures(noNormalizedFeatures);
    simulationEngine.runSimulations(numberOfIterations, daysToSimulate, tolerance);
    
    std::cout << "Simulation completed for SKU: " << sku << std::endl;
}