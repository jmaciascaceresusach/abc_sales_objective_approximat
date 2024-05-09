#include "../include/Parameter.h"
#include "../include/SimulationEngine.h"
#include <iostream>
#include <vector>
#include <cstdlib> // para std::atoi and std::atof
#include <fstream>  
#include <sstream>
#include <chrono>

/**
 * Función para calcular ventas simuladas en función de parámetros.
 * @param parameters Lista de parámetros utilizados para calcular las ventas.
 * @return Valor total de las ventas calculadas.
 */
double calculateSale(const std::vector<Parameter>& parameters) {
    double totalSaleValue = 0.0;
    for (const auto& param : parameters) {
        if (param.name == "customerType") {
            totalSaleValue += param.probability * 80;
        } else if (param.name == "typeOfSeller") {
            totalSaleValue += param.probability * 50;
        } else if (param.name == "numberOfProductsSold") {
            totalSaleValue += param.probability * 90;
        } else if (param.name == "saleDate") {
            totalSaleValue += param.probability * 20;
        } else if (param.name == "products") {
            totalSaleValue += param.probability * 10;
        } else if (param.name == "totalSaleValue") {
            totalSaleValue += param.probability * 11;
        } else if (param.name == "priceDiscounts") {
            totalSaleValue += param.probability * 21;
        } else if (param.name == "deliveryTime") {
            totalSaleValue += param.probability * 34;
        } else if (param.name == "productType") {
            totalSaleValue += param.probability * 24;
        } else if (param.name == "productList") {
            totalSaleValue += param.probability * 65;
        } else if (param.name == "inventoryLevel") {
            totalSaleValue += param.probability * 79;
        } else if (param.name == "perceptionOfRelationshipValue") {
            totalSaleValue += param.probability * 12;
        } else if (param.name == "marketParticipation") {
            totalSaleValue += param.probability * 45;
        } else if (param.name == "otherFactors") {
            totalSaleValue += param.probability * 72;
        }
    }
    return totalSaleValue;
}

/**
 * Lee una configuración completa desde un archivo para 8 parámetros.
 * @param configFilePath Ruta al archivo de configuración.
 * @param numberOfIterations Variable donde se almacenará el número de iteraciones.
 * @param salesObjective Variable donde se almacenará el objetivo de ventas.
 * @param tolerance Variable donde se almacenará la tolerancia.
 * @param customerType Variable donde se almacenará el valor inicial de customerType.
 * @param typeOfSeller Variable donde se almacenará el valor inicial de typeOfSeller.
 * @param numberOfProductsSold Variable donde se almacenará el valor inicial de numberOfProductsSold.
 * @param saleDate Variable donde se almacenará el valor inicial de saleDate.
 * @param products Variable donde se almacenará el valor inicial de products.
 * @param totalSaleValue Variable donde se almacenará el valor inicial de totalSaleValue.
 */
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
    } else {
        std::cerr << "Failed to open config file: " << configFilePath << std::endl;
    }
}

/**
 * Lee una configuración completa desde un archivo para 17 parámetros.
 * @param configFilePath Ruta al archivo de configuración.
 * @param numberOfIterations Variable donde se almacenará el número de iteraciones.
 * @param salesObjective Variable donde se almacenará el objetivo de ventas.
 * @param tolerance Variable donde se almacenará la tolerancia.
 * @param customerType Variable donde se almacenará el valor inicial de customerType.
 * @param typeOfSeller Variable donde se almacenará el valor inicial de typeOfSeller.
 * @param numberOfProductsSold Variable donde se almacenará el valor inicial de numberOfProductsSold.
 * @param saleDate Variable donde se almacenará el valor inicial de saleDate.
 * @param products Variable donde se almacenará el valor inicial de products.
 * @param totalSaleValue Variable donde se almacenará el valor inicial de totalSaleValue.
 * @param priceDiscounts Variable donde se almacenará el valor inicial de priceDiscounts.
 * @param deliveryTime Variable donde se almacenará el valor inicial de deliveryTime.
 * @param productType Variable donde se almacenará el valor inicial de productType.
 * @param productList Variable donde se almacenará el valor inicial de productList.
 * @param inventoryLevel Variable donde se almacenará el valor inicial de inventoryLevel.
 * @param perceptionOfRelationshipValue Variable donde se almacenará el valor inicial de perceptionOfRelationshipValue.
 * @param marketParticipation Variable donde se almacenará el valor inicial de marketParticipation.
 * @param otherFactors Variable donde se almacenará el valor inicial de otherFactors.
 */
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
    } else {
        std::cerr << "Failed to open config file: " << configFilePath << std::endl;
    }
}

