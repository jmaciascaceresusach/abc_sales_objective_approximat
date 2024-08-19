#include "../include/SimulationEngine.h"
#include "../include/DataLoader.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <limits>
#include <random>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cmath> // 19-08-2024 1245
#include <string> // 18-08-2024 1240

/*
iostream: para operaciones de entrada/salida.
fstream: para operaciones de archivo.
algorithm: para algoritmos estándar como std::min y std::max.
numeric: para funciones numéricas como std::accumulate.
limits: para obtener límites de tipos de datos.
random: para generación de números aleatorios.
chrono: para mediciones de tiempo.
iomanip: para manipulación de la salida formateada.
sstream: para manipulación de cadenas a través de streams.
*/

/*
Comentarios generales:
- El archivo implementa la clase SimulationEngine, que es responsable de realizar simulaciones para predecir precios futuros de productos basados en datos 
históricos y características específicas. La clase incluye métodos para agregar y establecer parámetros, normalizar y desnormalizar datos, cargar datos históricos 
y ejecutar las simulaciones. El método runSimulations es el núcleo de la clase, gestionando todo el proceso de simulación desde la preparación de datos hasta el 
registro de los resultados.
*/

// 05-08-2024 1027
// Inicializa el objeto SimulationEngine con un valor por defecto para numberOfRefinements.
SimulationEngine::SimulationEngine() : numberOfRefinements(10) {} // Valor por defecto

// Añade un parámetro a la lista de parámetros de la simulación.
void SimulationEngine::addParameter(const Parameter& parameter) {
    this->parameters.push_back(parameter);
}

// Establece los datos del producto (SKUData) para la simulación.
void SimulationEngine::setProductData(const SKUData& data) {
    this->skuData = data;
}

// Establece las características normalizadas y las añade a los parámetros de la simulación.
void SimulationEngine::setNormalizedFeatures(const std::map<std::string, double>& features) {
    this->normalizedFeatures = features;
    for (const auto& feature : features) {
        this->parameters.push_back(Parameter(feature.first, feature.second));
    }
}

// Establece las características no normalizadas.
void SimulationEngine::setNoNormalizedFeatures(const std::map<std::string, double>& features) {
    this->noNormalizedFeatures = features;
}

// Carga los valores medios y las desviaciones estándar desde archivos.
void SimulationEngine::loadMeanAndStdValues(const std::string& meanFilename, 
                                            const std::string& stdFilename,
                                            std::ofstream& logFileForSKU) {

    std::cout << "*** loadMeanAndStdValues ***" << std::endl;
    logFileForSKU << "*** loadMeanAndStdValues ***" << std::endl;

    std::cout << "Preparing to load the average values ​​and standard deviation of the example case..." << std::endl;
    logFileForSKU << "Preparing to load the average values ​​and standard deviation of the example case..." << std::endl;

    logFileForSKU << "Loading the average values ​​of the example case..." << std::endl;                                        
    meanValues = loadValues(meanFilename);

    logFileForSKU << "Loading the standard deviation values ​​of the example case..." << std::endl;      
    stdValues = loadValues(stdFilename);
}

// 04-08-2024 1714
// Normaliza y desnormaliza valores utilizando los valores medios y las desviaciones estándar.
double SimulationEngine::normalize(double value, const std::string& feature) {
    return (value - meanValues[feature]) / stdValues[feature];
}

// 04-08-2024 1714
double SimulationEngine::denormalize(double normalizedValue, const std::string& feature) {
    return normalizedValue * stdValues[feature] + meanValues[feature];
}

