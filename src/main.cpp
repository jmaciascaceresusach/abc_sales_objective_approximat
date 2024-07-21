#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <string>
#include <limits>
#include <utility>
#include <map>
#include "../include/Parameter.h"
#include "../include/SimulationEngine.h"

SKUData loadSKUData(const std::string& filename) {
    SKUData data;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return data;
    }

    // Leer la primera línea para obtener el SKU
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        if (std::getline(iss, token, ';')) {
            data.sku = token;
        }
    }

    data.globalMinPrice = std::numeric_limits<double>::max();
    data.globalMaxPrice = std::numeric_limits<double>::lowest();

    // Leer los intervalos
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        PriceInterval interval;

        // Ignorar el nombre del intervalo
        std::getline(iss, token, ';');

        // Leer el rango de precios
        if (std::getline(iss, token, ';')) {
            sscanf(token.c_str(), "(%lf, %lf)", &interval.minPrice, &interval.maxPrice);
        }

        // Leer el conteo (si está disponible)
        if (std::getline(iss, token, ';')) {
            interval.count = std::stoi(token);
        } else {
            interval.count = 0; // Si no hay conteo, asumimos 0
        }

        data.intervals.push_back(interval);

        // Actualizar min y max globales
        data.globalMinPrice = std::min(data.globalMinPrice, interval.minPrice);
        data.globalMaxPrice = std::max(data.globalMaxPrice, interval.maxPrice);
    }

    file.close();

    std::cout << "Loaded SKU data for " << data.sku << " with " 
              << data.intervals.size() << " intervals" << std::endl;
    std::cout << "Global price range: [" << data.globalMinPrice 
              << ", " << data.globalMaxPrice << "]" << std::endl;

    return data;
}

std::vector<double> loadNormalizedFeatures(const std::string& filename) {
    std::vector<double> features;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return features;
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, ':') && std::getline(iss, value)) {
            // Eliminar espacios en blanco
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            // Convertir el valor a double y añadirlo al vector
            try {
                double featureValue = std::stod(value);
                features.push_back(featureValue);
                std::cout << "Loaded feature " << key << ": " << featureValue << std::endl;
            } catch (const std::invalid_argument& ia) {
                std::cerr << "Invalid argument: " << ia.what() << std::endl;
            } catch (const std::out_of_range& oor) {
                std::cerr << "Out of range: " << oor.what() << std::endl;
            }
        }
    }

    file.close();

    std::cout << "Loaded " << features.size() << " normalized features" << std::endl;

    return features;
}

void readConfigFor3(const std::string& configFilePath, 
                    int& numberOfIterations,
                    double& tolerance,
                    int& daysToSimulate) {
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
                    else if (key == "tolerance") tolerance = std::stod(value);
                    else if (key == "daysToSimulate") daysToSimulate = std::stod(value);
                }
            }
        }
    } else {
        std::cerr << "Failed to open config file: " << configFilePath << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // Obtiene el tiempo de inicio
    auto start = std::chrono::high_resolution_clock::now();

    int numberOfIterations, daysToSimulate;
    double tolerance;

    readConfigFor3("../data/simulation_config.txt", 
                   numberOfIterations, 
                   tolerance,
                   daysToSimulate);

    // Inicializar el motor de simulación
    SimulationEngine simulationEngine;

    // Cargar datos del SKU
    SKUData skuData = loadSKUData("../data/matriz_intervals_df_Z285320.csv");
    
    // Cargar características normalizadas
    std::vector<double> normalizedFeatures = loadNormalizedFeatures("../data/df_features_Z285320_sku_norm.txt");
    
    SimulationEngine simulationEngine;
    simulationEngine.setProductData(skuData);
    simulationEngine.setNormalizedFeatures(normalizedFeatures);
    
    // Ejecutar simulaciones
    simulationEngine.runSimulations(numberOfIterations, daysToSimulate, tolerance);

    std::cout << "\n";
    std::cout << "***Configurations***" << std::endl;
    std::cout << "numberOfIterations: " << numberOfIterations << std::endl;
    std::cout << "tolerance: " << tolerance << std::endl;
    std::cout << "daysToSimulate: " << daysToSimulate << std::endl;
    std::cout << "\n";

    // Obtiene el tiempo de finalización
    auto end = std::chrono::high_resolution_clock::now();

    // Calcula la duración en milisegundos
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "***Duration***" << std::endl;
    std::cout << "Time: " << duration.count() << " milliseconds" << std::endl;
    std::cout << "\n";

    return 0;
}