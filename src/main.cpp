#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <string>
#include <limits>
#include <utility>
#include <map>
#include "../include/Parameter.h"
#include "../include/SimulationEngine.h"
#include "../include/DataLoader.h"

void readConfigFor3(const std::string& configFilePath, 
                    int& numberOfIterations,
                    double& tolerance,
                    int& daysToSimulate) {
    std::ifstream configFile(configFilePath);
    std::string line;

    if (configFile.is_open()) {
        while (getline(configFile, line)) {
            std::istringstream iss(line);
            std::string key;
            if (getline(iss, key, '=')) {
                std::string value;
                if (getline(iss, value)) {
                    if (key == "numberOfIterations") numberOfIterations = std::stoi(value);
                    else if (key == "tolerance") tolerance = std::stod(value);
                    else if (key == "daysToSimulate") daysToSimulate = std::stod(value);
                }
            }
        }
    } else {
        std::cerr << "Failed to open config file: " << configFilePath << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // Obtiene el tiempo de inicio
    auto start = std::chrono::high_resolution_clock::now();

    int numberOfIterations, daysToSimulate;
    double tolerance;

    readConfigFor3("../data/simulation_config.txt", 
                   numberOfIterations, 
                   tolerance,
                   daysToSimulate);

    // Inicializar el motor de simulación
    SimulationEngine simulationEngine;

    // Cargar datos del SKU
    SKUData skuData = loadSKUData("../data/matriz_intervals_df_Z285320.csv");
    
    // Cargar características normalizadas
    std::vector<double> normalizedFeatures = loadNormalizedFeatures("../data/df_features_Z285320_sku_norm.txt");
    
    simulationEngine.setProductData(skuData);
    simulationEngine.setNormalizedFeatures(normalizedFeatures);
    
    // Ejecutar simulaciones
    simulationEngine.runSimulations(numberOfIterations, daysToSimulate, tolerance);

    std::cout << "\n";
    std::cout << "***Configurations***" << std::endl;
    std::cout << "numberOfIterations: " << numberOfIterations << std::endl;
    std::cout << "tolerance: " << tolerance << std::endl;
    std::cout << "daysToSimulate: " << daysToSimulate << std::endl;
    std::cout << "\n";

    // Obtiene el tiempo de finalización
    auto end = std::chrono::high_resolution_clock::now();

    // Calcula la duración en milisegundos
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "***Duration***" << std::endl;
    std::cout << "Time: " << duration.count() << " milliseconds" << std::endl;
    std::cout << "\n";

    return 0;
}