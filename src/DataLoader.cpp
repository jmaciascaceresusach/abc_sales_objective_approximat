#include "../include/DataLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>

SKUData loadSKUData(const std::string& filename) {
    SKUData data;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return data;
    }

    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        if (std::getline(iss, token, ';')) {
            data.sku = token;
        }
    }

    std::cout << "\n*** loadSKUData ***" << std::endl;

    data.globalMinPrice = std::numeric_limits<double>::max();
    data.globalMaxPrice = std::numeric_limits<double>::lowest();

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        PriceInterval interval;

        std::getline(iss, token, ';'); // Ignorar el primer token

        if (std::getline(iss, token, ';')) {
            std::istringstream intervalStream(token);
            char dummy; // Para los paréntesis
            intervalStream >> dummy >> interval.minPrice >> dummy >> interval.maxPrice >> dummy;
        }

        if (std::getline(iss, token, ';')) {
            try {
                interval.count = std::stoi(token);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid count value: " << token << std::endl;
                interval.count = 0;
            } catch (const std::out_of_range& e) {
                std::cerr << "Count value out of range: " << token << std::endl;
                interval.count = 0;
            }
        } else {
            interval.count = 0;
        }

        data.intervals.push_back(interval);

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

    std::cout << "\n*** loadNormalizedFeatures ***" << std::endl;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, ':') && std::getline(iss, value)) {
            // Eliminar espacios en blanco
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            // Encontrar el valor entre paréntesis
            size_t start = value.find('(');
            size_t end = value.find(')', start);
            if (start != std::string::npos && end != std::string::npos) {
                std::string featureValueStr = value.substr(start + 1, end - start - 1);
                try {
                    double featureValue = std::stod(featureValueStr);
                    features.push_back(featureValue);
                    std::cout << "Loaded feature " << key << ": " << featureValue << std::endl;
                } catch (const std::invalid_argument& ia) {
                    std::cerr << "Invalid argument for " << key << ": " << ia.what() << std::endl;
                } catch (const std::out_of_range& oor) {
                    std::cerr << "Out of range for " << key << ": " << oor.what() << std::endl;
                }
            } else {
                std::cerr << "Could not find value between parentheses for " << key << std::endl;
            }
        }
    }

    file.close();

    std::cout << "Loaded " << features.size() << " normalized features" << std::endl;

    return features;
}

void loadSimulationConfig(const std::string& filename, int& numberOfIterations, int& tolerance, int& daysToSimulate) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::cout << "\n*** loadSimulationConfig ***" << std::endl;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            std::cout << "Key: " << key << ", Value: " << value << std::endl; // Depuración

            try {
                if (key == "numberOfIterations") {
                    numberOfIterations = std::stoi(value);
                    std::cout << "numberOfIterations set to " << numberOfIterations << std::endl; // Depuración
                } else if (key == "tolerance") {
                    tolerance = std::stoi(value);
                    std::cout << "tolerance set to " << tolerance << std::endl; // Depuración
                } else if (key == "daysToSimulate") {
                    daysToSimulate = std::stoi(value);
                    std::cout << "daysToSimulate set to " << daysToSimulate << std::endl; // Depuración
                }
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument for key " << key << ": " << value << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Out of range value for key " << key << ": " << value << std::endl;
            }
        }
    }

    file.close();
}