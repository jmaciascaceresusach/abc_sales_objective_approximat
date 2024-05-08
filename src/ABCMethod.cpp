/* Funcionalidad general 27032024
Implementa la lógica de refinamiento de parámetros en "ABCMethod". La función
"refineParameters" ajusta los parámetros basándose en un factor aleatorio pequeño
y luego selecciona el mejor conjunto de parámetros basándose en la distancia al
objetivo de ventas.

Problema encontrado (por solucionar): 
- El ajuste se realiza solo unas vez al principio,
basado en un factor aleatorio, sin considerar el resultado de las simulaciones posteriores
para realizar ajustes adicionales.
- Para el ABCMethod::refineParameters, ajustar los parámetros de manera más efectiva
en función de los resultados de cada simulación. Esto podría incluir técnicas de optimización
más avanzadas que simplemente ajustar los parámetros basándose en si están por encima o por
debajo del objetivo.
*/

#include "../include/ABCMethod.h"
#include <limits>
#include <random>
#include <cmath>
#include <algorithm>

ABCMethod::ABCMethod() {}

void readConfigSimple(const std::string& configFilePath, int& numberOfIterations) {
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
                }
            }
        }
    }
}

void ABCMethod::refineParameters(std::vector<Parameter>& parameters, 
                                 std::function<double(const std::vector<Parameter>&)> calculateSale, 
                                 double salesObjective, 
                                 double tolerance) {
    std::default_random_engine generator(std::random_device{}());
    std::normal_distribution<double> distribution(0.0, 0.01); // Ajuste pequeño

    for (auto& param : parameters) {
        double adjustment = distribution(generator);
        param.adjustProbability(adjustment);
    }

    normalizeParameters(parameters);

    double closestDistance = std::numeric_limits<double>::max();
    std::vector<Parameter> bestParameters = parameters;

    int numberOfIterations;
    readConfigSimple("simulation_config.txt", numberOfIterations);

    for (int i = 0; i < numberOfIterations; ++i) {
        double saleValue = calculateSale(parameters);
        double distance = calculateDistance(saleValue, salesObjective);

        if (distance < closestDistance) {
            closestDistance = distance;
            bestParameters = parameters;
        }

        if (distance <= tolerance) {
            break;
        }

        dynamicAdjustParametersGenetic(parameters, calculateSale, saleValue, salesObjective);
        normalizeParameters(parameters);
    }

    parameters = bestParameters;
}

// Ajuste dinámicamente los parámetros según los resultados de la simulación.
void ABCMethod::dynamicAdjustParameters(std::vector<Parameter>& parameters, double saleValue, double salesObjective) {
    double errorMargin = calculateDistance(saleValue, salesObjective);
    for (auto& param : parameters) {
        double adjustment = (saleValue < salesObjective) ? 0.01 : -0.01;
        param.adjustProbability(adjustment * errorMargin);
    }
}

/* 
Ajuste Basado en Gradiente:
Utiliza el gradiente de las diferencias entre las 
ventas simuladas y el objetivo para ajustar los parámetros. 
*/ 
void ABCMethod::dynamicAdjustParametersGradient(std::vector<Parameter>& parameters, double saleValue, double salesObjective) {
    double learningRate = 0.01;
    double error = saleValue - salesObjective;

    for (auto& param : parameters) {
        double gradiente = (saleValue < salesObjective) ? learningRate : -learningRate;
        param.adjustProbability(gradiente * std::abs(param.probability));
    }
}

/* 
Método de la Media Deslizante:
Ajusta los parámetros usando una media ponderada de
las iteraciones previas para suavizar las fluctuaciones.
*/
void ABCMethod::dynamicAdjustParametersSlidingAverage(std::vector<Parameter>& parameters, double saleValue, double salesObjective) {
    static std::vector<double> cumulativeProbabilities(parameters.size(), 0.0);
    static std::vector<int> count(parameters.size(), 0);
    double learningRate = 0.01;
    double error = saleValue - salesObjective;

    for (size_t i = 0; i < parameters.size(); ++i) {
        double adjustment = (error > 0 ? -1 : 1) * learningRate * std::abs(parameters[i].probability);
        cumulativeProbabilities[i] += adjustment;
        count[i]++;
        double smoothedProbability = cumulativeProbabilities[i] / count[i];
        parameters[i].probability = std::max(0.0, std::min(smoothedProbability, 1.0));
    }
}

/*
Optimización Basada en Algoritmos Evolutivos (Genéticos)
*/
void ABCMethod::dynamicAdjustParametersGenetic(std::vector<Parameter>& parameters, std::function<double(const std::vector<Parameter>&)> calculateSale, double saleValue, double salesObjective) {
    std::default_random_engine generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(-0.05, 0.05);

    std::vector<Parameter> newGeneration = parameters;
    for (auto& param : newGeneration) {
        double mutation = distribution(generator);
        param.adjustProbability(mutation);
    }

    double newSaleValue = calculateSale(newGeneration);
    double currentDistance = calculateDistance(saleValue, salesObjective);
    double newDistance = calculateDistance(newSaleValue, salesObjective);

    if (newDistance < currentDistance) {
        parameters = newGeneration;
    }

    normalizeParameters(parameters);
}

/*
Optimización por Recocido Simulado:
Usa recocido simulado para escapar de mínimos locales y mejorar la solución.
*/
void ABCMethod::dynamicAdjustParametersSimulatedAnnealing(std::vector<Parameter>& parameters, std::function<double(const std::vector<Parameter>&)> calculateSale, double saleValue, double salesObjective) {
    std::default_random_engine generator(std::random_device{}());
    std::normal_distribution<double> distribution(0.0, 0.05);
    double temperature = 1.0;

    std::vector<Parameter> newParameters = parameters;
    for (auto& param : newParameters) {
        double mutation = distribution(generator);
        param.adjustProbability(mutation);
    }

    double newSaleValue = calculateSale(newParameters);
    double currentDistance = calculateDistance(saleValue, salesObjective);
    double newDistance = calculateDistance(newSaleValue, salesObjective);

    if (newDistance < currentDistance || std::exp((currentDistance - newDistance) / temperature) > distribution(generator)) {
        parameters = newParameters;
    }

    temperature *= 0.9; // Reducción gradual de la temperatura

    normalizeParameters(parameters);
}

/*
Optimización Basada en Cuadrados Mínimos (Levenberg-Marquardt):
Usar el método de Levenberg-Marquardt para minimizar el error cuadrático.
*/
void ABCMethod::dynamicAdjustParametersLM(std::vector<Parameter>& parameters, double saleValue, double salesObjective) {
    double lambda = 0.001; // Parámetro de amortiguación
    double error = saleValue - salesObjective;

    for (auto& param : parameters) {
        double gradient = error * param.probability;
        double adjustment = (gradient / (1 + lambda * gradient)) * 0.01;
        param.adjustProbability(adjustment);
    }

    normalizeParameters(parameters);
}

double ABCMethod::calculateDistance(double saleValue, double salesObjective) {
    return std::abs(saleValue - salesObjective);
}

void ABCMethod::normalizeParameters(std::vector<Parameter>& parameters) {
    double totalProbability = 0.0;
    for (const auto& param : parameters) {
        totalProbability += param.probability;
    }

    if (totalProbability > 1.00000000) {
        for (auto& param : parameters) {
            param.probability /= totalProbability;
        }
    }
}
