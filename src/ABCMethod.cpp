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
        
        // Solo ajustamos los parámetros dinámicos
        for (auto& param : proposedParameters) {
            if (param.name == "total_num_count_products" || param.name == "total_price_products") {
                std::normal_distribution<> d(param.probability, 0.1);
                param.probability = std::max(0.0, std::min(1.0, d(gen)));
            }
        }

        double initialPrice = skuData.globalMinPrice + dis(gen) * (skuData.globalMaxPrice - skuData.globalMinPrice);
        std::vector<double> simulatedPrices = simulateFuturePrices(skuData, normalizedFeatures, daysToSimulate, initialPrice);
        double distance = calculateDistance(simulatedPrices, skuData, initialPrice, daysToSimulate);

        if (distance < tolerance) {
            acceptedParameters.push_back(proposedParameters);
        }
    }

    if (!acceptedParameters.empty()) {  
        for (size_t i = 0; i < parameters.size(); ++i) {
            if (parameters[i].name == "total_num_count_products" || parameters[i].name == "total_price_products") {
                double sum = 0.0;
                for (const auto& accepted : acceptedParameters) {
                    sum += accepted[i].probability;
                }
                parameters[i].probability = sum / acceptedParameters.size();
            }
        }
        
        normalizeParameters(parameters);
    } else {
        tolerance *= 1.1;
    }
}

std::vector<double> ABCMethod::simulateFuturePrices(const SKUData& skuData, 
                                                    const std::map<std::string, double>& normalizedFeatures,
                                                    int daysToSimulate,
                                                    double initialPrice) {
    std::vector<double> futurePrices;
    futurePrices.push_back(initialPrice);
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 1; i < daysToSimulate; ++i) {
        std::vector<double> probabilities;
        for (const auto& interval : skuData.listProducts) {
            double prob = calculateProbability((interval.first + interval.second) / 2, skuData, i);
            probabilities.push_back(prob);
        }

        std::discrete_distribution<> distribution(probabilities.begin(), probabilities.end());
        int selectedInterval = distribution(gen);

        std::uniform_real_distribution<> intervalDist(skuData.listProducts[selectedInterval].first,
                                                      skuData.listProducts[selectedInterval].second);
        double price = intervalDist(gen);

        futurePrices.push_back(price);
    }

    return futurePrices;
}

double ABCMethod::calculateDistance(const std::vector<double>& simulatedPrices, 
                                    const SKUData& skuData,
                                    double initialPrice,
                                    int daysToSimulate) {
    double distance = 0.0;
    for (int i = 0; i < daysToSimulate; ++i) {
        double expectedProbability = calculateProbability(simulatedPrices[i], skuData, i);
        double actualProbability = 1.0 / skuData.listProducts.size(); // Asumiendo distribución uniforme
        distance += std::abs(expectedProbability - actualProbability);
    }
    return distance / daysToSimulate;
}

void ABCMethod::normalizeParameters(std::vector<Parameter>& parameters) {
    double totalProbability = 0.0;
    for (const auto& param : parameters) {
        if (param.name == "total_num_count_products" || param.name == "total_price_products") {
            totalProbability += param.probability;
        }
    }
    for (auto& param : parameters) {
        if (param.name == "total_num_count_products" || param.name == "total_price_products") {
            param.probability /= totalProbability;
        }
    }
}

double ABCMethod::calculateProbability(double price, const SKUData& skuData, int day) {
    double probability = 0.0;

    // Verificar si el precio está en algún intervalo
    for (size_t i = 0; i < skuData.listProducts.size(); ++i) {
        const auto& interval = skuData.listProducts[i];
        if (price >= interval.first && price <= interval.second) {
            // Probabilidad base
            probability = 1.0 / skuData.listProducts.size();

            // Ajustar por la posición en el intervalo
            double positionInInterval = (price - interval.first) / (interval.second - interval.first);
            probability *= std::exp(-std::pow(positionInInterval - 0.5, 2) / 0.25);

            // Ajustar por el día (decae con el tiempo)
            probability *= std::exp(-0.05 * day);

            // Aquí se podrían agregar más factores:
            // - Tendencia histórica
            // - Estacionalidad
            // - Factores externos
            // - Autocorrelación con precios anteriores
            // - Volatilidad del producto

            break;
        }
    }

    return probability;
}