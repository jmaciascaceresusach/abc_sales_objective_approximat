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
#include "../include/Simulation.h"
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

    //std::string currentDate = getCurrentDate();
    int numberOfIterations = 0, tolerance = 0, daysToSimulate = 0;
    
    // 04-08-2024 1714
    std::string dayForSimulate, skuForSimulate;

    // 04-08-2024 1714
    loadSimulationConfig("../data/input/simulation_config_initial.txt", numberOfIterations, tolerance, daysToSimulate, dayForSimulate, skuForSimulate);

    if (numberOfIterations == 0 || tolerance == 0 || daysToSimulate == 0 || dayForSimulate.empty() || skuForSimulate.empty()) {
        std::cerr << "Failed to load simulation configuration correctly." << std::endl;
        return 1;
    }

    SimulationEngine simulationEngine;

    // 04-08-2024 1714
    if (skuForSimulate == "All") {
        std::vector<std::string> allSKUs = getAllSKUs();  // Nueva función para obtener todos los SKUs
        for (const auto& sku : allSKUs) {
            std::cout << "Running simulation for SKU: " << sku << std::endl;
            void runSimulationForSKU(SimulationEngine& simulationEngine, const std::string& sku, const std::string& dayForSimulate, int numberOfIterations, int daysToSimulate, double tolerance);
        }
    } else {
        void runSimulationForSKU(SimulationEngine& simulationEngine, const std::string& sku, const std::string& dayForSimulate, int numberOfIterations, int daysToSimulate, double tolerance);
    }

    // 04-08-2024 1714
    std::cout << "\n";
    std::cout << "*** Configurations ***" << std::endl;
    std::cout << "numberOfIterations: " << numberOfIterations << std::endl;
    std::cout << "tolerance: " << tolerance << std::endl;
    std::cout << "daysToSimulate: " << daysToSimulate << std::endl;
    std::cout << "dayForSimulate: " << dayForSimulate << std::endl;
    std::cout << "skuForSimulate: " << skuForSimulate << std::endl;
    std::cout << "\n";

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "*** Duration ***" << std::endl;
    std::cout << "Time: " << duration.count() << " milliseconds" << std::endl;
    std::cout << "\n";

    return 0;
}