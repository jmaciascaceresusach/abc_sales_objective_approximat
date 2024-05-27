#include "../include/ABCMethod.h"
#include <limits>
#include <random>
#include <cmath>
#include <algorithm>
#include <iostream>

/**
 * Constructor por defecto.
 */
ABCMethod::ABCMethod() {}

/**
 * Lee una configuración simple desde un archivo.
 * @param configFilePath Ruta al archivo de configuración.
 * @param numberOfIterations Variable donde se almacenará el número de iteraciones.
 */
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

/**
 * Refina parámetros usando el método ABC.
 * @param parameters Lista de parámetros a refinar.
 * @param calculateSale Función para calcular las ventas.
 * @param salesObjective Objetivo de ventas a alcanzar.
 * @param tolerance Tolerancia aceptable entre las ventas calculadas y el objetivo.
 */
void ABCMethod::refineParameters(std::vector<Parameter>& parameters, 
                                 std::function<double(const std::vector<Parameter>&)> calculateSale, 
                                 double salesObjective, 
                                 double tolerance) {
    std::default_random_engine generator(std::random_device{}());
    std::normal_distribution<double> distribution(0.0, 0.01); // Ajuste pequeño

    // Ajuste inicial de los parámetros basado en una distribución normal.
    for (auto& param : parameters) {
        double adjustment = distribution(generator);
        param.adjustProbability(adjustment);
    }

    normalizeParameters(parameters);

    double closestDistance = std::numeric_limits<double>::max();
    std::vector<Parameter> bestParameters = parameters;

    int numberOfIterations;
    readConfigSimple("simulation_config.txt", numberOfIterations);

    // Iterar para refinar los parámetros
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

        dynamicAdjustParameters(parameters, saleValue, salesObjective);
        normalizeParameters(parameters);
    }

    parameters = bestParameters;
}

/**
 * Ajusta dinámicamente los parámetros según los resultados de la simulación.
 * @param parameters Lista de parámetros a ajustar.
 * @param saleValue Valor de las ventas simuladas.
 * @param salesObjective Objetivo de ventas a alcanzar.
 */
void ABCMethod::dynamicAdjustParameters(std::vector<Parameter>& parameters, double saleValue, double salesObjective) {
    double errorMargin = calculateDistance(saleValue, salesObjective);
    std::default_random_engine generator(std::random_device{}());
    std::normal_distribution<double> distribution(0.0, 0.01);

    for (auto& param : parameters) {
        double randomFactor = distribution(generator);
        double adjustment = (saleValue < salesObjective) ? 0.01 : -0.01;
        param.adjustProbability(adjustment * errorMargin * (1 + randomFactor));

        std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
    }
}

/**
 * Ajuste basado en gradiente.
 * @param parameters Lista de parámetros a ajustar.
 * @param saleValue Valor de las ventas simuladas.
 * @param salesObjective Objetivo de ventas a alcanzar.
 */
void ABCMethod::dynamicAdjustParametersGradient(std::vector<Parameter>& parameters, double saleValue, double salesObjective) {
    double learningRate = 0.01;
    double error = saleValue - salesObjective;

    for (auto& param : parameters) {
        double gradiente = (saleValue < salesObjective) ? learningRate : -learningRate;
        param.adjustProbability(gradiente * std::abs(param.probability));

        std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
    }
}

/**
 * Ajuste basado en la media deslizante.
 * @param parameters Lista de parámetros a ajustar.
 * @param saleValue Valor de las ventas simuladas.
 * @param salesObjective Objetivo de ventas a alcanzar.
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

/**
 * Optimización basada en algoritmos evolutivos (genéticos).
 * @param parameters Lista de parámetros a ajustar.
 * @param calculateSale Función para calcular las ventas.
 * @param saleValue Valor de las ventas simuladas.
 * @param salesObjective Objetivo de ventas a alcanzar.
 */
void ABCMethod::dynamicAdjustParametersGenetic(std::vector<Parameter>& parameters, std::function<double(const std::vector<Parameter>&)> calculateSale, double saleValue, double salesObjective) {
    std::default_random_engine generator(std::random_device{}());
    std::uniform_real_distribution<double> distribution(-0.05, 0.05);

    std::vector<Parameter> newGeneration = parameters;
    for (auto& param : newGeneration) {
        double mutation = distribution(generator);
        param.adjustProbability(mutation);

        std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
    }

    double newSaleValue = calculateSale(newGeneration);
    double currentDistance = calculateDistance(saleValue, salesObjective);
    double newDistance = calculateDistance(newSaleValue, salesObjective);

    if (newDistance < currentDistance) {
        parameters = newGeneration;
    }

    normalizeParameters(parameters);
}

/**
 * Optimización por recocido simulado.
 * @param parameters Lista de parámetros a ajustar.
 * @param calculateSale Función para calcular las ventas.
 * @param saleValue Valor de las ventas simuladas.
 * @param salesObjective Objetivo de ventas a alcanzar.
 */
void ABCMethod::dynamicAdjustParametersSimulatedAnnealing(std::vector<Parameter>& parameters, std::function<double(const std::vector<Parameter>&)> calculateSale, double saleValue, double salesObjective) {
    std::default_random_engine generator(std::random_device{}());
    std::normal_distribution<double> distribution(0.0, 0.05);
    double temperature = 1.0;

    std::vector<Parameter> newParameters = parameters;
    for (auto& param : newParameters) {
        double mutation = distribution(generator);
        param.adjustProbability(mutation);

        std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
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

/**
 * Optimización basada en cuadrados mínimos (Levenberg-Marquardt).
 * @param parameters Lista de parámetros a ajustar.
 * @param saleValue Valor de las ventas simuladas.
 * @param salesObjective Objetivo de ventas a alcanzar.
 */
void ABCMethod::dynamicAdjustParametersLM(std::vector<Parameter>& parameters, double saleValue, double salesObjective) {
    double lambda = 0.001; // Parámetro de amortiguación
    double error = saleValue - salesObjective;

    for (auto& param : parameters) {
        double gradient = error * param.probability;
        double adjustment = (gradient / (1 + lambda * gradient)) * 0.01;
        param.adjustProbability(adjustment);

        std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
    }

    normalizeParameters(parameters);
}

/**
 * Calcula la distancia entre las ventas simuladas y el objetivo de ventas.
 * @param saleValue Valor de las ventas simuladas.
 * @param salesObjective Objetivo de ventas a alcanzar.
 * @return Distancia entre las ventas simuladas y el objetivo.
 */
double ABCMethod::calculateDistance(double saleValue, double salesObjective) {
    return std::abs(saleValue - salesObjective);
}

/**
 * Normaliza los parámetros para que sus probabilidades sumen 1 o menos.
 * @param parameters Lista de parámetros a normalizar.
 */
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