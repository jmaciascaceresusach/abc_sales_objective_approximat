#include "../include/SimulationEngine.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>

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

void SimulationEngine::runSimulations(int numberOfIterations, int daysToSimulate, double tolerance) {
    std::ofstream logFile("../data/output/simulation_log.txt");
    std::ofstream statsFile("../data/output/statistics_simulations.txt");

    if (!logFile.is_open() || !statsFile.is_open()) {
        std::cerr << "Error: Could not open output files for writing" << std::endl;
        return;
    }

    logFile << "Starting simulation with " << numberOfIterations << " iterations, "
            << daysToSimulate << " days to simulate, and tolerance " << tolerance << std::endl;
    logFile.flush();

    statsFile << "Iteration,AverageSaleValue,MinSaleValue,MaxSaleValue,Distance,Tolerance";
    for (const auto& param : parameters) {
        statsFile << "," << param.name;
    }
    statsFile << std::endl;

    std::vector<std::vector<double>> allSimulatedPrices;
    std::vector<double> bestSimulation;
    double bestDistance = std::numeric_limits<double>::max();

    for (int i = 0; i < numberOfIterations; ++i) {
        logFile << "Iteration " << i + 1 << " of " << numberOfIterations << std::endl;

        abcMethod.refineParameters(parameters, skuData, normalizedFeatures, daysToSimulate, tolerance);

        std::vector<double> simulatedPrices = abcMethod.simulateFuturePrices(skuData, normalizedFeatures, daysToSimulate);

        double distance = abcMethod.calculateDistance(simulatedPrices, skuData);
        double saleValue = std::accumulate(simulatedPrices.begin(), simulatedPrices.end(), 0.0);

        logFile << "  Distance: " << distance << std::endl;

        double averageSaleValue = saleValue / daysToSimulate;
        double minSaleValue = *std::min_element(simulatedPrices.begin(), simulatedPrices.end());
        double maxSaleValue = *std::max_element(simulatedPrices.begin(), simulatedPrices.end());

        statsFile << i + 1 << "," << averageSaleValue << "," << minSaleValue << "," << maxSaleValue 
                  << "," << distance << "," << tolerance;
        for (const auto& param : parameters) {
            statsFile << "," << param.probability;
        }
        statsFile << std::endl;

        if (distance < bestDistance) {
            bestDistance = distance;
            bestSimulation = simulatedPrices;
            logFile << "  New best simulation found" << std::endl;
            std::cout << "  New best simulation found" << std::endl;
        }

        allSimulatedPrices.push_back(simulatedPrices);

        logFile << "  Current parameters:" << std::endl;
        for (const auto& param : parameters) {
            logFile << "    " << param.name << ": " << param.probability << std::endl;
        }

        logFile << "  Simulation summary:" << std::endl;
        logFile << "    Average price: " << averageSaleValue << std::endl;
        logFile << "    Min price: " << minSaleValue << std::endl;
        logFile << "    Max price: " << maxSaleValue << std::endl;

        if (distance <= tolerance) {
            logFile << "Satisfactory simulation found. Stopping early." << std::endl;
            break;
        }
    }

    logFile << "\nFinal Results:" << std::endl;
    logFile << "Best simulation distance: " << bestDistance << std::endl;
    logFile << "Best simulation prices:" << std::endl;
    for (size_t i = 0; i < bestSimulation.size(); ++i) {
        logFile << "  Day " << i + 1 << ": " << bestSimulation[i] << std::endl;
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

    logFile.close();
    statsFile.close();

    std::cout << "Simulation completed. Results saved in simulation_log.txt and statistics_simulations.txt" << std::endl;
}