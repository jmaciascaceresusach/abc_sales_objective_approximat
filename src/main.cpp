/* Funcionalidad general 27032024
Implementa la clase "Parameter", proporcionando un constructor y un método
"adjustProbability" para ajustar la probabilidad del parámetro y asegurarse
de que permanezca en el rango [0,1].

Problema encontrado (por solucionar): 
- Se podrían aprovechar más funcionalidades
de C++ para manejar archivos y procesamiento de texto, mejorando la calidad y
robustez del código.
- Agregar los parámetros al archivo simulation_config para que dinámicamente
se puedan modificar.
- Visualizar el vector de salida y la cantidad de iteraciones de la simulación
que se alcanzaron a realizar
*/

#include "../include/Parameter.h"
#include "../include/SimulationEngine.h"
#include <iostream>
#include <vector>
#include <cstdlib> // para std::atoi and std::atof
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

// Función para calcular ventas simuladas en función de parámetros.
double calculateSale(const std::vector<Parameter>& parameters) {
    double totalSaleValue = 0.0;
    // Simular cálculo de venta. Este es un marcador de posición para la lógica real,
    // lo que normalmente implicaría el uso de parámetros para determinar el valor de venta,
    for (const auto& param : parameters) {
        if (param.name == "number of products sold") {
            totalSaleValue += param.probability * 100; // Lógica de ejemplo
        }
    }
    return totalSaleValue;
}

// Para 3 parámetros: 
// Parte 1: numberOfIterations, salesObjective y tolerance
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

/* Cambio (06052024) 
Para 8 parámetros: numberOfIterations, 
                   salesObjective, 
                   tolerance,
                   customerType, 
                   typeOfSeller, 
                   numberOfProductsSold, 
                   saleDate, 
                   products,
                   totalSaleValue
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

/* Cambio (07052024) 
Para 17 parámetros: numberOfIterations, 
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
                    otherFactors
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

int main(int argc, char* argv[]) {

    // Obtiene el tiempo de inicio
    auto start = std::chrono::high_resolution_clock::now();

    // Cambio (06052024) 
    // Guardar log en archivo externo (depurar)
    // std::ofstream out("simulation_output.txt");
    // std::streambuf* coutbuf = std::cout.rdbuf(); // Guarda el buffer viejo
    // std::cout.rdbuf(out.rdbuf()); // Redirecciona std::cout al archivo output.txt
    
    // Etapa 1
    /*if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " numberOfIterations salesObjective tolerance\n";
        return 1;
    }
    int numberOfIterations = std::atoi(argv[1]);
    double salesObjective = std::atof(argv[2]);
    double tolerance = std::atof(argv[3]);*/

    // Etapa 1
    // int numberOfIterations;
    // double salesObjective, tolerance;

    /* Cambio (06052024)
    int numberOfIterations;
    double salesObjective, tolerance;
    double customerType, typeOfSeller, numberOfProductsSold, saleDate, products, totalSaleValue;
    */

    // Cambio (07052024)
    int numberOfIterations;
    double salesObjective, tolerance;
    double customerType, typeOfSeller, numberOfProductsSold, saleDate, products, totalSaleValue;
    double priceDiscounts, deliveryTime, productType, productList, inventoryLevel, perceptionOfRelationshipValue;
    double marketParticipation, otherFactors;

    // Para 3 parámetros: 
    // Parte 1: numberOfIterations, salesObjective y tolerance
    // readConfig("simulation_config.txt", numberOfIterations, salesObjective, tolerance);

    /* Cambio (06052024) 
    Para 8 parámetros: numberOfIterations, 
                    salesObjective, 
                    tolerance,
                    customerType, 
                    typeOfSeller, 
                    numberOfProductsSold, 
                    saleDate, 
                    products,
                    totalSaleValue
    
    readConfigFor8("../simulation_config.txt", 
                    numberOfIterations, 
                    salesObjective, 
                    tolerance,
                    customerType,
                    typeOfSeller,
                    numberOfProductsSold,
                    saleDate,
                    products,
                    totalSaleValue);
    */

    /* Cambio (07052024) 
    Para 17 parámetros: numberOfIterations, 
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
                        otherFactors
    */
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

    // Cambio (07052024) 
    Parameter priceDiscountsParameter("priceDiscounts", priceDiscounts);
    Parameter deliveryTimeParameter("deliveryTime", deliveryTime);
    Parameter productTypeParameter("productType", productType);
    Parameter productListParameter("productList", productList);
    Parameter inventoryLevelParameter("inventoryLevel", inventoryLevel);
    Parameter perceptionOfRelationshipValueParameter("perceptionOfRelationshipValue", perceptionOfRelationshipValue);
    Parameter marketParticipationParameter("marketParticipation", marketParticipation);
    Parameter otherFactorsParameter("otherFactors", otherFactors);

    // Agregar parámetros
    simulationEngine.addParameter(customerTypeParameter);
    simulationEngine.addParameter(typeOfSellerParameter);
    simulationEngine.addParameter(numberOfProductsSoldParameter);
    simulationEngine.addParameter(saleDateParameter);
    simulationEngine.addParameter(productsParameter);
    simulationEngine.addParameter(totalSaleValueParameter);

    // Cambio (07052024) 
    simulationEngine.addParameter(priceDiscountsParameter);
    simulationEngine.addParameter(deliveryTimeParameter);
    simulationEngine.addParameter(productTypeParameter);
    simulationEngine.addParameter(productListParameter);
    simulationEngine.addParameter(inventoryLevelParameter);
    simulationEngine.addParameter(perceptionOfRelationshipValueParameter);
    simulationEngine.addParameter(marketParticipationParameter);
    simulationEngine.addParameter(otherFactorsParameter);

    // Objetivo de ventas y tolerancia (antigua, ahora se hace por archivo de entrada)
    //double salesObjective = 10000; // Ejemplo de objetivo de ventas
    //double tolerance = 300; // Desviación aceptable del objetivo

    // Ejecutar simulaciones
    simulationEngine.runSimulations(numberOfIterations, calculateSale, salesObjective, tolerance);

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

    // Cambio (07052024) 
    std::cout << "priceDiscounts: " << priceDiscounts << std::endl;
    std::cout << "deliveryTime: " << deliveryTime << std::endl;
    std::cout << "productType: " << productType << std::endl;
    std::cout << "productList: " << productList << std::endl;
    std::cout << "inventoryLevel: " << inventoryLevel << std::endl;
    std::cout << "perceptionOfRelationshipValue: " << perceptionOfRelationshipValue << std::endl;    
    std::cout << "marketParticipation: " << marketParticipation << std::endl;    
    std::cout << "otherFactors: " << otherFactors << std::endl;
    std::cout << "\n";

    // Parámetros refinados de salida
    // std::cout << "***Refined Parameters***" << std::endl;
    // for (const auto& param : simulationEngine.parameters) {
    //     std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
    // }
    // std::cout << "\n";

    // Obtiene el tiempo de finalización
    auto end = std::chrono::high_resolution_clock::now();

    // Calcula la duración en milisegundos
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "***Duration***" << std::endl;
    std::cout << "Time: " << duration.count() << " milliseconds" << std::endl;
    std::cout << "\n";
    
    // Cambio (06052024) 
    // Guardar log en archivo externo (depurar)
    // Restaura el buffer original para que std::cout escriba a la consola de nuevo
    // std::cout.rdbuf(coutbuf);
    // out.close();

    return 0;
}
