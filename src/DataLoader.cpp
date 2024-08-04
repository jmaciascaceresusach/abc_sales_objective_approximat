#include "../include/DataLoader.h"
#include "../include/SimulationEngine.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <limits>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <chrono>
#include <iomanip>
#include <sstream>

/*
Comentarios generales:
- Este archivo implementa las funciones auxiliares para cargar datos desde archivos.

Comentario específicos:
- Implementación de funciones como loadSKUData, loadNormalizedFeatures, loadNoNormalizedFeatures, loadSimulationConfig, getCurrentDate, inverse_z_score, calculate_z_score, loadValues.
*/

// 04-08-2024 1714 (v2)
void HistoricalData::loadFromCSV(const std::string& filename) { 
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line);
    std::istringstream headerStream(line);
    std::string feature;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        std::map<std::string, double> record;
        int i = 0;
        while (std::getline(headerStream, feature, ';')) {
            features.push_back(feature);
        }
        records.push_back(record);
    }
}

// 04-08-2024 1714
std::vector<std::string> getAllSKUs() {
    std::vector<std::string> skus;
    std::string basePath = "../data/input/";
    
    for (const auto& entry : std::filesystem::directory_iterator(basePath)) {
        if (entry.is_directory() && entry.path().filename().string().substr(0, 4) == "sku_") {
            skus.push_back(entry.path().filename().string().substr(4));
        }
    }
    
    return skus;
}

void runSimulationForSKU(SimulationEngine& simulationEngine, const std::string& sku, const std::string& dayForSimulate, int numberOfIterations, int daysToSimulate, double tolerance) {
    std::string basePath = "../data/input/sku_" + sku + "/" + dayForSimulate + "/";
    
    SKUData skuData = loadSKUData(basePath + sku + "_matriz_intervals_df_" + dayForSimulate + ".csv");
    std::map<std::string, double> normalizedFeatures = loadNormalizedFeatures(basePath + sku + "_df_features_sku_norm_" + dayForSimulate + ".txt");
    std::map<std::string, double> noNormalizedFeatures = loadNoNormalizedFeatures(basePath + sku + "_df_features_sku_" + dayForSimulate + ".txt");

    simulationEngine.loadMeanAndStdValues(basePath + sku + "_mean_values_features_sku_" + dayForSimulate + ".csv",
                                          basePath + sku + "_std_values_features_sku_" + dayForSimulate + ".csv");

    simulationEngine.setProductData(skuData);
    simulationEngine.setNormalizedFeatures(normalizedFeatures);
    simulationEngine.setNoNormalizedFeatures(noNormalizedFeatures);
    simulationEngine.runSimulations(numberOfIterations, daysToSimulate, tolerance);
    
    std::cout << "Simulation completed for SKU: " << sku << std::endl;
}

std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    return ss.str();
}

std::map<std::string, double> inverse_z_score(const std::map<std::string, double>& z_score_normalized,
                                              const std::map<std::string, double>& mean_values,
                                              const std::map<std::string, double>& std_values) {
    std::map<std::string, double> original_values;
    for (const auto& pair : z_score_normalized) {
        const std::string& column = pair.first;
        double z_score = pair.second;
        original_values[column] = z_score * std_values.at(column) + mean_values.at(column);
    }
    return original_values;
}

std::map<std::string, double> calculate_z_score(
    const std::map<std::string, double>& registro,
    const std::map<std::string, double>& mean_values,
    const std::map<std::string, double>& std_values) {
    
    std::map<std::string, double> z_scores;
    
    for (const auto& pair : registro) {
        const std::string& column = pair.first;
        double value = pair.second;
        
        // Verificar si la columna existe en mean_values y std_values
        if (mean_values.find(column) != mean_values.end() && 
            std_values.find(column) != std_values.end()) {
            
            double mean_value = mean_values.at(column);
            double std_value = std_values.at(column);
            
            // Evitar división por cero
            if (std_value != 0) {
                z_scores[column] = (value - mean_value) / std_value;
            } else {
                // Si la desviación estándar es cero, el z-score es indefinido
                // Puedes manejar este caso como prefieras, aquí lo dejamos en 0
                z_scores[column] = 0;
            }
        }
    }
    
    return z_scores;
}

std::map<std::string, double> loadValues(const std::string& filename) {
    std::map<std::string, double> values;
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line); // Skip header
    std::getline(file, line);
    std::istringstream iss(line);
    std::string value;
    int i = 0;
    while (std::getline(iss, value, ',')) {
        std::string key;
        switch(i) {
            case 0: key = "client_numeric"; break;
            case 1: key = "vendor_numeric"; break;
            case 2: key = "year"; break;
            case 3: key = "month"; break;
            case 4: key = "day"; break;
            case 5: key = "sku_count_products"; break;
            case 6: key = "total_num_count_products"; break;
            case 7: key = "total_price_products"; break;
        }
        values[key] = std::stod(value);
        i++;
    }
    return values;
}

