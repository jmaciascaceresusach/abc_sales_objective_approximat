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
void readConfig(const std::string& configFilePath, int& numberOfIterations, double& salesObjective, double& tolerance) {
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

// Para 8 parámetros: 
// Parte 1: numberOfIterations, salesObjective, tolerance; y
// Parte 2: customer type, type of seller, number of products sold, sale date, products y total sale value
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
    }
}

int main(int argc, char* argv[]) {
    
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

    // Cambio (06052024)
    int numberOfIterations;
    double salesObjective, tolerance, customerType, typeOfSeller, numberOfProductsSold, saleDate, products, totalSaleValue;

    // Para 3 parámetros: 
    // Parte 1: numberOfIterations, salesObjective y tolerance
    // readConfig("simulation_config.txt", numberOfIterations, salesObjective, tolerance);

    // Cambio (06052024)
    // Para 8 parámetros: 
    // Parte 1: numberOfIterations, salesObjective, tolerance; y
    // Parte 2: customer type, type of seller, number of products sold, sale date, products y total sale value
    readConfigFor8("simulation_config.txt", 
                    numberOfIterations, 
                    salesObjective, 
                    tolerance,
                    customerType,
                    typeOfSeller,
                    numberOfProductsSold,
                    saleDate,
                    products,
                    totalSaleValue);

    // Inicializar el motor de simulación
    SimulationEngine simulationEngine;

    // Definir y agregar parámetros
    /*Parameter customerType("customer type", 0.7);
    Parameter typeOfSeller("type of seller", 0.2);
    Parameter numberOfProductsSold("number of products sold", 0.3);
    Parameter saleDate("sale date", 0.1);
    Parameter products("products", 0.2);
    Parameter totalSaleValue("total sale value", 0.2);*/

    // Agregar parámetros
    simulationEngine.addParameter(customerType);
    simulationEngine.addParameter(typeOfSeller);
    simulationEngine.addParameter(numberOfProductsSold);
    simulationEngine.addParameter(saleDate);
    simulationEngine.addParameter(products);
    simulationEngine.addParameter(totalSaleValue);

    // Objetivo de ventas y tolerancia (antigua, ahora se hace por archivo de entrada)
    //double salesObjective = 10000; // Ejemplo de objetivo de ventas
    //double tolerance = 300; // Desviación aceptable del objetivo

    // Ejecutar simulaciones
    simulationEngine.runSimulations(numberOfIterations, calculateSale, salesObjective, tolerance);

    std::cout << "numberOfIterations: " << numberOfIterations << std::endl;
    std::cout << "salesObjective: " << salesObjective <<  std::endl;
    std::cout << "tolerance: " << tolerance <<  std::endl;

    // Parámetros refinados de salida
    std::cout << "Refined Parameters:" << std::endl;
    for (const auto& param : simulationEngine.parameters) {
        std::cout << "Parameter: " << param.name << ", Probability: " << param.probability << std::endl;
    }

    return 0;
}
