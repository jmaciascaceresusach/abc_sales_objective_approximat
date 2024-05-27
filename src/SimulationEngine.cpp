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
#include <sstream> // Incluir sstream para convertir std::thread::id a string

void logFunction(const std::string& message) {
    std::ofstream logFile("simulation_log.txt", std::ios_base::app);
    if (logFile.is_open()) {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_time);
        logFile << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << " - " << message << "\n";
        logFile.close();
    }
}

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
    int bestIteration = -1;

    std::cout << "\n**Start Simulation***\n";
    logFunction("**Start Simulation***");

    auto runAdjustmentSimple = [&, this](void (ABCMethod::*adjustFunc)(std::vector<Parameter>&, double, double), const std::string& methodName) {
        std::vector<Parameter> localParameters = parameters;
        double localClosestDistance = std::numeric_limits<double>::max();
        std::vector<Parameter> localBestParameters = localParameters;
        double localBestSaleValue = 0;
        int localBestIteration = -1;

        for (int i = 0; i < numberOfIterations; ++i) {
            double saleValue = calculateSale(localParameters);
            double distance = std::abs(saleValue - salesObjective);

            std::stringstream ss;
            ss << "\n";
            ss << "Thread " << std::this_thread::get_id() << " - Iteration: " << i << " - saleValue: " << saleValue << " - distance: " << distance;
            std::string logMsg = ss.str();
            std::cout << logMsg << std::endl;
            logFunction(logMsg);

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

            // Log the parameter adjustments
            std::stringstream adjustmentLog;
            adjustmentLog << "Adjusting parameters at iteration " << i << " using method: " << methodName;
            logFunction(adjustmentLog.str());

            (abcMethod.*adjustFunc)(localParameters, saleValue, salesObjective);

            // Log the new parameter values after adjustment
            std::stringstream newParamsLog;
            newParamsLog << "New parameter values after adjustment:";
            for (const auto& param : localParameters) {
                newParamsLog << " " << param.name << ": " << param.probability;
            }
            logFunction(newParamsLog.str());
        }

        {
            std::lock_guard<std::mutex> lock(mtx);
            if (localClosestDistance < closestDistance) {
                closestDistance = localClosestDistance;
                bestParameters = localBestParameters;
                bestMethod = methodName;
                bestIteration = localBestIteration;
            }
        }
    };

    /*auto runAdjustmentComplex = [&, this](void (ABCMethod::*adjustFunc)(std::vector<Parameter>&, std::function<double(const std::vector<Parameter>&)>, double, double), const std::string& methodName) {
        std::vector<Parameter> localParameters = parameters;
        double localClosestDistance = std::numeric_limits<double>::max();
        std::vector<Parameter> localBestParameters = localParameters;
        double localBestSaleValue = 0;
        int localBestIteration = -1;

        for (int i = 0; i < numberOfIterations; ++i) {
            double saleValue = calculateSale(localParameters);
            double distance = std::abs(saleValue - salesObjective);

            std::stringstream ss;
            ss << "\n";
            ss << "Thread " << std::this_thread::get_id() << " - Iteration: " << i << " - saleValue: " << saleValue << " - distance: " << distance;

            std::string logMsg = ss.str();
            std::cout << logMsg << std::endl;
            logFunction(logMsg);

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
                bestIteration = localBestIteration;
            }
        }
    };*/

    /*std::vector<std::thread> threads;
    threads.emplace_back([&] { runAdjustmentSimple(&ABCMethod::dynamicAdjustParameters, "dynamicAdjustParameters"); });
    threads.emplace_back([&] { runAdjustmentSimple(&ABCMethod::dynamicAdjustParametersGradient, "dynamicAdjustParametersGradient"); });
    threads.emplace_back([&] { runAdjustmentSimple(&ABCMethod::dynamicAdjustParametersSlidingAverage, "dynamicAdjustParametersSlidingAverage"); });
    threads.emplace_back([&] { runAdjustmentComplex(&ABCMethod::dynamicAdjustParametersGenetic, "dynamicAdjustParametersGenetic"); });
    threads.emplace_back([&] { runAdjustmentComplex(&ABCMethod::dynamicAdjustParametersSimulatedAnnealing, "dynamicAdjustParametersSimulatedAnnealing"); });
    threads.emplace_back([&] { runAdjustmentSimple(&ABCMethod::dynamicAdjustParametersLM, "dynamicAdjustParametersLM"); });*/

    std::vector<std::thread> threads;
    threads.emplace_back([&] { runAdjustmentSimple(&ABCMethod::dynamicAdjustParameters, "dynamicAdjustParameters"); });

    for (auto& thread : threads) {
        thread.join();
    }

    parameters = bestParameters;
    statsFile.close();

    std::cout << "\n***End Simulation***\n";
    logFunction("***End Simulation***");

    std::string resultMsg = "Best parameters found with sale value " + std::to_string(calculateSale(bestParameters)) + " using method: " + bestMethod + " in iteration " + std::to_string(bestIteration);
    std::cout << "\n***Results***\n";
    std::cout << resultMsg << std::endl;
    logFunction(resultMsg);

    std::cout << "\n***Best Parameters***\n";
    for (const auto& param : bestParameters) {
        std::string paramMsg = "Parameter: " + param.name + ", Probability: " + std::to_string(param.probability);
        std::cout << paramMsg << std::endl;
        logFunction(paramMsg);
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
    double learningRate = 0.001;

    for (auto& param : parameters) {
        double adjustment = (error > 0 ? -1 : 1) * learningRate * std::abs(param.probability);
        param.adjustProbability(adjustment);
        param.probability = std::max(0.0, std::min(param.probability, 1.0));

        std::string paramMsg = "Parameter: " + param.name + ", Probability: " + std::to_string(param.probability);
        std::cout << paramMsg << std::endl;
        logFunction(paramMsg);
    }
}