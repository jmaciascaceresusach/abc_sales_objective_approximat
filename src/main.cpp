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
fstream: para la manipulación de archivos.
iostream: para operaciones de entrada/salida.
sstream: para manipulación de cadenas a través de streams.
vector: para utilizar el contenedor de vectores.
chrono: para mediciones de tiempo.
string: para manipulación de cadenas.
limits: para obtener propiedades de los tipos aritméticos.
utility: para utilizar pares de datos.
map: para utilizar el contenedor de mapas.
iomanip: para la manipulación de la salida formateada.
*/

/*
Comentarios generales: 
- Este código configura y ejecuta una simulación basada en parámetros cargados desde un archivo de configuración. 
Se maneja la simulación de múltiples SKUs si se especifica "All", se mide el tiempo total de ejecución y se imprime tanto la configuración 
de la simulación como la duración y la fecha de finalización.
*/

// La función main comienza capturando el tiempo de inicio usando la biblioteca chrono para medir la duración total de la ejecución del programa.
int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now();

    // 05-08-2024 1008
    // Se definen varias variables para la configuración de la simulación, todas inicializadas a 0 o cadenas vacías.
    int numberOfIterations = 0, numberOfRefinements = 0, tolerance = 0, daysToSimulate = 0;
    std::string dayForSimulate, skuForSimulate;

    // 05-08-2024 1009
    // Se llama a la función loadSimulationConfig para cargar la configuración de la simulación desde un archivo.
    loadSimulationConfig("../data/input/simulation_config_initial.txt", numberOfIterations, numberOfRefinements, tolerance, daysToSimulate, dayForSimulate, skuForSimulate);
    std::cout << "\nLoading simulation configuration values ​​in progress..." << std::endl;

    // Se verifica si la configuración se cargó correctamente. Si alguna de las variables críticas es 0 o está vacía, el programa imprime un mensaje de error y termina.
    if (numberOfIterations == 0 || tolerance == 0 || daysToSimulate == 0 || dayForSimulate.empty() || skuForSimulate.empty()) {
        std::cerr << "Failed to load simulation configuration correctly." << std::endl;
        return 1;
    }
    std::cout << "Successful loading of simulation configuration values..." << std::endl;

    // 05-08-2024 1009
    // Se crea una instancia de SimulationEngine y se configura el número de refinamientos.
    SimulationEngine simulationEngine;
    simulationEngine.numberOfRefinements = numberOfRefinements;

    // 18-08-2024 1240
    std::ofstream logFileForSKU("../data/output/sku_" + skuForSimulate + "/" + dayForSimulate + "/simulation_sku_" + skuForSimulate + "_initial_" + dayForSimulate + ".txt");
    std::string currentDateTimeInitialForSKU = getCurrentDateTime();
    logFileForSKU << "*** Starting date: " << currentDateTimeInitialForSKU <<  " (Buenos Aires -3 UTC) ***\n" << std::endl;

    // 18-08-2024 1240
    std::string currentDateTimeFinalForSKU = getCurrentDateTime();

    // 04-08-2024 1714
    // Si skuForSimulate es "All", se obtiene una lista de todos los SKUs y se ejecuta la simulación para cada uno. De lo contrario, se ejecuta la simulación solo para el SKU especificado.
    if (skuForSimulate == "All") {
        std::cout << "Start loading SKU by value All..." << std::endl;
        // std::vector<std::string> allSKUs = getAllSKUs();  // Nueva función para obtener todos los SKUs
        std::vector<std::string> allSKUs = {"H401391", "P443799", "U391805", "Z285320", "Z449121"};
        std::cout << "Getting SKU names..." << std::endl;

        for (const auto& sku : allSKUs) {
            std::cout << "Running simulation for SKU (All): " << sku << std::endl;
            logFileForSKU << "Running simulation for SKU (All): " << sku << std::endl;

            logFileForSKU << "*** Starting simulation for SKU (All): " << sku << ", "<< currentDateTimeInitialForSKU <<  " (Buenos Aires -3 UTC) ***\n" << std::endl;
            runSimulationForSKU(simulationEngine, sku, dayForSimulate, numberOfIterations, daysToSimulate, tolerance, logFileForSKU);
        }
    } else {
        std::cout << "Running simulation for SKU (Only 1)..." << std::endl;
        logFileForSKU << "Running simulation for SKU (Only 1)..." << std::endl;

        logFileForSKU << "*** Starting simulation for SKU (Only 1): " << skuForSimulate << ", "<< currentDateTimeInitialForSKU <<  " (Buenos Aires -3 UTC) ***\n" << std::endl;
        runSimulationForSKU(simulationEngine, skuForSimulate, dayForSimulate, numberOfIterations, daysToSimulate, tolerance, logFileForSKU);
    }

    // 04-08-2024 1714
    // Imprime la configuración de la simulación.
    std::cout << "\n";
    std::cout << "*** Configurations ***" << std::endl;
    std::cout << "numberOfIterations: " << numberOfIterations << std::endl;
    std::cout << "numberOfRefinements: " << numberOfRefinements << std::endl;
    std::cout << "tolerance: " << tolerance << std::endl;
    std::cout << "daysToSimulate: " << daysToSimulate << std::endl;
    std::cout << "dayForSimulate: " << dayForSimulate << std::endl;
    std::cout << "skuForSimulate: " << skuForSimulate << std::endl;
    std::cout << "\n";

    // Calcula y imprime la duración total de la ejecución de la simulación.
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "*** Duration ***" << std::endl;
    std::cout << "Time: " << duration.count() << " milliseconds" << std::endl;
    std::cout << "\n";

    // Obtiene y imprime la fecha y hora de finalización de la simulación antes de terminar el programa.
    std::string currentDateTime = getCurrentDateTime();
    std::cout << "*** Finalization date: " << currentDateTime <<  " (Buenos Aires -3 UTC) ***" << std::endl;

    return 0;
}