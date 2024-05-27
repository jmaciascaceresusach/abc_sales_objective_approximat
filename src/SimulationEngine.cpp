#include "../include/SimulationEngine.h"
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <ctime>

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

    std::mutex mtx;
    std::vector<Parameter> bestParameters;
    double closestDistance = std::numeric_limits<double>::max();
    std::string bestMethod;

    std::cout << "\n**Start SimulationX***\n";

    auto runAdjustmentSimple = [&, this](void (ABCMethod::*adjustFunc)(std::vector<Parameter>&, double, double), const std::string& methodName) {
        std::vector<Parameter> localParameters = parameters;
        double localClosestDistance = std::numeric_limits<double>::max();
        std::vector<Parameter> localBestParameters = localParameters;
        double localBestSaleValue = 0;
        int localBestIteration = -1;

        for (int i = 0; i < numberOfIterations; ++i) {
            double saleValue = calculateSale(localParameters);
            double distance = std::abs(saleValue - salesObjective);

            std::cout << "Iteration: " << i << " - saleValue: " << saleValue << " - distance: " << distance << std::endl;

            {
                std::lock_guard<std::mutex> lock(mtx);
                statsFile << i << "," << saleValue << "," << distance << "," << salesObjective << "," << tolerance;
                for (const auto& param : localParameters) {
                    statsFile << "," << param.probability;
                }
                statsFile << "\n";
            }

            if (distance < localClosestDistance) {
                localClosestDistance = distance;
                localBestParameters = localParameters;
                localBestSaleValue = saleValue;
                localBestIteration = i;
            }

            (abcMethod.*adjustFunc)(localParameters, saleValue, salesObjective);
        }

        {
            std::lock_guard<std::mutex> lock(mtx);
            if (localClosestDistance < closestDistance) {
                closestDistance = localClosestDistance;
                bestParameters = localBestParameters;
                bestMethod = methodName;
            }
        }
    };

    auto runAdjustmentComplex = [&, this](void (ABCMethod::*adjustFunc)(std::vector<Parameter>&, std::function<double(const std::vector<Parameter>&)>, double, double), const std::string& methodName) {
        std::vector<Parameter> localParameters = parameters;
        double localClosestDistance = std::numeric_limits<double>::max();
        std::vector<Parameter> localBestParameters = localParameters;
        double localBestSaleValue = 0;
        int localBestIteration = -1;

        for (int i = 0; i < numberOfIterations; ++i) {
            double saleValue = calculateSale(localParameters);
            double distance = std::abs(saleValue - salesObjective);

            std::cout << "Iteration: " << i << " - saleValue: " << saleValue << " - distance: " << distance << std::endl;

            {
                std::lock_guard<std::mutex> lock(mtx);
                statsFile << i << "," << saleValue << "," << distance << "," << salesObjective << "," << tolerance;
                for (const auto& param : localParameters) {
                    statsFile << "," << param.probability;
                }
                statsFile << "\n";
            }

            if (distance < localClosestDistance) {
                localClosestDistance = distance;
                localBestParameters = localParameters;
                localBestSaleValue = saleValue;
                localBestIteration = i;
            }

            (abcMethod.*adjustFunc)(localParameters, calculateSale, saleValue, salesObjective);
        }

        {
            std::lock_guard<std::mutex> lock(mtx);
            if (localClosestDistance < closestDistance) {
                closestDistance = localClosestDistance;
                bestParameters = localBestParameters;
                bestMethod = methodName;
            }
        }
    }; 

    std::vector<std::thread> threads;
    threads.emplace_back([&] { runAdjustmentSimple(&ABCMethod::dynamicAdjustParameters, "dynamicAdjustParameters"); });
    threads.emplace_back([&] { runAdjustmentSimple(&ABCMethod::dynamicAdjustParametersGradient, "dynamicAdjustParametersGradient"); });
    threads.emplace_back([&] { runAdjustmentSimple(&ABCMethod::dynamicAdjustParametersSlidingAverage, "dynamicAdjustParametersSlidingAverage"); });
    threads.emplace_back([&] { runAdjustmentComplex(&ABCMethod::dynamicAdjustParametersGenetic, "dynamicAdjustParametersGenetic"); });
    threads.emplace_back([&] { runAdjustmentComplex(&ABCMethod::dynamicAdjustParametersSimulatedAnnealing, "dynamicAdjustParametersSimulatedAnnealing"); });
    threads.emplace_back([&] { runAdjustmentSimple(&ABCMethod::dynamicAdjustParametersLM, "dynamicAdjustParametersLM"); });

    for (auto& thread : threads) {
        thread.join();
    }

    parameters = bestParameters;
    statsFile.close();

    std::cout << "\n***End Simulation***\n";
    std::cout << "\n***Results***\n";
    std::cout << "Best parameters found with sale value " << calculateSale(bestParameters) << " using method: " << bestMethod << std::endl;
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