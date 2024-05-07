/* Funcionalidad general 27032024
Se encapsula la lógica de parámetros de simulación con el objetivo 
de alcanzar un objetivo de ventas específico. Se proporciona un método
"refineParameters" que ajusta los parámetros basándose en la distancia
entre los valores de ventas simulados y el objetivo de ventas.
*/

#ifndef ABCMETHOD_H
#define ABCMETHOD_H

#include "Parameter.h"
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>

class ABCMethod {
public:
    // Constructor
    ABCMethod();

    // Método para refinar parámetros usando el método ABC.
    void refineParameters(std::vector<Parameter>& parameters, std::function<double(const std::vector<Parameter>&)> calculateSale, double salesObjective, double tolerance);
    void dynamicAdjustParameters(std::vector<Parameter>& parameters, double saleValue, double salesObjective);

private:
    // Método auxiliar para calcular la distancia entre las ventas simuladas y el objetivo de ventas.
    double calculateDistance(double saleValue, double salesObjective);
};

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

void readConfig(const std::string& configFilePath, 
                int& numberOfIterations, 
                double& salesObjective, 
                double& tolerance) {
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
                    else if (key == "salesObjective") salesObjective = std::stod(value);
                    else if (key == "tolerance") tolerance = std::stod(value);
                }
            }
        }
    }
}

void readConfigFor8(const std::string& configFilePath, 
                    int& numberOfIterations, 
                    double& salesObjective, 
                    double& tolerance,
                    double& customerType,
                    double& typeOfSeller,
                    double& numberOfProductsSold,
                    double& saleDate,
                    double& products,
                    double& totalSaleValue) {
    std::ifstream configFile(configFilePath);
    std::string line;

    if (configFile.is_open()) {
        while (getline(configFile, line)) {
            std::istringstream iss(line);
            std::string key;
            if (getline(iss, key, '=')) {
                std::string value;
                if (getline(iss, value)) {
                    //std::cout << "Key: " << key << ", Value: " << value << std::endl;  // Debug output
                    if (key == "numberOfIterations") numberOfIterations = std::stoi(value);
                    else if (key == "salesObjective") salesObjective = std::stod(value);
                    else if (key == "tolerance") tolerance = std::stod(value);
                    else if (key == "customerType") customerType = std::stod(value);
                    else if (key == "typeOfSeller") typeOfSeller = std::stod(value);
                    else if (key == "numberOfProductsSold") numberOfProductsSold = std::stod(value);
                    else if (key == "saleDate") saleDate = std::stod(value);
                    else if (key == "products") products = std::stod(value);
                    else if (key == "totalSaleValue") totalSaleValue = std::stod(value);
                }
            }
        }
    }else {
        std::cerr << "Failed to open config file: " << configFilePath << std::endl;
    }
}

void readConfigFor17(const std::string& configFilePath, 
                    int& numberOfIterations, 
                    double& salesObjective, 
                    double& tolerance,
                    double& customerType,
                    double& typeOfSeller,
                    double& numberOfProductsSold,
                    double& saleDate,
                    double& products,
                    double& totalSaleValue,
                    double& priceDiscounts,
                    double& deliveryTime,
                    double& productType,
                    double& productList,
                    double& inventoryLevel,
                    double& perceptionOfRelationshipValue,
                    double& marketParticipation,
                    double& otherFactors) {
    std::ifstream configFile(configFilePath);
    std::string line;

    if (configFile.is_open()) {
        while (getline(configFile, line)) {
            std::istringstream iss(line);
            std::string key;
            if (getline(iss, key, '=')) {
                std::string value;
                if (getline(iss, value)) {
                    //std::cout << "Key: " << key << ", Value: " << value << std::endl;  // Debug output
                    if (key == "numberOfIterations") numberOfIterations = std::stoi(value);
                    else if (key == "salesObjective") salesObjective = std::stod(value);
                    else if (key == "tolerance") tolerance = std::stod(value);
                    else if (key == "customerType") customerType = std::stod(value);
                    else if (key == "typeOfSeller") typeOfSeller = std::stod(value);
                    else if (key == "numberOfProductsSold") numberOfProductsSold = std::stod(value);
                    else if (key == "saleDate") saleDate = std::stod(value);
                    else if (key == "products") products = std::stod(value);
                    else if (key == "totalSaleValue") totalSaleValue = std::stod(value);
                    else if (key == "priceDiscounts") priceDiscounts = std::stod(value);
                    else if (key == "deliveryTime") deliveryTime = std::stod(value);
                    else if (key == "productType") productType = std::stod(value);
                    else if (key == "productList") productList = std::stod(value);
                    else if (key == "inventoryLevel") inventoryLevel = std::stod(value);
                    else if (key == "perceptionOfRelationshipValue") perceptionOfRelationshipValue = std::stod(value);
                    else if (key == "marketParticipation") marketParticipation = std::stod(value);
                    else if (key == "otherFactors") otherFactors = std::stod(value);
                }
            }
        }
    }else {
        std::cerr << "Failed to open config file: " << configFilePath << std::endl;
    }
}

#endif // ABCMETHOD_H
