#include "../include/SimulationEngine.h"
#include <iostream>
#include <algorithm> // para std::min_element
#include <chrono>
#include <iomanip> // para std::put_time

/**
 * Implementación del constructor.
 */
SimulationEngine::SimulationEngine() {}

/**
 * Agrega un parámetro a la simulación.
 * @param parameter Parámetro a agregar.
 */
void SimulationEngine::addParameter(const Parameter& parameter) {
    this->parameters.push_back(parameter);
}

/**
 * Ejecuta simulaciones para aproximarse al objetivo de ventas.
 * @param numberOfIterations Número de iteraciones a ejecutar.
 * @param calculateSale Función para calcular las ventas.
 * @param salesObjective Objetivo de ventas a alcanzar.
 * @param tolerance Tolerancia aceptable entre las ventas calculadas y el objetivo.
 */
void SimulationEngine::runSimulations(int numberOfIterations, std::function<double(const std::vector<Parameter>&)> calculateSale, double salesObjective, double tolerance) {
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
        double distance = std::abs(saleValue - salesObjective);

        std::cout << "\n";
        std::cout << "Iteration: " << i << " - saleValue: " << saleValue << " - distance: " << distance << std::endl;

        statsFile << i << "," << saleValue << "," << distance << "," << salesObjective << "," << tolerance;
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

        adjustParameters(saleValue, salesObjective);
    }

    parameters = bestParameters;
    statsFile.close();

    std::cout << "\n***End Simulation***\n";

    std::cout << "\n***Results***\n";
    std::cout << "Best parameters found at iteration " << bestIteration << " with sale value " << bestSaleValue << std::endl;
    std::cout << "\n***Best Parameters***\n";
    for (const auto& param : bestParameters) {
        std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
    }

    abcMethod.refineParameters(parameters, calculateSale, salesObjective, tolerance);
}

/**
 * Ajusta parámetros en función de los resultados de la simulación.
 * @param saleValue Valor de las ventas simuladas.
 * @param salesObjective Objetivo de ventas a alcanzar.
 */
void SimulationEngine::adjustParameters(double saleValue, double salesObjective) {
    double error = saleValue - salesObjective;
    double learningRate = 0.01;

    for (auto& param : parameters) {
        double adjustment = (error > 0 ? -1 : 1) * learningRate * std::abs(param.probability);
        param.adjustProbability(adjustment);

        param.probability = std::max(0.0, std::min(param.probability, 1.0));
        std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
    }
}