SKUData loadSKUData(const std::string& filename) {
    SKUData data;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return data;
    }

    // Leer la primera línea (encabezados)
    std::getline(file, line);
    std::istringstream headerStream(line);
    std::string token;
    std::getline(headerStream, token, ';'); // Ignorar "sku"
    
    std::cout << "\n*** loadSKUData ***" << std::endl;

    // Leer los encabezados de list_products
    while (std::getline(headerStream, token, ';')) {
        if (token.find("list_products_") != std::string::npos) {
            data.listProducts.push_back(std::make_pair(0.0, 0.0));
        }
    }

    // Leer la segunda línea (datos)
    std::getline(file, line);
    std::istringstream dataStream(line);
    
    // Leer SKU
    std::getline(dataStream, data.sku, ';');

    // Leer intervalos de list_products
    for (auto& interval : data.listProducts) {
        std::string intervalStr;
        std::getline(dataStream, intervalStr, ';');
        sscanf(intervalStr.c_str(), "(%lf, %lf)", &interval.first, &interval.second);
    }

    // Leer min_price y max_price
    std::string minPriceStr, maxPriceStr;
    std::getline(dataStream, minPriceStr, ';');
    std::getline(dataStream, maxPriceStr, ';');
    data.globalMinPrice = std::stod(minPriceStr);
    data.globalMaxPrice = std::stod(maxPriceStr);

    file.close();

    std::cout << "Loaded SKU data for " << data.sku << " with " 
              << data.listProducts.size() << " price intervals" << std::endl;
    std::cout << "Global price range: [" << data.globalMinPrice 
              << ", " << data.globalMaxPrice << "]" << std::endl;

    return data;
}

std::map<std::string, double> loadNormalizedFeatures(const std::string& filename) {
    std::map<std::string, double> features;
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
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            size_t start = value.find('(');
            size_t end = value.find(')', start);
            if (start != std::string::npos && end != std::string::npos) {
                std::string featureValueStr = value.substr(start + 1, end - start - 1);
                try {
                    double featureValue = std::stod(featureValueStr);
                    features[key] = featureValue;
                    std::cout << "Loaded feature " << key << ": " << featureValue << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing value for " << key << ": " << e.what() << std::endl;
                }
            }
        }
    }

    file.close();
    std::cout << "Loaded " << features.size() << " normalized features" << std::endl;

    return features;
}

std::map<std::string, double> loadNoNormalizedFeatures(const std::string& filename) {
    std::map<std::string, double> features;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return features;
    }

    std::cout << "\n*** loadNoNormalizedFeatures ***" << std::endl;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, ':') && std::getline(iss, value)) {
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            size_t start = value.find('(');
            size_t end = value.find(')', start);
            if (start != std::string::npos && end != std::string::npos) {
                std::string featureValueStr = value.substr(start + 1, end - start - 1);
                try {
                    double featureValue = std::stod(featureValueStr);
                    features[key] = featureValue;
                    std::cout << "Loaded feature " << key << ": " << featureValue << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing value for " << key << ": " << e.what() << std::endl;
                }
            }
        }
    }

    file.close();
    std::cout << "Loaded " << features.size() << " no normalized features \n" << std::endl;

    return features;
}

// 04-08-2024 1714 (v2)
void loadSimulationConfig(const std::string& filename, int& numberOfIterations, int& tolerance, int& daysToSimulate, std::string& dayForSimulate, std::string& skuForSimulate) {
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;

        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            if (key == "numberOfIterations") numberOfIterations = std::stoi(value);
            else if (key == "tolerance") tolerance = std::stoi(value);
            else if (key == "daysToSimulate") daysToSimulate = std::stoi(value);
            else if (key == "dayForSimulate") dayForSimulate = value;
            else if (key == "skuForSimulate") skuForSimulate = value;
        }
    }

    std::cout << "\n*** loadSimulationConfig ***" << std::endl;
    std::cout << "numberOfIterations set to " << numberOfIterations << std::endl;
    std::cout << "tolerance set to " << tolerance << std::endl;
    std::cout << "daysToSimulate set to " << daysToSimulate << std::endl;
    std::cout << "dayForSimulate set to " << dayForSimulate << std::endl;
    std::cout << "skuForSimulate set to " << skuForSimulate << std::endl;
}

// 04-08-2024 1714
std::map<std::string, double> loadAttributeWeights(const std::string& filename) {
    std::map<std::string, double> weights;
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line); // Skip header
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string feature, weight;
        std::getline(iss, feature, ',');
        std::getline(iss, weight);
        weights[feature] = std::stod(weight);
    }
    return weights;
}

// 04-08-2024 1714
std::map<std::string, std::vector<std::pair<double, double>>> loadSKUIntervals(const std::string& filename) {
    std::map<std::string, std::vector<std::pair<double, double>>> intervals;
    std::ifstream file(filename);
    std::string line;
    std::getline(file, line); // Skip header
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string sku, interval;
        std::getline(iss, sku, ';');
        std::vector<std::pair<double, double>> skuIntervals;
        while (std::getline(iss, interval, ';')) {
            if (interval.find("list_products_") != std::string::npos) {
                double min, max;
                sscanf(interval.c_str(), "(%lf, %lf)", &min, &max);
                skuIntervals.push_back({min, max});
            }
        }
        intervals[sku] = skuIntervals;
    }
    return intervals;
}