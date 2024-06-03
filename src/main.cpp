#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <string>
#include <limits>
#include <utility>
#include "../include/Parameter.h"
#include "../include/SimulationEngine.h"

double calculateSale(const std::vector<Parameter>& parameters) {
    double totalSaleValue = 0.0;
    for (const auto& param : parameters) {
        if (param.name == "customerParam") {
            totalSaleValue += param.probability * 27;
        } else if (param.name == "dateParam") {
            totalSaleValue += param.probability * 16;
        } else if (param.name == "sellerParam") {
            totalSaleValue += param.probability * 22;
        } else if (param.name == "productParam") {
            totalSaleValue += param.probability * 5;
        } else if (param.name == "saleParam") {
            totalSaleValue += param.probability * 30;
        }
    }
    return totalSaleValue;
}

void readConfigFor8(const std::string& configFilePath, 
                    int& numberOfIterations, 
                    double& salesObjectiveInitial, 
                    double& salesObjectiveFinal, 
                    double& tolerance,
                    double& customerParam,
                    double& sellerParam,
                    double& saleParam,
                    double& dateParam,
                    double& productParam) {
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
                    else if (key == "salesObjectiveInitial") salesObjectiveInitial = std::stod(value);
                    else if (key == "salesObjectiveFinal") salesObjectiveFinal = std::stod(value);
                    else if (key == "tolerance") tolerance = std::stod(value);
                    else if (key == "customerParam") customerParam = std::stod(value);
                    else if (key == "sellerParam") sellerParam = std::stod(value);
                    else if (key == "saleParam") saleParam = std::stod(value);
                    else if (key == "dateParam") dateParam = std::stod(value);
                    else if (key == "productParam") productParam = std::stod(value);
                }
            }
        }
    } else {
        std::cerr << "Failed to open config file: " << configFilePath << std::endl;
    }
}

void analyzeStatistics(const std::string& statsFilePath) {
    std::ifstream statsFile(statsFilePath);
    if (!statsFile.is_open()) {
        std::cerr << "Failed to open statistics file: " << statsFilePath << std::endl;
        return;
    }

    std::string line;
    std::getline(statsFile, line); // Leer la primera línea de encabezados

    std::vector<std::string> parameterNames;
    std::istringstream headerStream(line);
    std::string headerToken;
    while (std::getline(headerStream, headerToken, ',')) {
        parameterNames.push_back(headerToken);
    }

    int bestIteration = -1;
    double minDistance = std::numeric_limits<double>::max();
    double bestSaleValue = 0.0;
    std::vector<std::pair<std::string, double>> bestParameters;

    while (std::getline(statsFile, line)) {
        std::istringstream iss(line);
        std::string token;

        if (!std::getline(iss, token, ',')) continue;
        int iteration;
        try {
            iteration = std::stoi(token);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid iteration value: " << token << std::endl;
            continue;
        }

        if (!std::getline(iss, token, ',')) continue;
        double saleValue;
        try {
            saleValue = std::stod(token);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid sale value: " << token << std::endl;
            continue;
        }

        if (!std::getline(iss, token, ',')) continue;
        double distance;
        try {
            distance = std::stod(token);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid distance value: " << token << std::endl;
            continue;
        }

        if (distance < minDistance) {
            minDistance = distance;
            bestIteration = iteration;
            bestSaleValue = saleValue;
            bestParameters.clear();
            int parameterIndex = 5;
            while (std::getline(iss, token, ',')) {
                try {
                    if (parameterIndex < parameterNames.size()) {
                        bestParameters.push_back({parameterNames[parameterIndex], std::stod(token)});
                        ++parameterIndex;
                    }
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid parameter value: " << token << std::endl;
                    bestParameters.clear();
                    break;
                }
            }
        }
    }

    if (bestIteration != -1) {
        std::cout << "\n***Best Iteration Analysis***\n";
        std::cout << "Best parameters found at iteration " << bestIteration << " with sale value " << bestSaleValue << " and distance " << minDistance << std::endl;
        std::cout << "\n***Best Parameters***\n";
        for (const auto& param : bestParameters) {
            std::cout << "Parameter: " << param.first << ", Probability: " << param.second << std::endl;
        }
    } else {
        std::cerr << "No valid data found in statistics file." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now();

    int numberOfIterations;
    double salesObjectiveInitial, salesObjectiveFinal, tolerance;
    double customerParam, sellerParam, saleParam, dateParam, productParam;

    readConfigFor8("../simulation_config.txt", 
                    numberOfIterations, 
                    salesObjectiveInitial,
                    salesObjectiveFinal, 
                    tolerance,
                    customerParam,
                    sellerParam,
                    saleParam,
                    dateParam,
                    productParam);

    SimulationEngine simulationEngine;

    Parameter customerParamParameter("customerParam", customerParam);
    Parameter sellerParamParameter("sellerParam", sellerParam);
    Parameter saleParamParameter("saleParam", saleParam);
    Parameter dateParamParameter("dateParam", dateParam);
    Parameter productParamParameter("productParam", productParam);

    simulationEngine.addParameter(customerParamParameter);
    simulationEngine.addParameter(sellerParamParameter);
    simulationEngine.addParameter(saleParamParameter);
    simulationEngine.addParameter(dateParamParameter);
    simulationEngine.addParameter(productParamParameter);

    simulationEngine.runSimulations(numberOfIterations, calculateSale, salesObjectiveFinal, tolerance);

    std::cout << "\n";
    std::cout << "***Configurations***" << std::endl;
    std::cout << "numberOfIterations: " << numberOfIterations << std::endl;
    std::cout << "salesObjectiveInitial: " << salesObjectiveInitial << std::endl;
    std::cout << "salesObjectiveFinal: " << salesObjectiveFinal << std::endl;
    std::cout << "tolerance: " << tolerance << std::endl;
    std::cout << "\n";

    std::cout << "***Startup Parameters***" << std::endl;
    std::cout << "customerParam: " << customerParam << std::endl;
    std::cout << "sellerParam: " << sellerParam << std::endl;
    std::cout << "saleParam: " << saleParam << std::endl;
    std::cout << "dateParam: " << dateParam << std::endl;
    std::cout << "productParam: " << productParam << std::endl;
    std::cout << "\n";

    analyzeStatistics("../build/statistics_simulations.txt");

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "\n";
    std::cout << "***Duration***" << std::endl;
    std::cout << "Time: " << duration.count() << " milliseconds" << std::endl;
    std::cout << "\n";

    return 0;
}
