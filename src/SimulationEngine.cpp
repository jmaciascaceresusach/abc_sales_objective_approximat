#include "../include/SimulationEngine.h"
#include <iostream>
#include <algorithm> // para std::min_element
#include <chrono>
#include <iomanip> // para std::put_time
#include <limits>
#include <random>
#include <cmath>

// Implementación del constructor
SimulationEngine::SimulationEngine() {}

// Agrega un parámetro a la simulación
void SimulationEngine::addParameter(const Parameter& parameter) {
    this->parameters.push_back(parameter);
}

void SimulationEngine::addHistoricalData(const std::vector<Parameter>& historicalData) {
    // Combinar datos históricos con parámetros actuales
    for (size_t i = 0; i < parameters.size(); ++i) {
        parameters[i].adjustProbability(historicalData[i].probability);
    }
    normalizeParameters(parameters);
}

// Función para leer los datos históricos desde un archivo
std::vector<Parameter> readHistoricalData(const std::string& filePath) {
    std::vector<Parameter> historicalData;
    std::ifstream file(filePath);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            double probability = std::stod(value);
            historicalData.push_back(Parameter(key, probability));
        }
    }

    return historicalData;
}

// Ejecuta simulaciones para aproximarse al objetivo de ventas
void SimulationEngine::runSimulations(int numberOfIterations, std::function<double(const std::vector<Parameter>&)> calculateSale, double salesObjectiveFinal, double tolerance) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time);

    // Abrir archivo para guardar estadísticas de simulación
    std::ofstream statsFile("statistics_simulations.txt");
    statsFile << "Creation Date and Time: " << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << "\n";
    statsFile << "Iteration,SaleValue,Distance,Objective,Tolerance";
    for (const auto& param : parameters) {
        statsFile << "," << param.name;
    }
    statsFile << "\n";

    double closestDistance = std::numeric_limits<double>::max();
    std::vector<Parameter> bestParameters = parameters;
    double bestSaleValue = 0;
    int bestIteration = -1;

    std::cout << "\n**Start Simulation***\n";

    // Ejecutar iteraciones de simulación
    for (int i = 0; i < numberOfIterations; ++i) {
        double saleValue = calculateSale(parameters);
        double distance = calculateDistance(saleValue, salesObjectiveFinal);

        std::cout << "\n";
        std::cout << "Iteration: " << i << " - saleValue: " << saleValue << " - distance: " << distance << std::endl;

        statsFile << i << "," << saleValue << "," << distance << "," << salesObjectiveFinal << "," << tolerance;
        for (const auto& param : parameters) {
            statsFile << "," << param.probability;
        }
        statsFile << "\n";

        if (distance < closestDistance) {
            closestDistance = distance;
            bestParameters = parameters;
            bestSaleValue = saleValue;
            bestIteration = i;
        }

        adjustParameters(saleValue, salesObjectiveFinal);
        monitorAndEvaluate(parameters, saleValue, salesObjectiveFinal); // Llamada a la función de monitoreo
    }

    parameters = bestParameters;
    statsFile.close();

    std::cout << "\n***End Simulation***\n";

    std::cout << "\n***Results (runSimulations)***\n";
    std::cout << "Best parameters found at iteration " << bestIteration << " with sale value " << bestSaleValue << std::endl;
    std::cout << "\n***Best Parameters***\n";
    for (const auto& param : bestParameters) {
        std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
    }

    abcMethod.refineParameters(parameters, calculateSale, salesObjectiveFinal, tolerance);
}

// Ajusta parámetros en función de los resultados de la simulación
void SimulationEngine::adjustParameters(double saleValue, double salesObjectiveFinal) {
    double error = saleValue - salesObjectiveFinal;
    double learningRate = 0.01;

    for (auto& param : parameters) {
        double adjustment = (error > 0 ? -1 : 1) * learningRate * std::abs(param.probability);
        param.adjustProbability(adjustment);

        param.probability = std::max(0.0, std::min(param.probability, 1.0));
        std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
    }
}

// Monitorea y evalúa el rendimiento de la simulación
void SimulationEngine::monitorAndEvaluate(const std::vector<Parameter>& parameters, double saleValue, double salesObjectiveFinal) {
    double error = calculateDistance(saleValue, salesObjectiveFinal);
    std::cout << "Monitoring - SaleValue: " << saleValue << ", Error: " << error << std::endl;

    // Aquí puedes agregar más lógica para la evaluación continua, como registros adicionales, alertas, etc.
}

// Calcula la distancia entre las ventas simuladas y el objetivo de ventas
double SimulationEngine::calculateDistance(double saleValue, double salesObjectiveFinal) {
    return std::abs(saleValue - salesObjectiveFinal);
}

// Refina parámetros usando el método ABC
void SimulationEngine::refineParameters(std::vector<Parameter>& parameters, 
                                        std::function<double(const std::vector<Parameter>&)> calculateSale, 
                                        double salesObjectiveFinal, 
                                        double tolerance) {
    abcMethod.initializeParameters(parameters); // Inicialización optimizada de parámetros

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
    abcMethod.readConfigSimple("simulation_config.txt", numberOfIterations);

    double initialTolerance = tolerance;

    for (int i = 0; i < numberOfIterations; ++i) {
        double saleValue = calculateSale(parameters);
        double distance = calculateDistance(saleValue, salesObjectiveFinal);

        if (distance < closestDistance) {
            closestDistance = distance;
            bestParameters = parameters;
        }

        if (distance <= tolerance) {
            break;
        }

        // Diversificación de Ajustes
        switch (i % 5) {
            case 0:
                abcMethod.dynamicAdjustParameters(parameters, saleValue, salesObjectiveFinal);
                break;
            case 1:
                abcMethod.dynamicAdjustParametersGradient(parameters, saleValue, salesObjectiveFinal);
                break;
            case 2:
                abcMethod.dynamicAdjustParametersSlidingAverage(parameters, saleValue, salesObjectiveFinal);
                break;
            case 3:
                abcMethod.dynamicAdjustParametersGenetic(parameters, calculateSale, saleValue, salesObjectiveFinal);
                break;
            case 4:
                abcMethod.dynamicAdjustParametersSimulatedAnnealing(parameters, calculateSale, saleValue, salesObjectiveFinal);
                break;
        }
        normalizeParameters(parameters);

        // Adaptación Dinámica de la Tolerancia
        tolerance = initialTolerance * (1.0 - (static_cast<double>(i) / numberOfIterations));
    }

    parameters = bestParameters;
}

// Normaliza los parámetros para que sus probabilidades sumen 1 o menos
void SimulationEngine::normalizeParameters(std::vector<Parameter>& parameters) {
    double totalProbability = 0.0;
    for (const auto& param : parameters) {
        totalProbability += param.probability;
    }

    if (totalProbability > 1.0) {
        for (auto& param : parameters) {
            param.probability /= totalProbability;
        }
    }
}