// 04-08-2024 2039
// Carga datos históricos desde un archivo CSV y muestra un resumen de los datos cargados.
void SimulationEngine::loadHistoricalData(const std::string& filename) {
    historicalData.loadFromCSV(filename);
    std::cout << "Attempting to load historical data from: " << filename << std::endl;
    if (historicalData.records.empty()) {
        std::cout << "Warning: No historical data loaded from " << filename << std::endl;
    } else {
        std::cout << "Loaded " << historicalData.records.size() << " records of historical data" << std::endl;
        std::cout << "Columns found: ";
        if (!historicalData.records.empty()) {
            for (const auto& pair : historicalData.records[0]) {
                std::cout << pair.first << ", ";
            }
        }
        std::cout << std::endl;
        // Imprimir las primeras filas para verificar
        for (int i = 0; i < std::min(5, static_cast<int>(historicalData.records.size())); ++i) {
            std::cout << "Record " << i << ": ";
            for (const auto& pair : historicalData.records[i]) {
                std::cout << pair.first << ": " << pair.second << ", ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << "\n" << std::endl;
    //abcMethod.setHistoricalData(historicalData);
}

// 18-08-2024 1245
void SimulationEngine::LinearRegression::fit(const std::vector<double>& x, const std::vector<double>& y) {
    double n = x.size();
    double sum_x = std::accumulate(x.begin(), x.end(), 0.0);
    double sum_y = std::accumulate(y.begin(), y.end(), 0.0);
    double sum_xy = std::inner_product(x.begin(), x.end(), y.begin(), 0.0);
    double sum_xx = std::inner_product(x.begin(), x.end(), x.begin(), 0.0);

    slope = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
    intercept = (sum_y - slope * sum_x) / n;
}

// 18-08-2024 1245
double SimulationEngine::LinearRegression::predict(double x) const {
    return slope * x + intercept;
}

// 18-08-2024 1245
double SimulationEngine::calculateMSE(const std::vector<double>& predicted, const std::vector<double>& actual) const {
    double sum_squared_error = 0.0;
    for (size_t i = 0; i < predicted.size(); ++i) {
        double error = predicted[i] - actual[i];
        sum_squared_error += error * error;
    }
    return sum_squared_error / predicted.size();
}

// 18-08-2024 1245
void SimulationEngine::compareWithLinearRegression(int daysToSimulate,
                                                   std::ofstream& logFile) {
    std::vector<double> days(daysToSimulate);
    std::iota(days.begin(), days.end(), 1);

    // Preparar datos históricos
    std::vector<double> historicalPrices;
    for (const auto& record : historicalData.records) {
        historicalPrices.push_back(record.at("total_price_products"));
    }

    // Asegurar que tenemos suficientes datos históricos
    if (historicalPrices.size() < daysToSimulate) {
        logFile << "Warning: Not enough historical data for comparison." << std::endl;
        return;
    }

    // Ajustar regresión lineal
    LinearRegression lr;
    lr.fit(days, historicalPrices);

    // Generar predicciones de regresión lineal
    std::vector<double> lr_predictions;
    for (int i = 0; i < daysToSimulate; ++i) {
        lr_predictions.push_back(lr.predict(i + 1));
    }

    // Calcular MSE para ABC y regresión lineal
    double mse_abc = calculateMSE(bestSimulation, historicalPrices);
    double mse_lr = calculateMSE(lr_predictions, historicalPrices);

    logFile << "Comparison Results:" << std::endl;
    logFile << "MSE for ABC method: " << mse_abc << std::endl;
    logFile << "MSE for Linear Regression: " << mse_lr << std::endl;

    // Calcular y mostrar la mejora porcentual
    double improvement = (mse_lr - mse_abc) / mse_lr * 100;
    logFile << "ABC method improvement over Linear Regression: " << improvement << "%" << std::endl;
}

// 04-08-2024 2114
/*
Preparación y Carga de Datos:
- Carga los pesos de los atributos y los intervalos de SKU desde archivos.
- Carga los datos históricos específicos del SKU y la fecha.

Verificación de Datos:
- Verifica que los datos históricos estén disponibles y que contengan la columna total_price_products.

Preparación para la Simulación:
- Abre archivos para registrar los logs y las estadísticas de la simulación.
- Inicializa las variables y los generadores de números aleatorios.

Ejecución del Bucle de Simulación:
- Para cada iteración, refina los parámetros, genera un precio inicial y simula los precios futuros.
- Calcula la distancia entre los precios simulados y los datos históricos.
- Registra los resultados en los archivos de log y estadísticas.

Post-procesamiento de Resultados:
- Calcula los precios promedio a lo largo de todas las simulaciones.
- Registra los parámetros finales, tanto normalizados como no normalizados.
- Cierra los archivos de log y estadísticas.

Resumen Final:
- Muestra un resumen de los resultados de la simulación en la consola.
*/
void SimulationEngine::runSimulations(int numberOfIterations, int daysToSimulate, double tolerance) {
    std::string currentDate = dayForSimulate;  // Asume que dayForSimulate es un miembro de la clase

    std::cout << "Entering runSimulations function" << std::endl; 
    std::cout << "Running simulations for SKU: " << skuData.sku << ", Date: " << currentDate << std::endl;

    attributeWeights = loadAttributeWeights("../data/input/attribute_weights.csv");
    skuIntervals = loadSKUIntervals("../data/input/matriz_intervals_df_prodx5_maxlp20.csv");

    loadHistoricalData("../data/input/sku_" + skuData.sku + "/" + currentDate + "/" + skuData.sku + "_filtered_df_features_sku_" + currentDate + ".csv");

    if (historicalData.records.empty()) {
        std::cout << "Error: No historical data available. Cannot proceed with simulation." << std::endl;
        return;
    }

    // Verificar si 'total_price_products' existe en los datos históricos
    if (!historicalData.records.empty() && historicalData.records[0].count("total_price_products") == 0) {
        std::cout << "Error: 'total_price_products' not found in historical data. Available columns are: ";
        for (const auto& feature : historicalData.features) {
            std::cout << feature << ", ";
        }
        std::cout << std::endl;
        return;
    }

    abcMethod.setHistoricalData(historicalData.records);
    
    std::ofstream logFile("../data/output/sku_" + skuData.sku + "/" + currentDate + "/simulation_log_" + currentDate + ".txt");
    std::ofstream statsFile("../data/output/sku_" + skuData.sku + "/" + currentDate + "/statistics_simulations_" + currentDate + ".txt");

    std::string currentDateTimeInitial = getCurrentDateTime();
    logFile << "*** Starting date: " << currentDateTimeInitial <<  " (Buenos Aires -3 UTC) ***\n" << std::endl;

    logFile << "Starting simulation for SKU " << skuData.sku << " with " << numberOfIterations << " iterations, "
            << daysToSimulate << " days to simulate, and tolerance " << tolerance << std::endl;

    std::cout << "Starting simulation for SKU " << skuData.sku << " with " << numberOfIterations << " iterations, "
            << daysToSimulate << " days to simulate, and tolerance " << tolerance << std::endl;

    statsFile << "Iteration,AverageSaleValue,MinSaleValue,MaxSaleValue,Distance,Tolerance";
    for (const auto& param : parameters) {
        statsFile << "," << param.name;
    }
    statsFile << std::endl;

    std::vector<std::vector<double>> allSimulatedPrices;
    std::vector<double> bestSimulation;
    double bestDistance = std::numeric_limits<double>::max();

    logFile << "\nInitial parameters (normalized features):" << std::endl;
    for (const auto& param : parameters) {
        logFile << "  " << param.name << ": " << param.probability << std::endl;
    }

    logFile << "\nInitial parameters (no normalized features):" << std::endl;
    for (const auto& feature : noNormalizedFeatures) {
        logFile << "  " << feature.first << ": " << feature.second << std::endl;
    }

    int acceptedSimulations = 0;
    int bestNumberSimulation = 0;
    int rejectedSimulations = 0; // 05-08-2024 1021

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(skuData.globalMinPrice, skuData.globalMaxPrice);

    std::cout << "\n*** iterationSimulations ***" << std::endl;
    logFile << "\n*** iterationSimulations ***" << std::endl;

    std::cout << "Starting main simulation loop..." << std::endl;
    logFile << "\n---Starting main simulation loop---" << std::endl;

    for (int i = 0; i < numberOfIterations; ++i) {
        std::cout << "\nStarting main simulation iteration " << i + 1 << " of " << numberOfIterations << std::endl;

        logFile << "\nIteration " << i + 1 << " of " << numberOfIterations << std::endl;

        std::cout << "Refining parameters..." << std::endl;

        // 18-08-2024 1240
        std::ofstream logFileDistanceRefine("../data/output/sku_" + skuData.sku + "/" + currentDate + "/simulation_cal_distance_refine_log_" + currentDate + "_iter_" + std::to_string(i + 1) + ".txt");
        std::string currentDateTimeInitialRefine = getCurrentDateTime();
        logFileDistanceRefine << "*** Starting date: " << currentDateTimeInitialRefine <<  " (Buenos Aires -3 UTC) ***\n" << std::endl;

        // 18-08-2024 1240
        abcMethod.refineParameters(parameters, 
                                   skuData, 
                                   normalizedFeatures, 
                                   daysToSimulate, 
                                   tolerance, 
                                   numberOfRefinements,
                                   currentDate,
                                   numberOfIterations,
                                   logFileDistanceRefine);

        // 18-08-2024 1240
        std::string currentDateTimeFinalRefine = getCurrentDateTime();
        logFileDistanceRefine << "\n*** Finishing date: " << currentDateTimeFinalRefine <<  " (Buenos Aires -3 UTC) ***" << std::endl;
        logFileDistanceRefine.close();

        std::cout << "Generating initial price..." << std::endl;
        double initialPrice = dis(gen);

        std::cout << "Simulating future prices..." << std::endl;
        std::vector<double> simulatedPrices = abcMethod.simulateFuturePrices(skuData, normalizedFeatures, daysToSimulate, initialPrice, logFileDistanceRefine);

        std::cout << "Calculating distance" << std::endl;

        // 18-08-2024 1240
        std::ofstream logFileDistance("../data/output/sku_" + skuData.sku + "/" + currentDate + "/simulation_cal_distance_log_" + currentDate + "_iter_" + std::to_string(i + 1) + ".txt");
        std::string currentDateTimeInitialDistance = getCurrentDateTime();
        logFileDistance << "*** Starting date: " << currentDateTimeInitialDistance <<  " (Buenos Aires -3 UTC) ***" << std::endl;

        double distance = abcMethod.calculateDistance(simulatedPrices, skuData, initialPrice, daysToSimulate, currentDate, numberOfIterations, logFileDistance);

        // 18-08-2024 1240
        std::string currentDateTimeFinalDistance = getCurrentDateTime();
        logFileDistance << "*** Finishing date: " << currentDateTimeFinalDistance <<  " (Buenos Aires -3 UTC) ***" << std::endl;
        logFileDistance.close();

        double saleValue = std::accumulate(simulatedPrices.begin(), simulatedPrices.end(), 0.0);

        double averageSaleValue = saleValue / daysToSimulate;
        double minSaleValue = *std::min_element(simulatedPrices.begin(), simulatedPrices.end());
        double maxSaleValue = *std::max_element(simulatedPrices.begin(), simulatedPrices.end());

        logFile << "  Distance: " << distance << std::endl;

        statsFile << i + 1 << "," << averageSaleValue << "," << minSaleValue << "," << maxSaleValue 
                  << "," << distance << "," << tolerance;
        for (const auto& param : parameters) {
            statsFile << "," << param.probability;
        }
        statsFile << std::endl;

        if (distance < bestDistance) {
            bestDistance = distance;
            bestSimulation = simulatedPrices;
            bestNumberSimulation++;            
            logFile << "  -> New best simulation found!! (info: the distance value has decreased)" << std::endl;
        }

        allSimulatedPrices.push_back(simulatedPrices);

        logFile << "  Simulation summary:" << std::endl;
        logFile << "    Initial price: " << initialPrice << std::endl;
        logFile << "    Average price: " << averageSaleValue << std::endl;
        logFile << "    Min price: " << minSaleValue << std::endl;
        logFile << "    Max price: " << maxSaleValue << std::endl;

        std::cout << "Results:\n";
        std::cout << "Iteration " << i + 1 << ": ";
        std::cout << "InitialPrice=" << initialPrice << ", ";
        std::cout << "AverageSaleValue=" << averageSaleValue << ", ";
        std::cout << "MinSaleValue=" << minSaleValue << ", ";
        std::cout << "MaxSaleValue=" << maxSaleValue << ", ";
        std::cout << "Distance=" << distance << std::endl;
        std::cout << "\n";

        // 05-08-2024 1410
        if (distance <= tolerance) {
            acceptedSimulations++;
            logFile << "  Satisfactory simulation found." << std::endl;
            logFile << "  Simulation accepted: " << acceptedSimulations << std::endl;
        } else {
            rejectedSimulations++;
            logFile << "  Simulation rejected." << rejectedSimulations << std::endl;
        }

        std::cout << "Completed main simulation iteration " << i + 1 << " of " << numberOfIterations << std::endl;
    }

    std::cout << "Exiting runSimulations function" << std::endl;
    logFile << "\n--- Finishing main simulation loop ---" << std::endl;

    logFile << "\nFinal Results:" << std::endl;
    logFile << "Best simulation distance: " << bestDistance << std::endl;
    logFile << "Best number simulation (#): " << bestNumberSimulation << std::endl;
    logFile << "Number of accepted simulations: " << acceptedSimulations << std::endl;
    
    // 18-08-2024 1920
    std::ofstream logFileBestSimulations("../data/output/sku_" + skuData.sku + "/" + currentDate + "/statistics_simulations_best_days_" + currentDate + ".txt");

    if (!bestSimulation.empty()) {
        logFile << "Best simulation prices:" << std::endl;
        logFileBestSimulations << "Day,Price" << std::endl;

        for (size_t i = 0; i < bestSimulation.size(); ++i) {
            logFile << "  Day " << i + 1 << ": " << bestSimulation[i] << std::endl;
            logFileBestSimulations << i + 1 << "," << bestSimulation[i] << std::endl;
        }
    } else {
        logFile << "No satisfactory simulation found." << std::endl;
    }

    // 18-08-2024 1920
    logFileBestSimulations.close();

    logFile << "Total accepted simulations: " << acceptedSimulations << std::endl;
    logFile << "Total rejected simulations: " << rejectedSimulations << std::endl;

    std::vector<double> averagePrices(daysToSimulate, 0.0);
    for (const auto& simulation : allSimulatedPrices) {
        for (size_t i = 0; i < simulation.size(); ++i) {
            averagePrices[i] += simulation[i];
        }
    }
    for (auto& price : averagePrices) {
        price /= allSimulatedPrices.size();
    }

    // 18-08-2024 1920
    std::ofstream logFileAveragePrices("../data/output/sku_" + skuData.sku + "/" + currentDate + "/statistics_simulations_average_prices_days_" + currentDate + ".txt");

    logFile << "\nAverage prices across all simulations:" << std::endl;
    logFileAveragePrices << "Day,Price" << std::endl;
    for (size_t i = 0; i < averagePrices.size(); ++i) {
        logFile << "  Day " << i + 1 << ": " << averagePrices[i] << std::endl;
        logFileAveragePrices << i + 1 << "," << averagePrices[i] << std::endl;
    }

    // 18-08-2024 1920
    logFileAveragePrices.close();

    logFile << "\nFinal parameters (normalized features):" << std::endl;
    for (const auto& param : parameters) {
        logFile << "  " << param.name << ": " << param.probability << std::endl;
    }

    // Calcular y mostrar los parámetros no normalizados
    std::map<std::string, double> normalizedParams;
    for (const auto& param : parameters) {
        normalizedParams[param.name] = param.probability;
    }
    std::map<std::string, double> originalParams = inverse_z_score(normalizedParams, meanValues, stdValues);

    logFile << "\nFinal parameters (no normalized features):" << std::endl;
    for (const auto& param : originalParams) {
        logFile << "  " << param.first << ": " << param.second << std::endl;
    }

    // Después de todas las simulaciones
    // 19-08-2024 1245
    compareWithLinearRegression(daysToSimulate, logFile);

    std::string currentDateTimeFinal = getCurrentDateTime();
    logFile << "\n*** Finishing date: " << currentDateTimeFinal <<  " (Buenos Aires -3 UTC) ***" << std::endl;

    logFile.close();
    statsFile.close();

    std::cout << "\nSimulation completed for SKU: " << skuData.sku << ". Results saved in simulation_log_" + currentDate + ".txt and statistics_simulations_" + currentDate + ".txt" << std::endl;
    std::cout << "Number of accepted simulations: " << acceptedSimulations << std::endl;
}