int main(int argc, char* argv[]) {
    // Obtiene el tiempo de inicio
    auto start = std::chrono::high_resolution_clock::now();

    int numberOfIterations;
    double salesObjective, tolerance;
    double customerType, typeOfSeller, numberOfProductsSold, saleDate, products, totalSaleValue;
    double priceDiscounts, deliveryTime, productType, productList, inventoryLevel, perceptionOfRelationshipValue;
    double marketParticipation, otherFactors;

    readConfigFor17("../simulation_config.txt", 
                    numberOfIterations, 
                    salesObjective, 
                    tolerance,
                    customerType,
                    typeOfSeller,
                    numberOfProductsSold,
                    saleDate,
                    products,
                    totalSaleValue,
                    priceDiscounts,
                    deliveryTime,
                    productType,
                    productList,
                    inventoryLevel,
                    perceptionOfRelationshipValue,
                    marketParticipation,
                    otherFactors);

    // Inicializar el motor de simulación
    SimulationEngine simulationEngine;

    // Definir y agregar parámetros
    Parameter customerTypeParameter("customerType", customerType);
    Parameter typeOfSellerParameter("typeOfSeller", typeOfSeller);
    Parameter numberOfProductsSoldParameter("numberOfProductsSold", numberOfProductsSold);
    Parameter saleDateParameter("saleDate", saleDate);
    Parameter productsParameter("products", products);
    Parameter totalSaleValueParameter("totalSaleValue", totalSaleValue);
    Parameter priceDiscountsParameter("priceDiscounts", priceDiscounts);
    Parameter deliveryTimeParameter("deliveryTime", deliveryTime);
    Parameter productTypeParameter("productType", productType);
    Parameter productListParameter("productList", productList);
    Parameter inventoryLevelParameter("inventoryLevel", inventoryLevel);
    Parameter perceptionOfRelationshipValueParameter("perceptionOfRelationshipValue", perceptionOfRelationshipValue);
    Parameter marketParticipationParameter("marketParticipation", marketParticipation);
    Parameter otherFactorsParameter("otherFactors", otherFactors);

    // Agregar parámetros al motor de simulación
    simulationEngine.addParameter(customerTypeParameter);
    simulationEngine.addParameter(typeOfSellerParameter);
    simulationEngine.addParameter(numberOfProductsSoldParameter);
    simulationEngine.addParameter(saleDateParameter);
    simulationEngine.addParameter(productsParameter);
    simulationEngine.addParameter(totalSaleValueParameter);
    simulationEngine.addParameter(priceDiscountsParameter);
    simulationEngine.addParameter(deliveryTimeParameter);
    simulationEngine.addParameter(productTypeParameter);
    simulationEngine.addParameter(productListParameter);
    simulationEngine.addParameter(inventoryLevelParameter);
    simulationEngine.addParameter(perceptionOfRelationshipValueParameter);
    simulationEngine.addParameter(marketParticipationParameter);
    simulationEngine.addParameter(otherFactorsParameter);

    // Ejecutar simulaciones
    simulationEngine.runSimulations(numberOfIterations, calculateSale, salesObjective, tolerance);

    std::cout << "\n";
    std::cout << "***Configurations***" << std::endl;
    std::cout << "numberOfIterations: " << numberOfIterations << std::endl;
    std::cout << "salesObjective: " << salesObjective << std::endl;
    std::cout << "tolerance: " << tolerance << std::endl;
    std::cout << "\n";

    std::cout << "***Startup Parameters***" << std::endl;
    std::cout << "customerType: " << customerType << std::endl;
    std::cout << "typeOfSeller: " << typeOfSeller << std::endl;
    std::cout << "numberOfProductsSold: " << numberOfProductsSold << std::endl;
    std::cout << "saleDate: " << saleDate << std::endl;
    std::cout << "products: " << products << std::endl;
    std::cout << "totalSaleValue: " << totalSaleValue << std::endl;
    std::cout << "priceDiscounts: " << priceDiscounts << std::endl;
    std::cout << "deliveryTime: " << deliveryTime << std::endl;
    std::cout << "productType: " << productType << std::endl;
    std::cout << "productList: " << productList << std::endl;
    std::cout << "inventoryLevel: " << inventoryLevel << std::endl;
    std::cout << "perceptionOfRelationshipValue: " << perceptionOfRelationshipValue << std::endl;    
    std::cout << "marketParticipation: " << marketParticipation << std::endl;    
    std::cout << "otherFactors: " << otherFactors << std::endl;
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
