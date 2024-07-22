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

int main(int argc, char* argv[]) {
    // Obtiene el tiempo de inicio
    auto start = std::chrono::high_resolution_clock::now();

    int numberOfIterations = 0, tolerance = 0, daysToSimulate = 0;

    // Cargar configuración de simulación
    loadSimulationConfig("../data/simulation_config.txt", numberOfIterations, tolerance, daysToSimulate);

    // Verificar si los valores fueron correctamente cargados
    if (numberOfIterations == 0 || tolerance == 0 || daysToSimulate == 0) {
        std::cerr << "Failed to load simulation configuration correctly." << std::endl;
        return 1;
    }

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