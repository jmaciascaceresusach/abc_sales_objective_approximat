#include "../include/ABCMethod.h"
#include <random>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

ABCMethod::ABCMethod() {}

void ABCMethod::initializeParameters(std::vector<Parameter>& parameters) {
    for (auto& param : parameters) {
        param.adjustProbability(0.5);
    }
}

void ABCMethod::refineParameters(std::vector<Parameter>& parameters, 
                                 const SKUData& skuData,
                                 const std::map<std::string, double>& normalizedFeatures,
                                 int daysToSimulate,
                                 double tolerance) {
    std::vector<std::vector<Parameter>> acceptedParameters;
    int numberOfSimulations = 1000;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < numberOfSimulations; ++i) {
        std::vector<Parameter> proposedParameters = parameters;
        
        for (auto& param : proposedParameters) {
            std::normal_distribution<> d(param.probability, 0.1);
            param.probability = std::max(0.0, std::min(1.0, d(gen)));
        }

        std::vector<double> simulatedPrices = simulateFuturePrices(skuData, normalizedFeatures, daysToSimulate);
        double distance = calculateDistance(simulatedPrices, skuData);

        if (distance < tolerance) {
            acceptedParameters.push_back(proposedParameters);
        }
    }

    if (!acceptedParameters.empty()) {  
        for (size_t i = 0; i < parameters.size(); ++i) {
            double sum = 0.0;
            for (const auto& accepted : acceptedParameters) {
                sum += accepted[i].probability;
            }
            parameters[i].probability = sum / acceptedParameters.size();
        }
        
        normalizeParameters(parameters);
    } else {
        tolerance *= 1.1;
    }

    std::cout << "\n*** Parameters initial ***" << std::endl;

    for (const auto& param : parameters) {
        std::cout << "  " << param.name << ": " << param.probability << std::endl;
    }
    std::cout << "Number of accepted simulations: " << acceptedParameters.size() << std::endl;
    std::cout << "Final tolerance: " << tolerance << std::endl;
}

std::vector<double> ABCMethod::simulateFuturePrices(const SKUData& skuData, 
                                                    const std::map<std::string, double>& normalizedFeatures,
                                                    int daysToSimulate) {
    std::vector<double> futurePrices;
    std::random_device rd;
    std::mt19937 gen(rd());

    // Inicializar con un tramo aleatorio
    std::uniform_int_distribution<> initialDist(0, skuData.listProducts.size() - 1);
    int currentInterval = initialDist(gen);

    for (int i = 0; i < daysToSimulate; ++i) {
        // Calcular probabilidades de transición (puedes ajustar esto según tus necesidades)
        std::vector<double> transitionProbs(skuData.listProducts.size(), 1.0 / skuData.listProducts.size());

        // Elegir el siguiente intervalo
        std::discrete_distribution<> transition(transitionProbs.begin(), transitionProbs.end());
        currentInterval = transition(gen);

        // Elegir un precio dentro del intervalo
        std::uniform_real_distribution<> intervalDist(skuData.listProducts[currentInterval].first,
                                                      skuData.listProducts[currentInterval].second);
        double price = intervalDist(gen);

        futurePrices.push_back(price);
    }

    return futurePrices;
}

double ABCMethod::calculateDistance(const std::vector<double>& simulatedPrices, const SKUData& skuData) {
    double totalDistance = 0.0;
    int outOfRangeCount = 0;

    std::cout << "Calculating distance for " << simulatedPrices.size() << " prices" << std::endl;

    for (const auto& price : simulatedPrices) {
        // Comprobar si el precio está dentro del rango global
        if (price < skuData.globalMinPrice || price > skuData.globalMaxPrice) {
            outOfRangeCount++;
            continue;
        }

        // Encontrar el intervalo más cercano
        double minDistance = std::numeric_limits<double>::max();
        for (const auto& interval : skuData.listProducts) {
            if (price >= interval.first && price <= interval.second) {
                minDistance = 0;
                break;
            }
            double distToMin = std::abs(price - interval.first);
            double distToMax = std::abs(price - interval.second);
            double intervalDistance = std::min(distToMin, distToMax);
            minDistance = std::min(minDistance, intervalDistance);
        }
        totalDistance += minDistance;
    }

    // Penalizar fuertemente los precios fuera del rango global
    totalDistance += outOfRangeCount * (skuData.globalMaxPrice - skuData.globalMinPrice);

    // Normalizar la distancia
    double normalizedDistance = totalDistance / simulatedPrices.size();

    std::cout << "Total distance: " << totalDistance << ", Out of range count: " << outOfRangeCount << std::endl;
    std::cout << "Normalized distance: " << normalizedDistance << std::endl;

    return normalizedDistance;
}

void ABCMethod::normalizeParameters(std::vector<Parameter>& parameters) {
    double totalProbability = 0.0;
    for (const auto& param : parameters) {
        totalProbability += param.probability;
    }
    for (auto& param : parameters) {
        param.probability /= totalProbability;
    }
}