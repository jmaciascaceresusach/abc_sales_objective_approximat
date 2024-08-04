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

/*
Comentarios generales:
- Este archivo implementa la clase SimulationEngine.

Comentario específicos:
- Implementación de métodos como addParameter, setProductData, setNormalizedFeatures, setNoNormalizedFeatures, loadMeanAndStdValues, runSimulations.
*/

SimulationEngine::SimulationEngine() {}

void SimulationEngine::addParameter(const Parameter& parameter) {
    this->parameters.push_back(parameter);
}

void SimulationEngine::setProductData(const SKUData& data) {
    this->skuData = data;
}

void SimulationEngine::setNormalizedFeatures(const std::map<std::string, double>& features) {
    this->normalizedFeatures = features;
    for (const auto& feature : features) {
        this->parameters.push_back(Parameter(feature.first, feature.second));
    }
}

void SimulationEngine::setNoNormalizedFeatures(const std::map<std::string, double>& features) {
    this->noNormalizedFeatures = features;
}

void SimulationEngine::loadMeanAndStdValues(const std::string& meanFilename, const std::string& stdFilename) {
    meanValues = loadValues(meanFilename);
    stdValues = loadValues(stdFilename);
}

// 04-08-2024 1714
double SimulationEngine::normalize(double value, const std::string& feature) {
    return (value - meanValues[feature]) / stdValues[feature];
}

// 04-08-2024 1714
double SimulationEngine::denormalize(double normalizedValue, const std::string& feature) {
    return normalizedValue * stdValues[feature] + meanValues[feature];
}

// 04-08-2024 1714
void SimulationEngine::loadHistoricalData(const std::string& filename) {
    historicalData.loadFromCSV(filename);
}

// 04-08-2024 1714
void SimulationEngine::runSimulations(int numberOfIterations, int daysToSimulate, double tolerance) {

    //std::string currentDate = getCurrentDate();
    std::string currentDate = "2024-08-03";

    attributeWeights = loadAttributeWeights("../data/input/attribute_weights.csv");
    skuIntervals = loadSKUIntervals("../data/input/matriz_intervals_df_prodx5_maxlp20.csv");
    loadHistoricalData("../data/input/sku_" + skuData.sku + "/" + getCurrentDate() + "/" + skuData.sku + "_filtered_df_features_sku_" + getCurrentDate() + ".csv");

    // Pasar los datos históricos al método ABC
    abcMethod.setHistoricalData(historicalData.records);
    
    std::ofstream logFile("../data/output/sku_Z285320/" + currentDate + "/simulation_log_" + currentDate + ".txt");
    std::ofstream statsFile("../data/output/sku_Z285320/" + currentDate + "/statistics_simulations_" + currentDate + ".txt");

    logFile << "Starting simulation with " << numberOfIterations << " iterations, "
            << daysToSimulate << " days to simulate, and tolerance " << tolerance << std::endl;

    std::cout << "Running simulations with " << numberOfIterations << " iterations, "
            << daysToSimulate << " days to simulate, and tolerance " << tolerance << std::endl;

    statsFile << "Iteration,AverageSaleValue,MinSaleValue,MaxSaleValue,Distance,Tolerance";
    for (const auto& param : parameters) {
        statsFile << "," << param.name;
    }
    statsFile << std::endl;

    std::vector<std::vector<double>> allSimulatedPrices;
    std::vector<double> bestSimulation;
    double bestDistance = std::numeric_limits<double>::max();

    logFile << "\nInitial parameters:" << std::endl;
    for (const auto& param : parameters) {
        logFile << "  " << param.name << ": " << param.probability << std::endl;
    }

    int acceptedSimulations = 0;
    int bestNumberSimulation = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(skuData.globalMinPrice, skuData.globalMaxPrice);

    std::cout << "*** iterationSimulations ***" << std::endl;

    for (int i = 0; i < numberOfIterations; ++i) {
        logFile << "\nIteration " << i + 1 << " of " << numberOfIterations << std::endl;

        abcMethod.refineParameters(parameters, skuData, normalizedFeatures, daysToSimulate, tolerance);

        double initialPrice = dis(gen);
        std::vector<double> simulatedPrices = abcMethod.simulateFuturePrices(skuData, normalizedFeatures, daysToSimulate, initialPrice);

        double distance = abcMethod.calculateDistance(simulatedPrices, skuData, initialPrice, daysToSimulate);
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

        std::cout << "Iteration " << i + 1 << ": ";
        std::cout << "InitialPrice=" << initialPrice << ", ";
        std::cout << "AverageSaleValue=" << averageSaleValue << ", ";
        std::cout << "MinSaleValue=" << minSaleValue << ", ";
        std::cout << "MaxSaleValue=" << maxSaleValue << ", ";
        std::cout << "Distance=" << distance << std::endl;

        if (distance <= tolerance) {
            acceptedSimulations++;
            logFile << "  Satisfactory simulation found." << std::endl;
        }
    }

    logFile << "\nFinal Results:" << std::endl;
    logFile << "Best simulation distance: " << bestDistance << std::endl;
    logFile << "Best number simulation (#): " << bestNumberSimulation << std::endl;
    logFile << "Number of accepted simulations: " << acceptedSimulations << std::endl;
    
    if (!bestSimulation.empty()) {
        logFile << "Best simulation prices:" << std::endl;
        for (size_t i = 0; i < bestSimulation.size(); ++i) {
            logFile << "  Day " << i + 1 << ": " << bestSimulation[i] << std::endl;
        }
    } else {
        logFile << "No satisfactory simulation found." << std::endl;
    }

    std::vector<double> averagePrices(daysToSimulate, 0.0);
    for (const auto& simulation : allSimulatedPrices) {
        for (size_t i = 0; i < simulation.size(); ++i) {
            averagePrices[i] += simulation[i];
        }
    }
    for (auto& price : averagePrices) {
        price /= allSimulatedPrices.size();
    }

    logFile << "\nAverage prices across all simulations:" << std::endl;
    for (size_t i = 0; i < averagePrices.size(); ++i) {
        logFile << "  Day " << i + 1 << ": " << averagePrices[i] << std::endl;
    }

    logFile << "\nFinal parameters:" << std::endl;
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

    logFile.close();
    statsFile.close();

    std::cout << "\nSimulation completed. Results saved in simulation_log_" + currentDate + ".txt and statistics_simulations_" + currentDate + ".txt" << std::endl;
    std::cout << "Number of accepted simulations: " << acceptedSimulations << std::endl;
}