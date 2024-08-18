#include "Simulation.h"
#include "DataLoader.h"
#include <iostream> // 04-08-2024 1810
#include <fstream> // 18-08-2024 1240
#include <string> // 18-08-2024 1240

/*
Simulation.h: Presumiblemente contiene las definiciones de la clase SimulationEngine y las funciones relacionadas con la simulación.
DataLoader.h: Presumiblemente contiene las definiciones de las funciones para cargar datos.
iostream: Biblioteca estándar de C++ para operaciones de entrada/salida.
*/

/*
Comentarios generales:
- La función runSimulationForSKU prepara y ejecuta una simulación para un SKU específico y un día dado. Realiza las siguientes acciones:
    - Imprime un mensaje indicando que se está preparando la simulación.
    - Construye la ruta base para los archivos de datos.
    - Carga los datos del SKU, las características normalizadas y no normalizadas desde archivos.
    - Carga los valores medios y las desviaciones estándar de las características desde archivos.
    - Configura el motor de simulación (SimulationEngine) con los datos cargados.
    - Ejecuta la simulación llamando a runSimulations con los parámetros especificados.

Esta función encapsula todo el proceso de preparación y ejecución de una simulación, asegurando que todos los datos necesarios se carguen y configuren 
correctamente antes de iniciar la simulación.
*/

// 04-08-2024 2014
// simulationEngine: Referencia a un objeto de SimulationEngine que se usará para ejecutar la simulación.
// sku: Cadena que representa el SKU (Stock Keeping Unit) del producto a simular.
// dayForSimulate: Cadena que representa el día para el cual se simularán los datos.
// numberOfIterations: Número de iteraciones a realizar en la simulación.
// daysToSimulate: Número de días a simular.
// tolerance: Tolerancia para la simulación.
void runSimulationForSKU(SimulationEngine& simulationEngine, 
                         const std::string& sku, 
                         const std::string& dayForSimulate, 
                         int numberOfIterations, 
                         int daysToSimulate, 
                         double tolerance,
                         std::ofstream& logFileForSKU) {

    // 18-08-2024 1240         
    std::cout << "Preparing simulation for SKU: " << sku << std::endl;
    logFileForSKU << "Preparing simulation for SKU: " << sku << std::endl;

    // basePath: Se construye la ruta base para los archivos de datos basados en el SKU y el día para simular.
    std::string basePath = "../data/input/sku_" + sku + "/" + dayForSimulate + "/";
    
    // skuData: Carga los datos del SKU desde un archivo CSV específico.
    // normalizedFeatures: Carga las características normalizadas desde un archivo de texto.
    // noNormalizedFeatures: Carga las características no normalizadas desde un archivo de texto.

    // 18-08-2024 1240
    std::cout << "Preparing to upload SKU information..." << std::endl;
    logFileForSKU << "Preparing to upload SKU information..." << std::endl;
    SKUData skuData = loadSKUData(basePath + sku + "_matriz_intervals_df_" + dayForSimulate + ".csv", logFileForSKU);

    // 18-08-2024 1240
    std::cout << "Preparing to load standardized features..." << std::endl;
    logFileForSKU << "Preparing to load standardized features..." << std::endl;
    std::map<std::string, double> normalizedFeatures = loadNormalizedFeatures(basePath + sku + "_df_features_sku_norm_" + dayForSimulate + ".txt", logFileForSKU);

    // 18-08-2024 1240
    std::cout << "Preparing to load non-standard features..." << std::endl;
    logFileForSKU << "Preparing to load non-standard features..." << std::endl;
    std::map<std::string, double> noNormalizedFeatures = loadNoNormalizedFeatures(basePath + sku + "_df_features_sku_" + dayForSimulate + ".txt", logFileForSKU);

    // loadMeanAndStdValues: Carga los valores medios y las desviaciones estándar de las características desde archivos CSV. Esto es importante para normalizar y desnormalizar 
    // los datos durante la simulación.
    // 18-08-2024 1240
    std::cout << "Preparing to load the average values ​​and standard deviation of the example case..." << std::endl;
    logFileForSKU << "Preparing to load the average values ​​and standard deviation of the example case..." << std::endl;
    simulationEngine.loadMeanAndStdValues(basePath + sku + "_mean_values_features_sku_" + dayForSimulate + ".csv",
                                          basePath + sku + "_std_values_features_sku_" + dayForSimulate + ".csv", logFileForSKU);

    // setProductData: Establece los datos del producto (SKUData) en el motor de simulación.
    // setNormalizedFeatures: Establece las características normalizadas en el motor de simulación.
    // setNoNormalizedFeatures: Establece las características no normalizadas en el motor de simulación.
    // setDayForSimulate: Establece el día para simular en el motor de simulación.
    // 18-08-2024 1240
    std::cout << "Adding SKU product list upload..." << std::endl;
    logFileForSKU << "Adding SKU product list upload..." << std::endl;
    simulationEngine.setProductData(skuData);

    // 18-08-2024 1240
    std::cout << "Adding the normalized features to the simulation..." << std::endl;
    logFileForSKU << "Adding the normalized features to the simulation..." << std::endl;
    simulationEngine.setNormalizedFeatures(normalizedFeatures);

    // 18-08-2024 1240
    std::cout << "Adding the no-normalized features to the simulation..." << std::endl;
    logFileForSKU << "Adding the no-normalized features to the simulation..." << std::endl;
    simulationEngine.setNoNormalizedFeatures(noNormalizedFeatures);

    // 18-08-2024 1240
    std::cout << "Adding the day of for the simulation..." << std::endl;
    logFileForSKU << "Adding the day of for the simulation..." << std::endl;
    simulationEngine.setDayForSimulate(dayForSimulate);

    // 18-08-2024 1240
    std::cout << "Finalizing simulation preparation for SKU..." << sku << std::endl;
    logFileForSKU << "Finalizing simulation preparation for SKU..." << sku << std::endl;

    // 18-08-2024 1240
    std::string currentDateTimeFinalForSKU = getCurrentDateTime();
    logFileForSKU << "\n*** Finishing date: " << currentDateTimeFinalForSKU <<  " (Buenos Aires -3 UTC) ***" << std::endl;
    logFileForSKU.close();

    // runSimulations: Ejecuta las simulaciones con el número de iteraciones, días a simular y tolerancia especificados.
    simulationEngine.runSimulations(numberOfIterations, daysToSimulate, tolerance);
}